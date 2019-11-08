/*************************************************************************
Header File Name		: relation.h
Author        			:
Purpose			    		:
**************************************************************************/
#ifndef __RELATION__
#define __RELATION__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// #include <check.h> //unit testing

#include "tuple.h"

// // Maximum bucket size in bytes
// #define MAX_BUCKET_SIZE 64*1024

//2^8 spaces
#define HIST_SIZE 256

// //64KB = 64*1000 = 64000 bytes
// //sizeof(uint64_t) = 8 bytes
// // 64000 / 8 = 8000
// #define MAX_ELEMENTS 8000

//MAX_ELEMENTS/2 because tuples
//have two uint64_t values each
#define MAX_TUPLES 64*1024/sizeof(tuple)

// #define MAX_TUPLES 50


#define NORM_ON 1
#define NORM_OFF 0

#define TRUE 1
#define FALSE 0

typedef struct relation{
  tuple *tuples;
  uint64_t num_tuples;

  int max_key_value;
  int normalize_on;
  int is_normalized;

}relation;

void denormalize_keys(relation* myrel);

/*
* initialize relation by giving filename of array
* and the number of the collumn (rel_num)
* ARGS:
*   -relation object pointer
*   -filename of file which contains array
*   -number of collumn to read from file (for the key value)
*   -NORM_ON or NORM_OFF depending on whether or not the
*     normalizing algorithm should be used
*/
void relation_init(relation* myrel, char* filename, int rel_num, int norm_bool);

void relation_init_data(relation* myrel, char* filename, int rel_num, int norm_bool);
/*
* initialize empty relation of a specific size (number of rows)
* ARGS:
*   -relation object pointer
*   -number of rows
*/
void relation_init_empty(relation ** rel, int rel_rows, int norm_bool);

void relation_set_max_key(relation *src, relation *dst);

int relation_norm_alg(relation *myrel);

void relation_copy_tuple(relation* src, relation* dst, int src_index, int dst_index);

void relation_print(relation* myrel);

void relation_print_range(relation* myrel, int start, int end);

/*
* makes histogram array based on the nth byte of
* the payloads.
* You can choose the range of the (tuple) array from which to
* compute the histogram from by giving a range_start
* and range_end.
* ARGS:
*   -relation object pointer
*   -index (int) to determine start of bucket
*   -index (int) to determine end of bucket
*   -number of bytes on which the histogram is to
*     be calculated (for leftmost byte of uint64_t - n=8)
* RETURNS: array with computed values
* AFTER: use free() to free dynamically allocated array
*/
int* relation_get_hist(relation *myrel, int range_start, int range_end, int nth_byte);

/*
* computes the starting positions of the sub buckets
* based on the histogram.
* ARGS: histogram array returned from relation_get_hist
* RETURNS: array with computed values
* AFTER: use free() to free dynamically allocated array
*/
int* get_psum_from_hist(int* hist);

void print_psum(int* psum);

void print_hist(int* hist);

void relation_destroy(relation* myrel);

tuple* relation_get_tuple(relation* myrel, int nth_tuple);

void bucket_print(relation * rel, int begin, int end, int rec);
#endif
