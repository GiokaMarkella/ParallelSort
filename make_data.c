#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int  main(int argc , char *argv []) {
    FILE* file = fopen("test_data2", "w");
    // uint64_t rows = 1000000;
    uint64_t rows = 50000;

    fprintf(file, "%lu,1\n",rows);
    uint64_t num;
    for(uint64_t i=0; i<rows; i++){
      num = rand();
      fprintf(file,"%lu,%lu\n", num,i);
    }
    fclose(file);

}
