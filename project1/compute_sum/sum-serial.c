#include "stdio.h"

#define MAX_LINE_LENGTH 5

int main(int argc, char** argv) {
  FILE* file;
  char* filename = "./input3.txt";
  char line[MAX_LINE_LENGTH];

  file = fopen(filename, "r");

  if( file == NULL ){
    printf("Could not open file: %s", filename);
  }
  else{
    int running_sum = 0;

    while(fgets(line, MAX_LINE_LENGTH, file) != NULL) {
      running_sum += atoi(line);
    }

    printf("Final sum is: %d", running_sum);

    fclose(file);
  }

  return 0;
}