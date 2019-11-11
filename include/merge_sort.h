/*************************************************************************
Header File Name		: merge_sort.h
Author        			:
Purpose			    		:
**************************************************************************/
#ifndef __MS__
#define __MS__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// #include <immintrin.h>
#include "vector.h"

void merge_sort(relation *rel, int64_t start, int64_t end);

void merge_sort_internal(relation *rel, int64_t start, int64_t end);

void sorting_network(vector *a, vector *b, vector *c, vector *d, vector *w, vector *x, vector *y, vector *z);

void merge(relation *rel, int64_t start1, int64_t end1, int64_t start2, int64_t end2);

void bitonic_merge_network(vector *v1, vector *v2);
#endif
