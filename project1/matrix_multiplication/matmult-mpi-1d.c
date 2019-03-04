#include "stdio.h"
#include "string.h"
#include "mpi.h"

// can be changed wrt input matrix dimensions.
#define MAX_LINE_LENGTH 100

// the matrix multiplication is of the form:
// A x B = C
// the variable naming is done in this manner.

// program inputs: 
// arg1 : the name of the file that contains matrix A
// arg2 : the name of the file that contains matrix B
// arg3 : the name of the file that will contain matrix C after program execution

// NOTES:
// 1- This program assumes the number of processors to be used is a perfect square
// 2- The dimensions of the input matrices is perfectly divisible by square root
//    of the number of processors. (e.g. input matrices are 30x30 then we use 9 processors).

// for code re-use
int dot_product(int *left, int *right, int size) {
    int sum = 0; 
    for(int idx = 0; idx < size; idx++) {
        sum += left[idx] * right[idx];
    }
    return sum;
}

int main(int argc, char** argv) {

    if(argc != 4) {
        printf("This program expects 3 filenames as input. First and second arguments are the names of the files that contains the matrix A and B respectively. The third argument is the name of the output file to be produced.\n");
        return 1;
    }

    MPI_Status status;
    int size, rank, ele, block_size, comp_per_node;
    int *row_block, *col_block;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // master process reads the file and generates input matrices.
    // IMPORTANT NOTE: A matrix is stored in row-major order but B matrix is stored in column major order
    // This tweak makes splitting matrices to rows and columns easier.
    if(rank == 0) { // master

        FILE *fp_A, *fp_B, *fp_C;

        // buffer for file read and write
        char buffer[MAX_LINE_LENGTH];

        if(argc != 4) {
            printf("This program expects 3 filenames as input. First and second arguments are the names of the files that contains the matrix A and B respectively. The third argument is the name of the output file to be produced.\n");
            return 1;
        }

        // input matrix filenames
        char * A_filename = argv[1];
        char * B_filename = argv[2];
        // output filename of the C matrix
        char * C_filename = argv[3];

        fp_A = fopen(A_filename, "r");
        fp_B = fopen(B_filename, "r");
        fp_C = fopen(C_filename, "w");

        if(fp_A ==  NULL) {
            printf("File can not be opened: %s", A_filename);
            return 1;
        } 
        if(fp_B == NULL) {
            printf("File can not be opened: %s", B_filename);
            return 1;
        }

        fgets(buffer, MAX_LINE_LENGTH, fp_A);
        int A_dim = atoi(buffer);
        int A[A_dim][A_dim];

        fgets(buffer, MAX_LINE_LENGTH, fp_B);
        int B_dim = atoi(buffer);
        int B[B_dim][B_dim];

        // store matrix B in column major order
        int row = 0;
        int col = 0;
        char * element;
        while ( fgets(buffer, MAX_LINE_LENGTH, fp_B) != NULL) {
            row = 0;
            element =  strtok(buffer, " ");
            while (element != NULL) {
                B[row][col] = atoi(element);
                row++;
                element = strtok(NULL, " ");
            }
            col++;
        }

        // store matrix A in row major order
        row = 0;
        col = 0;
        while ( fgets(buffer, MAX_LINE_LENGTH, fp_A) != NULL) {
            col = 0;
            element =  strtok(buffer, " ");
            while (element != NULL) {
                printf(" current element: %d\n", atoi(element));
                int toStore = atoi(element);
                A[row][col] = toStore;
                col++;
                element = strtok(NULL, " ");
            }
            row++;
        }

        for( int i = 0; i < A_dim; i++) {
            for( int j = 0; j < A_dim; j++) {
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }

        

        

        printf("\n");

        for( int i = 0; i < B_dim; i++) {
            for( int j = 0; j < B_dim; j++) {
                printf("%d ", B[i][j]);
            }
            printf("\n");
        }

        // since both A and B are square matrices, C also has the same dimension as A and B
        int C_dim = A_dim;
        int C[C_dim][C_dim];

        // define block size in order to pass to worker processors
        block_size = C_dim;

        // find computation per matrix
        comp_per_node = C_dim * C_dim / size;
        printf("computation_per_processor: %d\n", comp_per_node);

        // each processor needs to do comp_per_node dot product operations.
        int op_count = 0;
        int current_processor = 0;

        int flag = 1;

        for(int row = 0; row < C_dim; row++) {
            for(int col = 0; col < C_dim; col++) {

                row_block = A[row]; // get one row of A
                col_block = B[col]; // get one col of B

                // master processor's turn to compute
                if(current_processor == 0) {
                    
                    printf("Row block: %d, %d, %d, %d, %d, %d\n", row_block[0], row_block[1], row_block[2], row_block[3], row_block[4], row_block[5]);
                    printf("Col block: %d, %d, %d, %d, %d, %d\n", col_block[0], col_block[1], col_block[2], col_block[3], col_block[4], col_block[5]);
                    ele = dot_product(row_block, col_block, C_dim);
                    printf("computed this as ele: %d\n", ele);

                    // need to pass computation turn to another processor after the comp_per_node th operation
                    if(op_count + 1 >= comp_per_node) {
                        op_count = 0;
                        current_processor++;
                    }
                    else {
                        op_count++;
                    }
                }
                // worker processors' turn to compute
                else {
                    if (flag == 1) {
                        // first pass the computation per node to workers
                        int comp_tag = current_processor + 4 * size;
                        MPI_Send((void *)&comp_per_node, 1, MPI_INT, current_processor, comp_tag, MPI_COMM_WORLD);
                        flag = 0;

                    }
                    // pass the size of col and row blocks to the workers
                    int size_tag = current_processor;
                    MPI_Send((void *)&block_size, 1, MPI_INT, current_processor, size_tag, MPI_COMM_WORLD);
                    printf("master sent size to %d\n", current_processor);

                    // pass row block to worker process
                    int row_tag = current_processor +  size;
                    MPI_Send((void *)row_block, C_dim, MPI_INT, current_processor, row_tag, MPI_COMM_WORLD);
                    printf("master passed row block to %d\n", current_processor);

                    // pass col block to worker process
                    int col_tag = current_processor + 2 * size;
                    MPI_Send((void *)col_block, C_dim, MPI_INT, current_processor, col_tag, MPI_COMM_WORLD);
                    printf("master sent col block to %d\n", current_processor);

                    // receive the result of the operation
                    int ele_tag = current_processor + 3 * size;
                    MPI_Recv((void*)&ele, 1, MPI_INT, current_processor, ele_tag, MPI_COMM_WORLD, &status);
                    printf("master received [%d][%d] element from %d\n", row, col, current_processor);

                    // need to pass computation turn to another processor after the comp_per_node th operation
                    if(op_count + 1 == comp_per_node) {
                        op_count = 0;
                        flag = 1;
                        current_processor++;
                    }
                    else {
                        op_count++;
                    }

                }

                C[row][col] = ele;
            }
        }

        // Write results of the operation to the specified output file.
        for( int i = 0; i < C_dim; i++) {
            for( int j = 0; j < C_dim; j++) {
                fprintf(fp_C, "%d ", C[i][j]);
            }
            fprintf(fp_C, "\n");
        }

        // Also for testing, print the result to console
        for( int i = 0; i < C_dim; i++) {
            for( int j = 0; j < C_dim; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }

        printf("\n");

        // Close all file handles
        fclose(fp_A);
        fclose(fp_B);
        fclose(fp_C);


    }
    else { // worker

        int size_tag = rank;
        int row_tag = rank + size;
        int col_tag = rank + 2 * size;
        int ele_tag = rank + 3 * size;
        int comp_tag = rank + 4 * size;

        // First receive computation amount to do
        MPI_Recv((void*)&comp_per_node, 1, MPI_INT, 0, comp_tag, MPI_COMM_WORLD, &status);

        for (int i = 0; i < comp_per_node; i++){

            // Then receive block size
            MPI_Recv((void*)&block_size, 1, MPI_INT, 0, size_tag, MPI_COMM_WORLD, &status);
            printf("worker ranked %d received block size\n", rank);

            // Then create corresponding row and column blocks in stack
            int row[block_size], col[block_size];
            row_block = row;
            col_block = col;

            // Then receive row block and col block
            MPI_Recv((void*)row_block, block_size, MPI_INT, 0, row_tag, MPI_COMM_WORLD, &status);
            printf("worker ranked %d received row block\n", rank);
            MPI_Recv((void*)col_block, block_size, MPI_INT, 0, col_tag, MPI_COMM_WORLD, &status);
            printf("worker ranked %d received col block\n", rank);

            // Perform computation
            ele = dot_product(row_block, col_block, block_size);

            // Send result beck to master
            MPI_Send((void*)&ele, 1, MPI_INT, 0, ele_tag, MPI_COMM_WORLD);
            printf("worker ranked %d sent ele: %d\n", rank, ele);
        }

    }

    MPI_Finalize();

    return 0;
}

