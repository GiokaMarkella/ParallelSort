/*************************************************************************
Header File Name		: tuple.h
Author        			:
Purpose			    		:
**************************************************************************/
#ifndef __TUPLE__
#define __TUPLE__

#include <stdint.h>
#include <stdio.h>

typedef struct tuple{
  uint64_t key; //join key
  uint64_t payload; //row id

  // uint64_t rowId;
}tuple;

void tuple_init(tuple *mytuple, uint64_t key, uint64_t payload);

void tuple_set_key(tuple* mytuple, uint64_t key);

void tuple_set_payload(tuple* mytuple, uint64_t payload);

uint64_t tuple_get_key(tuple* mytuple);

uint64_t tuple_get_payload(tuple* mytuple);

void tuple_print(tuple* mytuple);

void tuple_copy(tuple* src, tuple* dst);

/*
* returns in int the value of the n'th byte
* the tuple's payload
* bytes are counted like: 8-7-6-5-4-3-2-1
* so to get the left-most byte n=8
*/
int tuple_get_nth_byte(tuple* mytuple, int n);
#endif
