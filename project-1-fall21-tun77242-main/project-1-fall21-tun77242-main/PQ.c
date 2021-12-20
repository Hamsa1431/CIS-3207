#include <stdio.h>
#include <stdlib.h>

#define ARRIVAL 8
#define CPU_BEGIN 2
#define CPU_FINISH 3
#define DISK_ARRIVAL 9
#define DISK1_BEGIN 10
#define DISK2_BEGIN 11
#define DISK1_FINISH 5
#define DISK2_FINISH 6
#define END 7

typedef struct Events
{
    int jobSeq;
    int type;
    int time;
}Event;

typedef struct PriorityQueue
{
    Event* event;
    int size;
    int maxSize;
}PQueue;

PQueue* initializePQ()
{
    PQueue* heap = malloc(sizeof(PQueue));
    Event* start = malloc(sizeof(Event));
    Event* end = malloc(sizeof(Event));
    
    if(heap == NULL || start == NULL || end == NULL)
    {
        perror("malloc fails");
        return NULL;
    }

    heap->maxSize = 10;
    heap->size = 2;

    start->jobSeq = 0;
    start->time = 0;
    start->type = 1;

    end->jobSeq = 0;
    end->time = 10000;
    end->type = 8;
    heap->event = start;
    heap->event[1] = *end;

    return heap;
}

int cmp(const void *a, const void *b)
{
    Event* c = (Event*) a;
    Event* d = (Event*) b;
    return c->time - d->time;
}

int isFull(PQueue* heap)
{
    if( heap->maxSize == heap->size)
    {
        return 1;
    }
    return 0;
}

void push(PQueue* heap, Event* one)
{
    if(isFull(heap))
    {
        return;
    }

    int time = one->time;
    heap->size += 1;
    heap->event[heap->size - 1] = *one;
    qsort(heap->event, heap->size,sizeof(Event), cmp);
}

Event* pop(PQueue* heap)
{
    Event* del = heap->event;
    heap->event = &heap->event[1];
    heap->size -= 1;
    return del;
}

void destroy(PQueue* heap)
{
    free(heap);
}

int isEmpty(PQueue* heap)
{
    return heap->size;
}

void printQ(PQueue* heap)
{
    int i;
    for (i = 0; i < heap->size; i++)
    {
        printf("Job seq:%-2d, time:%-5d type:%-2d\n", heap->event->jobSeq, heap->event->time, heap->event->type);
        heap->event ++;
    }
    
}