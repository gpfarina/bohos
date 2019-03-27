#include <tcb.e> 
#include <tcb.h>
#include <const.h>
#include <types.h>
#include <scheduler.h>

tcb_t   TCBLIST[MAXTHREADS];
HIDDEN tcb_t   *freeTcbList;
HIDDEN  uint    availableTcb;

/*
 * Intializes the pointer to free tcb list to the first
 * element of the tcb array (TCBLIST), then it links all the elements
 * in the array using t_next_l and t_next_r;
 */	

void initTcb(void){
    register int i;
    availableTcb = MAXTHREADS;
    freeTcbList = TCBLIST;
    for(i=0; i<MAXTHREADS-1; ++i){	
        TCBLIST[i].t_next_r = &TCBLIST[i+1];
        TCBLIST[i+1].t_next_l = &TCBLIST[i];	
    }
    TCBLIST[0].t_next_l = &TCBLIST[MAXTHREADS-1];
    TCBLIST[MAXTHREADS-1].t_next_r = TCBLIST;
}


/*
 * First of all the funcion checks if a tcb block is available.
 * We use a dummy tcb block to make this check. In fact if the fields
 * t_next_l and t_next_r point to the dummy block there is not an available
 * block so allocTcb returns NULL.
 * Otherwise opportunely sets link pointers fields in order to mantain the list
 * consistent. Then i returns the pointer to an unused tcb.
 * The fields whose tcb is returned are setted to default values (NULL, 0).
 */

tcb_t *allocTcb(void){
    tcb_t *retTcb;
    register int i;

    if(!availableTcb)
        return NULL;

    retTcb = freeTcbList;

    freeTcbList->t_next_r->t_next_l = freeTcbList->t_next_l;
    freeTcbList->t_next_l->t_next_r = freeTcbList->t_next_r;
    freeTcbList = freeTcbList->t_next_r;


    availableTcb--;
    for(i=0;i<SYSBRTYPENUMBER; i++)
        retTcb->sys5flag[i] = 0;
    retTcb->t_next_l =
        retTcb->t_next_r =
        retTcb->t_father = NULL; 
    retTcb->t_semAdd = NULL;
    retTcb->priority = 0;
    retTcb->cputime = 0;
    retTcb->quantum = SCHED_QUANTUMUS;

    return retTcb;
}


/*
 * Puts the tcb pointed by toFree in the free tcb list.
 * The last tcb that is returned to the free list is the next
 * that allocTcb will return.
 */

void freeTcb(tcb_t *toFree){
    if(availableTcb){
        toFree->t_next_r = freeTcbList->t_next_r;
        freeTcbList->t_next_r->t_next_l = toFree;
        freeTcbList->t_next_r = toFree;
        toFree->t_next_l = freeTcbList;
        freeTcbList = freeTcbList->t_next_r;
        }
    else{
        toFree->t_next_r = toFree;
        toFree->t_next_l = toFree;
        freeTcbList = toFree;
    }
    availableTcb++;
}


