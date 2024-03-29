#ifndef HI_RES_TIME_H
#define HI_RES_TIME_H

#include    <epicsTime.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef		long long		t_HiResTime;

double  	HiResTicksToSeconds(	t_HiResTime	);
double  	HiResTicksPerSecond( );

#ifdef	__cplusplus
}
#endif	/*	__cplusplus	*/


/*
 * Macro definitions for reading the timestamp counter
 * Add processor specific variants of the read_tsc() macro as needed and available
 * If read_tsc() is not defined here, a version of GetHiResTicks() is provided
 * as a function that provides a backup using clock_gettime()
 */
#if	defined(__x86_64__) || defined(_X86_64_)

#define		read_tsc( tscVal	)	     					\
do										      				\
{	/*	=A is for 32 bit mode reading 64 bit integer */		\
	unsigned long	tscHi, tscLo;      						\
	__asm__ volatile("rdtsc" : "=a" (tscLo), "=d" (tscHi) );    \
	tscVal	=  (t_HiResTime)( tscHi ) << 32;      			\
	tscVal	|= (t_HiResTime)( tscLo );      				\
}	while ( 0 );

/*	end of defined(__x86_64__)	*/
#elif	defined(__i386__)

#define	read_tsc(tscVal)	__asm__ volatile( "rdtsc": "=A" (tscVal) )

/*	end of defined(__i386__)	*/
#elif	defined(mpc7455) || defined(__PPC__)

#define		read_tsc( tscVal	)	     					\
do										      				\
{											      			\
	unsigned long	tscHi, tscHi_old, tscLo;				\
	do														\
	{														\
		__asm__ volatile("mftbu %0" : "=r" (tscHi_old));	\
		__asm__ volatile("mftb  %0" : "=r" (tscLo));		\
		__asm__ volatile("mftbu %0" : "=r" (tscHi));		\
	}	while (tscHi_old != tscHi);							\
	tscVal	=  (t_HiResTime)( tscHi ) << 32;      			\
	tscVal	|= (t_HiResTime)( tscLo );      				\
}	while ( 0 );

#endif	/*	end of	defined(mpc7455)	*/

#ifdef	__cplusplus
extern "C" {
#endif
#ifdef	read_tsc
static __inline__ t_HiResTime GetHiResTicks()
{
	t_HiResTime		tscVal	= 0;
	read_tsc( tscVal );
	return tscVal;
}
#else
extern t_HiResTime	GetHiResTicks();
#endif	/*	read_tsc	*/
#ifdef	__cplusplus
}
#endif	/*	__cplusplus	*/

#ifdef	__cplusplus
//	C++ only functions

epicsTime	HiResToEpicsTime(		t_HiResTime );

#endif	/*	__cplusplus	*/

#endif	/*  HI_RES_TIME_H	*/
