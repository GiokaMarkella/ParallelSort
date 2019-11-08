/*************************************************************************
Implementation File   		: radix_sort.c
Author      		        :
Purpose				        :
**************************************************************************/

#include "./include/radix_sort.h"

relation * left = NULL;
relation * right = NULL;

int hist_all_elements_in_one_bucket(int *hist, uint64_t global, int* psum){
  int flag=0;
  for(int i=0; i<HIST_SIZE; i++){

    if(hist[i]==global){
      // print_psum(psum);
      // printf("================hist is %lu at i %d\n", global, i);
      return 1;
      // if(flag==0) flag=1; //found first non-zero element
      // else return 0; //found second non-zero element
    }
    else if(hist[i]!=0){
      return 0;
    }
  }
  return 0;
}

void expand_psum_to_rel(int rec, int * psum, int* hist, relation * rel, bucket_info *bucket, relation * copy_rel, int copy_bool) {
    // Traverse through rel
    // For each row check where it should be put in copy_rel
    // (copy_rel is basically an ensemble of buckets)

    tuple* cur_tuple;            // current working tuple through traversion of rel
    int byte_val=0;               // byte val of corresponding byte of key based on rec number
    int new_pos;                // new position of tuple in new relation (copy_rel)

    int begin = bucket->start;
    int end = bucket->end;


    //traverse bucket
    copy_bool=0;
    if(copy_bool && hist_all_elements_in_one_bucket(hist, bucket->global_num_of_elements, psum)==1) copy_bool=1;
    // copy_bool=0;

    if(copy_bool==0){
      for(int pos=begin ; pos<=end ; pos++) {

          // Get tuple element of relation and corresponding sorting byte
          cur_tuple = &rel->tuples[pos];
          byte_val = tuple_get_nth_byte(cur_tuple, 8-rec+1);

          // Check based on psum where it should be put in copy_rel
          new_pos = bucket_get_new_position(bucket, byte_val, hist, psum);
          hist[byte_val]--;
          // if(new_pos==pos){
            // printf("22222 it happened when psum hist = %d, %d - %d / %d\n", hist[byte_val], end, begin, bucket->global_num_of_elements);
          // }
          tuple_copy(cur_tuple, &copy_rel->tuples[new_pos]);

      }
    }
    else{
      // printf("true\n");
      for(int pos=begin ; pos<=end ; pos++) {

          // Get tuple element of relation and corresponding sorting byte
          cur_tuple = &rel->tuples[pos];
          // byte_val = tuple_get_nth_byte(cur_tuple, 8-rec+1);
          //
          // // Check based on psum where it should be put in copy_rel
          // new_pos = bucket_get_new_position(bucket, byte_val, hist, psum);
          // hist[byte_val]--;
          // // new_pos=pos;
          // cur_tuple = &rel->tuples[pos];
          //
          new_pos=pos;
          tuple_copy(cur_tuple, &copy_rel->tuples[new_pos]);
      }
    }



}

void copy_bucket(relation * from_rel, bucket_info* this_bucket, relation * to_rel){
  int bucket_start = this_bucket->start;
  int bucket_end = this_bucket->end;
    for(int pos=bucket_start ;  pos<=bucket_end ; pos++) {
        tuple_copy(&from_rel->tuples[pos], &to_rel->tuples[pos]);
    }
}


relation * RadixSort(int rec, bucket_info this_bucket) {

    // print_bucket_start(&this_bucket);
    int begin = this_bucket.start;
    int end = this_bucket.end;

    // Set in which relation we are copying the results
    relation * copy_rel =  NULL;
    relation * cur_rel =  NULL;
    // If recursion number is odd -> copy to right relation ->> working on left
    if(rec%2 == 1) {
        copy_rel = right;
        cur_rel = left;
    }
    // If recursion number is even -> copy to left relation ->> working on right
    else if(rec%2 == 0) {
        copy_rel = left;
        cur_rel = right;
    }


    // Create histogram
    int * hist = relation_get_hist(cur_rel, begin, end, 8-rec+1);

    // Create psum
    int * psum = get_psum_from_hist(hist);
    // print_psum(psum);

    if(rec!=1 && bucket_has_all_elements(&this_bucket)){
      expand_psum_to_rel(rec, psum, hist, cur_rel, &this_bucket, copy_rel, 1);
    }
    else{
      // Build reorderd relation
      expand_psum_to_rel(rec, psum, hist, cur_rel, &this_bucket, copy_rel, 0);
    }

    // for each bucket
    uint64_t bucket_size=0;
    for(int bucket_num=0 ; bucket_num<HIST_SIZE ; bucket_num++) {

        bucket_info new_bucket = slice_bucket(&this_bucket, bucket_num, psum);
        if(bucket_is_empty(&new_bucket)) continue;


        // TODO CHECK RECURSION
        // printf("MAX TUPLES: %d , bucket_size: %d\n", MAX_TUPLES, bucket_end-bucket_start+1);
        // Else if bucket is the appropriate size
        // call quicksort for cur_rel and copy results also in the other relation (base on rec number)

        bucket_size = bucket_get_size(&new_bucket);
        if (bucket_size <= MAX_TUPLES) {

            // for(int i = new_bucket.start ; i < new_bucket.end ; i++)
            //     printf("%ld\n", copy_rel->tuples[i].key);

            // Calling quicksort to sort curretn bucket

            quickSort(copy_rel->tuples, new_bucket.start, new_bucket.end);

            // Copy sorted bucket in both relations (left and right relations)
            copy_bucket(copy_rel, &new_bucket, cur_rel);
        }
        else if(rec+1 == 9){
          copy_bucket(copy_rel, &new_bucket, cur_rel);
        }
        // If bucket is large -> Call radix sort for it
        // else if( bucket_end-bucket_start+1 > MAX_TUPLES) {
        else if(bucket_size > MAX_TUPLES){

            RadixSort(rec+1, new_bucket);

        }


      }
    free(hist);
    free(psum);
}

void RadixSort_Init(relation * rel) {
    left = rel;
    int norm_bool = relation_norm_alg(left);
    relation_init_empty(&right, rel->num_tuples, norm_bool);
    relation_set_max_key(left, right);
    bucket_info initial_bucket;
    bucket_init(&initial_bucket, 0, rel->num_tuples-1, rel->num_tuples);
    RadixSort(1, initial_bucket);                   // result is at the left relation
    relation_destroy(right);
    rel = left;
    free(right);
    denormalize_keys(left);
    // relation_print(rel);
}
