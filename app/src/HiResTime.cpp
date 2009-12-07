
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include "epicsThread.h"

#if	defined(linux) && defined(__x86_64__)
#include "asm/cpufeature.h"
#endif	//	linux

#include "HiResTime.h"
//	Force implementation of inlines for C callers
#pragma implementation "HiResTime.h"

using namespace     std;

//	Scale factor for ticks to seconds
//	The initial value gets overridden by the
//	CalibrateHiResTicksPerSec constructor during startup
double		hiResTicksPerSec	= 1.33333333e8;


class	CalibrateHiResTicksPerSec
{
public:
	CalibrateHiResTicksPerSec( )
	{
		const	double	calibDur	= 0.010;	//	Sleep for 10ms
		t_HiResTime      tscStart    =	GetHiResTicks();
		epicsThreadSleep( calibDur );
		t_HiResTime      tscEnd		=	GetHiResTicks();
		if ( tscEnd != tscStart )
			hiResTicksPerSec	=	calibDur /	static_cast<double>(tscEnd - tscStart);
		else
		{
			fprintf( stderr, "CalibrateHiResTicksPerSec: Failed to read valid HiResTick count!\n" );
		}
	}
};


CalibrateHiResTicksPerSec		theCalibrator;


#ifdef	read_tsc

#if 0	//	INLINE_GET_HI_RES_TICKS
extern "C" t_HiResTime GetHiResTicks()
{
	t_HiResTime		tscVal;
	read_tsc( tscVal );
	return tscVal;
}
#endif

#else	//	read_tsc not defined

extern "C" t_HiResTime GetHiResTicks()
{
	struct timeval		curTimeVal;
	gettimeofday( &curTimeVal, NULL );
	t_HiResTime			hiResTicks	= static_cast<t_HiResTime>( curTimeVal.tv_sec );
	hiResTicks	*= 1000000L;
	hiResTicks	+= static_cast<t_HiResTime>( curTimeVal.tv_usec );
	return hiResTicks;
}

#endif	//	read_tsc


extern "C" double  HiResTicksToSeconds(
	t_HiResTime		nTicks	)
{
	return nTicks / hiResTicksPerSec;
}


epicsTime HiResToEpicsTime( t_HiResTime valHiRes )
{
	double			dblHiRes	= HiResTicksToSeconds( valHiRes );
	struct timespec	valTimeSpec;
	valTimeSpec.tv_sec	=	static_cast<time_t>(	 dblHiRes );
	dblHiRes			-=	static_cast<double>(	 valTimeSpec.tv_sec );
	valTimeSpec.tv_nsec	=	static_cast<long int>(	dblHiRes * 1.0e9 );
	epicsTime		valEpicsTime(	valTimeSpec );

	return	valEpicsTime;
}


/**
 *  GetHiResTime()
 *  Return the highest available resolution time
 *  for the current operating system.
 *  Returns:    double time value in sec
 */
extern "C" double  GetHiResTime( )
{
    struct  timespec        tmHiResTime;
    struct  timespec        tmHiResClockRes;

#ifdef  CLOCK_MONOTONIC
    clock_gettime(	CLOCK_MONOTONIC, &tmHiResTime );
    clock_getres(	CLOCK_MONOTONIC, &tmHiResClockRes );
#else
    clock_gettime(	CLOCK_REALTIME, &tmHiResTime );
    clock_getres(	CLOCK_REALTIME, &tmHiResClockRes );
#endif  //  _POSIX_TIMERS

    double  dblHiResTime =   tmHiResTime.tv_sec;
    dblHiResTime += static_cast<double>(tmHiResTime.tv_nsec) / 1.0e9;

    return dblHiResTime;
}
