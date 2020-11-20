#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <ucontext.h>
#include <unistd.h>
#include <signal.h>

typedef struct tcb{
    int thread_id;
    int thread_priority;
    ucontext_t *thread_context;
    struct tcb *next;
}tcb;

void t_init();
void t_shutdown();
int t_create(void (*func)(int), int thr_id, int pri);
void t_terminate();
void t_yield();
void getNextReady();
void addToReadyList(tcb * threadNode);

tcb *running;
tcb *ready;
