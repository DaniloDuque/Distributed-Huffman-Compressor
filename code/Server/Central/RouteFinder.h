#ifndef ROUTE_FINDER
#define ROUTE_FINDER

#include "util.h"

route ** routeFinder(ll* frequencies){
    priority_queue pq;
    initPriorityQueue(&pq);
    for(int i = 0; i<MAX_SIZE; ++i){
        if(frequencies[i])
            push(&pq, createNode(frequencies[i], i));
    }
    node *tree = makeTree(&pq);
    displayTree(tree, "", 0);
    return makeRoutes(tree);
}

#endif
