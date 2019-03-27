#include <debug.h>
#include <types.h>
#include <timer.h>
#include <scheduler.h>
#include <scheduler.e>
#include <timer.e>
#include <localFunc.e>
#include <tcb.e>
#include <asl.e>
static int valore;
struct globTimerInfo gTimerInfo;
int TimerInterruptHandler(void){
    /* CLOCKINTERVAL: e' definita come i 100 millisecondi */
    if ( (TOD - gTimerInfo.lastPseudoClockTick) >= CLOCKINTERVAL ) {
          valore = TOD - gTimerInfo.lastPseudoClockTick;
          while (__kern_V(&gTimerInfo.timerSem)); 
    /* Il semaforo non deve MAI restare a 1 altrimenti il prossimo 
    proc che chiama la sys7 non si blocca */

        gTimerInfo.timerSem = 0;
        STCK(gTimerInfo.lastPseudoClockTick);
        gTimerInfo.lastPseudoClockTick -=((TOD - gTimerInfo.lastPseudoClockTick)
            - CLOCKINTERVAL); 
    }

    return SCHED_REASON_TICKT;
}


void __timer_initTimerStruct(void){

    gTimerInfo.timerSem = 0;
    STCK(gTimerInfo.lastPseudoClockTick);
    
}

/*Thi function set the next tick of the timer to happen in timeus
 * microseconds, doing so the timer interrupt line is also acked
 */

void __timer_ackAndSetTimerTick(unsigned int timeus){
    memaddr* timerRegister;

    timerRegister = (memaddr *)TIMERREGADDR;
    *timerRegister = GETCYCLES(timeus);
}


