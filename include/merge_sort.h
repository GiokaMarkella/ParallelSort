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

void merge_sort(relation *rel, int start, int end);

void bitonic_sort(vector *v1, vector *v2);
#endif
