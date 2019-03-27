#include <types.h>
#include <const.h>
#include <tcb.h>
extern tcb_t* mkEmptyThreadPQ(void);
extern int emptyThreadPQ(tcb_t *tp);
extern void insertThreadPQ(tcb_t **tp, tcb_t *t_ptr);
extern tcb_t* removeThreadPQ(tcb_t **tp);
extern tcb_t* outThreadPQ(tcb_t **tp, tcb_t *t_ptr);
extern tcb_t* headThreadPQ(tcb_t *tp);

