#include <tcb.h>
#include <asl.h>

extern void initASH(void);
extern tcb_t *headBlocked(int *);
extern tcb_t *removeBlocked(int *);
extern tcb_t *outBlocked(tcb_t *);
extern int insertBlocked(int *, tcb_t *);
extern inline semd_t **__asl_pscanListHash(int *);
