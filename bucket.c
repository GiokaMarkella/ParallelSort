/*************************************************************************
Implementation File   		: bucket.c
Author      		        :
Purpose				        :
**************************************************************************/

#include "./include/bucket.h"


int bucket_get_new_position(bucket_info *bucket, int byte_val, int* hist, int* psum){
  int new_pos;
  int flag;
  // if(byte_val == HIST_SIZE-1) new_pos = ((bucket->global_num_of_elements)-hist[byte_val]);
  if(byte_val == HIST_SIZE-1){ flag =1; new_pos = ((bucket->local_num_of_elements)-hist[byte_val]);}
  else {flag=2; new_pos = (psum[byte_val+1] - hist[byte_val]);}

  new_pos += bucket->start;

  return new_pos;
}


void bucket_init(bucket_info *bucket, int start, int end, int num_elements){
  bucket->start = start;
  bucket->end = end;
  bucket->local_num_of_elements = num_elements;

  //default - if no value is given
  bucket->psum_start = start;
  bucket->psum_end = end;
  bucket->global_num_of_elements = num_elements;
}

int bucket_has_all_elements(bucket_info *bucket){
  return (bucket->global_num_of_elements==bucket->local_num_of_elements);
}

uint64_t bucket_get_size(bucket_info *bucket){
  return (bucket->end-bucket->start)+1;
}

void bucket_init_empty(bucket_info *bucket){
  bucket->start = -1;
  bucket->end = -1;
  bucket->local_num_of_elements = -1;

  //default - if no value is given
  bucket->psum_start = -1;
  bucket->psum_end = -1;
  bucket->global_num_of_elements = -1;
}

void bucket_sub_init(bucket_info *new_bucket, bucket_info *parent_bucket, int bucket_start, int bucket_end){
  new_bucket->psum_start = bucket_start;
  new_bucket->psum_end = bucket_end;

  int parent_bucket_start = parent_bucket->start;
  new_bucket->start = bucket_start+parent_bucket_start;
  new_bucket->end = bucket_end+parent_bucket_start;

  new_bucket->global_num_of_elements = parent_bucket->global_num_of_elements;
  new_bucket->local_num_of_elements = (bucket_end-bucket_start)+1;
}

bucket_info slice_bucket(bucket_info *parent_bucket, int bucket_num, int* psum){
  bucket_info new_bucket;
  bucket_init_empty(&new_bucket);

  if(psum[bucket_num]==parent_bucket->local_num_of_elements) return new_bucket; //no more elements to sort
  if(bucket_num!= HIST_SIZE-1 && psum[bucket_num+1]==psum[bucket_num]) return new_bucket; //bucket is empty

  int bucket_start;
  int bucket_end;

  //sub-bucket start (according to psum)
  bucket_start = psum[bucket_num];

  //sub-bucket end (according to psum)
  if(bucket_num == HIST_SIZE-1) bucket_end = (parent_bucket->local_num_of_elements)-1;
  else bucket_end = psum[bucket_num+1]-1;

  bucket_sub_init(&new_bucket, parent_bucket, bucket_start, bucket_end);
  return new_bucket;
};

int bucket_is_empty(bucket_info* bucket){
  if(bucket->local_num_of_elements==0 ||bucket->local_num_of_elements==-1) return 1;
  else return 0;
}

void print_bucket_start(bucket_info* bucket){
  printf("======================================\n");
  printf("Sorting Bucket [%d, %d] START\n",bucket->start,bucket->end);

}

void print_bucket_end(bucket_info* bucket){
  printf("======================================\n");
  printf("Sorting Bucket [%d, %d] FINISH\n",bucket->start,bucket->end);

}
