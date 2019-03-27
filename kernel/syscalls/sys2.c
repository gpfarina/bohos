#include <scheduler.h>
#include <localFunc.e>
#include <libumps.e>

extern globThreadInfo gThreadInfo;


/*  Syscall 2:  The dispatcher will call this function in the case of the value in 
    the register a0 is TERMINATEPROCESS ( = 2 ).
    The Syscall 2 is simply used to terminate a process. */

void __sys2(void){

    if (gThreadInfo.currThread == NULL)
        PANIC();

    __kern_kill(&(gThreadInfo.currThread));
}

