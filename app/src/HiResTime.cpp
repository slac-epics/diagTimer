
#include <time.h>
#include <sys/time.h>
#include "HiResTime.h"

#if	defined(linux) && defined(__x86_64__)
#include "asm/cpufeature.h"
#endif	//	linux


using namespace     std;


#if	defined(__x86_64__) && defined(cpu_has_tsc)
double		hiResTicksPerSec	= 1.33333333e8;

extern "C" t_HiResTime GetHiResTicks()
{
	long long tscVal;
	asm volatile("rdtsc" : "=A" (tscVal));
	return tscVal;
}
//	end of defined(__x86_64__) && defined(cpu_has_tsc)
#elif	defined(__i386__)
double		hiResTicksPerSec	= 1.33333333e8;

extern "C" t_HiResTime GetHiResTicks()
{
	unsigned long	tscHi, tscLo;
	asm volatile("rdtsc" : "=a" (tscLo), "=d" (tscHi) );
	long long tscVal	=  static_cast<long long>( tscHi ) << 32;
	tscVal				|= static_cast<long long>( tscLo );
	return tscVal;
}
//	end of defined(__i386__)
#elif	defined(mpc7455)
double		hiResTicksPerSec	= 1.33333333e8;


extern "C" t_HiResTime GetHiResTicks()
{
	unsigned up_old, up, low;
	do
	{
		asm volatile("mftbu %0" : "=r" (up_old));
		asm volatile("mftb  %0" : "=r" (low));
		asm volatile("mftbu %0" : "=r" (up));
	}	while (up_old != up);

	t_HiResTime hiResTicks = up;
	hiResTicks <<= 32;
	hiResTicks |= low;
	return hiResTicks;
}
//	end of	defined(mpc7455)
#else
double		hiResTicksPerSec	= 1000000.0;


extern "C" t_HiResTime GetHiResTicks()
{
	struct timeval		curTimeVal;
	gettimeofday( &curTimeVal, NULL );
	t_HiResTime			hiResTicks	= static_cast<t_HiResTime>( curTimeVal.tv_sec );
	hiResTicks	*= 1000000L;
	hiResTicks	+= static_cast<t_HiResTime>( curTimeVal.tv_usec );
	return hiResTicks;
}
#endif	//	End of cpu specific hi res code


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
