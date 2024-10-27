#ifndef TREE_HEADER
#define TREE_HEADER
#include "Node.h"
#include "Priority_Queue.h"
#include <stdbool.h>
#include <math.h>

#define SET(msk, i) ((msk)|(1<<(i)))
#define TEST(msk, i) ((msk)&(1<<(i)))

node* makeTree(priority_queue* pq){
    while(pq->size > 1){
        node* n1 = pop(pq);
        node* n2 = pop(pq);
        push(pq, joinTrees(n1, n2));
    }node* root = pop(pq);
    return root;
}

void displayTree(node* root, char* prefix, bool islft) {
    if (root == NULL) return;
    printf("%s", prefix);
    printf("%s", (islft ? "├──" : "└──"));
    printf("%c (%d)\n", root->data, root->freq);
    char newPrefix[256];
    snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, (islft ? "│   " : "    "));
    displayTree(root->lft, newPrefix, true);
    displayTree(root->rght, newPrefix, false);
}

typedef struct {
    int len, msk;
} route;

void displayRoute(route* r, uchar c){
    printf("Character: %c  Length: %d  Route: ", c, r->len);
    for(int i = 0; i<r->len; ++i) printf("%u", (bool)TEST(r->msk, i));
    puts("");
}

route* newRoute(int len, int msk){
    route* r = (route*)malloc(sizeof(route));
    r->len = len, r->msk = msk;
    return r;
}

char* mskToString(int start, int msk, int len, char * r){
    for(int i = 0; i<len; ++i) r[start+i]=((bool)TEST(msk, i))+'0';
    return r;
}

//ascii-route/
char* toString(route* r, int asc){
    int digits = log10(asc);
    char * st = (char*)calloc(digits+ 4 + r->len, sizeof(char));
    int pot=10;
    int pos=0;
    for(int i=0; i<=digits; i++){
        st[pos]='0'+asc%pot;
        pot*=10;
        pos++;
    }
    st[pos]='-';
    st = mskToString(pos+1, r->msk, r->len, st);
    st[digits+1+r->len]='/';
    st[digits+2+r->len]='\0';
    return st;
}

void RecRoutes(node* root, int lvl, route r, route** routes) {
    if (!root) return;
    if (!root->lft && !root->rght) {routes[root->data] = newRoute(r.len, r.msk); return;}
    route lftRoute = {lvl+1, r.msk}, rghtRoute = {lvl+1, SET(r.msk, lvl)};  
    RecRoutes(root->lft, lvl+1, lftRoute, routes);  
    RecRoutes(root->rght, lvl+1, rghtRoute, routes);  
}

route** makeRoutes(node* root) {
    route** routes = (route**)calloc((1<<8), sizeof(route*));  
    route init = {0, 0};  
    RecRoutes(root, 0, init, routes);
    return routes;
}

#endif 
