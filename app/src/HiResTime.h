#ifndef HI_RES_TIME_H
#define HI_RES_TIME_H

#include    <epicsTime.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef		long long		t_HiResTime;

t_HiResTime	GetHiResTicks();

double  	HiResTicksToSeconds(	t_HiResTime	);

#ifdef	__cplusplus
}
#endif	/*	__cplusplus	*/


/*
 * Macro definitions for reading the timestamp counter
 * Add processor specific variants of the read_tsc() macro as needed and available
 * If read_tsc() is not defined here, a version of GetHiResTicks() is provided
 * as a function that provides a backup using gettimeofday()
 */
#if	defined(__x86_64__)

#define		read_tsc( tscVal	)	asm volatile( "rdtsc": "=A" (tscVal) )

//	end of defined(__x86_64__)
#elif	defined(__i386__)

#define		read_tsc( tscVal	)	     						 		\
do										      							\
{											      						\
	unsigned long	tscHi, tscLo;      									\
	asm volatile("rdtsc" : "=a" (tscLo), "=d" (tscHi) );      			\
	long long tscVal	=  static_cast<long long>( tscHi ) << 32;      	\
	tscVal				|= static_cast<long long>( tscLo );      		\
}	while ( 0 );

//	end of defined(__i386__)
#elif	defined(mpc7455)

#define		read_tsc( tscVal	)	     					\
do										      				\
{											      			\
	unsigned long	tscHi, tscHi_old, tscLo;				\
	do														\
	{														\
		asm volatile("mftbu %0" : "=r" (tscHi_old));		\
		asm volatile("mftb  %0" : "=r" (tscLo));			\
		asm volatile("mftbu %0" : "=r" (tscHi));			\
	}	while (tscHi_old != tscHi);							\
	tscVal	=  static_cast<long long>( tscHi ) << 32;      	\
	tscVal	|= static_cast<long long>( tscLo );      		\
}	while ( 0 );

#endif	//	end of	defined(mpc7455)

#ifdef	__cplusplus
//	C++ functions

#ifdef	read_tsc
#define	INLINE_GET_HI_RES_TICKS
extern "C" inline t_HiResTime GetHiResTicks()
{
	t_HiResTime		tscVal	= 0;
	read_tsc( tscVal );
	return tscVal;
}
#endif	/*	read_tsc	*/

epicsTime	HiResToEpicsTime(		t_HiResTime );

#endif	/*	__cplusplus	*/

#endif      //  HI_RES_TIME_H
