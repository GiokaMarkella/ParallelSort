/*************************************************************************
Implementation File   		: tuple.c
Author      		        	:
Purpose				           	:
**************************************************************************/
#include "./include/tuple.h"

void tuple_init(tuple *mytuple, uint64_t key, uint64_t payload){
  mytuple->key = key;
  mytuple->payload = payload;
}

void tuple_set_key(tuple* mytuple, uint64_t key){
  mytuple->key = key;
}

void tuple_set_payload(tuple* mytuple, uint64_t payload){
  mytuple->payload = payload;
}

uint64_t tuple_get_payload(tuple* mytuple){
  return mytuple->payload;
}

uint64_t tuple_get_key(tuple* mytuple){
  return mytuple->key;
}

void tuple_print(tuple* mytuple){
  printf("key:%lu, payload:%lu\n",mytuple->key, mytuple->payload);
}

void tuple_copy(tuple* src, tuple* dst){
  uint64_t key = tuple_get_key(src);
  uint64_t payload = tuple_get_payload(src);
  tuple_set_key(dst, key);
  tuple_set_payload(dst, payload);
}

/*
* returns in int the value of the n'th byte
* the tuple's key
* bytes are counted like: 8-7-6-5-4-3-2-1
* so to get the left-most byte n=8
*/
int tuple_get_nth_byte(tuple* mytuple, int n){
  if(n>8 || n<1) return 0;
  int shift_num = (n-1)*8;

  // uint64_t num = mytuple->payload;
  uint64_t num = mytuple->key;

  uint64_t mask = 255;
  if(n!=1){ //not needed when n==1
    num = num>>shift_num;
  }


  num = num&mask;
  return num;
}
