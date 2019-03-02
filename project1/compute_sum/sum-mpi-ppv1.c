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
    int size, rank, number_count;
    int *arr_slice;
    int local_sum;

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

            // determine how to partition file contents to processors wrt file size and count of processors
            int quotient = line_count / (size);
            int remainder = line_count % (size);

            if(remainder == 0){
                number_count = quotient;
            }
            else{
                number_count = quotient + 1;
                remainder--;
            }

            // master process does its own summation here. 
            int index_so_far = 0;

            int aggregator = 0;
            
            while(index_so_far < number_count){
                aggregator += input[index_so_far];
                index_so_far++;
            }

            // now, send the corresponding data to the workers which sums the data.
            for (int src = 1; src < size; src++){

                // send size of the array to sum
                if(remainder == 0){
                    number_count = quotient;
                }
                else{
                    number_count = quotient + 1;
                    remainder--;
                }

                if(index_so_far >= line_count){
                    number_count = 0;
                    MPI_Send((void *)&number_count, 1, MPI_INT, src, 0xACE5, MPI_COMM_WORLD);
                    continue;
                }

                MPI_Send((void *)&number_count, 1, MPI_INT, src, 0xACE5, MPI_COMM_WORLD);

                // send the sub-array for the worker
                arr_slice = malloc(number_count * sizeof(int));
                for (int i = 0; i < number_count; i++){
                    arr_slice[i] = input[index_so_far + i];
                }
                MPI_Send((void *)arr_slice, number_count, MPI_INT, src, 0xAAAA, MPI_COMM_WORLD);
                free(arr_slice);

                // update start index
                index_so_far += number_count;
                printf("%d\n", index_so_far);
            }

            
            // receive the local sums from workers and aggregate them.
            for (int src = 1; src < size; src++){
                MPI_Recv((void *)&local_sum, 1, MPI_INT, src, 0xBBBB, MPI_COMM_WORLD, &status);
                aggregator += local_sum;
            }

            printf("%d\n", aggregator);

            // deallocate input array
            free(input);
        }

    }
    // other processes receive data and send the local sum of the numbers received.
    else {
        // receive the size of the data to sum.
        MPI_Recv((void *)&number_count, 1, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD, &status);

        if(number_count != 0){
            arr_slice = malloc(number_count * sizeof(int));

            // receive ptr to the array to sum.
            MPI_Recv((void *)arr_slice, number_count, MPI_INT, 0, 0xAAAA, MPI_COMM_WORLD, &status);

            // workers do their job here
            local_sum = 0;
            for(int i = 0; i < number_count; i++){
                local_sum += arr_slice[i];
            }

            free(arr_slice);
        }
        else{
            local_sum = 0;
        }

        

        // send local sum back to master process
        MPI_Send((void *)&local_sum, 1, MPI_INT, 0, 0xBBBB, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();

    return 0;
}