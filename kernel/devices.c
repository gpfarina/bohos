#include <const.h>
#include <devices.h>
#include <types.h>
#include <interrupts.h>
#include <devices.e>
#include <asl.h>
#include <asl.e>
#include <scheduler.h>
#include <threadq.e>
#include <tcb.h>
#include <interrupts.e>
#include <localFunc.e>
#include <debug.h>

int                     termBuffs[8][2];
int                     termFlag[8][2];

/*  integer for semaphores associated to devices but not the timer, or
 *  terminals*/
int                     devSem[NUMLINES][NUMDEV];

/*  integer for terminals semaphores. We have 2 semaphores for each terminals
 *  (recv/transmit) */
int                     termSem[8][2];


int             termSem[8][2];
extern  globThreadInfo  gThreadInfo;




void terminalHandler(void){
    unsigned int    devicePending;
    unsigned int    *addrStatusRecv, *addrStatusTrmit, statusRecv=0, statusTrmit=0;
    
    /*This is the terminal which generated the interrupt */
    devicePending   =   __intrpt_getDevice(LINETERMINAL); 
    DEBUG("Terminal Handler\n");
    /*  So, now, we know that terminal with index devicePending has generated an interrupt
        but we have to understand if the interrupt generated is caused by a received char
        or a transmitted char or both.Now we get info about the specific terminal. Note that we also  
        perform a shift operation because the status is in the first 8 bit of the status reg
    */


    addrStatusRecv      =   RECVTERM(devicePending);
    statusRecv          =   ((*addrStatusRecv)&0xFF);
    /*  The status about the transmitter subDevice is in the register with in the other device is named 
        DATA0 reg... we perform the bitwise and operation too for the same reason of the previous 
        instruction
    */
    addrStatusTrmit     =   TRANSTERM( devicePending);
    statusTrmit         =   ((*addrStatusTrmit)&0xFF);

    switch(statusTrmit){
            case 0:
                break;
            case 1:
                break;  
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                /*  if no process has already perform a sys8 operation in this term subdevice...
                    that is: if the V operation in this semaphore is not going to unblock someone   
                */
                if(termSem[devicePending][WAITFORTRANS] >= 0 ){
                    /*We save the status byte register*/
                    termBuffs[devicePending][WAITFORTRANS]        =   (*addrStatusTrmit);
                    termFlag[devicePending][WAITFORTRANS]         =   1;

                }
                else{
                    /*We save the status word in the thread's v0 that we unblock*/
                    tcb_t   *head;
                    head = headBlocked(&(termSem[devicePending][WAITFORTRANS]));
                    head->proc_state.s_v0=(*addrStatusTrmit);
                    __kern_V(&(termSem[devicePending][WAITFORTRANS]));
                }
/*  Then we ack the terminal subdevice... note that the command reg of 
    he transmitter  subDevice is in the register with in the other 
    device is named DATA1 reg*/
         WRITEONDEV(TRANS_COMMAND(LINETERMINAL, devicePending), TERMINAL_ACK);          
                break;
            default:
                break;
    }
}


void __devices_initDevicesStruct(void){

   int i, j;
   /*initializing the devices semaphore's values and their descriptors*/
    for(i=0;i<NUMLINES;++i)
        for(j=0; j<NUMDEV;++j)
            devSem[i][j]    =   0;

    for(i=0;i<8;++i)
        for(j=0; j<2;++j)
            termSem[i][j]   =   0;
    
    for(i=0; i<8; ++i)
        for(j=0;j<2;++j)
            termBuffs[i][j]=termFlag[i][j]  =   0;
    
}


