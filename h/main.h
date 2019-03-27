#ifndef _MAIN_H_
#define _MAIN_H_
#include <types.h>
#include <tcb.h>

#define EXCEPTIONFLAGS          (0x00000000)

#define TLBE_NEWAREA            (0x200001A4)            
#define PGME_NEWAREA            (0x200002BC)

#define RAM_BASEPHYSICALADDR    0x10000000
#define RAM_INSTALLEDRAMSIZE    0x10000004
#define RAMTOP                  (*((memaddr *)RAM_BASEPHYSICALADDR)+ *((memaddr *)RAM_INSTALLEDRAMSIZE))

#define KERNEL_MODE             0
#define USER_MODE               1

#define GET_PREVIOUSMODE(reg)           ((reg & 0x8)>>3)
#define GET_MODE (reg)                  ((reg & 0x2)>>1)


#endif
