#include <asl.h>
#include <const.h>
#include <types.h>
#include <threadq.e>
#include <asl.e>


/*We use a macro so we can change hash function just modifing
 *  * this line*/
#define __asl_hash(v)     (__asl_prettyGoodHash(v) % ASHSIZE)


HIDDEN semd_t SemTable[MAXSEM];
HIDDEN semd_t *ASH[ASHSIZE];
HIDDEN semd_t *headFreeList; /* pointer to head of SemFreeList */

/* private function declarations */
HIDDEN inline unsigned int __asl_prettyGoodHash(int *);

/* 
 * Init Function. It initializes the semdFree list, pointed by h, to contain all
 * the elements ofthe array SemTable.
 * This method will be only called once during
 * data structure initialization.   
 */

void initASH(void){
    register int i;

    headFreeList = SemTable;
    for (i=0; i<ASHSIZE; ++i)
        ASH[i]=NULL;      

    /* concatenating the free list*/
    for (i=0; i<MAXSEM-1; i++)
	    SemTable[i].s_next=&SemTable[i+1];
    SemTable[MAXSEM-1].s_next = NULL;
}


/* 
 * Returns a pointer to the ThreadBlk that is at the head of the process
 * queue associated with the semaphore semAdd. Return NULL if semAdd is not
 * found on the ASH or if the process queue associated with semAdd is empty.
 * Using the __pscanListHash function, that return a pointer to the pointer to
 * that particular SemDescriptor.
 */

tcb_t *headBlocked(int *semAdd){
    semd_t **q;

    if(semAdd != NULL){
        q = __asl_pscanListHash(semAdd);

        if (*q==NULL)
     	    return NULL;
        if (emptyThreadQ((*q)->s_threadQ))
     	    return NULL;
        else
     	    return headThreadQ((*q)->s_threadQ);
    }else
        return NULL;

}


/* 
 * Search the ASH for a descriptor of this semaphore. If none is found,
 * return NULL; otherwise, remove the first (i.e. head) ThreadBlk from the
 * process queue of the found semaphore descriptor and return a pointer 
 * to it. If the process queue for this semaphore becomes empty 
 * (emptyProcQ(s procq) is TRUE), remove the semaphore descriptor from
 * the ASH and return it to the semdFree list.
 */

tcb_t *removeBlocked(int *semAdd){
    semd_t **q;
    semd_t *k = *q;
    tcb_t *removed;

    q = __asl_pscanListHash(semAdd);
    k = *q;
    if (*q==NULL)
        /*semaphore not found in the ash*/
        return NULL;
    else{
 	    removed = removeThreadQ(&(*q)->s_threadQ);
        if(removed != NULL)
            removed->t_semAdd = NULL;
    }

    /*the queue is empty so remove the descriptor*/
    if (emptyThreadQ((*q)->s_threadQ)){
 	    *q = (*q)->s_next; 
	    k->s_next = headFreeList;
	    headFreeList = k;
    }

    return removed;
}


/* 
 * Remove the ThreadBlk pointed to by p from the process queue associated with
 * p's semaphore (p p_semAdd) on the ASH. If ThreadBlk pointed to by p does
 * not appear in the process queue associated with p's semaphore, which is
 * an error condition, return NULL; otherwise, return p.
 */

tcb_t *outBlocked(tcb_t *p){
    semd_t **t;
    tcb_t* ret;
    if(p->t_semAdd != NULL){
        t = __asl_pscanListHash(p->t_semAdd);
        if((ret=outThreadQ(&(*t)->s_threadQ, p)) != NULL)
            ret->t_semAdd = NULL;  
    }
    else ret = NULL;
    return ret;
}


/* 
 * Insert the ThreadBlk pointed to by p at the tail of the process queue
 * associated with the semaphore whose physical address is semAdd and set
 * the semaphore address of p to semAdd. If the semaphore is currently not
 * active (i.e. there is no descriptor for it in the ASH), allocate a new
 * descriptor from the semdFree list, insert it in the ASH (at the
 * appropriate position), initialize all of the fields (i.e. set s_semAdd to
 * semAdd, and s procq to mkEmptyProcQ()), and proceed as above.
 * If a new semaphore descriptor needs to be allocated and the semdFree list
 * is empty, return TRUE. In all other cases return FALSE.
 */

int insertBlocked(int *semAdd, tcb_t *p){
    semd_t *k;
    semd_t **tmp;

    tmp = __asl_pscanListHash(semAdd); 
    k = *tmp;
    /*check if to create a new semd descriptor*/
    if (*tmp == NULL){
        if (headFreeList == NULL)
            return TRUE;
        else {
    	    k = headFreeList; 
            headFreeList = headFreeList->s_next;
    	    *tmp = k;
    	    k->s_semAdd = semAdd;
        	k->s_threadQ = mkEmptyThreadQ();
    	    k->s_next = NULL;
        }
    }  
    /*effectively insert the process in the queue*/
    p->t_semAdd = semAdd;
    insertBackThreadQ(&(k->s_threadQ), p);
    return FALSE;
}


/* 
 * Scan for a Semaphore with SemAdd integer into the Hash Table and
 * the linked lists with. Return the pointer to the pointer to that
 * particular Semaphore Descriptor, so it is changeable later. NULL if
 * it isn't on the ASH table.
 */

inline semd_t** __asl_pscanListHash (int *semAdd){
    /* pointer to the firsts linked list  */
    semd_t **s;

    s = &ASH[__asl_hash(semAdd)];

    while (*s!=NULL){ 
 	    if ((*s)->s_semAdd==semAdd)
		    return s;
	    else
		    s = &((*s)->s_next);
    }

    return s; 
}


HIDDEN inline unsigned int __asl_prettyGoodHash(int *semAdd){
    
    register unsigned int a=(unsigned int)semAdd;

    a = (a+0x7ed55d16) + (a<<12);
    a = (a^0xc761c23c) ^ (a>>19);
    a = (a+0x165667b1) + (a<<5);
    a = (a+0xd3a2646c) ^ (a<<9);
    a = (a+0xfd7046c5) + (a<<3);
    a = (a^0xb55a4f09) ^ (a>>16);
    return a;

}

