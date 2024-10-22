#ifndef TREE_HEADER
#define TREE_HEADER

#include <stdlib.h>
#define TREE_HEADER
#define uchar unsigned char

typedef struct node {
    int freq;
    uchar data;
    struct node *lft, *rght;
} node;

node* createNode(int freq, uchar data) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->freq = freq;
    newNode->data = data;
    newNode->lft = NULL;
    newNode->rght = NULL;
    return newNode;
}

node* joinTrees(node* l, node* r) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->freq = l->freq + r->freq;
    newNode->data = '\0';  
    newNode->lft = l;
    newNode->rght = r;
    return newNode;
}

#endif
