/*************************************************************************
Implementation File   		: resultsList.h
Author      		        :
Purpose				        :
**************************************************************************/

#ifndef __RESULTSLIST__
#define __RESULTSLIST__
    #include <stdio.h>
    #include "relation.h"

    #define MAX_BUFFER_ELEMEMENTS 1024*1024/sizeof(uint64_t)


    typedef struct resultsList
    {
        uint64_t *buffer;
        uint64_t sizeUsed;
        struct resultsList *next;

    } resultsList;

    void nodeInit(resultsList *node);

    void addNode(resultsList **current);

    void addResults(resultsList **current, uint64_t rowid1, uint64_t rowid2);

    void addTOBufer(resultsList *current, uint64_t rowid1, uint64_t rowid2);

    void printList(resultsList *root);

    void listDestroy(resultsList * root);

    void createCSVFile(resultsList * root, char *fileName);

#endif
