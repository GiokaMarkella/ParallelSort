// Need for clock_gettime()
// Have to be at the begining of the file
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "./include/relation.h"
#include "./include/resultsList.h"
#include "./include/sort_merge_join.h"

#define ARGS_NUM 9
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

int read_arguments(relation *r1, relation *r2, int argc , char *argv []){
  char *filepath1;
  char *filepath2;
  char *norm_alg;
  int norm_bool = NORM_OFF;
  char *data_type; //new or old type of data
  int new_data_bool;

  if(argc!=ARGS_NUM){
    return WRONG_ARGS;
  }

  for(int i=0; i<4; i++){
    if(!strcmp(argv[i], "-norm")){ //normalizing algorithm
      norm_alg = malloc(strlen(argv[i+1])*sizeof(char)+1);
      strcpy(norm_alg, argv[i+1]);
      if(!strcmp(norm_alg, "on")){
        printf("norm on!\n");
        norm_bool = NORM_ON;
        free(norm_alg);
      }
      else if(!strcmp(norm_alg, "off")){
        printf("norm off!\n");

        norm_bool = NORM_OFF;
        free(norm_alg);
      }
      else{
        return WRONG_ARGS;
      }
      i++;
    }
    if(!strcmp(argv[i], "-data")){ //type of data
      data_type = malloc(strlen(argv[i+1])*sizeof(char)+1);
      strcpy(data_type, argv[i+1]);
      if(!strcmp(data_type, "new")){

        new_data_bool = TRUE;
        free(data_type);
      }
      else if(!strcmp(data_type, "old")){

        new_data_bool = FALSE;
        free(data_type);
      }
      else{
        return WRONG_ARGS;
      }
      i++;
    }
  }

  for(int i=4; i<argc; i++){
    if(!strcmp(argv[i], "-r1")){ //first relation
      filepath1 = malloc(strlen(argv[i+1])*sizeof(char)+1);
      strcpy(filepath1, argv[i+1]);
      i++;
    }
    if(!strcmp(argv[i], "-r2")){ //second relation
      filepath2 = malloc(strlen(argv[i+1])*sizeof(char)+1);
      strcpy(filepath2, argv[i+1]);
      i++;
    }
  }

  if(new_data_bool==TRUE){
    relation_init_data(r1, filepath1, 1, norm_bool);
    relation_init_data(r2, filepath2, 1, norm_bool);
  }
  else{
    relation_init(r1, filepath1, 1, norm_bool);
    relation_init(r2, filepath2, 1, norm_bool);
  }
  // relation_init_data(r1, filepath1, 1, norm_bool);
  // relation_init_data(r2, filepath2, 1, norm_bool);
  free(filepath1);
  free(filepath2);
  return OK;
}

int main(int argc , char *argv []) {

    // Initialize relations in appropriate forms
    relation r1, r2;
    int ret_val = read_arguments(&r1, &r2, argc , argv);
    if(ret_val==WRONG_ARGS){
      printf("Wrong arguments given!\n");
      return 1;
    }

    time_t start, finish;
    printf("start!\n");
    struct timespec vartime = timer_start();

    // Call main method
    resultsList * result= SortMergeJoin(&r1, &r2);
    double time_elapsed_nanos = timer_end(vartime);
    printf("Time taken: %lf\n", time_elapsed_nanos/(long)1e9);
    // TODO copy result in csv file
    char *fileName = malloc(strlen("Results") + 5);
    strcpy(fileName, "Results");
    createCSVFile(result, fileName);

    free(fileName);



    // CLEANUP
    listDestroy(result);
    relation_destroy(&r1);
    relation_destroy(&r2);

}
