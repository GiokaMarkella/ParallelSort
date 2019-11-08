/*************************************************************************
Header File Name		: merge.h
Author        			:
Purpose			    	:
**************************************************************************/
#ifndef __MERGE__
#define __MERGE__

#include <stdio.h>
#include "relation.h"
#include "resultsList.h"

/*
    Main method that calls radix sorting for two relation and after that it merges
    them my joining them in a parallel way. The result is saved in a list of
    buffers of size 1MB each.
    ARGS:
        Relations that need to be joined
*/
// result* SortMergeJoin(relation *rel1, relation *rel2);                   // right one

// void SortMergeJoin(relation *rel1, relation *rel2);

/*
    Method that implements parallel join of two sorted
    relations based on specific key. Takes care of cartesian join.
    Pushes result in a result struct list.
    ARGS:
        Relations that need to be sorted.
        Elements int -> used to return number of elements
*/
resultsList * Merge(relation * rel1, relation *rel2, uint64_t *elements_num);
// void Merge(relation * rel1, relation * rel2);

/*
    Method used for joining tuples which keys appear in duplicates in either or
    both of the given relations.
    ARGS:
        rel*: relation
        key: key for join
        pos_rel*: first tuple (position) of corresponding relation that has the specific key
*/
uint64_t  CartesianJoin(relation *rel1, relation * rel2, uint64_t key, int *pos_rel1, int *pos_rel2);

/*
    Calculates the range in which a specific key appears in the provided relation
    ARGS:
        rel: relations
        start: first tuple (position) of corresponding relation that has the specific key
        key: key for join
    RETURNS:
        End of range
*/
int find_key_range_end(relation *rel, int start, uint64_t key, uint64_t array_end);

#endif
