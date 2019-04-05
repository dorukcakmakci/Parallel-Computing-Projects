#include "utils.h"
#include "math.h"
#include "mpi.h"
#include "time.h"

// defined to sort id value pairs together
struct Pair {
    int id;
    int val;
    int rank;
};
typedef struct Pair Pair;

// mpi reduction operation base
void pair_compare( Pair * in, Pair * inout, int * len, MPI_Datatype *datatype) {
    if(in->id == -1 && inout->id == -1) { //invalid pair
        // do nothing since both are invalid
    }
    else if(in->id == -1) {
        // do nothing since inout is not invalid
    }
    else if(inout->id == -1) {
        *inout = *in;
    }
    // now no pairs are invalid
    else if(in->val <= inout->val) {
        *inout = *in;
    }
}


// define sorting function
static int id_val_cmp(const void *a, const void *b) {
  const Pair *da = a, *db = b;
  int result = 0;
  if( da->val < db->val) result = -1;
  else if( da->val > db->val) result = 1;
  return result;
}

void kreduce(int * leastk, int * myids, int * myvals, int k, int world_size, int my_rank){

    MPI_Barrier(MPI_COMM_WORLD);

    // create a new reduction operation
    MPI_Op min;
    MPI_Op_create((MPI_User_function *) pair_compare, 1, &min);

    // for each process hold a current index for the myvals array that shows the location of the minimum similarity value
    int index_of_minimum = 0;

    // current_pair->rank = my_rank
    // current_pair->val = current minimum similarity value
    // current_pair->id = document id associated with the similarity value
    Pair *current_pair = malloc(3 * sizeof(int));
    current_pair->rank = my_rank;

    // master will receive the pair containing minimum similarity value in min_pair
    Pair *min_pair = malloc(3 * sizeof(int));

    // every process receives the rank of the process having the minimum value in rank_of_min
    int rank_of_min = -1; 

    // All processors will reduce their minimum value pair at most K times
    for(int i = 0; i < k; i++) {

        if(index_of_minimum < k){
            // init the id and similarity val of current minimum similarity valued document (rank is initialized before loop)
            // all processes does this operation
            current_pair->id = myids[index_of_minimum];
            current_pair->val = myvals[index_of_minimum];
        }
        else {
            // invalidate current processor since all of the data is exhausted
            current_pair->id = -1;
            current_pair->val = -1;
        }


        // reduce pairs to the master process
        MPI_Reduce(current_pair, min_pair, 3, MPI_INT, min, 0, MPI_COMM_WORLD);


        // master broadcasts the rank of the process that sent the minimum similarity value and
        // initializes the ith smallest similarity value
        if(my_rank == 0) {
            // printf("min_pair_rank %d", min_pair->rank);
            rank_of_min = min_pair->rank;
            leastk[i] = min_pair->id;
        }

        MPI_Bcast(&rank_of_min, 1, MPI_INT, 0, MPI_COMM_WORLD);

        MPI_Barrier(MPI_COMM_WORLD);

        // the process that had the current minimum value needs to update its index
        if(my_rank == rank_of_min) {
            index_of_minimum++;
        }
    }

}

int main(int argc, char **argv) {

    

    //--------------------  serial part  --------------------
    // arguments must be as the following:
    // W: the name of the file containing the set of documents
    // Q: the name of the file containing the set of queries on documents
    // D: the size of the dictionary
    // K: the number of least documents to return
    // check if the input has these properties

    clock_t start = clock();
    clock_t diff;

    double serial_time;
    double parallel_time;

    if( argc != 5) {
        printf("The program inputs are faulty!\n");
        return -1;
    }

    
    int D = atoi(argv[1]);
    int K = atoi(argv[2]);
    char *W_filename = argv[3];
    char *Q_filename = argv[4];

    int **documents, **queries, *document_ids;

    int query_count = read_query_file(Q_filename, D, K, &queries);
    int document_count = read_document_file(W_filename, D, K, &documents, &document_ids);

    // copy the data at heap to stack
    int docs[document_count][D];
    int doc_ids[document_count];
    int qs[query_count][D];

    for(int i = 0; i < document_count; i++){
        doc_ids[i] = document_ids[i];
        for(int j = 0; j < D; j++){
            docs[i][j] = documents[i][j];
        }
    }

    for(int i = 0; i < query_count; i++){
        for(int j = 0; j < D; j++){
            qs[i][j] = queries[i][j];
        }
    }
    
    // delete the data stored at heap
    free(document_ids);
    for(int i = 0; i < document_count; i++) {
        free(documents[i]);
    }
    free(documents);

    for(int i = 0; i < query_count; i++) {
        free(queries[i]);
    }
    free(queries);

    diff = clock() - start;

    serial_time = (double)diff * 1000 / CLOCKS_PER_SEC;

    //-------------------- parallel part --------------------

    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double t1, t2;
    t1 = MPI_Wtime();

    //printf("I am processor %d / %d and I have documents[1][2] as %d\n", (rank+1), size, docs[1][2]);
    

    // first we need to separate the query files and documents among the processors.
    // note that master also does computation.

    // everyone determines their contiguous set of the documents to compute similarity.
    // Assumption: Since the sizes of myids and myvals are declared as K in the project description,
    // (the number of processes used) * K must be equal to the number of documents and the remainder of document count 
    // of dividing with K must be 0!!  

    // int start_index = K*rank ;
    // int computation_size = K;

    int quotient = document_count / size;
    int remainder = document_count % size;
    int start_index = rank >= remainder ? quotient*rank + remainder : quotient*rank +rank;
    int computation_size = rank >= remainder ? quotient : quotient + 1;

    //printf("I am processor %d / %d and I have start_index: %d and computation_size as %d\n", (rank+1), size, start_index, computation_size);

    // if computation size is 0, don't do an operation on any data !!!!!!!

    // data allocation per processor
    int * myids = malloc(computation_size * sizeof(int));
    int * myvals = malloc(computation_size * sizeof(int));
    int * least_k = NULL;
    if (rank == 0) {
        least_k = malloc(K * sizeof(int));
    }

    

    // compute similarity values and the inputs for kreduce function
    for(int i = 0; i < computation_size; i++) {
        myvals[i] = 0;
        myids[i] = doc_ids[start_index + i];
        for(int j = 0; j < D; j++) {
            myvals[i] +=  pow(docs[start_index + i][j], qs[0][j]);
        }
    }

    

    // combine id and similarity values of documents to sort them together based on similarity value.
    Pair *combined = malloc(computation_size * sizeof(Pair));

    for(int i = 0; i < computation_size; i++) {
        combined[i].id = myids[i];
        combined[i].val = myvals[i];
    }

    // sort document ids and similarity values together based on similarity values
    qsort(combined, computation_size, (sizeof *combined), id_val_cmp);

    int * myids_2 = malloc(K * sizeof(int));
    int * myvals_2 = malloc(K * sizeof(int));

    // get the sorted order of ids and values
    for(int i = 0; i < K; i++) {
        if( K <= computation_size) {
            myids_2[i] = combined[i].id;
            myvals_2[i] = combined[i].val;
        }
        else {
            if( i < computation_size) {
                myids_2[i] = combined[i].id;
                myvals_2[i] = combined[i].val;
            }
            else {
                myvals_2[i] = INT_MAX;
                myids_2[i] = -1;
            }
        }
        
        
    }

    free(combined);

    printf("I am processor %d / %d and I have myids: ", (rank+1), size);
    for(int i = 0; i < K; i++)
        printf("%d ", myids_2[i]);
    printf("and myvals: ");
    for (int j = 0; j < K; j++)
        printf("%d ", myvals_2[j]);
    printf("\n");

    kreduce(least_k, myids_2, myvals_2, K, size, rank);

    t2 = MPI_Wtime();

    MPI_Finalize();

    if(least_k != NULL) {
        parallel_time = 1000*(t2 -t1);
        // only the serial part for master process is printed
        printf("Serial Part: %f ms\n", serial_time);
        printf("Parallel Part: %f ms\n", parallel_time);
        printf("Total Time: %f ms\n", (serial_time + parallel_time));
        printf("Least k = %d ids:\n", K);
        for(int i = 0; i < K; i++) {
            printf("%d\n", least_k[i]);
        }
    }
    t2 = MPI_Wtime();

    
    
    
    return 0;



    
}