#include <tcb.e>
#include <tcb.h>
#include <asl.e>
#include <types.h>
#include <syscall.h>
#include <scheduler.h>
#include <timer.h>

extern  globThreadInfo  gThreadInfo;

void __sys6(void){
    
    gThreadInfo.currThread->proc_state.s_v0 = 
        gThreadInfo.currThread->cputime +
        (TOD - gThreadInfo.initTime);

}
