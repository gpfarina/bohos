#include <common.e>
#include <types.h>
/*
 * Calculates the logarithm in base 2 of an unsigned long int
 */

inline unsigned short int __ulog2(register unsigned long int n){
    register int count;

    count = 0;
    while( n > 1 ){
        n = n >> 1;
        count++;
    }
    return count;
}

/*ACTUNG:!!!
    The caller have to care of about calling this function only with interrupt disabled
*/

void * __kern_memcpy(void *dst, const void *src, size_t bytes){
    char *dstTemp=(char *)dst;
    char *srcTemp=(char *)src;
    while(bytes--)    
        *dstTemp++=*srcTemp++;
    return dst;
}
