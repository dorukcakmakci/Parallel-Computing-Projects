#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "limits.h"

// to ensure the read buffer does not overflow but can be improved TODO
#define MAX_LINE_LENGTH 30000 

int read_query_file(char * Q_filename, int D, int K, int*** queries);
int read_document_file(char * W_filename, int D, int K, int*** documents, int ** document_ids);