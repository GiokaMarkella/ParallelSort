// Need for clock_gettime()
// Have to be at the begining of the file
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include <immintrin.h>

#include "./include/relation.h"

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

    int64_t* a = malloc(sizeof(int64_t)*4);
    int64_t* b = malloc(sizeof(int64_t)*4);
    int64_t* c = malloc(sizeof(int64_t)*4);
    // int64_t* max = malloc(sizeof(int64_t)*4);


    //avx + avx2 min max that at least works
    //-------------------------------------------------------------------------------------
    __m256i t = _mm256_set_epi64x(6944444444444444481, 7833333333333333352, 7833333333333333353, 6944444444444444484);
    __m256i t2 = _mm256_set_epi64x(7833333333333333351, 6944444444444444482, 6944444444444444483, 7833333333333333354);

    __m256i gr = _mm256_cmpgt_epi64 (t2, t);
    //
    // __m256i mask_1 = _mm256_and_si256(gr, t2);
    // __m256i mask_2 = _mm256_andnot_si256(gr, t);
    //
    // __m256i mask_3 = _mm256_andnot_si256(gr, t2);
    // __m256i mask_4 = _mm256_and_si256(gr, t);
    //
    // _mm256_storeu_si256( (__m256i *)&c[0], gr);
    //
    // __m256i max = _mm256_xor_si256(mask_1, mask_2);
    // __m256i min = _mm256_xor_si256(mask_3, mask_4);
    //
    //
    // _mm256_storeu_si256( (__m256i *)&a[0], max);
    // _mm256_storeu_si256( (__m256i *)&b[0], min);

    __m256i minv = _mm256_blendv_epi8(t, t2, gr);
    __m256i maxv = _mm256_blendv_epi8(t2, t, gr);

    _mm256_storeu_si256( (__m256i *)&a[0], maxv);
    _mm256_storeu_si256( (__m256i *)&b[0], minv);

    // printf("my int sizes are %lu,%lu,%lu,%lu, %ld\n",c[0],c[1],c[2],c[3], sizeof(__m512i));
    //
    printf("my ints are %lu,%lu,%lu,%lu, %ld\n",a[0],a[1],a[2],a[3], sizeof(__m512i));
    printf("my ints are %lu,%lu,%lu,%lu, %ld\n",b[0],b[1],b[2],b[3], sizeof(__m512i));
    // //-------------------------------------------------------------------------------------


    // // Initialize relations in appropriate forms
    // relation r1, r2;
    // int ret_val = read_arguments(&r1, &r2, argc , argv);
    // if(ret_val==WRONG_ARGS){
    //   printf("Wrong arguments given!\n");
    //   return 1;
    // }
    //
    // time_t start, finish;
    // printf("start!\n");
    // struct timespec vartime = timer_start();
    //
    // // Call main method
    // resultsList * result= SortMergeJoin(&r1, &r2);
    // double time_elapsed_nanos = timer_end(vartime);
    // printf("Time taken: %lf\n", time_elapsed_nanos/(long)1e9);
    // // TODO copy result in csv file
    // char *fileName = malloc(strlen("Results") + 5);
    // strcpy(fileName, "Results");
    // createCSVFile(result, fileName);
    //
    // free(fileName);
    //
    //
    //
    // // CLEANUP
    // listDestroy(result);
    // relation_destroy(&r1);
    // relation_destroy(&r2);

}
