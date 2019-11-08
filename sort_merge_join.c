/*************************************************************************
Implementation File   		: sort_merge_join.c
Author      		        :
Purpose				        :
**************************************************************************/

#include <stdio.h>
#include "./include/sort_merge_join.h"
#include "./include/resultsList.h"
#include "./include/relation.h"
#include "./include/radix_sort.h"
#include "./include/merge.h"

resultsList* SortMergeJoin(relation *r1, relation *r2) {
    // Radix sort on relation 1 and 2
    // relation_print(r1);
    // relation_print(r2);
    RadixSort_Init(r1);
    RadixSort_Init(r2);
    printf("sort ended!\n");
    // relation_print(r1);
    // relation_print(r2);
    // Parallel join of sorted relations
    uint64_t list_size;
    // relation_print(r1);
    // relation_print(r2);

    resultsList * result = Merge(r1, r2, &list_size);

    printf("number of elements: %lu\n",list_size);
    // printList(result);
    return result;
}
