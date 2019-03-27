#ifndef _TIMER_H_
#define _TIMER_H_

#include <const.h>
#include <scheduler.h>

#define GETCYCLES(T) ((* ((cpu_t *) TIMESCALEADDR)) * (T))
#define DEFCYCLES (GETCYCLES(SCHED_QUANTUMUS))
#define TOD ((* ((cpu_t *) TODLOADDR)) / (* ((cpu_t *) TIMESCALEADDR)))

#define TIMERREGADDR        0x10000020

typedef struct globTimerInfo{
    int timerSem;
    int lastPseudoClockTick;
}globTimerInfo;

#endif

