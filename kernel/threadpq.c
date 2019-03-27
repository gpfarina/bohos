#include <threadpq.e>
#include <tcb.e>
#include <tcb.h>
#include <common.e>
#include <const.h>
#include <types.h>
#include <threadpq.h>

/*
 * This is the support array for thread queue heap
 */

HIDDEN  int TPQS[MAXTHREADS];


/*
 * External declarations
 */

extern tcb_t TCBLIST[MAXTHREADS];

/* Private function definitions */

HIDDEN inline uint __tpq_sOffst(tcb_t* t_ptr);
HIDDEN inline void __tpq_updateTPQSinf(tcb_t *dest,tcb_t *src);
HIDDEN int __tpq_goDown(tcb_t *tp,tcb_t **last,
        register int nNodes,BOOL insOrFind);
HIDDEN BOOL __tpq_swapNodes(tcb_t* t_ptr1, tcb_t* t_ptr2);
HIDDEN inline void __tpq_sSwapTcb(register tcb_t **a,register tcb_t **b);
HIDDEN void __tpq_sSwapL(register tcb_t *tcb_father, register tcb_t *tcb_child);
HIDDEN void __tpq_sSwapR(register tcb_t *tcb_father, register tcb_t *tcb_child);
HIDDEN inline void __tpq_unlinkNode(tcb_t *t_ptr);

/*
 * Returns a empty thread priority queue and init the TPQS support array or
 * NULL if no tcb_t are avaiable.
 */

tcb_t* mkEmptyThreadPQ(void){
    return NULL;
}


/*
 * Returns true if the priority queue whose root is pointed by tp is
 * empty.
 */

int emptyThreadPQ(tcb_t *tp){
    return (__TPQ_ISNULL(tp));
}


/*
 * Adds a node t_ptr to the priority queue pointed by tp
 */

void insertThreadPQ(tcb_t **tp, tcb_t *t_ptr){
    tcb_t *tTmp,*root;
    register uint ofst,snNodes;
    register BOOL cicle;

    root = *tp;
    if (__TPQ_ISNULL(root)){
        root = t_ptr;
        root->t_father = NULL;
        ofst = __tpq_sOffst(root);
        root->t_next_r = root->t_next_l = NULL;
        TPQS[ofst] = 1;

    }
    else{
        ofst = __tpq_sOffst(root);
        snNodes = __tpq_goDown(root,&tTmp,TPQS[ofst],TRUE);

        /*insert the new node depending on the last ,
         * most significant, bit of snNodes*/

        if ((snNodes & __TPQ_MAXAND) == __TPQ_MAXAND){
            tTmp->t_next_r = t_ptr;
        }
        else{
            tTmp->t_next_l = t_ptr;
        }

        /* Link new node */
        t_ptr->t_father = tTmp;
        t_ptr->t_next_r = t_ptr->t_next_l = NULL;

        /* from now tTmp is the node to insert*/
        tTmp = t_ptr;

        cicle = TRUE;
        /*raising phase; adjust priority(position) of nodes in the heap*/
        while ((!__TPQ_ISFATHER(NULL, tTmp)) && cicle){
            if (tTmp->priority > tTmp->t_father->priority )
                cicle = __tpq_swapNodes(tTmp , tTmp->t_father);
            else
                cicle = FALSE;
        }
        /*if t_ptr is the new root of the tpq update necessary fields*/
        if (__TPQ_ISFATHER(NULL, t_ptr)){
            __tpq_updateTPQSinf(t_ptr, root);
            root = t_ptr;
        }

        /*increase the number of nodes of the, eventually new, root */
        TPQS[__tpq_sOffst(root)]++;
    }
    *tp = root;
}


/*
 * Removes the root element from the thread priority queue,
 * it returns NULL if the queue is empty
 */

tcb_t* removeThreadPQ(tcb_t **tp){
    return outThreadPQ(tp,*tp);
}


/*
 * Removes the element pointed by t_ptr whose root is pointed by tp, if the
 * element is not in the specified priority queue it returns NULL.
 */

tcb_t* outThreadPQ(tcb_t **tp, tcb_t *t_ptr){
 /* tp is the pointer to the pointer to the root */
    BOOL swapped;
    tcb_t *tmp,*tmp2;
    int updateRoot;
    
    if (__TPQ_ISNULL(*tp))
       return NULL; 

    /* check if the thread belongs to this heap */
    tmp = t_ptr;
    if(t_ptr == *tp && __TPQ_ISNULL(t_ptr->t_next_r) && __TPQ_ISNULL(t_ptr->t_next_l)){
        __tpq_unlinkNode(t_ptr);
        *tp = NULL;
        return tmp;
    }
    while (!__TPQ_ISFATHER(NULL, tmp))
        /* going up into the heap */
        tmp = tmp->t_father;
    /* it exists! */
    if (tmp == *tp){
 
        /* check for the last inserted node. tmp will point to that */
        __tpq_goDown(*tp,&tmp,TPQS[__tpq_sOffst(*tp)],FALSE);

     
        /*from now tmp is the last node in the heap*/

        /*if the node to remove is the only node and
         * is the root update *tp */
        if ( (*tp) == tmp ){
             *tp = NULL;
             /* unlinking */
             __tpq_unlinkNode(t_ptr);
        }
        else{


            /* swap with the last inserted */
            swapped = __tpq_swapNodes(tmp,t_ptr);
           
            /* unlinking */
            __tpq_unlinkNode(t_ptr);
           
            /* if swapped, continue. else Return Ptr */
            if (swapped) {
                if (*tp == t_ptr){
                    /* update the root to the previously swapped node */
                    __tpq_updateTPQSinf(tmp,*tp);
                    *tp = tmp;
                    updateRoot = 1;
                }
                else
                    updateRoot = 0;

                /* cicle if the children exits and cicle==1 */
                while (((!__TPQ_ISCHILDDX(NULL, tmp)) || (!__TPQ_ISCHILDSX(NULL, tmp)))
                     && swapped == TRUE){
                     
                     /* assign to tmp2 the child node of tmp 
                      * with the hightest priority*/

                    /* both children exist */
                    if ((!__TPQ_ISCHILDDX(NULL, tmp)) && (!__TPQ_ISCHILDSX(NULL, tmp)))
                        /* tmp will be the pointer to the major child */
                        tmp2 = (tmp->t_next_r->priority >
                                 tmp->t_next_l->priority) ? tmp->t_next_r :
                                     tmp->t_next_l;
                    else if (!__TPQ_ISCHILDDX(NULL, tmp)
                             && tmp->t_next_r->priority > tmp->priority)
                        /* only right child exists */
                        tmp2 = tmp->t_next_r;
                    else if (tmp->t_next_l->priority > tmp->priority)
                        /* only left child exists */
                        tmp2 = tmp->t_next_l;
                    else
                        swapped = FALSE;

                    if (swapped){
                        /* swapping the parent with his tmpor child */
                        swapped = __tpq_swapNodes(tmp,tmp2);
                        /* eventually also update the root on first swap */
                        if (updateRoot){
                            __tpq_updateTPQSinf(tmp2,*tp);
                            *tp = tmp2;
                            updateRoot = 0;
                        }
                    }
                }    
            }
        }
    }
    else /* the node (t_ptr) doesn't belong to any heap */
        return NULL;

    if (!__TPQ_ISNULL(*tp))
        TPQS[__tpq_sOffst(*tp)]--;    

    return t_ptr;        
}


/*
 * Returns the pointer to the root of the priority queue pointed by tp
 */

tcb_t* headThreadPQ(tcb_t *tp){
    return tp;
}


/*
 * Returns the offset in the TPQS of the corresponding element in TCBLIST
 * NB mips memory grows to low addresses
 */

HIDDEN inline  uint __tpq_sOffst(tcb_t* t_ptr){
    return (t_ptr-TCBLIST);
}


/*
 * Update TPQS information of a node with the ones of another node (usefull
 * when a new node becomes "root" of a tpq)
 */

HIDDEN inline void __tpq_updateTPQSinf(tcb_t *dest,tcb_t *src){
    if (dest != src)
        TPQS[__tpq_sOffst(dest)] = TPQS[__tpq_sOffst(src)];
}

/*
 * given the number of nodes
 * the root and a pointer that will point to the last inserted node, it returns
 * a number that tells me where to insert (0 left, 1 right)
 */

HIDDEN int __tpq_goDown(tcb_t *tp,tcb_t **last
    ,register  int nNodes,BOOL insOrFind){

    register tcb_t *tTmp;
    register int i,log,minus;
    register  int snNodes;
    tTmp = tp;
    if (nNodes == 0)
        return -1;

    if (insOrFind){
        snNodes = nNodes = nNodes+1;
        /* we want the last not saturated level */
        minus = -1;
    }
    else{
        /* we want the last node of the last level,
         * so the number of nodes
         * must not be incremented */
        minus = 0;
        snNodes = nNodes;
    }
    /* shift for looking the most significant bit of snNodes
     * "anding" with MAXAND
     */
    while ((snNodes & __TPQ_MAXAND) != __TPQ_MAXAND)
        snNodes = snNodes << 1;

    /* skip the root*/
    snNodes = snNodes << 1;
    
    /*number of levels of the heap*/
    log =  __ulog2(nNodes);

    /* down to the node where to insert;
     * go left if the most significant bit of snNodes is 0
     * go right otherwise.
     */
    for (i=1; i<= (log+minus);i++){
        if ((snNodes & __TPQ_MAXAND) == __TPQ_MAXAND)
            tTmp = tTmp->t_next_r;
        else
            tTmp = tTmp->t_next_l;
        snNodes = snNodes << 1;
    }

    *last = tTmp;

    return snNodes;
}
 
 
/*
 * Switches (ANY) two nodes of the priority thread queue.
 * Is to note the many distintions that must be done to update
 * also fathers->child, fathers->father->child etc.. links.
 * Retrurns true if the two nodes are swapped, false otherwise.
 */
 
HIDDEN BOOL __tpq_swapNodes(tcb_t* t_ptr1, tcb_t* t_ptr2){
    BOOL ret;
    tcb_t **tTmp,**tTmp2;
 
    if(t_ptr1 != t_ptr2){
 
    /* if one of the nodes to swap is the father of the other we have to manage
     *  this case separately, using the function __sSwapL and __sSwapR.
     *  Moreover we have to use this functions with two tcbs has parameters:
     *   the first must be the father of the second 
     */
        if(__TPQ_ISFATHER(t_ptr1, t_ptr2)){
            if (__TPQ_ISCHILDSX(t_ptr2, t_ptr1)){
 
                __tpq_sSwapL(t_ptr1 , t_ptr2);
            }
            else if (__TPQ_ISCHILDDX(t_ptr2, t_ptr1)){
                __tpq_sSwapR(t_ptr1, t_ptr2);
            }
        }
        else if(__TPQ_ISFATHER(t_ptr2, t_ptr1)){
            if(__TPQ_ISCHILDSX(t_ptr1, t_ptr2)){
 
                __tpq_sSwapL(t_ptr2, t_ptr1);
 
            }
            else if (__TPQ_ISCHILDDX(t_ptr1, t_ptr2)){
                __tpq_sSwapR(t_ptr2, t_ptr1);
            }
        }
        else{
        /****   GENERAL CASE: the nodes are not relatives    ****/
            /*swap father of right child*/
            if((!__TPQ_ISCHILDDX(NULL, t_ptr1)) && (!__TPQ_ISCHILDDX(NULL, t_ptr2)))
                __tpq_sSwapTcb(&(t_ptr1->t_next_r->t_father) ,
                    &(t_ptr2->t_next_r->t_father));
            else if (__TPQ_ISCHILDDX(NULL, t_ptr1) && (!__TPQ_ISCHILDDX(NULL, t_ptr2)))
                t_ptr2->t_next_r->t_father = t_ptr1;
            else if ((!__TPQ_ISCHILDDX(NULL, t_ptr1)) && __TPQ_ISCHILDDX(NULL, t_ptr2))
                t_ptr1->t_next_r->t_father = t_ptr2;
 
            /*swap father of left child*/
            if ((!__TPQ_ISCHILDSX(NULL, t_ptr1)) && (!__TPQ_ISCHILDSX(NULL, t_ptr2)))
                    __tpq_sSwapTcb(&(t_ptr1->t_next_l->t_father) , 
                                    &(t_ptr2->t_next_l->t_father));
            else if (__TPQ_ISCHILDSX(NULL, t_ptr1) && (!__TPQ_ISCHILDSX(NULL, t_ptr2)))
                    t_ptr2->t_next_l->t_father = t_ptr1;
            else if ((!__TPQ_ISCHILDSX(NULL, t_ptr1)) && __TPQ_ISCHILDSX(NULL, t_ptr2))
                    t_ptr1->t_next_l->t_father = t_ptr2;
 
            /*swap child of father*/
            if ((!__TPQ_ISFATHER(NULL, t_ptr1)) && (!__TPQ_ISFATHER(NULL, t_ptr2))){
                if (__TPQ_ISCHILDSX(t_ptr1 , t_ptr1->t_father))
                    tTmp = &(t_ptr1->t_father->t_next_l);
                else
                    tTmp = &(t_ptr1->t_father->t_next_r);

                if (__TPQ_ISCHILDSX(t_ptr2 , t_ptr2->t_father))
                    tTmp2 = &(t_ptr2->t_father->t_next_l);
                else
                    tTmp2 = &(t_ptr2->t_father->t_next_r);
 
                __tpq_sSwapTcb( tTmp , tTmp2);
 
            }
            else if((__TPQ_ISFATHER(NULL, t_ptr1)) && (!(__TPQ_ISFATHER(NULL, t_ptr2)))){
 
                if (__TPQ_ISCHILDSX(t_ptr2 , t_ptr2->t_father))
                    t_ptr2->t_father->t_next_l = t_ptr1;
                else
                    t_ptr2->t_father->t_next_r = t_ptr1;

            }
            else if ((!__TPQ_ISFATHER(NULL, t_ptr1)) && __TPQ_ISFATHER(NULL, t_ptr2)){

                if (__TPQ_ISCHILDSX(t_ptr1, t_ptr1->t_father)){
                    t_ptr1->t_father->t_next_l = t_ptr2;

                }
                else{
                    t_ptr1->t_father->t_next_r = t_ptr2;
               }
 
            }
 
            __tpq_sSwapTcb(&(t_ptr1->t_next_l) , &(t_ptr2->t_next_l));
            __tpq_sSwapTcb(&(t_ptr1->t_next_r) , &(t_ptr2->t_next_r));
            __tpq_sSwapTcb(&(t_ptr1->t_father) , &(t_ptr2->t_father));
 
       }
        ret = TRUE;
    }
    else
        ret = FALSE;
 
    return ret;
}
 
 
/*
 * Swap two tcb_t* pointers
 */
 
HIDDEN inline void __tpq_sSwapTcb(register tcb_t **a,register tcb_t **b){
    register tcb_t *tmp;

    tmp=*a;
    *a=*b;
    *b=tmp;
}


/*
 * Support function for __tpq_swapNodes: it swaps a node with his left child
 */

HIDDEN void __tpq_sSwapL(register tcb_t *tcb_father
        ,register tcb_t* tcb_child){

    /* If the right child of the nodes is not empty the pointers to the father
     *  of the right children are swapped
     */
    if ((!__TPQ_ISNULL(tcb_child->t_next_r)) 
      && (!__TPQ_ISNULL(tcb_father->t_next_r)))
        __tpq_sSwapTcb(&(tcb_child->t_next_r->t_father),
            &(tcb_father->t_next_r->t_father));
    
    /* if the right child of tcb_child is NULL the right children are swapped,
     *  but not the father of the children: only the father of the right child
     *   of tcb_father is updated, because tcb_child hasn't the right child 
     */
    else if (__TPQ_ISNULL(tcb_child->t_next_r) 
        && (!__TPQ_ISNULL(tcb_father->t_next_r)))
            tcb_father->t_next_r->t_father = tcb_child;        

    /* if the right child of tcb_father is NULL the right children are swapped,
     *  but not the father of the children: only the father of the right child 
     *  of tcb_child is updated, because tcb_father hasn't the right child 
     */
    else if ((!__TPQ_ISNULL(tcb_child->t_next_r)) 
        && __TPQ_ISNULL(tcb_father->t_next_r))
            tcb_child->t_next_r->t_father = tcb_father;
    
    /* the left child of the father is updated */
    tcb_father->t_next_l = tcb_child->t_next_l;
    /* finally the right children are swapped */
    __tpq_sSwapTcb(&(tcb_child->t_next_r), &(tcb_father->t_next_r));

    
    if (!__TPQ_ISNULL(tcb_father->t_father)){
        if (__TPQ_ISCHILDDX(tcb_father, tcb_father->t_father))
            /* if tcb_father is the right child of his father 
             * this pointer is updated to tcb_child 
             */
            tcb_father->t_father->t_next_r = tcb_child; 
            
        else
            /* if tcb_father is the left child of his father this pointer
             *  is updated to tcb_child 
             */
            tcb_father->t_father->t_next_l = tcb_child;  
    }
    /* the father of tcb_child is updated to the father of tcb_father */
    tcb_child->t_father = tcb_father->t_father;
    /* the father of tcb_father is updated to tcb_child */
    tcb_father->t_father = tcb_child;
    /* if the left child of tcb_child isn't NULL, the pointer to the father or
     *  this node is updated to tcb_father 
     */
    if (!__TPQ_ISNULL(tcb_child->t_next_l))
        tcb_child->t_next_l->t_father = tcb_father;
    /* the left child of tcb_child is updated to tcb_father*/
    tcb_child->t_next_l = tcb_father;
}


/*
 * Support function for __tpq_swapNodes: it swaps a node with his left child
 */

HIDDEN void __tpq_sSwapR(register tcb_t *tcb_father
        , register tcb_t *tcb_child){
   /* If the left child of the nodes is not empty the pointers to the father
    *  of the left children are swapped
    */
    if (!__TPQ_ISNULL(tcb_child->t_next_l) 
            && (!__TPQ_ISNULL(tcb_father->t_next_l)))
        
        __tpq_sSwapTcb(&(tcb_child->t_next_l->t_father)
                , &(tcb_father->t_next_l->t_father));
    
    /* if the left child of tcb_child is NULL the left children are swapped,
     *  but not the father of the children: only the father of the left child
     *   of tcb_father is updated, because tcb_child hasn't the left child 
     */
    else if (__TPQ_ISNULL(tcb_child->t_next_l) 
            && (!__TPQ_ISNULL(tcb_father->t_next_l)))
        tcb_father->t_next_l->t_father = tcb_child;

    /* if the left child of tcb_father is NULL the left children are swapped, 
     * but not the father of the children: only the father of the left child 
     * of tcb_child is updated, because tcb_father hasn't the left child 
     */
    else if ((!__TPQ_ISNULL(tcb_child->t_next_l)) 
            && __TPQ_ISNULL(tcb_father->t_next_l))
          tcb_child->t_next_l->t_father = tcb_father;
    /* the right child of the father is updated */
    tcb_father->t_next_r=tcb_child->t_next_r;
    /* finally the left children are swapped */
    __tpq_sSwapTcb(&(tcb_child->t_next_l), &(tcb_father->t_next_l));

    if (!__TPQ_ISNULL(tcb_father->t_father)){
        if (__TPQ_ISCHILDDX(tcb_father, tcb_father->t_father))
            /* if tcb_father is the right child of his father this pointer 
             * is updated to tcb_child
             */
           tcb_father->t_father->t_next_r=tcb_child;     
        else
            /* if tcb_father is the left child of his father this pointer 
             * is updated to tcb_child
             */
            tcb_father->t_father->t_next_l=tcb_child;
    }
    /* the father of tcb_child is updated to the father of tcb_father */
    tcb_child->t_father=tcb_father->t_father;
    /* the father of tcb_father is updated to tcb_child */
    tcb_father->t_father=tcb_child;
    /* if the right child of tcb_child isn't NULL, the pointer to the father 
     * o this node is updated to tcb_father */
    if (!__TPQ_ISNULL(tcb_child->t_next_r))
        tcb_child->t_next_r->t_father=tcb_father;
    /* the right child of tcb_child is updated to tcb_father*/
    tcb_child->t_next_r=tcb_father;
}


/*
 * Unlink a leaf node from the priority queue, 
 * also unlink the nodes that links to this one
 */

HIDDEN inline void __tpq_unlinkNode(tcb_t *t_ptr){

    /* unlink the fathers->children link to t_ptr */
    if (!__TPQ_ISFATHER(NULL, t_ptr)){
        if (__TPQ_ISCHILDSX(t_ptr, t_ptr->t_father))
            t_ptr->t_father->t_next_l = NULL;
        else
            t_ptr->t_father->t_next_r = NULL;
        t_ptr->t_father = NULL;
    }
    
    t_ptr->t_next_l = t_ptr->t_next_r = NULL;
}
