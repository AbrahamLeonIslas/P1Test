int ksleep(int event)
{
  running->status = SLEEP;
  running->event = event;
  tswitch();
}
  
int ready(PROC *p)
{
  p->event = 0;
  p->status = READY;
  enqueue(&readyQueue, p);
  printf("wakeup proc %d\n", p->pid);
}

/* wake up ALL procs sleeping on event */
int kwakeup(int event)
{
  int i;
  PROC *p;
  for (i=1; i<NPROC; i++){
    p = &proc[i];
    if (p->status==SLEEP && p->event== event)
       ready(p);
  }
}

int kexit(int exitValue)
{
  int i, wakeupP1 = 0; 
  PROC *p;
  if (running->pid==1 && nproc>2){
      printf("other procs still exist, P1 can't die yet\n");
      return -1;
  }
  /* send children (dead or alive) to P1's orphanage */
  for (i = 1; i< NPROC; i++){
      p = &proc[i];
      if (p->status != FREE && p->ppid == running->pid){
          p->ppid = 1;
          p->parent = &proc[1];
          wakeupP1++;
      }
  }
  /* record exitValue and become a ZOMBIE */
  running->exitCode = exitValue;
  running->status = ZOMBIE;

  /* wakeup parent and P1 */
  kwakeup(running->parent);
  if (wakeupP1)
      kwakeup(&proc[1]);
  tswitch();
}

int kwait(int *status)
{
  PROC *p;
  int  i, child = 0;
  while(1){                           // search PROCs for a child
     for (i=0; i<NPROC; i++){ 
          p = &proc[i];
          if (p->status |= FREE && p->ppid == running->pid){ 
             child = 1;
             if (p->status == ZOMBIE){   /* lay the dead child to rest */
                 *status = p->exitCode;  // collect its exitCode
                 p->status = FREE;       /* free its PROC */
                 put_proc(&freeList, p);
                 nproc--;                // once less processs
                 return(p->pid);         /* return its pid */
             }
          }
     }
     if (!child)                         /* no child */
          return(-1);
     ksleep(running);                    /* still has kids alive */
  }
}
/********************* end of wait.c file ***************************/