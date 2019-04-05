#include "utils.h"

int read_query_file(char * Q_filename, int D, int K, int*** queries){

    FILE *Q_file;
    char line[MAX_LINE_LENGTH]; 

    Q_file = fopen(Q_filename, "r");

    // read the query file
    if( Q_file == NULL ){
        printf("Could not open file: %s", Q_filename);
        return -1;
    }

    int line_count = 0;

    while(fgets(line, MAX_LINE_LENGTH, Q_file) != NULL) {
        line_count++;
    }

    fclose(Q_file);

    int num_queries = line_count;

    (*queries) = malloc( num_queries * sizeof(int * ));
    for (int i = 0; i < num_queries; i++) {
        (*queries)[i] = malloc(D * sizeof(int));
    } 
    

    // re-read query file to get queries
    Q_file = fopen(Q_filename, "r");

    int index = 0;
    char * ele;
    while ( fgets(line, MAX_LINE_LENGTH, Q_file) != NULL) {

        int j = 0;
        ele =  strtok(line, " ");

        while (ele != NULL) {

            (*queries)[index][j] = atoi(ele);
            j++;
            ele = strtok(NULL, " ");
        }

        index++;
    }

    fclose(Q_file);

    return num_queries;


}

int read_document_file(char * W_filename, int D, int K, int*** documents, int ** document_ids){

    FILE *W_file;
    char line[MAX_LINE_LENGTH];

    W_file = fopen(W_filename, "r");

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

    (*documents) = malloc( num_docs * sizeof(int * ));
    for (int i = 0; i < num_docs; i++) {
        (*documents)[i] = malloc(D * sizeof(int));
    } 

    (*document_ids) = malloc(num_docs * sizeof(int));

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
        (*document_ids)[index] = atoi(ele);
        

        // get words of the document
        int j = 0;
        
        // need to separate two times for the following substring in input line: ': '
        ele = strsep(&running, delimiters);
        ele = strsep(&running, delimiters);
        
        while(ele != NULL){
            (*documents)[index][j] = atoi(ele);
            j++;
            ele = strsep(&running, delimiters);
        }

        index++;
    }

    fclose(W_file);

    // documents contain num_docs documents of size D.
    // document_ids contain  num_docs document ids.

    return num_docs;


}