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

void test_func(vector *v){
  vector new_v = _mm256_set_epi64x(0,0,0,0);
  int control = 82;
  // new_v = _mm256_permute4x64_epi64 (*v, 82);
  new_v = _mm256_permute4x64_epi64 (*v, SHUF_COPY);
  printf("========\n");
  print_vector(v);
  printf("========\n");

  print_vector(&new_v);
  printf("========\n");

}

void test2_func(vector *v1, vector *v2){
  vector tmp = _mm256_set_epi64x(0,0,0,0);
  vector tmp2 = _mm256_set_epi64x(0,0,0,0);

  tmp = _mm256_permute2x128_si256 (*v2, *v1, 19);
  // tmp = _mm256_permute4x64_epi64 (*v1, 224);

  tmp2 = _mm256_permute2x128_si256 (*v2, *v1, 2);

  printf("========\n");
  print_vector(v1);
  printf("========\n");
  print_vector(v2);
  printf("========\n");

  print_vector(&tmp);
  printf("========\n");
  print_vector(&tmp2);
  printf("========\n");
}
