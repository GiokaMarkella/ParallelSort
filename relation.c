/*************************************************************************
Implementation File   		: relation.c
Author      		        	:
Purpose				           	:
**************************************************************************/
#include "./include/relation.h"

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

void relation_init_data(relation* myrel, char* filename, int rel_num){
  FILE* file = fopen(filename, "r");

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
