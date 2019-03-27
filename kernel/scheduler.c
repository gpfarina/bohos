#include <tcb.h>
#include <types.h>
#include <const.h>
#include <scheduler.h>
#include <interrupts.h>
#include <timer.h>
#include <syscall.h>
#include <tcb.e>
#include <common.e>
#include <scheduler.e>
#include <libumps.e>
#include <threadpq.e>
#include <libumps.e>
#include <timer.e>
#include <asl.e>
#include <debug.h>
/* Global info about threads (ready queue, counters, running thread etc...
 */
globThreadInfo   gThreadInfo;
/*kernel mode flag*/
extern globTimerInfo    gTimerInfo;
extern int f;
static int LILLI;
/* local definitions */
HIDDEN void __sched_aging();
HIDDEN void __sched_saveCpuStateToTcb(int, unsigned int);
HIDDEN void __sched_saveThreadTimes(tcb_t * ,int );
HIDDEN void __sched_scheduleNewFromReadyPQ(void);
static int cielo;
void scheduler(int reason){
    int schedanother;
    /* flag indicating if we are returning in the scheduler because an interrupt
     * has called the intHandler and we was in the scheduler (while(1) 
     * twiddling) , so we know that we could schedule someone other else
     */
    static int intinsched;

    schedanother = 0;
    switch(reason){
        case 42:
                schedanother = 0;
                cielo+=1;
                break;
        case SCHED_REASON_INTRPT:
            if(!emptyThreadPQ(gThreadInfo.readyQueue) && intinsched){
                schedanother = 1;
                intinsched = 0;
            }
            break;
        case SCHED_REASON_TICKT:
            schedanother = 1;          
            if(!__sched_iskilled(gThreadInfo.currThread)){
                    __sched_saveThreadTimes(gThreadInfo.currThread,
                         SCHED_QUANTUMUS);
                if(!intinsched)
                    __sched_saveCpuStateToTcb(0, IE_OLDAREA);
                if(!__sched_isblocked(gThreadInfo.currThread))
                    insertThreadPQ(&gThreadInfo.readyQueue , 
                        gThreadInfo.currThread);
            }
            break;
        case SCHED_REASON_SYSC:
           if (__sched_iskilled(gThreadInfo.currThread))
               schedanother = 1;
           else if( __sched_isblocked(gThreadInfo.currThread)){    
            
                __sched_saveThreadTimes(gThreadInfo.currThread, 
                        SCHED_QUANTUMUS - (TOD - gThreadInfo.initTime));
                schedanother = 1;
           }
           break;
        case SCHED_REASON_PASSUPFAILED:
        case SCHED_REASON_FRMMAIN:
            schedanother = 1;
            break;
    }

    if(schedanother){
        if(emptyThreadPQ(gThreadInfo.readyQueue) 
            && reason != SCHED_REASON_FRMMAIN
&& (__sched_iskilled(gThreadInfo.currThread) || __sched_isblocked(gThreadInfo.currThread))){ 
            if(!gThreadInfo.threadCount)
                HALT();
            else if(gThreadInfo.softBlockCount == 0){
                LILLI = 0xbeef;
                PANIC();
            }
            else{

                intinsched = 1;
                __timer_ackAndSetTimerTick(CLOCKINTERVAL -
                        (TOD - gTimerInfo.lastPseudoClockTick));
                if(headBlocked(&gTimerInfo.timerSem) != NULL){     
                    setSTATUS(getSTATUS() | SET_INTMASKENABLED | 0xFF01);
                }
                else{
                  setSTATUS((getSTATUS() | SET_INTMASKENABLED | 0xFF01 )&0xFB01);
                }
                while(1)
                    ;
            }
        }
        else{
            __sched_scheduleNewFromReadyPQ();
        }
    
    }
    __sched_aging();
    LDST(&(gThreadInfo.currThread->proc_state))  ;
}

HIDDEN void __sched_aging(){
}

HIDDEN void __sched_saveCpuStateToTcb(int incPCflag, unsigned int  AREA){
    state_t *oldArea = (void *)AREA;
    
    __kern_memcpy((void *) (&gThreadInfo.currThread->proc_state), 
            (void*)oldArea, sizeof(state_t));

    /*do not increment also t9, for architectural reasons*/
}

HIDDEN void __sched_saveThreadTimes(tcb_t * thread,int newQuantum){

        thread->cputime += TOD-gThreadInfo.initTime;
        if(newQuantum <= 0)
            newQuantum = SCHED_QUANTUMUS;
        thread->quantum = newQuantum;
}

HIDDEN void __sched_scheduleNewFromReadyPQ(){

    gThreadInfo.currThread = removeThreadPQ(&(gThreadInfo.readyQueue));
 
    STCK(gThreadInfo.initTime); 
    __timer_ackAndSetTimerTick((gThreadInfo.currThread)->quantum);
}

inline int __sched_iskilled(tcb_t* thread){
    return thread == NULL;
}

inline int __sched_isblocked(tcb_t* thread){
    return thread->t_semAdd != NULL;
}

void __sched_initSchedStruct(state_t* startState){

    /*Create the ready queue*/
    gThreadInfo.readyQueue =   mkEmptyThreadPQ();
    /*creating a thread...*/
    gThreadInfo.currThread   =   allocTcb();


    /* To copy structs we don't use a direct assignement instruction,
     * but we use __kern_memcpy.
     * This in order to avoid references to memcpy of libc by gcc
     */
    __kern_memcpy((void *)(&gThreadInfo.currThread->proc_state), (void *)(startState), sizeof(state_t));

    /*at the beginning this is the only thread in the ready queue*/
    insertThreadPQ(&(gThreadInfo.readyQueue), gThreadInfo.currThread);
    gThreadInfo.threadCount     =   1;
    gThreadInfo.softBlockCount  =   0;

 
}

