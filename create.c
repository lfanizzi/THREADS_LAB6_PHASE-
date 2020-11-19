#include "thread.h"

int t_create(void (*func)(int), int thr_id, int pri, int ready_index){
   tcb new;
   new.thread_id = thr_id;
   new.thread_priority = pri;
   //new.thread_context = func;
   pthread_create(thr_id,NULL,func,pri);
   ready[ready_index] = new;
   ready_index ++;
    return 0;
}