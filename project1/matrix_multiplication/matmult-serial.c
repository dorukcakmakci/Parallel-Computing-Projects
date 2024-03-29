#include "stdio.h"
#include "string.h"
#include <time.h>

// can be changed wrt input matrix dimensions.
#define MAX_LINE_LENGTH 1000

// the matrix multiplication is of the form:
// A x B = C
// the variable naming is done in this manner.

int main(int argc, char** argv) {

  clock_t start = clock(), diff;

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
  
  fgets(buffer, MAX_LINE_LENGTH, fp_B);
  int B_dim = atoi(buffer);

  int A[A_dim][A_dim], B[B_dim][B_dim];

  int row = 0;
  int col;
  char * ele;
  while ( fgets(buffer, MAX_LINE_LENGTH, fp_A) != NULL) {
    col = 0;
    ele =  strtok(buffer, " ");
    while (ele != NULL) {
      A[row][col] = atoi(ele);
      col++;
      ele = strtok(NULL, " ");
    }
    row++;
  }

  row = 0;
  while ( fgets(buffer, MAX_LINE_LENGTH, fp_B) != NULL) {
    col = 0;
    ele =  strtok(buffer, " ");
    while (ele != NULL) {
      B[row][col] = atoi(ele);
      col++;
      ele = strtok(NULL, " ");
    }
    row++;
  }

  // since both A and B are square matrices, C also has the same dimension as A and B
  int C_dim = A_dim;
  fprintf(fp_C, "%d\n", C_dim);

  int C[C_dim][C_dim];

  // serial matrix multiplication
  for (int i = 0; i < C_dim; i++) {
    for (int j = 0; j < C_dim; j++) {
      C[i][j] = 0;
      for (int k = 0; k < A_dim; k++)
      {
        C[i][j] += A[i][k] * B[k][j];
      }
      fprintf(fp_C, "%d ", C[i][j]);
    }
    fprintf(fp_C, "\n");
  }

  fclose(fp_A);
  fclose(fp_B);
  fclose(fp_C);

  diff = clock() - start;

  int msec = (double)diff * 1000000 / CLOCKS_PER_SEC;
  //printf("Time taken %d microseconds\n", msec);  

  return 0;
}