#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "cuda.h"

// kernel function for matrix vector product computation
__global__ void computeMatrixVectorProduct(int *d_row_ptr, int *d_col_ind, double *d_vector, double * d_result_vector, double* d_values, int num_of_threads, int matrix_dim, int num_of_ele) {
    
    // first each thread computes which portion of the data to work on which is represented as [start_index, end_index)
    int start_index, end_index;

    start_index = 0;
    end_index = 0;

    int quotient = matrix_dim / num_of_threads;
    int remainder = matrix_dim % num_of_threads;
    int number_count = quotient;

    if(threadIdx.x < remainder) {
        number_count = quotient + 1;
    }

    for (int i = 0; i < threadIdx.x; i++) {
        if (i < remainder) {
            start_index += quotient + 1;
        }
        else {
            start_index += quotient;
        }
    }

    end_index = start_index + number_count;

    if(end_index > matrix_dim) {
        end_index = matrix_dim;
    }

    int cur_col_ind;
    int next_col_ind;

    for (int row = start_index; row < end_index; row++) {

        d_result_vector[row] = 0;

        // last row
        if(row == matrix_dim - 1) {

            cur_col_ind = d_row_ptr[row];

            while(cur_col_ind < num_of_ele && cur_col_ind >= 0) {
                d_result_vector[row] += d_values[d_col_ind[cur_col_ind]] * d_vector[d_col_ind[cur_col_ind]];
                cur_col_ind++;
            }
        }

        // empty row
        else if(d_row_ptr[row] == d_row_ptr[row + 1]){
            d_result_vector[row] = 0;
        }

        // if all the following rows are empty
        else if( d_row_ptr[row] >= num_of_ele ) {
            d_result_vector[row] = 0;
        }

        // and other case
        else{

            cur_col_ind = d_row_ptr[row];
            next_col_ind = d_row_ptr[row + 1];

            while( cur_col_ind  < next_col_ind) {
                d_result_vector[row] += d_values[d_col_ind[cur_col_ind]] * d_vector[d_col_ind[cur_col_ind]];
                cur_col_ind++;
            }

        }
    }

    
}

int main(int argc, char ** argv) {

    int thread_count = atoi(argv[1]);
    int iter_count = atoi(argv[2]);
    int flag = atoi(argv[3]);
    char *filename = argv[4];

    // read sparse matrix file and initialize row_ptr, col_ind and values arrays
    // the following code assumes input matrix is a square matrix
    FILE *matrix = fopen(filename, "r");
    int num_cols, num_rows, num_ele;
    fscanf(matrix, "%d %d %d\n", &num_rows, &num_cols, &num_ele);
    int row_ptr[num_rows];

    // initialize index counter and row_ptr
    int counter[num_rows];
    for (int i = 0; i < num_rows; i++) {
        counter[i] = 0;
        row_ptr[i] = 0;
    }

    // first read the file to find the number of non-zero elements in each row
    int cur_row, cur_col;
    double cur_val;

    // the maximum number of non-zero values is given in the mtx file but the actual numbe rof elements may be less than that
    // so hold an actual count of values
    int actual_count = 0;

    while (EOF != fscanf(matrix, "%d %d %le\n", &cur_row, &cur_col, &cur_val)) {
        cur_row -= 1;
        cur_col -= 1;
        row_ptr[cur_row]++;
        actual_count++;
    } 

    int col_ind[actual_count];
    double values[actual_count];

    // convert row_ptr to an increasing array by adding an element and the element before that.
    int  temp = row_ptr[0];
    int temp_2;
    row_ptr[0] = 0;

    for(int j = 1; j <= num_rows - 1; j++) {
        temp_2 = row_ptr[j];
        row_ptr[j] = row_ptr[j-1] + temp;
        temp = temp_2;
    }

    fclose(matrix);

    // read file again to initialize values
    matrix = fopen(filename, "r");

    // discard first line 
    fscanf(matrix, "%d %d %d\n", &num_rows, &num_cols, &num_ele);

    while (EOF != fscanf(matrix, "%d %d %le\n", &cur_row, &cur_col, &cur_val)) {
        cur_row -= 1;
        cur_col -= 1;
        if( cur_row < num_rows - 1 ) {
            temp = row_ptr[cur_row] + counter[cur_row];
            if(temp < row_ptr[cur_row + 1]) {
                col_ind[temp] = cur_col;
                values[temp] = cur_val;
            }
            counter[cur_row]++;
        }
        else {
                temp = row_ptr[cur_row] + counter[cur_row];
                if(temp < actual_count) {
                    col_ind[temp] = cur_col;
                    values[temp] = cur_val;
                }
                counter[cur_row]++;
        }
    }

    double *x = (double *) malloc(num_rows * sizeof(double));
    for( int i = 0; i < num_rows; i++) {
        x[i] = 1;
    }

    // print sparse matrix and vector before computation
    if(flag == 1){
        printf("Before Computation:\n");

        printf("Vector X: \n");
        for(int i = 0; i < num_rows; i++) {
            printf("%le ", x[i]);
        }
        printf("\n");

        

        int i;
        printf("Initial Matrix: \n");
        printf("row_ptr array: \n");
        for (i = 0; i <= num_rows - 1; i++) {
            printf("%d ", row_ptr[i]);
        }
        printf("\n");

        printf("col_ind array: \n");
        for (i = 0; i <= actual_count - 1; i++) {
            printf("%d ", col_ind[i]);
        }
        printf("\n");

        printf("values array: \n");
        for (i = 0; i <= actual_count - 1; i++) {
            printf("%le ", values[i]);
        }
        printf("\n");
    }

    // computation starts here

    // initialize device related structures 
    double *d_values, *d_vector, *d_result_vector;
    int *d_row_ptr, *d_col_ind;

    cudaMalloc(&d_values, actual_count * sizeof(double));
    cudaMalloc(&d_vector, num_rows * sizeof(double));
    cudaMalloc(&d_result_vector, num_rows * sizeof(double));
    cudaMalloc(&d_row_ptr, num_rows * sizeof(int));
    cudaMalloc(&d_col_ind, actual_count * sizeof(int));

    cudaMemcpy(d_values, values, actual_count * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_col_ind, col_ind, actual_count * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_row_ptr, row_ptr, num_rows * sizeof(int), cudaMemcpyHostToDevice);

    int iter;
    for(iter = 0; iter < iter_count; iter++) {
        cudaMemcpy(d_vector, x, num_rows * sizeof(double), cudaMemcpyHostToDevice);

        // call kernel code for sparse matrix and vector multiplication
        computeMatrixVectorProduct<<<1, thread_count>>>(d_row_ptr, d_col_ind, d_vector, d_result_vector, d_values, thread_count, num_rows, actual_count);

        cudaMemcpy(x, d_result_vector, num_rows * sizeof(double), cudaMemcpyDeviceToHost);

        // free device matrices

        // // for testing purposes
        // // printf("1: %s\n", cudaGetErrorString(cudaGetLastError()));
        // printf("Iter: %d, Vector X: \n", iter);
        // for(int i = 0; i < num_rows; i++) {
        //     printf("%le ", x[i]);
        // }
        // printf("\n");
    }

    cudaFree(d_values);
    cudaFree(d_vector);
    cudaFree(d_row_ptr);
    cudaFree(d_col_ind);
    cudaFree(d_result_vector);

    // print resulting vector from computation
    if(flag == 1 || flag == 2){

        printf("After Computation:\n");

        printf("Vector X: \n");
        for(int i = 0; i < num_rows; i++) {
            printf("%le ", x[i]);
        }
        printf("\n");
    }

    return 0;

}