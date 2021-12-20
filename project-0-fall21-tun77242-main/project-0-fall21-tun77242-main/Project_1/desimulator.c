#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CPU_ARRIVAL 0
#define CPU_FINISH 1
#define DISK_ARRIVAL 2
#define DISK_FINISH 3
#define SIMULATION_END 4

#define IDLE 0
#define BUSY 1

typedef struct event
{
    int jobID;
    int eventType;
    int time;
    struct event* nextPriQue;
    struct event* nextFIFOQue;
};

typedef struct FIFOQueue
{
    int jobTotal;
    int size;
    int memory;
    struct event *front, *rear;
};

struct event* newEvent(int ID)
{
    struct event* temp = (struct event*)malloc(sizeof(struct event));
    temp->jobID = ID;
    temp->nextPriQue = NULL;
    
    return temp;
}

struct FIFOQueue* createQueue()
{
    struct FIFOQueue* queue = (struct FIFOQueue*)malloc(sizeof(struct FIFOQueue));
    queue->front = queue->rear = NULL;
    queue->size = 0;
    queue->jobTotal = 0;

    return queue;
}

int randTime();
event startEvent();
void push();
void pop();
void enQueue();
void deQueue();
void CPU_handler(event);
void DISK_handler(event);

//Main loop
while (mainQueue is not empty and time is not fin_tim) 
{
    event = pop(mainQueue);
    currentTime = event.time;
    switch(event.type)
    {
        case CPU_ARRIVAL: processCPU(); 
        break;
        case CPU_FINSIHED: processCPU();
        break;
        case DISK_ARRIVAL: processDISK();
        break;
        case DISK1_FINISHED: processDISK();
        break;
        case DISK2_FINISHED: processDISK();
        break;
        //...
    }
}

if (event type == CPU_ARRIVAL)
{
    // Create a new event
    create a new event
    push event to the priorityQueue
    push event to cpuQueue
} 
else
{
    // Work with exisiting event that has finished
    cpu.status = idle OR cpu.eventType = NULL (Keep track of if component is handling an event or free)
    if(job should not quit)
    {
        // Prob says event should not quit
        if(job should go to disk)
        {
            // Prob says event should go to disk
            create new event for the smallest disk queue 
            add event to the priority queue
        }
        else
        {
            // Prob says event should go to network
            create new event for the network queue
            add event to priorityQueue
        }
    }
    else
    {
        // Prob says job should quit
        job has finished
    }
}

if(cpuQueue is not empty and cpu does not have a event running)
{
    event = deque(cpuQueue)
    create new CPU_FINISHED event with random execute time
    // job is not finished now, but will be next time it is accessed form the priority queue
    push "finished" event to priorityQueue
    set cpu status to busy(current event)
}

//clock
int randTime()
{

}

//initializing event
event startEvent()
{
    
}

//push event for priority queue
void push()
{
    
}

//pop event for priority queue
void pop()
{

}

//enQueue for fifo queue
void enQueue()
{

}

//deQueue for fifo queue
void deQueue()
{

}

//CPU handler
void CPU_handler()
{

}

//DISK handler
void DISK_handler()
{

}