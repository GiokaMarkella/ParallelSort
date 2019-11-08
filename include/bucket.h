/*************************************************************************
Header File Name		: bucket.h
Author        			:
Purpose			        :
**************************************************************************/
#ifndef __BUCKET__
#define __BUCKET__

#include <stdio.h>
#include "relation.h"

typedef struct bucket_info{
  int start;  //start in main array
  int end;    //end in main array

  int psum_start; //start according to psum array
  int psum_end;    //end according to psum array

  int global_num_of_elements; //number of elements in main array
  int local_num_of_elements;  //number of elements in local array

}bucket_info;

int bucket_get_new_position(bucket_info *bucket, int byte_val, int* hist, int* psum);

// void copy_bucket(relation * from_rel, int bucket_start, int bucket_end, relation ** to_rel)

void bucket_init(bucket_info *bucket, int start, int end, int num_elements);

int bucket_has_all_elements(bucket_info *bucket);

uint64_t bucket_get_size(bucket_info *bucket);

void bucket_init_empty(bucket_info *bucket);

void bucket_sub_init(bucket_info *new_bucket, bucket_info *parent_bucket, int bucket_start, int bucket_end);

bucket_info slice_bucket(bucket_info *parent_bucket, int bucket_num, int* psum);

int bucket_is_empty(bucket_info* bucket);

void print_bucket_start(bucket_info* bucket);

void print_bucket_end(bucket_info* bucket);


#endif
