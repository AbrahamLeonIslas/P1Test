#include "type.h"

PROC proc[NPROC], *running, *freeList, *readyQueue;
int procSize = sizeof(PROC);
int nproc;
extern int color;

#include "wait.c"
#include "kernel.c"

int init()
{
    PROC *p;
    int i;
    printf("init ....");

    for (i=0; i<NPROC; i++){   // initialize all procs
         p = &proc[i];
         p->pid = i;
         p->status = FREE;
         p->priority = 0;     
         p->next = &proc[i+1];
    }
    freeList = &proc[0];      // all procs are in freeList
    proc[NPROC-1].next = 0;
    readyQueue;

    /**** create P0 as running ******/
    p = get_proc();
    p->status = RUNNING;
    running = p;
    printf("done\n");
} 
            
 main()
{
    printf("MTX starts in main()\n");
    init();          // initialize and create P0 as running
    kfork();         // P0 kfork() P1
    while(1){
       printf("P0 running\n");
       while(!readyQueue);
       printf("P0 switch process\n");
       tswitch();   // P0 switch to run P1
    }
}

int scheduler()
{
    if (running->status == RUNNING){
       running->status = READY;
       enqueue(&readyQueue, running);
    }
    running = dequeue(&readyQueue);
    running->status = RUNNING;
    color = 0x000A + (running->pid % 5);
}
