#ifndef _CONST_H_
#define _CONST_H_

#define MAXTHREADS 32
#define ASHSIZE     7
#define MAXSEM 20
/* number of types of system call breackpoint exceptions */
#define SYSBRTYPENUMBER 3 



#define PAGESIZE        4096    /* page size in bytes */
#define WORDLEN         4       /* word size in bytes */
#define PTEMAGICNO      0x2A


#define ROMPAGESTART    0x20000000   /* ROM Reserved Page */


/* timer, timescale, TOD-LO and other bus regs */
#define RAMBASEADDR     0x10000000
#define TODLOADDR       0x1000001C
#define INTERVALTMR     0x10000020  
#define TIMESCALEADDR   0x10000024


/* utility constants */
#define TRUE        1
#define FALSE       0
#define HIDDEN      static
#define EOS         '\0'

#define NULL ((void *)0xFFFFFFFF)


/* vectors number and type */
#define VECTSNUM    4

#define TLBTRAP     0
#define PROGTRAP    1


#define SYSTRAP     2

#define TRAPTYPES   3


/* device register field number for terminal devices */
#define RECVSTATUS  0
#define RECVCOMMAND 1
#define TRANSTATUS  2
#define TRANCOMMAND 3


/* device common STATUS codes */
#define UNINSTALLED 0
#define READY       1
#define BUSY        3

/* device common COMMAND codes */
#define RESET       0
/* operations */
#define MIN(A,B)    ((A) < (B) ? A : B)
#define MAX(A,B)    ((A) < (B) ? B : A)
#define ALIGNED(A)  (((unsigned)A & 0x3) == 0)

/* Useful operations */
#define STCK(T) ((T) = ((* ((cpu_t *) TODLOADDR)) / (* ((cpu_t *) TIMESCALEADDR))))

/*Number maximum of semaphores allocables*/
#define MAXSEM          20

/* hardware constants */
#define PRINTCHR        2
#define BYTELEN 8
#define RECVD   5

#define CLOCKINTERVAL   100000UL        /* interval to V clock semaphore */

#define TERMSTATMASK    0xFF
#define CAUSEMASK               0xFF
#define VMOFF                   0xF8FFFFFF

#define SYSCAUSE                (0x8 << 2)
#define BUSERROR                6
#define RESVINSTR       10
#define ADDRERROR               4

#define QPAGE                   1024

#define IEPBITON                0x4
#define KUPBITON                0x8
#define KUPBITOFF               0xFFFFFFF7

#define CAUSEINTMASK    0xFC00
#define CAUSEINTOFFS    10

#define MINLOOPTIME             30000
#define LOOPNUM                 10000

#define CLOCKLOOP               10
#define MINCLOCKLOOP    3000    

#define BADADDR                 0xFFFFFFFF
#define TERM0ADDR               0x10000250
/* Software and other constants */
#define PRGOLDVECT              4
#define TLBOLDVECT              2
#define SYSOLDVECT              6

/* system call codes */
#define CREATETHREAD    1       /* create thread */
#define TERMINATETHREAD 2       /* terminate thread */
#define VERHOGEN                3       /* V a semaphore */
#define PASSERN                 4       /* P a semaphore */
#define SPECTRAPVEC             5       /* specify trap vectors for passing up */
#define GETCPUTIME              6       /* get cpu time used to date */
#define WAITCLOCK               7       /* delay on the clock semaphore */
#define WAITIO                  8       /* delay on a io semaphore */

#define CREATENOGOOD    -1

/* just to be clear */
#define SEMAPHORE               int

#endif
