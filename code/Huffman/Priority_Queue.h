#ifndef PRIORITY_QUEUE_HEADER 
#define PRIORITY_QUEUE_HEADER

#include "Node.h"
#define MAX (1<<8)

typedef struct {
    node* tree[MAX+1];  
    int size;         
} priority_queue;

void initPriorityQueue(priority_queue* pq) {
    pq->size = 0;  
}

void swap(node *a, node *b) {
    node temp = *a;  
    *a = *b;
    *b = temp;
}

void push(priority_queue *pq, node* a) {
    if (pq->size >= MAX) return;
    pq->tree[pq->size] = a;  
    pq->size++;
    for (int i = pq->size-1; i>0; i = (i-1)>>1) {
        if (pq->tree[i]->freq >= pq->tree[(i-1)>>1]->freq) return;
        swap(pq->tree[i], pq->tree[(i-1)>>1]);
    }
}

node* pop(priority_queue *pq) {
    if (pq->size == 0) {
        node* emptyNode = {0};  
        return emptyNode;
    }
    node* min = pq->tree[0];  
    pq->tree[0] = pq->tree[pq->size - 1];  
    pq->size--;
    int i = 0;
    while ((2*i+1) < pq->size) { 
        int lft = 2 * i + 1, rght = 2 * i + 2, smll = i;
        if (pq->tree[lft]->freq < pq->tree[smll]->freq) smll = lft;
        if (rght < pq->size && pq->tree[rght]->freq < pq->tree[smll]->freq) smll = rght;
        if (smll == i) break;  
        swap(pq->tree[i], pq->tree[smll]);  
        i = smll;  
    }return min;  
}

#endif
