#include "stdio.h"
#include "string.h"
//#include "mpi.h"

// to ensure the read buffer does not overflow but can be improved TODO
#define MAX_LINE_LENGTH 1000 

// kreduce implementation
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

    FILE *W_file, *Q_file;

    char line[MAX_LINE_LENGTH]; 
    W_file = fopen(W_filename, "r");
    Q_file = fopen(Q_filename, "r");

    // read the document file
    if( W_file == NULL ){
        printf("Could not open file: %s", W_filename);
        return -1;
    }

    // find the number of documents by pre-reading file
    int line_count = 0;

    while(fgets(line, MAX_LINE_LENGTH, W_file) != NULL) {
        line_count++;
    }

    fclose(W_file);

    int num_docs = line_count;

    //re-read the document file to store its contents in arrays.
    int documents[line_count][D];
    int document_ids[line_count];

    W_file = fopen(W_filename, "r");

    int index = 0;
    char *remainder;
    char * ele;
    const char delimiters[] = ": ";

    while(fgets(line, MAX_LINE_LENGTH, W_file) != NULL) {

        // line structure --> id: w0 w1 w2 ... wd
        char * running = strdup(line);

        // get document id
        ele = strsep(&running, delimiters);
        document_ids[index] = atoi(ele);

        // get words of the document
        int j = 0;
        
        // need to separate two times for the following substring in input line: ': '
        ele = strsep(&running, delimiters);
        ele = strsep(&running, delimiters);
        
        while(ele != NULL){
            documents[index][j] = atoi(ele);
            j++;
            ele = strsep(&running, delimiters);
        }

        index++;
    }

    fclose(W_file);

    // read the query file
    if( Q_file == NULL ){
        printf("Could not open file: %s", Q_filename);
        return -1;
    }

    line_count = 0;

    while(fgets(line, MAX_LINE_LENGTH, Q_file) != NULL) {
        line_count++;
    }

    fclose(Q_file);

    int num_queries = line_count;

    int queries[line_count][D];

    // re-read query file to get queries
    Q_file = fopen(Q_filename, "r");

    index = 0;
    while ( fgets(line, MAX_LINE_LENGTH, Q_file) != NULL) {

        int j = 0;
        ele =  strtok(line, " ");

        while (ele != NULL) {

            queries[index][j] = atoi(ele);
            j++;
            ele = strtok(NULL, " ");
        }

        index++;
    }

    fclose(Q_file);

    // test if documents are stored
    for(int i = 0; i < num_docs; i++){
        printf("%d: ", document_ids[i]);
        for(int j = 0; j < D; j++){
            printf("%d ", documents[i][j]);
        }
        printf("\n");
    }

    // test if queries are stored
    for(int i = 0; i < num_queries; i++){
        for(int j = 0; j < D; j++){
            printf("%d ", queries[i][j]);
        }
        printf("\n");
    }

    
    //-------------------- parallel part --------------------
    return 0;
}
