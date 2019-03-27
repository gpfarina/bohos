#include <threadq.e>
#include <tcb.e>
#include <tcb.h>
#include <types.h>
#include <const.h>


/* 
 * Returns a pointer to an empty queue: NULL.
 */

tcb_t *mkEmptyThreadQ(void){
    return NULL;
}


/*
 * Returns true if the queue whose tail pointer is *tp is empty, i.e. if the 
 * tail pointer is NULL.
 */

int emptyThreadQ(tcb_t *tp){
    return (tp == NULL);
}


/*  
 * Inserts the thread block *t_ptr at the back of the thread queue whose tail
 * pointer is *tp, modifying the values of the pointer t_next_l and t_next_r.
 */

void insertBackThreadQ(tcb_t **tp, tcb_t *t_ptr){
    if (emptyThreadQ(*tp)){
        *tp = t_ptr;
        t_ptr->t_next_r=t_ptr;
        t_ptr->t_next_l=t_ptr;
    }
    else{
        t_ptr->t_next_r=(*tp)->t_next_r;
        (*tp)->t_next_r->t_next_l = t_ptr;
        (*tp)->t_next_r=t_ptr;
        t_ptr->t_next_l=(*tp);
        *tp=t_ptr;
    }
}


/*  
 * Inserts the thread block *t_ptr at the front of the thread queue (before the 
 * tail) whose tail pointer is *tp, modifying the values of the pointer t_next_l
 * and t_next_r.
 */

void insertFrontThreadQ(tcb_t **tp,tcb_t *t_ptr){
    if (emptyThreadQ(*tp)){
        *tp = t_ptr;
        t_ptr->t_next_r=t_ptr;
        t_ptr->t_next_l=t_ptr;
    }
    else{
        t_ptr->t_next_r=(*tp)->t_next_r;
        (*tp)->t_next_r->t_next_l = t_ptr;
        (*tp)->t_next_r=t_ptr;
        t_ptr->t_next_l=(*tp);
    }
}


/*  
 * Removes the thread block at the front of the queue whose tail pointer is *tp
 * without freeing it.
 * Returns NULL if the queue is empty. 
 */

tcb_t * removeThreadQ(tcb_t **tp){
    tcb_t *ret;
    
    if (emptyThreadQ(*tp)) 
        ret = NULL;
        
    /* the queue has only an element */
    else if (((*tp) == (*tp)->t_next_l) && ((*tp) == (*tp)->t_next_r)){
        ret = *tp;
        *tp = NULL;
    }
    /* general case */
    else{        
        ret = (*tp)->t_next_r;
        (*tp)->t_next_r = (*tp)->t_next_r->t_next_r;
        (*tp)->t_next_r->t_next_l = (*tp);
    }
    return ret;
}


/*
 * If *t_ptr is not in the queue (or if the queue is empty) returns NULL.
 * Otherwise removes the thread block *t_ptr from the queue whose tail pointer
 * is *tp and returns it.
 */
 
tcb_t* outThreadQ(tcb_t **tp, tcb_t *t_ptr){
    tcb_t *ret,*tTmp;

    if(emptyThreadQ(*tp))
        ret = NULL;
    else if(*tp == t_ptr)
        /* the tail is the element to remove */
        ret = removeThreadQ(tp);
    else{
        if(((*tp)->t_next_r == (*tp)->t_next_l ) && (*tp)->t_next_r == *tp){
            /* the queue contains only an element: obviously the tail */
	    if(*tp == t_ptr){
                ret = *tp;
                *tp = NULL;
            }
            else
                ret = NULL;
        }
        else{
            /* general case: we scan the list until tTmp points to the element to remove (t_ptr) or it is not in the queue */
            tTmp = (*tp)->t_next_r;
            while(tTmp != t_ptr && tTmp != *tp)
                tTmp = tTmp->t_next_r;
            if(tTmp == t_ptr){
                if(*tp == t_ptr)
                    *tp = tTmp->t_next_l;

                ret = tTmp;
                tTmp->t_next_r->t_next_l = tTmp->t_next_l;
                tTmp->t_next_l->t_next_r = tTmp->t_next_r;

            }
            else
                ret = NULL;
       }
    }
    return ret;

}


/*
 * Returns the thread block at the front of the queue whose tail pointer is *tp,
 * without removing it.
 */
 
tcb_t * headThreadQ(tcb_t *tp){
    if (emptyThreadQ(tp))
        return NULL;
    return (tp->t_next_r);
}
