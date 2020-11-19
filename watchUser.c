#include "sh.h"
#include <utmpx.h>
#include <unistd.h>

extern linkedList *listOfUsersHead;

void *watchUser(){
    while (1){
        sleep(20);

        struct utmpx *up;
        linkedList *temp = listOfUsersHead;
        char *name;

        while (temp != NULL){
            name = temp->name;
            setutxent();
            up = getutxent();
            while (up != NULL){
                if (strcmp(up->ut_user, name) == 0 && temp->isOn == ON && up->ut_type == USER_PROCESS){
                    if(up->ut_host[0] == '\0'){
                        printf("\n%s has logged on %s from ----\n", up->ut_user, up->ut_line); 
                    }else{
                        printf("\n%s has logged on %s from %s\n", up->ut_user, up->ut_line, up->ut_host);
                    }
                }
                up = getutxent();
            }

            temp = temp->next;
        }
    }
}
