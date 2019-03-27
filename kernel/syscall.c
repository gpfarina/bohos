#include <main.h>
#include <interrupts.h>
#include <scheduler.h>
#include <syscall.h>
#include <types.h>
#include <const.h>
#include <tcb.h>
#include <sys1.e>
#include <sys2.e>
#include <sys3.e>
#include <sys4.e>
#include <sys5.e>
#include <sys6.e>
#include <sys7.e>
#include <sys8.e>
#include <syscall.e>
#include <libumps.e>
#include <localFunc.e>
#include <scheduler.e>
#include <common.e>

extern  globThreadInfo  gThreadInfo;
extern  void pgmTrapHandler(void);

/*We use this function just in this file so we declare them static*/
static  void __do_bkOrSys9(int cause);
static  void __do_syscall(void);
static  void __do_dispatch(int servNo);


/*   With this instruction we can reach the OLDAREA of system calls
     simply using a pointer to a state_t struct
*/
static state_t *oldArea =   (state_t *)SE_OLDAREA;
/*
    This is the function that dispatches between
    all the system call.
*/
static int qui;
extern int p5bbb;
void syscDispatcher(void){
 /* We use this flag to check if we were in kernel mode
    before that the dispatcher was called in this case
    something is wrong.
    */
    /*With this instruction we avoid to fall in an infinite loop*/
    oldArea->s_pc+=WORDLEN;
    __kern_memcpy((void*) (&gThreadInfo.currThread->proc_state), 
                            (void *)SE_OLDAREA, sizeof(state_t));
    if(GET_EXCCODE(oldArea->s_cause)==CAUSE_SYSCALL)
            __do_syscall();
    else if(GET_EXCCODE(oldArea->s_status)==CAUSE_BREAK)
            __do_bkOrSys9(CAUSE_BREAK);

    scheduler(SCHED_REASON_SYSC);
}
void __do_syscall(void){
        int servNo;
        servNo  =   oldArea->s_a0;
        if(!IS_INDEXOK(servNo)){
            __kern_kill(&gThreadInfo.currThread);    
            return;
            }   
        if(servNo == USERDEF_SYSCALL)
             __do_bkOrSys9(CAUSE_SYS9);
 
        if(GET_PREVIOUSMODE(oldArea->s_status)==KERNEL_MODE)
                __do_dispatch(servNo);
        else{
            /*We copy from oldarea of syscall&bk to pgmtrap oldarea*/
            __kern_memcpy((void *)PGME_OLDAREA, (void*)SE_OLDAREA, 
                                                sizeof(state_t));
            /*Then we set the RESERVED INSTRUCTION flag*/
            ((state_t *)PGME_OLDAREA)->s_cause &= 0xB000FF28;
            ((state_t *)PGME_OLDAREA)->s_cause |= RESERVED_INSTRUCTION;
            pgmTrapHandler();
        }
}
void __do_bkOrSys9(int cause){
    /*if(cause == CAUSE_SYS9){*/
        if(!__sysc_testPassingUp(gThreadInfo.currThread, SYSTRAP))
            __kern_kill(&gThreadInfo.currThread);
        else{
            __kern_memcpy((void *)gThreadInfo.currThread->sys5OldArea[SYSTRAP], 
                                        (void *)SE_OLDAREA, sizeof(state_t));
            LDST(gThreadInfo.currThread->sys5NewArea[SYSTRAP]);
        }
   /* }
    else{
        What we have to do in case of a break instruction?
    }*/
}
void __do_dispatch(int servNo){
    switch(servNo){
        case CREATETHREAD:
            __sys1();
            break;
        case TERMINATETHREAD:
            __sys2();
            break;
        case VERHOGEN:
            __sys3();
            break;
        case PASSERN:
            __sys4();
            break;
        case SPECTRAPVEC:
            __sys5();
            break;
        case GETCPUTIME:
            __sys6();
            break;
        case WAITCLOCK:
            __sys7();
            break;
        case WAITIO:
            __sys8(oldArea->s_a1, oldArea->s_a2, oldArea->s_a3);
            break;
    }
}

/* Test for passing up, returns true if the thread has specifyed an handler 
 * for that syscall type
 */

int __sysc_testPassingUp(tcb_t* currThread,int numTrap){
        return currThread->sys5flag[numTrap];
}


