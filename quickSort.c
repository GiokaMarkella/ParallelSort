/*************************************************************************
Implementation File   		: quickSort.c
Author      		        :
Purpose				        :
**************************************************************************/

#include "./include/quickSort.h"

// A utility function to swap two elements
void swap(tuple *arr, int a, int b)
{
    tuple temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
   array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition (tuple *arr, int low, int high)
{
    uint64_t pivot = arr[high].key;    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j].key < pivot)
        {
            i++;    // increment index of smaller element
            swap(arr, i, j);
        }
    }
    swap(arr, i+1, high);
    return (i + 1);
}


void insertion_sort(tuple *arr, int start, int end){
  tuple index;
  int j;
  for(int i=start+1; i<=end; i++){
    index.key = arr[i].key;
    index.payload = arr[i].payload;

    for(j=i-1; (j>=start && arr[j].key>index.key); j--){
      arr[j+1].key = arr[j].key;
      arr[j+1].payload = arr[j].payload;

    }
    arr[j+1].key = index.key;
    arr[j+1].payload = index.payload;

  }

}


/* The main function that implements QuickSort
 arr --> Array to be sorted,
 low  --> Starting index,
 high  --> Ending index */
void quickSort(tuple *arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);    //pi is partitioning index

        // Separately sort elements before
        // partition and after partition
        if((pi-1)-low<=10){
          // quickSort(arr, low, pi - 1);

          insertion_sort(arr, low, pi-1);
        }
        else{
          quickSort(arr, low, pi - 1);
        }
        if(high-(pi+1)<=10){
          // quickSort(arr, pi + 1, high);
          insertion_sort(arr, pi+1, high);
        }
        else{
          quickSort(arr, pi + 1, high);
        }
    }
}
