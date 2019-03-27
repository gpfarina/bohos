#include <tcb.h>

extern int __kern_V(int *);
extern void __kern_P(tcb_t *, int *);
extern void __kern_kill(tcb_t **);
