#ifndef TREE_HEADER
#define TREE_HEADER
#define uchar unsigned char

typedef struct {
    int freq;
    uchar data;
    node *lft, node *rght;
    node(int f, uchar d): freq(f), data(d), lft(nullptr), rght(nullptr){}
    node(node *l, node *r): c('\0'), lft(l), rght(r), freq(l->freq + r->freq){}
} node;







#endif
