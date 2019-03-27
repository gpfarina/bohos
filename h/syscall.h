#ifndef _SYSCALL_H
#define _SYSCALL_H
#include <tcb.h>

#define SE_OLDAREA      0x20000348 
#define SE_NEWAREA      0x200003D4


#define PGME_OLDAREA    0x20000230
#define TLBE_OLDAREA    0x20000118
#define CREATETHREAD            1       
#define TERMINATETHREAD         2       
#define VERHOGEN                3
#define PASSERN                 4
#define SPECTRAPVEC             5
#define GETCPUTIME              6
#define WAITCLOCK               7
#define WAITIO                  8
#define USERDEF_SYSCALL         9

#define RESERVED_INSTRUCTION    (0x0A<<2)

#define IS_INDEXOK(servNo)      (servNo>=1 && servNo<=9)


#define CAUSE_SYSCALL           8           
#define CAUSE_BREAK             9


#define CAUSE_SYS9              10      /*Used just dispatch between a break instruction and a sys9*/
#endif


