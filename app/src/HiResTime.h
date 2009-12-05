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


#ifdef	__cplusplus
//	C++ functions

epicsTime	HiResToEpicsTime(		t_HiResTime );

//	Do we need this?   Might overflow.
//	t_HiResTime	EpicsTimeToHiRes(	const & epicsTime );

#endif	/*	__cplusplus	*/

#endif      //  HI_RES_TIME_H
