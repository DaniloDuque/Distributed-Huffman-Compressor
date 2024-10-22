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

void swap(node a, node b) {
    node t = a;
    a = b;
    b = t;
}

void push(priority_queue * pq, node a){
    pq->tree[pq->size]=a;
    for(int i = pq->size; i; i>>=1){
        if(pq->tree[i].freq >= pq->tree[i>>1].freq) return;
        swap(pq->tree[i>>1], pq->tree[i]);
    }pq->size++;
}

node pop(priority_queue *pq) {
    node min = pq->tree[1]; 
    pq->tree[1] = pq->tree[pq->size];  
    pq->size--;  
    int i = 1;
    while (2*i <= pq->size) {
        int lft = i<<1, rght = lft+1, smll = i;
        if(pq->tree[lft].freq < pq->tree[smll].freq) smll = lft;
        if(rght <= pq->size && pq->tree[rght].freq < pq->tree[smll].freq) smll = rght;
        if(smll == i) break;  
        swap(pq->tree[i], pq->tree[smll]);
        i = smll;  
    }return min;  
}

#endif
