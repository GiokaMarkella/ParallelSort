/*************************************************************************
Implementation File   		: merge_sort.c
Author      		        	:
Purpose				           	:
**************************************************************************/
#include "./include/merge_sort.h"


//TODO fix for smaller arrays
// and arrays that arent even (+check edge cases)
void merge_sort(relation *rel, int start, int end){
  if(start<end && (end-start>4*4)){
    int middle = start + (end-start)/2;
    // int middle = start + (end-start)/2;
    // printf("start %d, middle %d, end %d\n", start,middle,end);
    merge_sort(rel,start,middle);
    merge_sort(rel,middle,end);
    merge(rel, start,middle,middle,end);
  }
  else if(end-start<=4*4){
    // printf("start,end : %d %d\n", start,end);
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

    // for(int i=start; i<start+4*4; i+=4){
    //   printf("group %d:\n",i);
    //   for(int j=i; j<i+4; j++){
    //     printf("%lu\n",rel->values[j]);
    //   }
    // }

    merge(rel, start, start+4, start+4, start+8);
    merge(rel, start+8, start+12, start+12, start+16);
    merge(rel, start, start+8, start+8, start+16);

  }
}

typedef struct in{
  int start;
  int end;
  int64_t head;
}in;

void in_init(in *in1, int start, int end){
  in1->start = start;
  in1->end = end;
}

int in_eof(in *in1){
  return (!(in1->start<in1->end));
}

int fetch_next4(vector  *v, relation *rel, in* in1){
  load_vector_consecutive(v, &rel->values[in1->start]);

  // printf("fetch @ %d:\n",in1->start);
  // print_vector(v);

  in1->start +=4;
  if(in1->start<in1->end){
    in1->head = rel->values[in1->start];
    return 0;
  }
  return 1;
}

typedef struct output_buffer{
  int64_t *values;
  int64_t index;
  int64_t size;
}output_buffer;

void output_buffer_init(output_buffer *mybuff, int64_t size){
  mybuff->size=size;
  mybuff->values = malloc(sizeof(int64_t)*size);
  mybuff->index = 0;
}

void output_buffer_destroy(output_buffer *mybuff){
  if(mybuff->values!= NULL) free(mybuff->values);
}

void output_buffer_print(output_buffer *mybuff){
  printf("buffer:\n");
  for(int64_t i=0; i<mybuff->index; i++){
    printf("%ld, %lu\n",i, mybuff->values[i]);
  }
}

void output_buffer_emit_vector(output_buffer *mybuff, vector *v){
  store_vector_consecutive(&mybuff->values[mybuff->index], v);
  mybuff->index += 4; //move output index
  output_buffer_print(mybuff);
}

void output_buffer_emit_value(output_buffer *mybuff, int64_t value){
  mybuff->values[mybuff->index] = value;
  mybuff->index++;
  output_buffer_print(mybuff);
}


void merge(relation *rel, int start1, int end1, int start2, int end2){
  vector a;
  vector b;
  in in1,in2;
  int ret_val=0;

  output_buffer out;
  output_buffer_init(&out, (end2-start1));

  int start = start1;

  //e.g. start = 0, end = 4 => start < end
  in_init(&in1, start1, end1);
  in_init(&in2, start2, end2);

  //fetch 4
  ret_val = fetch_next4(&a, rel, &in1);

  //fetch 4
  ret_val += fetch_next4(&b, rel, &in2);

  int break_flag=0;

  do{
    bitonic_merge_network(&a,&b); //merge
    output_buffer_emit_vector(&out, &a);

    //pick next group of 4
    if(!ret_val){

      if(in1.head < in2.head){
        ret_val = fetch_next4(&a, rel, &in1);
        if(ret_val){
          break_flag=1;
          break;
        }
      }
      else{
        ret_val = fetch_next4(&a, rel, &in2);
        if(ret_val){
          break_flag=1;
          break;
        }
      }

    }
    else{
      //-------does it actually go in here?
      output_buffer_emit_vector(&out, &b);
      break;
    }
  }while(1);

  if(break_flag){ //one of the merging blocks ended

    bitonic_merge_network(&a,&b); //merge last fetched couple
    output_buffer_emit_vector(&out, &a);

    if(in_eof(&in1)){ //eof1 -> emit in2
      do{
        ret_val = fetch_next4(&a, rel, &in2);
        bitonic_merge_network(&a,&b); //merge
        output_buffer_emit_vector(&out, &a);
      }while(!ret_val);

      output_buffer_emit_vector(&out, &b);
    }
    else{ //eof2 -> emit in1
      do{
        ret_val = fetch_next4(&a, rel, &in1);
        bitonic_merge_network(&a,&b); //merge
        output_buffer_emit_vector(&out, &a);
      }while(!ret_val);
      output_buffer_emit_vector(&out, &b);

    }
  }


  for(int i=0; i<in2.end-start; i++){
    rel->values[start+i] = out.values[i];
  }
  // relation_print_range(rel,start,in2.end);
  output_buffer_destroy(&out);
}

void bitonic_merge_network(vector *v1, vector *v2){

  // print_vector(v1);
  // printf("==========\n");
  // print_vector(v2);
  // printf("==========\n");

  // *v1 = mirror_vector(v1);
  *v2 = mirror_vector(v2);

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

  // print_vector(v1);
  // printf("==========\n");
  // print_vector(v2);
  // printf("==========\n");

}
