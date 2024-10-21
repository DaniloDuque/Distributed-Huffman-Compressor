#ifndef PRIORITY_QUEUE_HEADER 

#include "Tree.h"
#define MAX 300

typedef struct {
    node tree[MAX+1];
    int size;
} priority_queue;

void initPriorityQueue(priority_queue* pq){
    pq->size=0;
}

void swap(node *a, node *b){
    a^=b; 
    b^=a;
    a^=b;
}

void push(priority_queue * pq, node *a){
    node &tree = pq->tree;
    int &sz = pq->size;
    tree[sz]=a;
    for(int i = sz; i; i>>=1){
        if(tree[i] >= tree[i>>1]) return;
        swap(tree[i>>1], tree[i]);
    }sz++;
}

node *pop(priority_queue *pq) {
    if (pq->size == 0) return NULL;  
    node *min = pq->tree[1]; 
    pq->tree[1] = pq->tree[pq->size];  
    pq->size--;  
    int i = 1;
    while (i * 2 <= pq->size) {
        int left = i * 2;
        int right = left + 1;
        int smallest = i;
        if (*pq->tree[left] < *pq->tree[smallest]) smallest = left;
        if (right <= pq->size && *pq->tree[right] < *pq->tree[smallest]) smallest = right;
        if (smallest == i) break;  
        swap(&pq->tree[i], &pq->tree[smallest]);
        i = smallest;  
    }return min;  
}

#endif
