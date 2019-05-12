#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE_LENGTH 100
#define EPSILON 1e-200

// macros
#define iszero(a) \
    ({ __typeof__ (a) _a = (a); \
            fabs(_a) < EPSILON ?  1 : 0; })

// #define max(a,b) \
//    ({ __typeof__ (a) _a = (a); \
//        __typeof__ (b) _b = (b); \
//      _a > _b ? _a : _b; })

 
// auxilary function to read sparse matrix from file
// main program

// int iszero(double *x){
//     return x > 0 ?
// }
int main(int argc, char** argv) {

    if (argc != 5) {
        printf("ERROR: Argument list is wrong!\n");
        return -1;
    }

    int num_of_threads = atoi(argv[1]);
    int number_of_iterations = atoi(argv[2]);
    int flag = atoi(argv[3]);
    char *filename = argv[4];

    // read sparse matrix file and initialize row_ptr, col_ind and values arrays
    // the following code assumes input matrix is a square matrix
    FILE *mtx = fopen(filename, "r");
    char line[MAX_LINE_LENGTH];
    const char delimiters[] = " ";

    // allocate space for row_ptr, col_ind and values arrays using data from the first line of the matrix file
    fgets(line, MAX_LINE_LENGTH, mtx);
    char * running = strdup(line);
    int num_rows = atoi(strsep(&running, delimiters));
    int num_cols = atoi(strsep(&running, delimiters));
    int num_ele = atoi(strsep(&running, delimiters));
    int row_ptr[num_rows], col_ind[num_ele];
    double values[num_ele];

    //index for counter for each row
    int counter[num_rows];
    for (int i = 0; i < num_rows; i++) {
        counter[i] = 0;
        row_ptr[i] = 0;
    }

    // first read the file to find the number of non-zero elements in each row
    int cur_row, cur_col;
    double cur_val;
    while(fgets(line, MAX_LINE_LENGTH, mtx) != NULL) {
        running = strdup(line);
        cur_row = atoi(strsep(&running, delimiters)) - 1;
        cur_col = atoi(strsep(&running, delimiters)) - 1;
        cur_val = atof(strsep(&running, delimiters));
        if(iszero(cur_val)) {
            row_ptr[cur_row]++;
        }
        else {
            continue;
        }

    }    

    /* test */
    printf("row_ptr Test: \n");
    for (int i = 0; i < num_rows; i++) {
        printf("row_ptr[%d] = %d\n", i, row_ptr[i]);
    }
    /* end test*/

    return 0;

    // convert row_ptr to an increasing array by adding an element and the element before that.
    int  temp = row_ptr[0];
    int temp_2;
    row_ptr[0] = 0;
    for(int j = 1; j <= num_rows - 1; j++) {
        temp_2 = row_ptr[j];
        row_ptr[j] = row_ptr[j-1] + temp;
        temp = temp_2;
    }

    /* test */
    printf("row_ptr Test: \n");
    for (int i = 0; i < num_rows; i++) {
        printf("row_ptr[%d] = %d\n", i, row_ptr[i]);
    }
    /* end test*/


    fclose(mtx);

    // read file again to initialize values
    mtx = fopen(filename, "r");

    // discard  first line
    fgets(line, MAX_LINE_LENGTH, mtx);

    while(fgets(line, MAX_LINE_LENGTH, mtx) != NULL) {
        running = strdup(line);
        cur_row = atoi(strsep(&running, delimiters)) - 1;
        cur_col = atoi(strsep(&running, delimiters)) - 1;
        cur_val = atof(strsep(&running, delimiters));
        if( cur_row < num_rows - 1 ) {
            if (iszero(cur_val)) {
                temp = row_ptr[cur_row] + counter[cur_row];
                if(temp < row_ptr[cur_row + 1]) {
                    col_ind[temp] = cur_col;
                    values[temp] = cur_val;
                }
                counter[cur_row]++;
            }
        }
        else {
            if ( iszero(cur_val)) {
                temp = row_ptr[cur_row] + counter[cur_row];
                if(temp < num_ele) {
                    col_ind[temp] = cur_col;
                    values[temp] = cur_val;
                }
                counter[cur_row]++;
            }
        }
    }

    /* test */
    printf("Counter Test: \n");
    for (int i = 0; i < num_rows; i++) {
        printf("%dth row counter = %d\n", i+1, counter[i]);
    }
    /* end test*/ 

    // now since sparse matrix is created, initialize vector
    double x[num_rows];
    for( int i = 0; i < num_rows; i++) {
        x[i] = 1;
    }

    if(flag == 1){

        printf("Vector X: \n");
        for(int i = 0; i < num_rows; i++) {
            printf("%lf ", x[i]);
        }
        printf("\n");

        printf("Sparse Matrix(only non zero elements): \n");
        int i, j;
        for (i = 0; i < num_rows - 1; i++) {
            int cur_index = row_ptr[i];
            int next_index = row_ptr[i+1];
            printf("Row %d:\n", i+1);
            while (cur_index < next_index) {
                printf("\tCol: %d --> %lf\n", col_ind[cur_index], values[cur_index]);
                cur_index++;
            }
            printf("\n");
        }
    }


    // computation starts here
    int iter;
    for(iter = 0; iter < number_of_iterations; iter++) {
        // matrix vector computation here
    }

    
    return 0;
}