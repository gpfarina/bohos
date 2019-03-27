#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#define SCHED_REASON_INTRPT       0
#define SCHED_REASON_TICKT        1
#define SCHED_REASON_QUANTUM      2
#define SCHED_REASON_SYSC         3
#define SCHED_REASON_FRMMAIN      4
#define SCHED_REASON_PASSUPFAILED 5

#define SCHED_QUANTUMUS           5000

#define INCPC                     1
#define NOINCPC                   0

#include <tcb.h>


typedef struct globThreadInfo{
    unsigned int threadCount;
    unsigned int softBlockCount;
    unsigned short int  lastThreadPreempted;
    pQueue  readyQueue;
    tcb_t*  currThread;
    int initTime;
} globThreadInfo;

#endif
