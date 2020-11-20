#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <ucontext.h>
#include <unistd.h>

typedef struct tcb{
    int thread_id;
    int thread_priority;
    ucontext_t *thread_context;
    struct tcb *next;
}tcb;

void t_init(void);
void t_shutdown(void);
int t_create(void (*func)(int), int thr_id, int pri);
void t_terminate(void);
void t_yield(void);
void getNextReady();
void addToReadyList(tcb * threadNode);



tcb *running;
tcb *ready;
