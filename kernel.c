int body();

int atoi(char *s)
{
  int v = 0;
  while(*s){
    v = v*10 + (*s-'0');
    s++;
  }
  return v;
}


int geti()
{
  char s[16];
  gets(s);
  return atoi(s);
}

PROC *kfork()
{
    PROC *p = get_proc();
    int i;
    if (p==0){
       printf("no more PROC, kfork() failed\n");
       return 0;
    }
    p->status = READY;
    p->priority = 1;          // priority = 1 for all proc except P0
    p->ppid = running->pid;   // parent = running
    p->parent = running;

    p->kstack[SSIZE-1] = (int)body;
    for (i=1; i<10; i++)
        p->kstack[i]= 0 ;
    p->ksp = &(p->kstack[SSIZE-9]);
    enqueue(&readyQueue, p);  // enter p into readyQueue by priority
    return p;                 // return child PROC pointer
}         

int do_tswitch()
{
  printf("proc %d tswitch()\n", running->pid);
  tswitch();
  printf("proc %d resumes\n", running->pid);
}

int do_kfork()
{
  PROC *p;
  printf("proc %d kfork a child\n", running->pid);
  p = kfork();
  if (p==0)
    printf("kfork failed\n");
  else
    printf("child pid = %d\n", p->pid);
}

int do_exit()
{
  int i;
  printf("proc %d exit\n", running->pid);
  running->status = FREE;
  put_proc(running);
  tswitch();
}

int do_sleep()
{
  int e; 
  printf("input an event value to sleep: ");
  e = geti();
  printf("proc %d going to sleep on event=%d\n", running->pid, e);
  ksleep(e);
}

int do_wakeup()
{
  int e; 
  printf("input an event value to wakeup: ");
  e = geti();
  printf("proc %d wakeup procs sleeping on event=%d\n", running->pid, e);

  kwakeup(e);
}

int body()
{
  char c;
  printf("proc %d resumes to body()\n", running->pid);

  while(1){
    printf("-----------------------------------------\n");
    printList("freelist  ", freeList);
    printQ("readyQueue", readyQueue);
    printf("-----------------------------------------\n");

   printf("proc %drunning: priority=%d parent=%d enter a char [s|f|q|z|a] : ",
           running->pid, running->priority, running->parent->pid );
    c = getc(); printf("%c\n", c);
    switch(c){
       case 'f' : do_kfork();   break;
       case 's' : do_tswitch(); break;
       case 'q' : do_exit();    break;
       case 'z' : do_sleep();   break;
       case 'a' : do_wakeup();  break;
    }
  }
}
