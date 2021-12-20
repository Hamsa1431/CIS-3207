#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "queue.c"
#include "rand.c"
#include "PQ.c"
#include "readConfig.c"
//#include "stats.c"

#define IDLE 0
#define BUSY 1
#define QUEUESIZE 10

void process_Arrival(Event* task, PQueue* eventQ);
void process_CPU(Event* task, PQueue* eventQ);
void process_DISK(Event* task, PQueue* eventQ);
void fileRead();

int cpu_status = 0;
int timeGlobal = 0;
int disk1_status = 0;
int disk2_status = 0;

int INI_TIME;
int FIN_TIME;
int ARRIVE_MIN;
int ARRIVE_MAX;
int QUIT_PROB;
int CPU_MIN;
int CPU_MAX;
int DISK1_MIN;
int DISK1_MAX;
int DISK2_MIN;
int DISK2_MAX;

Node queue_CPU;
Node queue_DISK1;
Node queue_DISK2;

int main()
{
    fileRead();
    srand(getSeed());
    Event* task;
    PQueue* EventQueue = initializePQ();

    initializeQ(&queue_CPU);
    initializeQ(&queue_DISK1);
    initializeQ(&queue_DISK2);

    FILE* fp = fopen("log.txt", "w");
    fclose(fp);

    Event job1;
    job1.jobSeq = 1;
    job1.type = ARRIVAL;
    job1.time = 0;
    push(EventQueue, &job1);

    while(isEmpty(EventQueue)&&(timeGlobal < FIN_TIME))
    {
        task = pop(EventQueue);
        switch (task->type)
        {
            case ARRIVAL:
                timeGlobal = task->time;
                process_CPU(task, EventQueue);
                break;
            case CPU_BEGIN:
                process_CPU(task, EventQueue);
                break;
            case CPU_FINISH:
                timeGlobal = task->time;
                process_CPU(task, EventQueue);
                break;
            case DISK_ARRIVAL:
                timeGlobal = task->time;
                process_DISK(task, EventQueue);
                break;
            case DISK1_BEGIN:
            case DISK2_BEGIN:
                process_DISK(task, EventQueue);
                break;
            case DISK1_FINISH:
            case DISK2_FINISH:
                timeGlobal = task->time;
                process_DISK(task, EventQueue);
                break;
            case 7:
            {
                FILE* fp = fopen("log.txt", "a");
                fprintf(fp, "At time %-7d Job%-3d exits\n", timeGlobal, task->jobSeq);
                fclose(fp);
                printf("At time %-7d Job%-3d exits\n", timeGlobal, task->jobSeq);
                break;
            }
        }
    }

    destroy(EventQueue);
    destroyPtr();
    return 1;

    stat_Print_CPUStats();
    stat_Print_DISKStats();
}

void process_Arrival(Event* task, PQueue* eventQ)
{
    FILE* file = fopen("log.txt", "a");
    fprintf(file, "At time %-7d Job%-3d arrives\n", task->time, task->jobSeq);
    fclose(file);
    printf("At time %-5d Job%-7d arrives\n", task->time, task->jobSeq);
    enQueue(&queue_CPU, task->jobSeq);

    Event next_job;
    next_job.jobSeq = task->jobSeq + 1;
    next_job.type = ARRIVAL;
    next_job.time = randNum(CPU_MIN, CPU_MAX) + timeGlobal;

    push(eventQ, &next_job);
}

void process_CPU(Event* task, PQueue* eventQ)
{

    FILE* file = fopen("log.txt", "a");

    if(task->type == ARRIVAL)
    {
        process_Arrival(task, eventQ);
    }

    if(task->type == CPU_BEGIN)
    {
        Event task_fin;
        task_fin.jobSeq = task->jobSeq;
        task_fin.time = randNum(CPU_MIN, CPU_MAX) + timeGlobal;
        task_fin.type = CPU_FINISH;
        push(eventQ, &task_fin);
    }

    if(task->type == CPU_FINISH)
    {
        if(probabilityGet(QUIT_PROB) == 0)
        {
            Event disk_task;
            disk_task.jobSeq = task->jobSeq;
            disk_task.time = task->time;
            disk_task.type = DISK_ARRIVAL;

            push(eventQ, &disk_task);
        }
        else
        {
            Event all_fin;
            all_fin.jobSeq = task->jobSeq;
            all_fin.time = task->time;
            all_fin.type = END;
            push(eventQ, &all_fin);
        }

        cpu_status = IDLE;
        printf("At time %-7d job%-3d finishes at CPU\n",timeGlobal, task->jobSeq );
        fprintf(file, "At time %-7d job%-3d finishes at CPU\n",timeGlobal, task->jobSeq);

    }

    if( (queue_CPU.current>=1) && (cpu_status) == IDLE)
    {
        int seq = deQueue(&queue_CPU);
        printf("At time %-7d job%-3d begins at CPU\n",task->time, seq );
        fprintf(file, "At time %-7d job%-3d begins at CPU\n",task->time, seq);
        Event job_begin;
        job_begin.jobSeq = seq;
        job_begin.time = timeGlobal;
        job_begin.type = CPU_BEGIN;

        push(eventQ, &job_begin);

        cpu_status = BUSY;
    }
    fclose(file);
}

void process_Arrival_DISK(Event* task, PQueue* eventQ)
{
    FILE* file = fopen("log.txt", "a");
    fprintf(file, "At time %-7d Job%-3d arrives at Disk\n", task->time, task->jobSeq);
    fclose(file);
    printf("At time %-7d Job%-3d arrives at Disk\n", task->time, task->jobSeq);
    int size1 = queue_DISK1.current;
    int size2 = queue_DISK2.current;
    if(disk1_status == IDLE && disk2_status == IDLE)
    {
        if(size1 > size2)
        {
            enQueue(&queue_DISK2, task->jobSeq);
        }
        else
        {
            enQueue(&queue_DISK1, task->jobSeq);
        }
    }
    else
    {
        if(disk1_status == IDLE && disk2_status == BUSY)
        {
            enQueue(&queue_DISK1, task->jobSeq);
        }

        if(disk2_status == IDLE && disk1_status == BUSY)
        {
            enQueue(&queue_DISK2, task->jobSeq);
        }
    }
}

void process_DISK(Event* task, PQueue* eventQ)
{
    FILE* file = fopen("log.txt", "a");

    switch (task->type)
    {
        case DISK_ARRIVAL:
            process_Arrival_DISK(task, eventQ);
        break;
    
        case DISK1_BEGIN :
        {
            Event task_fin1;
            task_fin1.jobSeq = task->jobSeq;
            task_fin1.time = randNum(DISK1_MIN, DISK1_MAX) + timeGlobal;
            task_fin1.type = DISK1_FINISH;
            push(eventQ, &task_fin1);

            break;
        }
        case DISK2_BEGIN :
        {
            Event task_fin2;
            task_fin2.jobSeq = task->jobSeq;
            task_fin2.time = randNum(DISK2_MIN, DISK2_MAX) + timeGlobal;
            task_fin2.type = DISK2_FINISH;
            push(eventQ, &task_fin2);

            break;
        }
        case DISK1_FINISH:
        {
            printf("At time %-7d Job%-3d finishes at Disk1\n", task->time, task->jobSeq);
            fprintf(file, "At time %-7d Job%-3d finishes at Disk1\n", task->time, task->jobSeq);
            enQueue(&queue_CPU, task->jobSeq);
            disk1_status = IDLE;

            break;
        }

        case DISK2_FINISH:
        {
            printf("At time %-7d Job%-3d finishes at Disk2\n", task->time, task->jobSeq);
            fprintf(file, "At time %-7d Job%-3d finishes at Disk2\n", task->time, task->jobSeq);
            enQueue(&queue_CPU, task->jobSeq);
            disk2_status = IDLE;

            break;
        }
    }

    if((queue_DISK1.current >= 1) && (disk1_status == IDLE))
    {
        int seq = deQueue(&queue_DISK1);
        printf("At time %-7d job%-3d begins at Disk1\n",task->time, seq );
        fprintf(file, "At time %-7d job%-3d begins at Disk1\n",task->time, seq);
        
        Event job_begin;
        job_begin.jobSeq = seq;
        job_begin.time = timeGlobal;
        job_begin.type = DISK1_BEGIN;

        push(eventQ, &job_begin);
        disk1_status = BUSY;
    }

    if((queue_DISK2.current >= 1) && (disk2_status == IDLE))
    {
        int seq = deQueue(&queue_DISK2);
        printf("At time %-7d job%-3d begins at Disk2\n",task->time, seq );
        fprintf(file, "At time %-7d job%-3d begins at Disk2\n",task->time, seq);
        Event job_begin;
        job_begin.jobSeq = seq;
        job_begin.time = timeGlobal;
        job_begin.type = DISK2_BEGIN;

        push(eventQ, &job_begin);
        disk2_status = BUSY;
    }
    
    fclose(file);
}

void fileRead()
{
    readFile();
    
    INI_TIME = getIniTime();
    FIN_TIME = getFinTime();
    ARRIVE_MIN = getArrivalMin();
    ARRIVE_MAX = getArrivalMax();
    QUIT_PROB = getQuitProb();
    CPU_MIN = getCPUMin();
    CPU_MAX = getCPUMax();
    DISK1_MIN = getDisk1Min();
    DISK1_MAX = getDisk1Max();
    DISK2_MIN = getDisk2Min();
    DISK2_MAX = getDisk2Max();
}
