/*************************************************************************
Implementation File   		: relation.c
Author      		        	:
Purpose				           	:
**************************************************************************/
#include "./include/relation.h"


void relation_init_data(relation* myrel, char* filename, int rel_num){
  FILE* file = fopen(filename, "r");

  if(file!=NULL){

    uint64_t rows, collumns= 0;
    uint64_t key;
    uint64_t payload;
    uint64_t max;
    fscanf(file,"%lu,%lu\n",&rows,&collumns);
    printf("rows: %lu, collumns: %lu\n",rows,collumns);

    // myrel->num_values = rows;                              //set number of tuples
    myrel->num_values = 16;

    myrel->values=malloc(sizeof(int64_t)*(myrel->num_values));
    max=0;
    for(int i=0; i<myrel->num_values; i++){

      fscanf(file,"%lu,%lu\n",&key,&payload);
      myrel->values[i] = key; //

    }

    myrel->max_key_value = max;

    fclose(file);
  }
}


void relation_destroy(relation* myrel){
  free(myrel->values);
}


void relation_print(relation* myrel){
  // denormalize_keys(myrel);
  printf("rows: %ld\n",myrel->num_values);
  for(int i=0; i<myrel->num_values; i++){
    printf("row: %d", i);
    // tuple_print(&myrel->tuples[i]);
  }
}

void relation_print_range(relation* myrel, int start, int end){

  printf("rows: %ld\n",myrel->num_values);
  for(int i=start; i<end; i++){
    printf("row: %d", i);
    // tuple_print(&myrel->tuples[i]);
  }

}
