/*************************************************************************
Implementation File   		: merge.c
Author      		        :
Purpose				        :
**************************************************************************/

#include "./include/merge.h"

// Root of result list and a helping pointer
// to identify current working result node
resultsList *root = NULL, *current = NULL;

resultsList * Merge(relation * rel1, relation *rel2, uint64_t *num_of_elements) {

    *num_of_elements=0;
    // printf("1 elements: %lu\n",*num_of_elements);
    root = malloc(sizeof(resultsList));
    nodeInit(root);
    current = root;

    // Loop for traversing through both relations
    // while there are still elements that need to be compared

    int pos_rel1 = 0;
    int pos_rel2 = 0;
    uint64_t key_rel1;
    uint64_t key_rel2;
    uint64_t payload_rel1;
    uint64_t payload_rel2;
    int end_rel1 = rel1->num_tuples-1;
    int end_rel2 = rel2->num_tuples-1;

    // printf("start rel 1: %d , end rel 1: %d\n", pos_rel1, end_rel1);
    // printf("start rel 2: %d , end rel 2: %d\n", pos_rel2, end_rel2);
    while(pos_rel1 <= end_rel1 && pos_rel2 <= end_rel2) {

        key_rel1 = rel1->tuples[pos_rel1].key;
        key_rel2 = rel2->tuples[pos_rel2].key;
        payload_rel1 = rel1->tuples[pos_rel1].payload;
        payload_rel2 = rel2->tuples[pos_rel2].payload;
        // printf("comapring: %ld and %ld\n", key_rel1, key_rel2);

        // current key in relation 1 is smaller than the corresponding one in relation 2
        // move to next tuple in relation 1
        if(key_rel1 < key_rel2){
            // if different keys and last ones in each relation
            if(pos_rel1 == end_rel1 && pos_rel2 == end_rel1) {
                pos_rel1++;
                pos_rel2++;
            }
            else {
                pos_rel1++;
            }
        }
        // current key in relation 2 is smaller than the corresponding one in relation 1
        // move to next tuple in relation 2
        else if(key_rel1 > key_rel2) {
            // if different keys and last ones in each relation
            if(pos_rel1 == end_rel1 && pos_rel2 == end_rel1) {
                pos_rel1++;
                pos_rel2++;
            }
            else {
                pos_rel2++;
            }
        }
        // same keys -> join
        else {
            // end of both relations -> single same key in each one of them
            if(pos_rel1 == end_rel1 && pos_rel2 == rel2->num_tuples){
                //  TODO push in result list
                // printf("key1: %ld , key2: %ld\n", key_rel1, key_rel2);
                // *num_of_elements++;
                // printf("2 elements: %lu\n",*num_of_elements);
                // printf("1\n");
                addResults(&current, payload_rel1, payload_rel2);
                pos_rel1++;
                pos_rel2++;
            }
            // not in the end of the relation 1 but in the end of relation 2
            else if(pos_rel1 != end_rel1 && pos_rel2 == end_rel2){
                // if single key in rel1
                if(rel1->tuples[pos_rel1+1].key != key_rel1) {
                    //  TODO push in result list
                    // printf("key1: %ld , key2: %ld\n", key_rel1, key_rel2);
                    // *num_of_elements++;
                    // printf("2 elements: %lu\n",*num_of_elements);
                    // printf("2\n");
                    addResults(&current, payload_rel1, payload_rel2);
                }
                // dublicate keys in relation 1
                else{
                    // printf("3\n");
                    // *num_of_elements += CartesianJoin(rel1, rel2, key_rel1, &pos_rel1, &pos_rel2);
                    CartesianJoin(rel1, rel2, key_rel1, &pos_rel1, &pos_rel2);

                    // printf("3 elements: %lu\n",*num_of_elements);
                }

                // no need to check further on relation 1
                pos_rel1 = end_rel1+1;
                pos_rel2 = end_rel2+1;
            }
            // not in the end of the relation 2 but in the end of relation 1
            else if(pos_rel2 != end_rel2 && pos_rel1 == end_rel1){
                // if single key in rel2
                if(rel2->tuples[pos_rel2+1].key != key_rel2) {
                    //  TODO push in result list
                    // printf("key1: %ld , key2: %ld\n", key_rel1, key_rel2);
                    // *num_of_elements++;
                    // printf("4 elements: %lu\n",*num_of_elements);

                    // printf("4\n");
                    addResults(&current, payload_rel1, payload_rel2);
                }
                // dublicate keys in relation 2
                else{
                    // printf("5\n");
                    // *num_of_elements += CartesianJoin(rel1, rel2, key_rel1, &pos_rel1, &pos_rel2);
                    CartesianJoin(rel1, rel2, key_rel1, &pos_rel1, &pos_rel2);

                    // printf("5 elements: %lu\n",*num_of_elements);
                }

                // no need to check further on relation 2
                pos_rel2 = end_rel2+1;
                pos_rel1 = end_rel1+1;
            }
            // not in the end of neither relations
            else {
                // single same keys in both relations
                // printf("pos_rel %d,%d end_rel1 %d, key1: %ld , key2: %ld\n",pos_rel1,pos_rel1,end_rel1, key_rel1, key_rel2);

                if(((pos_rel1+1<=end_rel1)&&(pos_rel2+1<=end_rel2)) && rel1->tuples[pos_rel1+1].key!=key_rel1 && rel2->tuples[pos_rel2+1].key != key_rel2) {
                    // TODO push in result list
                    // printf("--pos_rel %d,%d end_rel1 %d, key1: %ld , key2: %ld\n",pos_rel1,pos_rel1,end_rel1, key_rel1, key_rel2);
                    // *num_of_elements++;
                    // printf("6 elements: %lu\n",*num_of_elements);
                    // printf("6\n");
                    addResults(&current, payload_rel1, payload_rel2);
                    pos_rel1++;
                    pos_rel2++;
                }
                // dublicate keys in either one of the relations
                else if((pos_rel1==end_rel1 && pos_rel2!=end_rel2 && rel2->tuples[pos_rel2+1].key != key_rel2)||(pos_rel1!=end_rel1 && pos_rel2==end_rel2 &&rel1->tuples[pos_rel1+1].key!=key_rel1)){
                  // printf("--pos_rel %d,%d end_rel1 %d, key1: %ld , key2: %ld\n",pos_rel1,pos_rel1,end_rel1, key_rel1, key_rel2);
                  // *num_of_elements++;
                  // printf("6\n");
                  addResults(&current, payload_rel1, payload_rel2);
                  pos_rel1++;
                  pos_rel2++;
                }
                else if((pos_rel1==end_rel1)&&(pos_rel2==end_rel2)){
                  // printf("--pos_rel %d,%d end_rel1 %d, key1: %ld , key2: %ld\n",pos_rel1,pos_rel1,end_rel1, key_rel1, key_rel2);
                  // *num_of_elements++;
                  // printf("7 elements: %lu\n",*num_of_elements);
                  // printf("6\n");
                  addResults(&current, payload_rel1, payload_rel2);
                  pos_rel1++;
                  pos_rel2++;
                }
                else{
                  // printf("7\n");
                  // *num_of_elements += CartesianJoin(rel1, rel2, key_rel1, &pos_rel1, &pos_rel2);
                  CartesianJoin(rel1, rel2, key_rel1, &pos_rel1, &pos_rel2);

                  // printf("8 elements: %lu\n",*num_of_elements);
                  // continue; //????
                }
            }
        }
    }
    // printList(root);
    // printf("9 elements: %lu, %lu\n",*num_of_elements, MAX_BUFFER_ELEMEMENTS);
    return root;
}

uint64_t CartesianJoin(relation *rel1, relation * rel2, uint64_t key, int * pos_rel1, int * pos_rel2) {
    uint64_t array_end1 = (rel1->num_tuples-1); ///////////////////
    uint64_t array_end2 = (rel2->num_tuples-1); ///////////////////
    int end_rel1 = find_key_range_end(rel1, *pos_rel1, key, array_end1);
    int end_rel2 = find_key_range_end(rel2, *pos_rel2, key, array_end2);
    uint64_t num_of_elements = 0;
    // printf("relation1 : key start: %d , key end: %d\n", *pos_rel1, end_rel1);
    // printf("relation2 : key start: %d , key end: %d\n", *pos_rel2, end_rel2);

    // Join in a cartesian way both relations based on specific key
    for(int i=*pos_rel1 ; i<=end_rel1 ; i++) {
        for(int j=*pos_rel2 ; j<=end_rel2 ; j++) {
            // TODO push in result list
            // printf("key1: %ld , key2: %ld\n", rel1->tuples[*pos_rel1].key, rel2->tuples[*pos_rel2].key);
            // num_of_elements++;
            // printf("8\n");
            // printf("positions: %d,%d\n",)
            // addResults(&current, rel1->tuples[*pos_rel1].payload, rel2->tuples[*pos_rel2].payload);
            addResults(&current, rel1->tuples[i].payload, rel2->tuples[j].payload);

            // addResults(&current, rel1->tuples[i].key, rel2->tuples[j].key);

        }
    }

    *pos_rel1 = end_rel1+1;
    *pos_rel2 = end_rel2+1;
    return num_of_elements;
}

// void ParallelJoin(relation *rel1, relation * rel2, uint64_t key, int * pos_rel1, int * pos_rel2)

int find_key_range_end(relation *rel, int start, uint64_t key, uint64_t array_end) {
    int end = start;

    while((end<=array_end) && (rel->tuples[end].key == key)) end++;
    // printf("end %d\n",end);
    return end-1;
}
