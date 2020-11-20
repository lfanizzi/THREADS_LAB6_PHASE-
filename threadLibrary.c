#include "ud_thread.h"

extern tcb *running;
extern tcb *ready;

void t_init(){
    tcb *main;
    main = malloc(sizeof(tcb));

    main->thread_id = 0;
    main->thread_priority = 1;
    main->thread_context = malloc(sizeof(ucontext_t));
    getcontext(main->thread_context); //Context is that of the current user thread.
    main->next = NULL;

    running = main;
    ready = NULL;
}

void t_shutdown(){
    tcb *current = ready;
    tcb *previous;

    while (current != NULL){
        free(current->thread_context);
        previous = current;
        current = current->next;
        free(previous);
    }

    free(running->thread_context);
    free(running);
}

int t_create(void (*func)(int), int threadId, int priority){

    tcb *new = malloc(sizeof(tcb));
    size_t stackSize = 0x10000;

    new->thread_id = threadId;
    new->thread_priority = priority;
    new->thread_context = malloc(sizeof(ucontext_t));
    new->next = NULL;

    getcontext(new->thread_context);//Must call getcontext before calling makecontext!!

    new->thread_context->uc_stack.ss_sp = malloc(stackSize);
    new->thread_context->uc_stack.ss_size = stackSize;
    new->thread_context->uc_stack.ss_flags = 0;
    makecontext(new->thread_context, func, 1, threadId);

    addToReadyList(new);

    return 0;
}

void t_terminate(){
    tcb *tmp = running;
    //mbox_destroy(&tmp->mailbox);
    free(tmp->thread_context->uc_stack.ss_sp);
    free(tmp->thread_context);
    free(tmp);

    getNextReady();

    if (running != NULL) {
        setcontext(running->thread_context);
    }

}

void t_yield(){
    tcb *previousRunning;
    previousRunning = running;
    
    getNextReady();
    addToReadyList(previousRunning);

    // printf("runningId: %d\n", running->thread_id);
    // for(tcb *temp = ready; temp != NULL; temp = temp->next){
    //     printf("readyId: %d\n", temp->thread_id);
    // }

    printf("end\n");
    swapcontext(previousRunning->thread_context, running->thread_context);
}

void getNextReady(){
    if (ready != NULL) {
        running = ready;
        ready = ready->next;
        running->next = NULL;
    }
}

void addToReadyList(tcb * threadNode){

    threadNode->next = NULL;

    if(ready == NULL){
        ready = threadNode;
    }else{

         if (threadNode->thread_priority < ready->thread_priority) {
            threadNode->next = ready;
            ready = threadNode;
        } else {

            tcb *temp = ready;
            while (temp->next != NULL && threadNode->thread_priority >= temp->next->thread_priority){
                temp = temp->next;
            }
            temp->next = threadNode;
        }
    } 

}