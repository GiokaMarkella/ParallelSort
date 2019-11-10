/*************************************************************************
Implementation File   		: vector.c
Author      		        	:
Purpose				           	:
**************************************************************************/
#include "./include/vector.h"


void set_vector(vector* v, int64_t* mem_addr){
  *v = _mm256_set_epi64x(*mem_addr,*(mem_addr+4),*(mem_addr+8),*(mem_addr+12));
}

void load_vector_consecutive(vector* v, int64_t* mem_addr){
  *v = _mm256_set_epi64x(*mem_addr,*(mem_addr+1),*(mem_addr+2),*(mem_addr+3));
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

void store_vector_consecutive(int64_t* mem_addr, vector* v){
  // int64_t* a = malloc(sizeof(int64_t)*4); //TODO shuffle??
  // _mm256_storeu_si256( (vector *)&a[0], *v);
  _mm256_storeu_si256( (vector *)&mem_addr[0], *v);
  //
  // rel->values[start] = a[3];
  // rel->values[start+4] = a[2];
  // rel->values[start+8] = a[1];
  // rel->values[start+12] = a[0];

  // free(a);
}

/*
* returns the shuffled mirror of vector v
*
* v: a1,a2,a3,a4 => new_v: a4,a3,a2,a1
*/
vector mirror_vector(vector *v){
  vector new_v;
  new_v = _mm256_permute4x64_epi64 (*v, SHUF_MIRROR);
  return new_v;
}

/*
* switches lower half of v1 vector
* with the higher half of v2 vector
*
* v1: a1,a2,(a3,a4)     a1,a2,b1,b2
*          /         =>
* v2: (b1,b2),b3,b4     a3,a4,b3,b4
*/
void permutate_half_vector(vector *v1, vector *v2){
  // vector tmp = _mm256_set_epi64x(0,0,0,0);
  // vector tmp2 = _mm256_set_epi64x(0,0,0,0);
  vector tmp;
  vector tmp2;;
  tmp = _mm256_permute2x128_si256 (*v2, *v1, 19); //->1,3
  tmp2 = _mm256_permute2x128_si256 (*v2, *v1, 2); //->0,2
  *v2 = tmp;
  *v1 = tmp2;
}

/*
* switches elements from v1 and v2 like this:
*
* v1: a1,(a2),a3,(a4)     a1,b1,a3,b3
*        /       /     =>
* v2: (b1),b2,(b3),b4     a2,b2,a4,b4
*/
void permutate_quarter_vector(vector *v1, vector *v2){
  vector tmp3;
  vector tmp4;

  permutate_half_vector(v1,v2);
  tmp3 = _mm256_permute4x64_epi64 (*v2, 141); //shuffle elements
  tmp4 = _mm256_permute4x64_epi64 (*v1, 141); //shuffle elements

  //previous --
  // tmp = _mm256_permute2x128_si256 (*v2, *v1, 19);
  // tmp3 = _mm256_permute4x64_epi64 (tmp, 141); //shuffle elements
  //
  // tmp2 = _mm256_permute2x128_si256 (*v2, *v1, 2);
  // tmp4 = _mm256_permute4x64_epi64 (tmp2, 141); //shuffle elements
  permutate_half_vector(&tmp4,&tmp3);
  *v1=tmp3;
  *v2=tmp4;
  // *v1=tmp5;
  // *v2=tmp6;

}

/*
* switches elements from v1 and v2 like:
*
* v1: a1,a2,a3,a4     a1,b1,a2,b2
*                  =>
* v2: b1,b2,b3,b4     a3,b3,a4,b4
*/
void permutate_quarter_vector2(vector *v1, vector *v2){
  vector tmp3;
  vector tmp4;

  permutate_half_vector(v1,v2);
  tmp3 = _mm256_permute4x64_epi64 (*v2, 216); //shuffle elements

  tmp4 = _mm256_permute4x64_epi64 (*v1, 216); //shuffle elements

  *v1=tmp4;
  *v2=tmp3;
}
