
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <epicsThread.h>
#include <epicsExport.h>
#include <registryFunction.h>
#include <iocsh.h>
#include <epicsExport.h>

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
double			hiResTicksPerSec	= 1.33333333e8;

unsigned int	DEBUG_HI_RES_TIME	= 0;

class	CalibrateHiResTicksPerSec
{
public:
	CalibrateHiResTicksPerSec( )
	{
		const double	calibDur	= 2.0;	//	duration in seconds
		t_HiResTime     tscStart	= GetHiResTicks();
		ms_initialTickCount			= tscStart;
		ms_initialTimeOfDay			= epicsTime::getCurrent();
		epicsThreadSleep( calibDur );
		t_HiResTime     tscEnd		= GetHiResTicks();
		epicsTime		endTime		= epicsTime::getCurrent();
		double			actualDur	= endTime - ms_initialTimeOfDay;
		if ( tscEnd != ms_initialTickCount )
		{
			hiResTicksPerSec	= static_cast<double>(tscEnd - tscStart) / actualDur;
			if ( DEBUG_HI_RES_TIME >= 1 )
			{
				cout << "CalibrateHiResTicksPerSec: " << (tscEnd - tscStart)
					 << " ticks per " << actualDur	<< " sec"	<< endl;
				cout << "CalibrateHiResTicksPerSec: ScaleFactor=" << hiResTicksPerSec
					 << " = " << 1.0e9 / hiResTicksPerSec << " ns per tick" << endl;
				cout << "CalibrateHiResTicksPerSec: InitialTick=" << ms_initialTickCount << endl;
				char	buf[256];
				ms_initialTimeOfDay.strftime( buf, 256, "%H:%M:%S.%09f" );
				cout << "CalibrateHiResTicksPerSec: InitialTOD =" << string(buf) << endl;
			}
		}
		else
		{
			cerr << "CalibrateHiResTicksPerSec: Failed to read valid HiResTick count!" << endl;
		}
	}

	static double	HiResTicksToSeconds( t_HiResTime tickCount )
	{
		bool		fRecalcScaleFactor	= false;	// not working yet

		if ( fRecalcScaleFactor )
		{
			t_HiResTime		curTicks	= GetHiResTicks();
			epicsTime		curTime		= epicsTime::getCurrent();
			double			totalDur	= curTime - ms_initialTimeOfDay;
			double			scaleFactor	= static_cast<double>(curTicks - ms_initialTickCount) / totalDur;
			cout << "HiResTicksToSeconds: curTicks    =" << curTicks	<< endl;
			char	buf[256];
			curTime.strftime( buf, 256, "%H:%M:%S.%09f" );
			cout << "HiResTicksToSeconds: curTime     =" << string(buf)	<< endl;
			cout << "HiResTicksToSeconds: totalDur    =" << totalDur	<< endl;
			cout << "HiResTicksToSeconds: scaleFactor =" << scaleFactor	<< endl;
			hiResTicksPerSec	= scaleFactor;
		}
		if ( DEBUG_HI_RES_TIME >= 2 )
		{
			cout << "HiResTicksToSeconds: tickCount   =" << tickCount	<< endl;
			cout << "HiResTicksToSeconds: seconds	  =" << tickCount / hiResTicksPerSec	<< endl;
		}
		return tickCount / hiResTicksPerSec;
	}

private:
	static t_HiResTime		ms_initialTickCount;
	static epicsTime		ms_initialTimeOfDay;
};


CalibrateHiResTicksPerSec		theCalibrator;
t_HiResTime		CalibrateHiResTicksPerSec::ms_initialTickCount	= 0LL;
epicsTime		CalibrateHiResTicksPerSec::ms_initialTimeOfDay;


#ifndef	read_tsc

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
#if 0
	return nTicks / hiResTicksPerSec;
#else
	return CalibrateHiResTicksPerSec::HiResTicksToSeconds( nTicks );
#endif
}


epicsTime HiResToEpicsTime( t_HiResTime valHiRes )
{
	double			dblHiRes	= HiResTicksToSeconds( valHiRes );
	struct timespec	valTimeSpec;
	valTimeSpec.tv_sec	=	static_cast<time_t>(	dblHiRes );
	dblHiRes			-=	static_cast<double>(	valTimeSpec.tv_sec );
	valTimeSpec.tv_nsec	=	static_cast<long int>(	dblHiRes * 1.0e9 );
	epicsTime		valEpicsTime(	valTimeSpec );

	return	valEpicsTime;
}


/* Register GetHiResTicks */
static const iocshFuncDef   GetHiResTicksFuncDef	= { "GetHiResTicks", 0, NULL };
static int  GetHiResTicksCallFunc( const iocshArgBuf * args )
{
	return static_cast<int>( GetHiResTicks( ) );
}
void GetHiResTicksRegister(void)
{
	iocshRegister( &GetHiResTicksFuncDef, reinterpret_cast<iocshCallFunc>(GetHiResTicksCallFunc) );
}

epicsExportRegistrar( GetHiResTicksRegister );


/* Register HiResTicksToSeconds */
static const iocshArg		HiResTicksToSecondsArg0		= { "numTicks", iocshArgInt };
static const iocshArg	*	HiResTicksToSecondsArgs[1]	= { &HiResTicksToSecondsArg0 };
static const iocshFuncDef   HiResTicksToSecondsFuncDef	= { "HiResTicksToSeconds", 1, HiResTicksToSecondsArgs };
static double  HiResTicksToSecondsCallFunc( const iocshArgBuf * args )
{
	return HiResTicksToSeconds( static_cast<t_HiResTime>( args[0].ival ) );
}
void HiResTicksToSecondsRegister(void)
{
	iocshRegister( &HiResTicksToSecondsFuncDef, reinterpret_cast<iocshCallFunc>(HiResTicksToSecondsCallFunc) );
}

epicsExportRegistrar( HiResTicksToSecondsRegister );

