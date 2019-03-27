#include <debug.h>
#include <tcb.h>
#include <main.h>
#include <asl.h>
#include <localFunc.e>
#include <threadpq.e>
#include <asl.e>
#include <scheduler.h>
#include <tcb.e>
#include <scheduler.e>
#include <libumps.e>
#include <localFunc.e>

extern globThreadInfo gThreadInfo;
/*in attesa di creare un localFunc.h dichiaro qua alcune funzioni*/

void __kern_kill(tcb_t** thread){
    gThreadInfo.threadCount--;
    freeTcb(*thread);
    *thread = NULL;
}

/*  Execute a V operation on the semaphore pointed by semAdd. */
int __kern_V(int *semAdd){
    register short int unblockedsomeone;
    
    if(headBlocked(semAdd)!= NULL){
    
        insertThreadPQ(&(gThreadInfo.readyQueue), removeBlocked(semAdd));
        gThreadInfo.softBlockCount--;
    
        unblockedsomeone = 1;
    }
    else 
        unblockedsomeone = 0;
        
    (*semAdd)++;
        
    return unblockedsomeone;
}
/*  Execute a P operation on the semaphore pointed by semAdd. */
void __kern_P(tcb_t *t_ptr, int *semAdd){	
    if(*semAdd<=0){
        if(insertBlocked(semAdd, t_ptr))
            /*no more semd_t to alloc*/
            PANIC();
        gThreadInfo.softBlockCount++;
        outThreadPQ(&gThreadInfo.readyQueue, t_ptr);  
    }
    (*semAdd)--;

}



