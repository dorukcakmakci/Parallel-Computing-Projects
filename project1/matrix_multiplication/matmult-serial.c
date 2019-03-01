#include "stdio.h"
#include "string.h"

// can be changed wrt input matrix dimensions.
#define MAX_LINE_LENGTH 10

// the matrix multiplication is of the form:
// A x B = C
// the variable naming is done in this manner.

int main(int argc, char** argv) {

  FILE *fp_A, *fp_B, *fp_C;

  // buffer for file read and write
  char buffer[MAX_LINE_LENGTH];

  // input matrix filenames
  char * A_filename = "./A.txt";
  char * B_filename = "./B.txt";
  // output filename of the C matrix
  char * C_filename = "./C.txt";

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

  printf("\n");

  // since both A and B are square matrices, C also has the same dimension as A and B
  int C_dim = A_dim;

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

  for( int i = 0; i < C_dim; i++) {
    for( int j = 0; j < C_dim; j++) {
        printf("%d ", C[i][j]);
    }
    printf("\n");
  }

  printf("\n");

  fclose(fp_A);
  fclose(fp_B);
  fclose(fp_C);


  return 0;
}