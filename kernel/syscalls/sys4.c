#include <syscall.h>
#include <localFunc.e>
#include <types.h>
#include <scheduler.h>
#include <debug.h>
extern globThreadInfo   gThreadInfo;

/*  Syscall 4:  a dispatcher will call this function in the case of the value in 
    the register a0 is PASSEREN ( = 4 ).
    The Syscall 4 is used to perform a P operation on a semaphore, whose address
    is saved in the register a1. */

void __sys4(void){

    state_t *oldArea;
    oldArea = (state_t *)SE_OLDAREA;
    
    __kern_P(gThreadInfo.currThread, (int *)oldArea->s_a1);
}


