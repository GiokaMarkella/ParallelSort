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
    // load_vector_consecutive(&a, &rel->values[start]);
    // load_vector_consecutive(&b, &rel->values[start+4]);
    // load_vector_consecutive(&c, &rel->values[start+8]);
    // load_vector_consecutive(&d, &rel->values[start+12]);

    // bitonic_sort(&a,&b);
    merge(rel, start, start+4, start+4, start+8);
    merge(rel, start+8, start+12, start+12, start+16);
    merge(rel, start, start+8, start+8, start+16);

    for(int i=0; i<4*4; i+=4){
      printf("group %d:\n",i);
      for(int j=i; j<i+4; j++){
        printf("%lu\n",rel->values[j]);
      }
    }

  }
}

void merge(relation *rel, int start1, int end1, int start2, int end2){
  vector a;
  vector b;
  int64_t head1, head2;
  int start = start1;
  //output buffer
  int64_t* output = malloc(sizeof(int64_t)*(end2-start1));
  int64_t output_index = 0;

  //fetch 4
  load_vector_consecutive(&a, &rel->values[start1]);
  start1 +=4;
  if(start1<end1) head1 = rel->values[start1];

  //fetch 4
  load_vector_consecutive(&b, &rel->values[start2]);
  start2 += 4;
  if(start2<end2) head2 = rel->values[start2];


  int break_flag=0;
  //e.g. start = 0, end = 4 => start < end
  do{

    bitonic_merge_network(&a,&b); //merge

    store_vector_consecutive(output+output_index, &a); //emit smaller
    output_index += 4; //move output index

    //pick next group of 4
    if(start1<end1 && start2<end2){

      if(head1 < head2){
        load_vector_consecutive(&a, &rel->values[start1]);
        start1 +=4;
        if(!start1<end1){
          break_flag=1;
          break;
        }
        head1 = rel->values[start1];
      }
      else{
        load_vector_consecutive(&a, &rel->values[start2]);
        start2 +=4;
        if(!start2<end2){
          break_flag=1;
          break;
        }
        head2 = rel->values[start2];
      }

    }
    else{
      store_vector_consecutive(output+output_index, &b); //emit b
      output_index += 4; //move output index
    }
  }while(start1<end1 && start2<end2);


  if(break_flag){
    bitonic_merge_network(&a,&b); //merge
    store_vector_consecutive(&output[output_index], &a); //emit a
    output_index += 4; //move output index
    store_vector_consecutive(&output[output_index], &b); //emit b
    output_index += 4; //move output index

    if(!start1<end1){ //eof1 -> emit in2
      for(int i=start2; i<end2; i++){
        output[output_index] = rel->values[i];
        output_index++;
      }
    }
    else{ //eof2 -> emit in1
      for(int i=start1; i<end1; i++){
        output[output_index] = rel->values[i];
        output_index++;
      }
    }
  }


  for(int i=0; i<end2-start; i++){
    rel->values[start+i] = output[i];
  }
  // for(int i=0; i<output_index; i++){
  //   printf("%d: %lu \n",i,output[i]);
  // }
  free(output);
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
