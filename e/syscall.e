#include <tcb.h>

extern void syscDispatcher(void);
extern int __sysc_testPassingUp(tcb_t* currThread,int syscnum);


