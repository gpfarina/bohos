#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef	XDEBUG

#define	DEBUG(str)	addokbuf(str)


#else
#define	DEBUG(str)      
#endif

#endif
