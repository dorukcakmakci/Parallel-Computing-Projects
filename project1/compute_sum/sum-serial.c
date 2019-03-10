#include "stdio.h"
#include <time.h>

#define MAX_LINE_LENGTH 100

int main(int argc, char** argv) {

  clock_t start = clock();
  clock_t diff;

  FILE* file;

  if(argc != 2) {
    printf("This program expects the name of the file that contains a list of integers.\n");
    return 1;
  }
  
  char* filename = argv[1];

  char line[MAX_LINE_LENGTH];

  file = fopen(filename, "r");

  if( file == NULL ){
    printf("Could not open file: %s", filename);
  }
  else{
    long running_sum = 0;

    while(fgets(line, MAX_LINE_LENGTH, file) != NULL) {
      running_sum += atoi(line);
    }

    printf("%ld\n", running_sum);

    fclose(file);
  }

  diff = clock() - start;

  int msec = (double)diff * 1000000 / CLOCKS_PER_SEC;
  //printf("Time taken %d microseconds\n", msec);

  return 0;
}