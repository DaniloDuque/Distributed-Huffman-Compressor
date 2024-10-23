#include <stdio.h>
#include <stdlib.h>
#include "Node.h"
#include "Tree.h"

int main() {

    priority_queue pq;
    initPriorityQueue(&pq);  // Initialize the priority queue

    int n, f;
    uchar c;
    scanf("%d", &n);
    while(n--){
        scanf("%d %c", &f, &c);
        push(&pq, createNode(f, c));
    }

    node* tree = makeTree(&pq);
    displayTree(tree, "", false);

    //routes[i] contains the route for the i'th byte
    // for example route['a'] could be: {5, 10110}
    route** routes = makeRoutes(tree);
    for(int i = 0; i<MAX; ++i)
        if(routes[i])
            displayRoute(routes[i], i);

    return 0;
}
