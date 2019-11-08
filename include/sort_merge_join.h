/*************************************************************************
Implementation File   		: sort_merge_join.h
Author      		        :
Purpose				        :
**************************************************************************/

#ifndef __SORT_MERGE_JOIN__
#define __SORT_MERGE_JOIN__

    #include "resultsList.h"
    #include "relation.h"

    /*
        Main metho that takes care fo sorting relations and merging/joining the results
        ARGS:
            two relations
        RETURNS:
            a results list
    */
    resultsList* SortMergeJoin(relation *r1, relation *r2);

#endif