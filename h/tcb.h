#ifndef _TCB_H_
#define _TCB_H_

#include <const.h>
#include <types.h>

typedef struct tcb_t{
    struct tcb_t *t_next_l, *t_next_r,
                 /*pointers needed for priority queue (heap)*/
                 *t_father;
    int priority;
    state_t proc_state;
    int *t_semAdd;
    /* Stores the time of cpu for wich the thread has been executed for */
    unsigned long int   cputime;

    /*This represent the remaining quantum of the Thread*/
    unsigned short int    quantum;

    /*This fields are needed for sys5 handling and passing up*/
    unsigned short int sys5flag[SYSBRTYPENUMBER]; 
    state_t *sys5OldArea[SYSBRTYPENUMBER], *sys5NewArea[SYSBRTYPENUMBER];

}tcb_t;

typedef tcb_t*  pQueue;

#endif
