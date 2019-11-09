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

#define TRUE 1
#define FALSE 0

typedef struct relation{
  int64_t *values;
  uint64_t num_values;
  int max_key_value;
}relation;

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
// void relation_init(relation* myrel, char* filename, int rel_num, int norm_bool);

void relation_init_data(relation* myrel, char* filename, int rel_num, int norm_bool);

void relation_print(relation* myrel);

void relation_print_range(relation* myrel, int start, int end);

void relation_destroy(relation* myrel);

#endif
