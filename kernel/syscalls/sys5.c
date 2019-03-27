#include <types.h>
#include <syscall.h>
#include <localFunc.e>
#include <scheduler.h>

extern globThreadInfo gThreadInfo;

void __sys5(void){
      state_t *oldArea;
      
      oldArea = (state_t *)SE_OLDAREA;

      /*Check for wich sys/br type currThread wants to specify a handler*/
      if((oldArea->s_a1 > SYSBRTYPENUMBER-1 || oldArea->s_a1 <0)
              /*already specifyed the syscall handler*/
              || gThreadInfo.currThread->sys5flag[oldArea->s_a1] )
              /*empty new/old area
              || (oldArea->s_a2 == NULL || oldArea->s_a3 == NULL))
             maybe here a check on the addresses */
          __kern_kill(&(gThreadInfo.currThread));        

       else{
           gThreadInfo.currThread->sys5flag[oldArea->s_a1] = 1;
           
           gThreadInfo.currThread->sys5OldArea[oldArea->s_a1] =
               (state_t*) oldArea->s_a2;
           gThreadInfo.currThread->sys5NewArea[oldArea->s_a1] = 
               (state_t*) oldArea->s_a3;

       }

}
