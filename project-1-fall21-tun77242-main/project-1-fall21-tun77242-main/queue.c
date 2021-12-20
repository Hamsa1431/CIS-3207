#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int current;
    int sequence[14];
    int size;
}Node;

void initializeQ(Node *queue)       //starts the array empty
{
    if(queue == NULL)
    {
        return;
    }

    queue->current = 0;         //initial values of queue set to 0
    queue->size = 14;           // size of the queue is assigned
    
    int i;
    for (i = 0; i < queue->size; i++)
    {
        (queue->sequence)[i] = 0;
    }
}

int enQueue(Node *queue, int jobSeq)
{
    int size = queue->size;
    int *seq = queue->sequence;

    if (queue->current == size)     //if teh queue is full, it returns
    {
        return 0;
    }

    seq[queue->current] = jobSeq;       //while its not full, adds new one to the queue
    queue->current += 1;
    return 1;
}

void queueAdjust(int *arr, int *size)
{
    int i;
    for (i = 0; i < *size - 1; i++)
    {
        arr[i] = arr[i + 1];
    }

    arr[*size - 1] = 0;     //value of last one in the queue is set to 0
    *size -= 1;            //makes an empty space in the queue
}

int deQueue(Node *queue)
{
    if (queue == NULL)
    {
        return 0;
    }

    int del = queue->sequence[0];       //points to the first position in the queue
    queueAdjust(queue->sequence, &queue->current);      //adjusts the queue and then that size will be set to current size of queue
    return del;             // returns the next value in the queue
}

void toString(Node *queue)      //loops through the queue to print the jobs
{
    int i;
    for (i = 0; i < queue->size; i++)
    {
        printf("Job:%d\t", queue->sequence[i]);
    }

    puts("");
}