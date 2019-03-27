#include <types.h>
#include <syscall.h>
#include <localFunc.e>
#include <debug.h>
#include <sys3.e>

/*  Syscall 3:  a dispatcher will call this function in the case of the value in 
    the register a0 is VERHOGEN ( = 3 ).
    The Syscall 3 is used to perform a V operation on a semaphore, whose address
    is saved in the register a1. */

void __sys3(void){

    state_t *oldArea;
    oldArea = (state_t *)SE_OLDAREA;
    __kern_V((int *)oldArea->s_a1);
}


