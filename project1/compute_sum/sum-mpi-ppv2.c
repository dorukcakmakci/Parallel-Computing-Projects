#include "stdio.h"
#include "mpi.h"

// subject to change 
#define MAX_LINE_LENGTH 5 

int main(int argc, char **argv) {

    if(argc != 2) {
        printf("This program expects the name of the file that contains a list of integers.\n");
        return 1;
    }

    MPI_Status status;
    int size, rank, number_count, local_sum;
    int *arr;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // master process reads the file and sends individual integers to all of the processes.
    if (rank == 0) {

        FILE* file;
 
        char* filename = argv[1];

        char line[MAX_LINE_LENGTH];

        file = fopen(filename, "r");

        if( file == NULL ){
            printf("Could not open file: %s", filename);
        }
        else{
            
            int line_count = 0;
            
            while(fgets(line, MAX_LINE_LENGTH, file) != NULL) {
                line_count++;
            }

            fclose(file);

            // re-read file to get its contents to the dynamically allocated array.
            file = fopen(filename, "r");

            int *input = malloc( line_count * sizeof(int));

            int index = 0;

            while(fgets(line, MAX_LINE_LENGTH, file) != NULL) {
                input[index] = atoi(line);
                index++;
            }

            number_count = line_count;
            arr = input;
        }
    }

    // master process broadcasts the array length to the workers
    MPI_Bcast((void *)&number_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // wait for everyone to receive the array length
    MPI_Barrier(MPI_COMM_WORLD);

    if(rank != 0){
        // worker processes allocate space for the array to be received.
        arr = malloc(number_count * sizeof(int));
    }

    // master process broadcasts the array to the workers
    MPI_Bcast((void *)arr, number_count, MPI_INT, 0, MPI_COMM_WORLD);

    // wait for everyone to receive the array itself
    MPI_Barrier(MPI_COMM_WORLD);

    // everyone determines their portion of the data to compute.
    int quotient = number_count / size;
    int remainder = number_count % size;
    int start_index = rank >= remainder ? quotient*rank + remainder : quotient*rank +rank;
    int computation_size = rank >= remainder ? quotient : quotient + 1;

    // collective computation of portions
    local_sum = 0;

    // edgecase: processor count can be much more than the count of numbers to sum.
    if(start_index < number_count){
        for(int i = 0; i < computation_size; i++){
            local_sum += arr[start_index + i];
        }
    }
    
    //use all reduce to compute final sum
    int global_sum = 0;
    MPI_Allreduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // free the allocated heap space for all processors
    free(arr);

    if(rank == 0) {
        printf("%d\n", global_sum);
    }

    MPI_Finalize();

    return 0;
}