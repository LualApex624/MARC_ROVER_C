//
// Created by flasque on 19/10/2024.
//

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"

void enqueue(t_queue *p_queue, t_position pos) {
// la file ne doit pas être pleine
if ((p_queue->last - p_queue->first) >= p_queue->size) {
fprintf(stderr, "Error: queue is full, cannot enqueue\n");
return;
}
p_queue->values[(p_queue->last) % p_queue->size] = pos;
p_queue->last++;
}

t_position dequeue(t_queue *p_queue) {
// la file ne doit pas être vide
if (p_queue->last == p_queue->first) {
fprintf(stderr, "Error: queue is empty, cannot dequeue\n");
t_position invalid_position = {-1, -1};
return invalid_position; // Retourner une valeur invalide pour indiquer l'erreur
}
p_queue->first++;
return p_queue->values[(p_queue->first - 1) % p_queue->size];
}




/**
 * @brief Function to create a queue
 * @param size : the size of the queue
 * @return the queue
 */
t_queue createQueue(int size)
{
// the size of the queue must be positive
assert(size > 0);
t_queue queue;
queue.size = size;
queue.first = 0;
queue.last = 0;
queue.values = (t_position *)malloc(size * sizeof(t_position));
return queue;
}

