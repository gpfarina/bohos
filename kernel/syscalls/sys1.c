#include <types.h>
#include <tcb.h>
#include <scheduler.h>
#include <syscall.h>
#include <common.e>
#include <threadpq.e>
#include <tcb.e> 

extern globThreadInfo   gThreadInfo;



/*  Syscall 1: a dispatcher will call this function if the value in a0  
    a0 is CREATETHREAD ( = 1 ).
    The Syscall 1 causes the creation of a new process, that will start with the 
    priority saved in the register a2 and the processor state saved in a1. */

void __sys1(void){
    tcb_t *newProc;
    state_t *oldArea;

    oldArea = (state_t *)SE_OLDAREA;

    newProc = allocTcb();
    if (newProc == NULL){
    /*  In this case there are no free TCBs, and a new process can't be created */
        oldArea->s_v0 = -1;
    }
    else{
        newProc->priority = oldArea->s_a2;
        __kern_memcpy((void *)&(newProc->proc_state),(void*) oldArea->s_a1, sizeof(state_t));
        insertThreadPQ(&(gThreadInfo.readyQueue), newProc); /*da verificare l'utilizzo dei puntatori*/
        gThreadInfo.threadCount++;
        oldArea->s_v0 = 0; /* Everything is right */
    }

}

