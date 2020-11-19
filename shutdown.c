#include "thread.h"
void t_shutdown(){
    free(ready);
    free(running);
}