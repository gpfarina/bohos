#ifndef _DEVICES_H_
#define _DEVICES_H_

#define STARTDEVADDRESS     0x10000000


#define LINETIMER           2
#define LINEDISK            3
#define LINETAPE            4
#define LINENETWORK         5
#define LINEPRINTER         6
#define LINETERMINAL        7
/*usefull to manage device... in particular terminals*/
#define STATUS_FIELD        0x00
#define COMMAND_FIELD       0x04
#define DATA0_FIELD         0x08
#define DATA1_FIELD         0x0C


#define DEVBASEADDR(line, devNo)    ( STARTDEVADDRESS + 0x50 +  ((line-3)*0x80) + (devNo*0x10) )

#define STATUS(line, devNo)         ((unsigned int *) (DEVBASEADDR(line, devNo) + STATUS_FIELD))
#define COMMAND(line, devNo)        ((unsigned int *) (DEVBASEADDR(line, devNo) + COMMAND_FIELD))
#define DATA0(line, devNo)          ((unsigned int *) (DEVBASEADDR(line, devNo) + DATA0_FIELD))
#define DATA1(line, devNo)          ((unsigned int *) (DEVBASEADDR(line, devNo) + DATA1_FIELD))


/*Waiting for the subdevice to write or receive a char*/
#define WAITFORTRANS                0 
#define WAITFORRECV                 1

/*the terminal device register has same fields with different meaning*/
#define RECVTERM(devNo)             STATUS(LINETERMINAL,devNo)
#define TRANSTERM(devNo)            DATA0(LINETERMINAL,devNo)

#define TRANS_COMMAND(line,devNo)    DATA1(line,devNo)
#define RECV_COMMAND(line,devNo)     COMMAND(line,devNo)

#define TERMINAL_RESET              0
#define TERMINAL_ACK                1
#define TERMINAL_TRANSCHAR          2
#define TERMINAL_RECVCHAR           2                 

#define WRITEONDEV(addr, cmd)       {unsigned int *p; p=(unsigned int *)addr; *p=cmd;}
#define READFROMDEV(addr, val)      {unsigned int *p; p=(unsigned int *)addr; val=*p;} 

/*to manage timer we use another interface...*/
#define TIMER           0x10000020
#define READTIMER(val)  {unsigned int *p; p=(unsigned int *)TIMER; val=*p;} 
#define WRITETIME(val)  {unsigned int *p; p=(unsigned int *)TIMER; *p=val;}            



#endif


