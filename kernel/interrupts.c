#include <debug.h>
#include <types.h>
#include <const.h>
#include <interrupts.h>
#include <scheduler.h>
#include <libumps.e>
#include <devices.e>
#include <interrupts.e>
#include <scheduler.e>
#include <timer.e>


/*
    TODO:   - maybe the order of this instructions: V/ack must be exchanged.
*/

/*This is the dispather*/
static int caino;
extern globThreadInfo gThreadInfo;
void intDispatcher(void){

    state_t     *oldData;
    unsigned int    minLinePending;
    register unsigned int schedreason;


    oldData     =   (state_t *)IE_OLDAREA;
    /*If we are here means that an interrupt request has
        raised. So if CAUSE_INTERRUPT isn't setted in oldArea
        something is wrong and we go in PANIC()
    */
    if (GET_EXCCODE(oldData->s_cause) !=CAUSE_INTERRUPT)
            PANIC();      
    
    minLinePending  =   __intrpt_getMinLine(oldData->s_cause);
    
    schedreason = SCHED_REASON_INTRPT;
    switch(minLinePending){
        case 0:
            caino=0xdeadbeef;
            setCAUSE(getCAUSE()  & ~(255<<8));
            scheduler(42);
            break;
        case 1:
            caino=0xABCDEF;
            setCAUSE(getCAUSE() & ~(255<<8) );
            scheduler(42);
            break;
        case 2:
            schedreason = TimerInterruptHandler();
            break;
        case 3:
            /*disk handler*/
        case 4:
            /*tape handler*/
        case 5:
            /*net device handler*/
        case 6:
            /*printer handler*/
            break;
        case 7:
            terminalHandler();
            break;
        default:        /*Something goes wrong...*/
            caino = 0x123123;
            PANIC();
    }
    scheduler(schedreason);
}


unsigned int __intrpt_getMinLine(unsigned int cReg){
    unsigned int bits    =   GET_IPENDING(cReg);
    unsigned short int  line    =   0;

    while(!(bits&0x01)){
        line++;
        bits>>=1;
    }

    return line;
}


unsigned int __intrpt_getDevice(unsigned int line){

    unsigned int bp, devRet=0;
    /*get the base address to the word of the particular class devices line
     * where to see the devices pending active bit
     */
    bp  =   *((memaddr *) ((IPENDING_BITMAP)+(WORDLEN*(line-3))));

    /*if we have more than one interrupt pending of the same device class 
        we serve the first that we get*/

    while(!(bp&0x01)){
        devRet++;
        bp>>=1;
    }

    return devRet;

}

