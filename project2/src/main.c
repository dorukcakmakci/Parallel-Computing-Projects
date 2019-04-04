#include "utils.h"
#include "mpi.h"

void kreduce(int * leastk, int * myids, int * myvals, int k, int world_size, int my_rank){

}

int main(int argc, char **argv) {

    //--------------------  serial part  --------------------

    // arguments must be as the following:
    // W: the name of the file containing the set of documents
    // Q: the name of the file containing the set of queries on documents
    // D: the size of the dictionary
    // K: the number of least documents to return
    // check if the input has these properties

    if( argc != 5) {
        printf("The program inputs are faulty!\n");
        return -1;
    }

    char *W_filename = argv[1];
    char *Q_filename = argv[2];
    int D = atoi(argv[3]);
    int K = atoi(argv[4]);

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

    //-------------------- parallel part --------------------
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("I am processor %d / %d and I have documents[1][2] as %d\n", (rank+1), size, docs[1][2]);
    

    // first we need to separate the query files and documents among the processors.
    // note that master also does computation.


    MPI_Finalize();
    return 0;



    
}