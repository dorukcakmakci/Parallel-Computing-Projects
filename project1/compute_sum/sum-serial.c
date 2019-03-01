#include "stdio.h"

#define MAX_LINE_LENGTH 5

int main(int argc, char** argv) {

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
    int running_sum = 0;

    while(fgets(line, MAX_LINE_LENGTH, file) != NULL) {
      running_sum += atoi(line);
    }

    printf("%d\n", running_sum);

    fclose(file);
  }

  return 0;
}