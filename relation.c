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
*/
// void relation_init(relation* myrel, char* filename, int rel_num, int norm_bool){
//   FILE* file = fopen(filename, "r");
//   myrel->is_normalized = FALSE;
//   myrel->normalize_on = NORM_OFF;
//   if(file!=NULL){
//
//     uint64_t rows, collumns= 0;
//     uint64_t key;
//     uint64_t payload;
//     uint64_t max;
//
//     fread(&rows, sizeof(uint64_t), 1, file);     //get number of rows
//     fread(&collumns, sizeof(uint64_t), 1, file); //get number of collumns
//
//     printf("rows: %ld\n",rows);
//     printf("collumns: %ld\n",collumns);
//
//     myrel->num_tuples = rows;                              //set number of tuples
//     // myrel->num_tuples = 10;
//     myrel->tuples = malloc(sizeof(uint64_t)*rows*collumns); //allocate space for tuples
//
//
//
//     for(int i=2; i<rows+2; i++){
//
//       fread(&payload, sizeof(uint64_t), 1, file);
//       // printf("%ld\n",key);
//       tuple_set_payload(&myrel->tuples[i-2], payload);
//     }
//
//     fseek(file, sizeof(uint64_t)*((rows*rel_num)+2),SEEK_SET);
//
//     max=0;
//     for(int i=0; i<rows; i++){
//       fread(&key, sizeof(uint64_t), 1, file);
//       tuple_set_key(&myrel->tuples[i], key);
//       if((norm_bool==NORM_ON) && max<=key){
//         max=key;
//       }
//     }
//
//     myrel->max_key_value = max;
//     myrel->is_normalized = FALSE;
//     myrel->normalize_on = norm_bool;
//     if(norm_bool==TRUE){
//       normalize_keys(myrel);
//     }
//     // relation_print(myrel);
//
//     fclose(file);
//   }
// }

void relation_init_data(relation* myrel, char* filename, int rel_num){
  FILE* file = fopen(filename, "r");

  if(file!=NULL){

    uint64_t rows, collumns= 0;
    uint64_t key;
    uint64_t payload;
    uint64_t max;
    fscanf(file,"%lu,%lu\n",&rows,&collumns);
    printf("rows: %lu, collumns: %lu\n",rows,collumns);

    myrel->num_values = rows;                              //set number of tuples
    // myrel->values = 10;

    myrel->values=malloc(sizeof(int64_t)*(myrel->num_values));
    max=0;
    for(int i=0; i<myrel->num_values; i++){

      fscanf(file,"%lu,%lu\n",&key,&payload);
      myrel->values[i] = value; //

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

  printf("rows: %ld\n",myrel->num_values;
  for(int i=start; i<end; i++){
    printf("row: %d", i);
    // tuple_print(&myrel->tuples[i]);
  }

}
