#include <tcb.h>
extern tcb_t * mkEmptyThreadQ(void);
extern int emptyThreadQ(tcb_t *tp);
extern void insertBackThreadQ(tcb_t **tp, tcb_t *t_ptr);
extern void insertFrontThreadQ(tcb_t **tp,tcb_t *t_ptr);
extern tcb_t * removeThreadQ(tcb_t **tp);
extern tcb_t * outThreadQ(tcb_t **tp, tcb_t *t_ptr);
extern tcb_t * headThreadQ(tcb_t *tp);
