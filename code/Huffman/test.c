#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"

int main() {

    priority_queue pq;
    initPriorityQueue(&pq);

    node* n1 = createNode(5, 'a');
    node* n2 = createNode(9, 'b');
    node* n3 = createNode(3, 'c');
    node* n4 = createNode(12, 'd');
    node* n5 = createNode(7, 'e');

    push(&pq, *n1);
    push(&pq, *n2);
    push(&pq, *n3);
    push(&pq, *n4);
    push(&pq, *n5);

    printf("Orden de prioridad (frecuencia más baja primero):\n");
    while (pq.size > 0) {
        node minNode = pop(&pq);
        printf("Nodo con frecuencia: %d, dato: %c\n", minNode.freq, minNode.data);
    }

    free(n1);
    free(n2);
    free(n3);
    free(n4);
    free(n5);

    return 0;
}

