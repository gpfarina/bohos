#include <tcb.e>
#include <tcb.h>
#include <asl.e>
#include <types.h>
#include <syscall.h>
#include <scheduler.h>
#include <main.h>
#include <devices.h>
#include <localFunc.e>
#include <debug.h>

extern  int             termSem[8][2];
extern  int             termFlag[8][2];
extern  int             termBuffs[8][2];
extern  globThreadInfo  gThreadInfo;

int __sys8(int a1, int a2, int a3){
    if(a1==LINETERMINAL){
        if(termSem[a2][a3]<=0){
            __kern_P(gThreadInfo.currThread, &termSem[a2][a3]);
        }
        else{ 
            /*interrupt has not been received yet*/
            if(!termFlag[a2][a3]){
                __kern_P(gThreadInfo.currThread, &termSem[a2][a3]);
            }
            else{
                termFlag[a2][a3]    =   0;
                gThreadInfo.currThread->proc_state.s_v0  =   termBuffs[a2][a3];        
            }
        }
    }
    else{
    /*Not implemented yet...*/
    
    }
    return 0;
}


