// Need for clock_gettime()
// Have to be at the begining of the file
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

// #include <immintrin.h>

#include "./include/relation.h"
#include "./include/merge_sort.h"


#define ARGS_NUM 3
#define WRONG_ARGS -1
#define OK 0
#define TRUE 1
#define FALSE 0

// call this function to start a nanosecond-resolution timer
struct timespec timer_start(){
    struct timespec start_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
    return start_time;
}

// call this function to end a timer, returning nanoseconds elapsed as a long
long timer_end(struct timespec start_time){
    struct timespec end_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
    long diffInNanos = (end_time.tv_sec - start_time.tv_sec) * (long)1e9 + (end_time.tv_nsec - start_time.tv_nsec);
    return diffInNanos;
}

int read_arguments(relation *r, int argc , char *argv []){
  char *filepath;

  if(argc!=ARGS_NUM){
    return WRONG_ARGS;
  }

  for(int i=0; i<argc; i++){
    if(!strcmp(argv[i], "-r")){ //first relation
      filepath = malloc(strlen(argv[i+1])*sizeof(char)+1);
      strcpy(filepath, argv[i+1]);
      i++;
    }
  }

  relation_init_data(r, filepath, 1);
  free(filepath);
  return OK;
}

int main(int argc , char *argv []) {

    relation r;
    int ret_val = read_arguments(&r, argc , argv);

    merge_sort(&r, 0, r.num_values);
    relation_print(&r);
    relation_destroy(&r);
}
