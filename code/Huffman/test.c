#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"

int main() {

    priority_queue pq;
    initPriorityQueue(&pq);  // Initialize the priority queue

    // Create nodes with frequency and data
    node* n1 = createNode(5, 'a');
    node* n2 = createNode(9, 'b');
    node* n3 = createNode(3, 'c');
    node* n4 = createNode(12, 'd');
    node* n5 = createNode(7, 'e');

    // Push nodes into the priority queue
    push(&pq, *n1);
    printf("%d\n", pq.size);  // Print the size of the queue after each push
    push(&pq, *n2);
    printf("%d\n", pq.size);
    push(&pq, *n3);
    printf("%d\n", pq.size);
    push(&pq, *n4);
    printf("%d\n", pq.size);
    push(&pq, *n5);
    printf("%d\n", pq.size);

    // Pop nodes from the priority queue and display their frequencies and data
    printf("Priority order (lowest frequency first):\n");
    while (pq.size > 0) {
        node minNode = pop(&pq);
        printf("Node with frequency: %d, data: %c\n", minNode.freq, minNode.data);
    }

    // Free allocated memory for nodes
    free(n1);
    free(n2);
    free(n3);
    free(n4);
    free(n5);

    return 0;
}
