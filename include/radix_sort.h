/*************************************************************************
Header File Name		: radix_sort.h
Author        			:
Purpose			        :
**************************************************************************/
#ifndef __RADIX_SORT__
#define __RADIX_SORT__

#include <stdio.h>
#include "relation.h"
#include "bucket.h"
#include "quickSort.h"

extern relation * left;
extern relation * right;



/* Main method that recursively implements radix sorting of each relation
    ARGS:
        rec: recursion number
        begin: firts row (element) of bucket
        end: last row (element) of bucket
    RETURNS:
        final sorted relation
*/
relation * RadixSort(int rec, bucket_info this_bucket);

/*
    Method that initializes arguments and appropriate
    structs for radix sorting. Calls Radix Sort main method.
    ARGS:
        rel: relation that needs to be radix sorted
*/
void RadixSort_Init(relation * rel);

/*
    Copy results of psum in a new relation
    ARGS:
        rec: recursion number
        psum: array of position values
        hist: histogram
        rel: relation for expanfding results of psum
        copy_rel: relation in which results will be stored
        begin: beginning of relation (it might be a bucket)
        end: end of relation
*/
void expand_psum_to_rel(int rec, int * psum, int* hist, relation * rel, bucket_info *bucket, relation * copy_rel, int copy_bool);
/*
    Copy sorted bucket from one relation to another
    in equivalent positions.
*/
void copy_bucket(relation * from_rel, bucket_info* this_bucket, relation * to_rel);
// void copy_bucket(relation * from_rel, int bucket_start, int bucket_end, relation ** to_rel);

#endif
