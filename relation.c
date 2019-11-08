/*************************************************************************
Implementation File   		: relation.c
Author      		        	:
Purpose				           	:
**************************************************************************/
#include "./include/relation.h"
void normalize_keys(relation* myrel);

// void denormalize_keys(relation* myrel);

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
void relation_init(relation* myrel, char* filename, int rel_num, int norm_bool){
  FILE* file = fopen(filename, "r");
  myrel->is_normalized = FALSE;
  myrel->normalize_on = NORM_OFF;
  if(file!=NULL){

    uint64_t rows, collumns= 0;
    uint64_t key;
    uint64_t payload;
    uint64_t max;

    fread(&rows, sizeof(uint64_t), 1, file);     //get number of rows
    fread(&collumns, sizeof(uint64_t), 1, file); //get number of collumns

    printf("rows: %ld\n",rows);
    printf("collumns: %ld\n",collumns);

    myrel->num_tuples = rows;                              //set number of tuples
    // myrel->num_tuples = 10;
    myrel->tuples = malloc(sizeof(uint64_t)*rows*collumns); //allocate space for tuples



    for(int i=2; i<rows+2; i++){

      fread(&payload, sizeof(uint64_t), 1, file);
      // printf("%ld\n",key);
      tuple_set_payload(&myrel->tuples[i-2], payload);
    }

    fseek(file, sizeof(uint64_t)*((rows*rel_num)+2),SEEK_SET);

    max=0;
    for(int i=0; i<rows; i++){
      fread(&key, sizeof(uint64_t), 1, file);
      tuple_set_key(&myrel->tuples[i], key);
      if((norm_bool==NORM_ON) && max<=key){
        max=key;
      }
    }

    myrel->max_key_value = max;
    myrel->is_normalized = FALSE;
    myrel->normalize_on = norm_bool;
    if(norm_bool==TRUE){
      normalize_keys(myrel);
    }
    // relation_print(myrel);

    fclose(file);
  }
}

void relation_init_data(relation* myrel, char* filename, int rel_num, int norm_bool){
  FILE* file = fopen(filename, "r");
  myrel->is_normalized = FALSE;
  myrel->normalize_on = NORM_OFF;
  if(file!=NULL){

    uint64_t rows, collumns= 0;
    uint64_t key;
    uint64_t payload;
    uint64_t max;
    fscanf(file,"%lu,%lu\n",&rows,&collumns);
    printf("rows: %lu, collumns: %lu\n",rows,collumns);


    myrel->num_tuples = rows;                              //set number of tuples
    // myrel->num_tuples = 10;
    // myrel->tuples = malloc(sizeof(uint64_t)*rows*collumns); //allocate space for tuples
    myrel->tuples=malloc(sizeof(tuple)*(myrel->num_tuples));
    max=0;
    for(int i=0; i<myrel->num_tuples; i++){

      // fread(&payload, sizeof(uint64_t), 1, file);
      // printf("%ld\n",key);
      fscanf(file,"%lu,%lu\n",&key,&payload);
      tuple_set_payload(&myrel->tuples[i], payload);
      tuple_set_key(&myrel->tuples[i], key);
      if((norm_bool==NORM_ON) && max<=key){
          max=key;
      }
    }

    myrel->max_key_value = max;
    myrel->is_normalized = FALSE;
    myrel->normalize_on = norm_bool;
    if(norm_bool==TRUE){
      normalize_keys(myrel);
    }
    // relation_print(myrel);

    fclose(file);
  }
}

/*
* initialize empty relation of a specific size (number of rows)
* ARGS:
*   -relation object pointer
*   -number of rows
*/
void relation_init_empty(relation ** rel, int rel_rows, int norm_bool) {
  *rel = malloc(sizeof(struct relation));
  (*rel)->num_tuples = rel_rows;                              //set number of tuples
  (*rel)->tuples = malloc(sizeof(struct tuple)*rel_rows);      //allocate space for tuples
  (*rel)->normalize_on = norm_bool;
  if((*rel)->normalize_on == NORM_ON){
    (*rel)->is_normalized = TRUE;
  }
  else{
    (*rel)->is_normalized = FALSE;
  }
}

void relation_set_max_key(relation *src, relation *dst){
  if(src->normalize_on){
    dst->max_key_value = src->max_key_value;
  }
  else{
    dst->max_key_value = 0;
  }
}

int relation_norm_alg(relation *myrel){
  return myrel->normalize_on;
}

void relation_destroy(relation* myrel){
  free(myrel->tuples);
}

tuple* relation_get_tuple(relation* myrel, int nth_tuple){
  if(nth_tuple>myrel->num_tuples-1){
    return NULL;
  }
  else{
    return &(myrel->tuples[nth_tuple]);
  }
}
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
*   -number of byte on which the histogram is to
*     be calculated (for leftmost byte of uint64_t - n=8)
* RETURNS: array with computed values
* AFTER: use free() to free dynamically allocated array
*/
int* relation_get_hist(relation *myrel, int range_start, int range_end, int nth_byte){
  int* hist = malloc(HIST_SIZE*sizeof(int));
  memset(hist, 0, HIST_SIZE*sizeof(int));


  int byte_val=0;
  tuple* mytup;
  for(int i=range_start; i<=range_end; i++){
    mytup = relation_get_tuple(myrel, i);
    byte_val = tuple_get_nth_byte(mytup, nth_byte);
    hist[byte_val]++;
  }

  return hist;
}

/*
* computes the starting positions of the sub buckets
* based on the histogram.
* ARGS: histogram array returned from relation_get_hist
* RETURNS: array with computed values
* AFTER: use free() to free dynamically allocated array
*/
int* get_psum_from_hist(int* hist){
  int* psum = malloc(sizeof(int)*HIST_SIZE);
  memset(psum, 0, HIST_SIZE*sizeof(int));

  int prev = 0;
  for(int i=1; i<HIST_SIZE; i++){ //starts from 1 because psum[0] is always 0
    psum[i] = prev+ hist[i-1];
    prev = psum[i];
  }


  return psum;
}

void print_psum(int* psum){

  int prev=-1;
  for(int i=0; i<HIST_SIZE; i++){
    if(psum[i]!=prev){
      printf("psum[%d]: %d\n",i, psum[i]);
    }
    prev=psum[i];
  }
}

void print_hist(int* hist){

  int prev=-1;
  for(int i=0; i<HIST_SIZE; i++){
    if(hist[i]!=prev){
      printf("hist[%d]: %d\n",i, hist[i]);
    }
    prev=hist[i];
  }
}

uint64_t get_max_pos_value(){
  uint64_t max_possible_val = 0;
  int size = sizeof(uint64_t);
  // uint64_t myint = 1;
  // for(int i=0; i<(size*8)-1; i++){
  //   max_possible_val = max_possible_val | myint;
  //   myint = myint<<1;
  // }

  uint64_t myint = 255;
  for(int i=0; i<8; i++){
    max_possible_val = max_possible_val | myint;
    myint = myint<<8;
  }
  return max_possible_val;
}


void normalize_keys(relation* myrel){
  if(myrel->is_normalized==TRUE) return;
  if(myrel->normalize_on==NORM_OFF) return;

  uint64_t max_possible_val = get_max_pos_value();
  // printf("max %lu\n", max_possible_val);
  uint64_t norm_value = max_possible_val/myrel->max_key_value;

  // printf("%ld\n", max_possible_val);
  // printf("%ld\n", norm_value);

  uint64_t new_val;
  // int error_flag=0;
  for(int i=0; i<myrel->num_tuples; i++){
    new_val = myrel->tuples[i].key*norm_value;

    // uint64_t verify;
    // verify = new_val/norm_value;
    // if(verify!= myrel->tuples[i].key){
    //   error_flag=1;
    //   printf("error!\n");
    //   break;
    // }

    tuple_set_key(&myrel->tuples[i], new_val);
  }
  // if(!error_flag){
  //   printf("verification ok!\n");
  // }
  myrel->is_normalized=TRUE;
}

void denormalize_keys(relation* myrel){
  if(myrel->is_normalized==FALSE) return;

  uint64_t max_possible_val = get_max_pos_value();
  uint64_t norm_value = max_possible_val/myrel->max_key_value;

  uint64_t old_val;

  for(int i=0; i<myrel->num_tuples; i++){

    old_val = myrel->tuples[i].key/norm_value;

    // myrel->tuples[i].key = myrel->tuples[i].key*norm_value;
    tuple_set_key(&myrel->tuples[i], old_val);
  }
  myrel->is_normalized=FALSE;
}


void relation_copy_tuple(relation* src, relation* dst, int src_index, int dst_index){
  tuple_copy(&src->tuples[src_index], &dst->tuples[dst_index]);
}

void relation_print(relation* myrel){
  // denormalize_keys(myrel);
  printf("rows: %ld\n",myrel->num_tuples);
  for(int i=0; i<myrel->num_tuples; i++){
    printf("row: %d", i);
    tuple_print(&myrel->tuples[i]);
  }
}

void relation_print_range(relation* myrel, int start, int end){
  if(myrel->normalize_on==NORM_ON){
    denormalize_keys(myrel);
  }
  printf("rows: %ld\n",myrel->num_tuples);
  for(int i=start; i<end; i++){
    printf("row: %d", i);
    tuple_print(&myrel->tuples[i]);
  }
  if(myrel->normalize_on==NORM_ON){
    normalize_keys(myrel);
  }
}

void bucket_print(relation * rel, int begin, int end, int rec){
  printf("BUCKET - byte: %d\n", rec);
  for(int pos=begin ; pos<=end ; pos++) {
    printf("row: %d %d", pos, tuple_get_nth_byte(&(rel->tuples[pos]), 8-rec+1));
    tuple_print(&rel->tuples[pos]);
  }
}
