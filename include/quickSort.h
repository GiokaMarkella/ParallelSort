/*************************************************************************
Implementation File   		: quickSort.h
Author      		        :
Purpose				        :
**************************************************************************/

#ifndef __QUICKSORT__
#define __QUICKSORT__

    #include <stdio.h>
    #include "relation.h"

    void swap(tuple *arr, int a, int b);

    int partition (tuple *arr, int low, int high);

    void quickSort(tuple *arr, int low, int high);

#endif
