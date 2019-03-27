#ifndef _INTERRUPTS_H_ 
#define _INTERRUTPS_H_


#define IE_OLDAREA          (0x20000000)
#define IE_NEWAREA          (0x2000008C)
#define MASK_EXCCODE        (0x7C)

/* get cause of exception */
#define GET_EXCCODE(reg)    ((reg & 0xFF)>>2)

/*bit of interrupt timer line*/
#define CAUSE_INTERRUPT     (0x00)
#define SET_INTMASKENABLED  (0x1)

/*interrupt mask pending*/
#define GET_IPENDING(reg)   ((reg & 0xFF00)>>8)

/*base address of the first word about wich devices has generated interrupt on
 * the fist line, from here to 5 word (0x1000004C) where there is the last word
 */
#define IPENDING_BITMAP     (0x1000003C)     


/*  We exclude from here line 0 line 1 that are for soft int but also the interrupt line for the timer
 *  because we need just ONE semaphore for it, and in the end, we exclude also the terminal line
 *  because we have to manage the terminal in a different way. So we have 4 lines.
 */
#define NUMLINES                4 
#define NUMDEV                  8

#endif

