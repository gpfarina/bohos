#ifndef _H_THREADPQ_H
#define _H_THREADPQ_H




/* THIS IS A MAGIC NUMBER. It is used to look up the most
 * significant bit of the number representing the number of nodes 
 * in a heap, 32 is a big enough number to handle a very big number
 * of threads. 
 */
#define __TPQ_MAXAND            32    

/*VERY USEFUL MASCRO*/

#define __TPQ_ISFATHER(A, B)    (B->t_father==A)

#define __TPQ_ISCHILDDX(A, B)   (B->t_next_r==A)
#define __TPQ_ISCHILDSX(A, B)   (B->t_next_l==A)

#define __TPQ_ISNULL(A)         (A==NULL)


#endif
