#ifndef UTIL_HEADER
#define UTIL_HEADER

#include "../config.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define TEST(msk, i) ((msk)&(1<<(i)))
#define SET(msk, i) ((msk)|=(1<<(i)))
#define uchar unsigned char
#define BUFFER_SIZE (1<<13)
#define ll long long

struct node; 

typedef struct node {
    uchar sym;
    struct node* left;
    struct node* right;
} node;

int lenTable[256];
int pathTable[256];

node* createNode(uchar s){
    node* nd = (node*)malloc(sizeof(node));
    nd->left = NULL;
    nd->right = NULL;
    nd->sym = s;
    return nd;
}

node* insert(int lvl, int len, int msk, uchar data, node* root) {
    if (root == NULL) root = createNode('\0');
    if (lvl == len) { 
        root->sym = data;
        return root;
    }
    if (TEST(msk, lvl)) root->right = insert(lvl + 1, len, msk, data, root->right);
    else root->left = insert(lvl + 1, len, msk, data, root->left);
    return root;
}

void displayTree(node* root, char* prefix, bool islft) {
    if (root == NULL) return;
    printf("%s", prefix);
    printf("%s", (islft ? "├──" : "└──"));
    printf("%c\n", root->sym); 
    char newPrefix[256];
    snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, (islft ? "│   " : "    "));
    displayTree(root->left, newPrefix, true);
    displayTree(root->right, newPrefix, false);
}

int reverse(int msk, int len){
    int r = 0;
    for(int i = 0; i<len; i++)
        if(TEST(msk, i)) SET(r, len-i-1);
    return r;
}

#endif