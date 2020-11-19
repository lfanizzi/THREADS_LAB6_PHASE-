
#include "sh.h"

int noclobber(int no_clobber){
    printf("Executing built-in [noclobber]\n");
    if(no_clobber == 1){
        no_clobber = 0;
    }
    else{
        no_clobber = 1;
    }
    printf("%d\n", no_clobber);
    return no_clobber; 
}

