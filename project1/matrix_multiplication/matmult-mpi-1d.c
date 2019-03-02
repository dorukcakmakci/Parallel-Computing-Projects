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

int main(int argc, char** argv) {

    if(argc != 4) {
        printf("This program expects 3 filenames as input. First and second arguments are the names of the files that contains the matrix A and B respectively. The third argument is the name of the output file to be produced.\n");
        return 1;
    }

    MPI_Status status;
    int size, rank;

    MPI_Init(&argc, &argv);

    // Insert mm code here.

    MPI_Finalize();

    return 0;
}