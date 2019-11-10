/*************************************************************************
Header File Name		: vector.h
Author        			:
Purpose			    		:
**************************************************************************/
#ifndef __VECTOR__
#define __VECTOR__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <immintrin.h>
#include "relation.h"

#define SHUF_MIRROR 27
#define SHUF_COPY 228

typedef __m256i vector;

void set_vector(vector* v, int64_t* mem_addr);

void load_vector_consecutive(vector* v, int64_t* mem_addr);

void min_max_compare_vectors(vector v1, vector v2, vector* minv, vector* maxv);

void print_vector(vector *v);

void store_vector(relation *rel, int start, vector* v);

void store_vector_consecutive(int64_t* mem_addr, vector* v);

vector mirror_vector(vector *v);

void permutate_half_vector(vector *v1, vector *v2);

void permutate_quarter_vector(vector *v1, vector *v2);

void permutate_quarter_vector2(vector *v1, vector *v2);
#endif
