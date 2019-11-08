/*************************************************************************
Implementation File   		: resultsList.c
Author      		        :
Purpose				        :
**************************************************************************/


#include "./include/resultsList.h"

void nodeInit(resultsList *node)
{
    node->buffer = malloc(sizeof(uint64_t)*MAX_BUFFER_ELEMEMENTS);
    node->sizeUsed = 0;
    node->next = NULL;
}

void addNode(resultsList **current)
{
    // printf("new node\n");
    (*current)->next = malloc(sizeof(resultsList));
    nodeInit((*current)->next);

    *current = (*current)->next;
}

void addResults(resultsList **current, uint64_t rowid1, uint64_t rowid2)
{
    // printf("About to add %ld and %ld\n", rowid1, rowid2);
    if(MAX_BUFFER_ELEMEMENTS - (*current)->sizeUsed >= 2)
    {
        addTOBufer(*current, rowid1, rowid2);
    }
    else
    {
        //printf("New Node\n");
        addNode(current);
        addTOBufer(*current, rowid1, rowid2);
    }
}


void addTOBufer(resultsList *current, uint64_t rowid1, uint64_t rowid2)
{
    current->buffer[current->sizeUsed] = rowid1;
    current->buffer[current->sizeUsed+1] = rowid2;
    current->sizeUsed += 2;
}

void printList(resultsList *root)
{
    resultsList *current = root;
    int nodeNum = 1;
    uint64_t count =0;
    printf("RESULTS LIST \n");
    while(current != NULL)
    {
        printf("--------------------------------\nBuffer %d : ", nodeNum);
        printf("\n");
        for(int i = 0 ; i < current->sizeUsed ; i+=2)
        {

            printf("%lu Joined row %lu (rel1) with row %lu (rel2).\n",count, current->buffer[i], current->buffer[i+1]);
            count++;
        }
        current = current->next;
        nodeNum++;
    }
    // printf("Amount of elements: %ld\n",count);
}

void listDestroy(resultsList * root) {
    resultsList * cur = NULL;
    resultsList * toDel = root;
    if(root != NULL) {
        do {
            cur = toDel->next;
            free(toDel->buffer);
            free(toDel);
            toDel = cur;
        } while(cur != NULL);
    }
}

void createCSVFile(resultsList * root, char *fileName)
{
    printf("\nCreating %s.csv file\n\n", fileName);

    FILE *fp;

    fileName = strcat(fileName,".csv");
    fp = fopen(fileName, "w+");

    fprintf(fp,"RowId1;RowId2\n");

    resultsList *current = root;
    uint64_t counter = 0;
    while(current != NULL)
    {
        for(int i = 0 ; i < current->sizeUsed ; i+=2)
        {
            fprintf(fp,"%ld;%ld\n", current->buffer[i], current->buffer[i+1]);
            counter++;
        }
        current = current->next;
    }
    printf("Number of elements: %ld\n", counter);
    fclose(fp);
}
