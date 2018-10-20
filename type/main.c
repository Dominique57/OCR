#include "queue.h"

int main()
{
    Queue *q = InitQueue();
    int a = 1, b = 2, c = 3;
    Enqueue(&a, q);
    Enqueue(&b, q);
    Enqueue(&c, q);
    printQueue(q);
    /*
    for (int i = 0; i < 3; ++i)
    {
        int val = Dequeue(q);
        printf("%i\n", val);
    }
    */
}