#ifndef _ASL_H_
#define _ASL_H_

#include <tcb.h>


typedef struct semd_t{
    struct semd_t *s_next;
    int    *s_semAdd;
    tcb_t  *s_threadQ;
}semd_t;


#endif
