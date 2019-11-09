/*************************************************************************
Implementation File   		: vector.c
Author      		        	:
Purpose				           	:
**************************************************************************/
#include "./include/vector.h"


void set_vector(vector* v, int64_t* mem_addr){
  *v = _mm256_set_epi64x(*mem_addr,*(mem_addr+4),*(mem_addr+8),*(mem_addr+12));
}


void min_max_compare_vectors(vector v1, vector v2, vector* minv, vector* maxv){
  vector gr = _mm256_cmpgt_epi64 (v1, v2);
  *minv = _mm256_blendv_epi8(v1, v2, gr);
  *maxv = _mm256_blendv_epi8(v2, v1, gr);
}

void print_vector(vector* v){
  int64_t* a = malloc(sizeof(int64_t)*4);
  _mm256_storeu_si256( (vector *)&a[0], *v);
  printf("my vector is %lu,%lu,%lu,%lu\n",a[0],a[1],a[2],a[3]);
  free(a);
}

void store_vector(relation *rel, int start, vector* v){
  int64_t* a = malloc(sizeof(int64_t)*4); //TODO shuffle??
  _mm256_storeu_si256( (vector *)&a[0], *v);
  rel->values[start] = a[3];
  rel->values[start+4] = a[2];
  rel->values[start+8] = a[1];
  rel->values[start+12] = a[0];

  free(a);
}
