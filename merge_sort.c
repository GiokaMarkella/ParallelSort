/*************************************************************************
Implementation File   		: merge_sort.c
Author      		        	:
Purpose				           	:
**************************************************************************/
#include "./include/merge_sort.h"

void merge_sort(relation *rel, int start, int end){
  if(start<end && (end-start>4*4)){
    int middle = start + (end-start)/2;
    merge_sort(rel,start,middle);
    merge_sort(rel,middle+1,end);
  }
  else if(end-start<=4*4){

    //sorting network
    vector a,b,c,d,e,f,g,h,i,j,w,x,y,z;
    set_vector(&a, &rel->values[start]);
    set_vector(&b, &rel->values[start+1]);
    set_vector(&c, &rel->values[start+2]);
    set_vector(&d, &rel->values[start+3]);

    min_max_compare_vectors(a,b, &e, &f);
    min_max_compare_vectors(c,d, &g, &h);


    min_max_compare_vectors(e,g,&w,&i);
    min_max_compare_vectors(f,h,&j,&z);

    min_max_compare_vectors(j,i,&x,&y);
    //--sorting network

    store_vector(rel, start, &w);
    store_vector(rel, start+1, &x);
    store_vector(rel, start+2, &y);
    store_vector(rel, start+3, &z);

    //bitonic test
    load_vector_consecutive(&a, &rel->values[start]);
    load_vector_consecutive(&b, &rel->values[start+4]);
    load_vector_consecutive(&c, &rel->values[start+8]);
    load_vector_consecutive(&d, &rel->values[start+12]);

    bitonic_sort(&a,&b);

    for(int i=0; i<4*4; i+=4){
      printf("group %d:\n",i);
      for(int j=i; j<i+4; j++){
        printf("%lu\n",rel->values[j]);
      }
    }

  }
}

void bitonic_merge_network(vector *v1, vector *v2){
  // printf("==========\n");
  // print_vector(v1);
  // printf("==========\n");
  // print_vector(v2);
  *v1 = mirror_vector(v1);

  vector min,max;

  min_max_compare_vectors(*v1,*v2,&min,&max);
  *v1 = min;
  *v2 = max;

  permutate_half_vector(v1, v2);

  min_max_compare_vectors(*v1,*v2,&min,&max);
  *v1 = min;
  *v2 = max;

  permutate_quarter_vector(v1, v2);

  min_max_compare_vectors(*v1,*v2,&min,&max);
  *v1 = min;
  *v2 = max;

  permutate_quarter_vector2(v1,v2);
}
