#include "thread.h"
//#include "sh.h"

	void t_init(){
    tcb main;
    running = malloc(sizeof(tcb));
    running[1] = main;
	}
