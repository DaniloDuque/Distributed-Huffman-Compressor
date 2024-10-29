#ifndef MAKE_TABLE
#define MAKE_TABLE

#include "util.h"

extern ll freqTable[MAX_SIZE];
extern mutex keys[MAX_SIZE];

void updateTable(ll *table) {
    for(int i = 0; i<MAX_SIZE; ++i) {
        lock(&keys[i]);
        freqTable[i] += table[i];
        unlock(&keys[i]);
    }
}

#endif
