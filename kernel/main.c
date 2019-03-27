#include <const.h>
#include <types.h>
#include <tcb.h>
#include <main.h>
#include <syscall.h>
#include <interrupts.h>
#include <scheduler.h>
#include <libumps.e>
#include <asl.e>
#include <common.e>
#include <threadpq.e>
#include <tcb.e>
#include <scheduler.e>
#include <interrupts.e>
#include <devices.e>
#include <main.e>
#include <syscall.e>
#include <debug.h>
#include <timer.e>
#include <localFunc.e>


extern globThreadInfo gThreadInfo;

/*test from p2test.c*/
extern  void test(void);
int main(void){
    state_t     startState;

    /* Init level2 support structures, semaphores, thread block descriptor*/
    initTcb();          
    initASH();
	/* populating 4 new areas, sysc/bp exceptions, interrupts, pgmg and tlb
     *  exceptions
     */
    __kern_popRomAreas();      
    

    /*creating a new processor state*/
    startState.s_pc     =  (memaddr)  test;     
    /*we have to do this... for architectural reasons*/
    startState.s_t9     =  (memaddr)  test;   
    startState.s_status =   0xFF01;
    startState.s_sp     =   RAMTOP - PAGESIZE;   

    /*Init the scheduler with a new thread, the p2test*/
    __sched_initSchedStruct(&startState);
    /*Init timer structures, semaphore, lasttickcount*/
    __timer_initTimerStruct(); 
    /*Init devices semaphores and relative structures*/
    __devices_initDevicesStruct();
    
    /* the inizialization phase is terminated so we call the scheduler.
     * The control will never return back to this point so we put here
     * a PANIC() instruction (so if something goes wrong we know)  
     */
    scheduler(SCHED_REASON_FRMMAIN);
    PANIC();
    
    /*just to avoid an annoing warning*/
    return 0;
}


/*  In this function we populate the new areas from which the ROM have to load
    a procesor state whenever an exception is raised.
*/

void __kern_popRomAreas(void){
    __initrom_initRomHandler((state_t*)SE_NEWAREA , (memaddr)syscDispatcher);
    __initrom_initRomHandler((state_t*)IE_NEWAREA , (memaddr)intDispatcher);
    __initrom_initRomHandler((state_t*)TLBE_NEWAREA , (memaddr)tlbHandler);
    __initrom_initRomHandler((state_t*)PGME_NEWAREA , (memaddr)pgmTrapHandler);

}

void __initrom_initRomHandler(state_t* areaAddr, memaddr handler){

    STST(areaAddr);
    areaAddr->s_pc = areaAddr->s_t9 = (memaddr) handler;
    areaAddr->s_sp=RAMTOP;
    areaAddr->s_status= EXCEPTIONFLAGS;
}

/*Dummy functions.
  We use these function just to see if all compiles and links*/
void tlbHandler(void){

    if(__sysc_testPassingUp(gThreadInfo.currThread, TLBTRAP)){
        __kern_memcpy(gThreadInfo.currThread->sys5OldArea[TLBTRAP],
                (void *)TLBE_OLDAREA, sizeof(state_t));
        LDST(gThreadInfo.currThread->sys5NewArea[TLBTRAP]);
    }
    else{
        __kern_kill(&(gThreadInfo.currThread));
        scheduler(SCHED_REASON_PASSUPFAILED);
    }
}
void pgmTrapHandler(void){
    if(__sysc_testPassingUp(gThreadInfo.currThread, PROGTRAP)){
        __kern_memcpy(gThreadInfo.currThread->sys5OldArea[PROGTRAP],
                (void *)PGME_OLDAREA, sizeof(state_t)); 
        LDST(gThreadInfo.currThread->sys5NewArea[PROGTRAP]);
    }
    else{
        __kern_kill(&(gThreadInfo.currThread));
        scheduler(SCHED_REASON_PASSUPFAILED);
    }
       
}




