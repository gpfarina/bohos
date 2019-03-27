#include <timer.h>
#include <types.h>
#include <scheduler.h>
#include <localFunc.e>
#include <tcb.e>
#include <asl.e>

extern globThreadInfo gThreadInfo;
extern globTimerInfo  gTimerInfo;
void __sys7(void){

    __kern_P(gThreadInfo.currThread, &(gTimerInfo.timerSem));

}
