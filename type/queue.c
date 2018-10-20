#include "queue.h"

/*
void FreeQueue(Queue *q)
{
    free(q->head);
    free(q->tail);
    free(q);
}

void FreeQelt(Queue_elt *quelt)
{
    free(quelt->value);
    free(quelt->next);
    free(quelt->prev);
    free(quelt);
}
*/

Queue_elt* InitQueue_elt(void *value)
{
    Queue_elt *qelt = malloc(sizeof(Queue_elt));
    qelt->value = value;
    qelt->next = NULL;
    qelt->prev = NULL;
    return qelt;
}

Queue* InitQueue()
{
    Queue *queue = malloc(sizeof(Queue));;
    queue->isEmpty = 1;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void Enqueue(void *value, Queue *q)
{
    Queue_elt *qelt = InitQueue_elt(value);
    if (q->isEmpty)
    {
        q->tail = qelt;
        q->head= qelt;
        q->isEmpty = 0;
    }
    else
    {
        q->head->next = qelt;
        qelt->prev = q->head;
        q->head = qelt;
    }
}

void* Dequeue(Queue *q)
{
    if (q->isEmpty)
    {
        return NULL;
    }
    else
    {
        void* val;
        if (q->head == q->tail)
        {
            Queue_elt *qelt = q->head;
            val = qelt->value;
            q->head = NULL;
            q->tail = NULL;
            q->isEmpty = 1;
            //FreeQelt(qelt);
            return val;
        }
        else
        {
            Queue_elt *qelt = q->tail;
            val = qelt->value;
            qelt->next->prev = NULL;
            q->tail = qelt->next;
            //FreeQelt(qelt);
            return val;
        }
    }
}

void printQueue(Queue *q)
{
    if (q->tail != NULL)
    {
        Queue_elt *qelt = q->tail;
        printf("%i\n", *(int*)(qelt->value));
        while (qelt->next != NULL)
        {
            qelt = qelt->next;
            printf("%i\n", *(int*)(qelt->value));
        }
    }
}