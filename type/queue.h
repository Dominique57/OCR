#ifndef QUEUE_h
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>

typedef struct Queue_elt Queue_elt;
struct Queue_elt
{
    void* value;
    Queue_elt *next;
    Queue_elt *prev;
};
typedef struct Queue Queue;
struct Queue
{
    int isEmpty;
    Queue_elt *head;
    Queue_elt *tail;
};

Queue_elt* InitQueue_elt(void* value);
void FreeQelt(Queue_elt *quelt);
Queue* InitQueue();
void FreeQueue(Queue *q);
void Enqueue(void* value, Queue *q);
void* Dequeue(Queue *q);
void printQueue(Queue *q);


#endif