
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include "epicsThread.h"
#include "epicsExport.h"
#include "registryFunction.h"
#include "iocsh.h"
#include "epicsTime.h"

//	Force implementation of inlines for C callers
#pragma implementation "HiResTime.h"
#include "HiResTime.h"

using namespace     std;

int	DEBUG_HI_RES_TIME		= 1;


class	CalibrateHiResTicksPerSec
{
public:
	CalibrateHiResTicksPerSec( )
	{
		Initialize();
		//	Don't Calibrate now as EPICS clock may not be ready
		//	Calibrate();
	}

	static void Initialize( )
	{
		epicsTimeStamp		timeStamp;
		if ( epicsTimeGetCurrent( &timeStamp ) == epicsTimeOK )
		{
			ms_initialTickCount		= GetHiResTicks();
			ms_initialTimeOfDay		= timeStamp;
			ms_initialized			= true;
			ms_calibrated			= false;
			if ( DEBUG_HI_RES_TIME >= 2 )
			{
				char	buf[256];
				ms_initialTimeOfDay.strftime( buf, 256, "%H:%M:%S.%06f" );
				cout << "CalibrateHiResTicksPerSec: ms_initialTimeOfDay   = " << string(buf) << endl;
				cout << "CalibrateHiResTicksPerSec: ms_initialTickCount   = " << ms_initialTickCount << endl;
			}
		}
	}

	static void Calibrate( )
	{
		const double	calibDur	= 1.0;	//	duration in sec
		t_HiResTime     tscStart	= GetHiResTicks();
		epicsTime		startTime	= epicsTime::getCurrent();
		epicsThreadSleep( calibDur );
		t_HiResTime     tscEnd		= GetHiResTicks();
		epicsTime		endTime		= epicsTime::getCurrent();
		double			actualDur	= endTime - startTime;
		if ( tscEnd != tscStart && actualDur != 0 )
		{
			ms_hiResTicksPerSec	= static_cast<double>(tscEnd - tscStart) / actualDur;
			if ( DEBUG_HI_RES_TIME >= 1 )
			{
				cout << "CalibrateHiResTicksPerSec: " << (tscEnd - tscStart)
					 << " ticks per " << actualDur	<< " sec"	<< endl;
				cout << "CalibrateHiResTicksPerSec: ScaleFactor = " << ms_hiResTicksPerSec
					 << " => " << 1.0e9 / ms_hiResTicksPerSec << " ns per tick" << endl;
			}
			if ( DEBUG_HI_RES_TIME >= 2 )
			{
				cout << "CalibrateHiResTicksPerSec: InitialTick = " << tscStart << endl;
				char	buf1[256];
				startTime.strftime( buf1, 256, "%H:%M:%S.%06f" );
				cout << "CalibrateHiResTicksPerSec: startTime   = " << string(buf1) << endl;
				char	buf2[256];
				endTime.strftime( buf2, 256, "%H:%M:%S.%06f" );
				cout << "CalibrateHiResTicksPerSec: endTime     = " << string(buf2) << endl;
			}
			ms_calibrated	= true;
		}
		else
		{
			cerr << "CalibrateHiResTicksPerSec: Failed to read valid HiResTick count!" << endl;
		}
	}

	static double	HiResTicksToSeconds( t_HiResTime tickCount )
	{
		if ( !ms_initialized || ms_initialTimeOfDay == ms_undefEpicsTime )
			Initialize();

		// tested recalc but didn't improve over initial calibration
		bool	fRecalcScaleFactor	= false;
		if ( ms_initialized && !ms_calibrated )
			Calibrate();
		else if ( ms_initialized && ms_calibrated && fRecalcScaleFactor )
		{
			t_HiResTime		curTicks	= GetHiResTicks();
			epicsTime		curTime		= epicsTime::getCurrent();
			double			totalDur	= curTime - ms_initialTimeOfDay;
			double			scaleFactor	= static_cast<double>(curTicks - ms_initialTickCount) / totalDur;
			if ( DEBUG_HI_RES_TIME >= 2 )
			{
				cout << "HiResTicksToSeconds: curTicks    = " << curTicks	<< endl;
				char	buf[256];
				curTime.strftime(				buf, 256, "%H:%M:%S.%06f" );
				cout << "HiResTicksToSeconds: curTime     = " << string(buf)	<< endl;
				ms_initialTimeOfDay.strftime(	buf, 256, "%H:%M:%S.%06f" );
				cout << "HiResTicksToSeconds: InitialTOD  = " << string(buf)	<< endl;

				cout << "HiResTicksToSeconds: totalDur    = " << totalDur		<< endl;
				cout << "HiResTicksToSeconds: old scaleFactor = " << ms_hiResTicksPerSec	<< endl;
				cout << "HiResTicksToSeconds: new scaleFactor = " << scaleFactor	<< endl;
			}
			ms_hiResTicksPerSec	= scaleFactor;
		}
		if ( DEBUG_HI_RES_TIME >= 2 )
		{
			cout << "HiResTicksToSeconds: tickCount   = " << tickCount	<< endl;
			cout << "HiResTicksToSeconds: seconds     = " << tickCount / ms_hiResTicksPerSec	<< endl;
		}
		return tickCount / ms_hiResTicksPerSec;
	}

	static double HiResTicksPerSecond( )
	{
		return ms_hiResTicksPerSec;
	}

private:
	static double			ms_hiResTicksPerSec;
	static t_HiResTime		ms_initialTickCount;
	static epicsTime		ms_initialTimeOfDay;
	static epicsTime		ms_undefEpicsTime;
	static bool				ms_initialized;
	static bool				ms_calibrated;
};


//	Create a calibrator which will calibrate the HiRes tick resolution in its constructor
CalibrateHiResTicksPerSec		theCalibrator;

//	Keep the initial tick count for calibration purposes
t_HiResTime		CalibrateHiResTicksPerSec::ms_initialTickCount	= 0LL;

//	Keep the initial timeOfDay for calibration purposes
epicsTime		CalibrateHiResTicksPerSec::ms_initialTimeOfDay;

//	Keep an undefined epicsTime till the epicsTime class supports an isDefined() function
epicsTime		CalibrateHiResTicksPerSec::ms_undefEpicsTime;

//	Scale factor for ticks to seconds
//	The initial value gets overridden by the
//	CalibrateHiResTicksPerSec constructor during startup
double			CalibrateHiResTicksPerSec::ms_hiResTicksPerSec	= 1.33333333e8;

//	Set to true on valid initialization
bool			CalibrateHiResTicksPerSec::ms_initialized		= false;

//	Set to true on valid calibration
bool			CalibrateHiResTicksPerSec::ms_calibrated		= false;


#ifndef	read_tsc

extern "C" t_HiResTime GetHiResTicks()
{
	struct timespec		curTimeVal;
#ifdef _POSIC_CPUTIME
	clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &curTimeVal );
#else
	clock_gettime( CLOCK_MONOTONIC, &curTimeVal );
#endif
	t_HiResTime			hiResTicks	= static_cast<t_HiResTime>( curTimeVal.tv_sec );
	hiResTicks	*= 1000000000L;
	hiResTicks	+= static_cast<t_HiResTime>( curTimeVal.tv_nsec );
	return hiResTicks;
}

#endif	//	read_tsc


extern "C" double  HiResTicksToSeconds(
	t_HiResTime		nTicks	)
{
	double	seconds = CalibrateHiResTicksPerSec::HiResTicksToSeconds( nTicks );
	return seconds;
}


epicsTime HiResToEpicsTime( t_HiResTime valHiRes )
{
	double			dblHiRes = CalibrateHiResTicksPerSec::HiResTicksToSeconds( valHiRes );
	struct timespec	valTimeSpec;
	valTimeSpec.tv_sec	=	static_cast<time_t>(	dblHiRes );
	dblHiRes			-=	static_cast<double>(	valTimeSpec.tv_sec );
	valTimeSpec.tv_nsec	=	static_cast<long int>(	dblHiRes * 1.0e9 );
	epicsTime		valEpicsTime(	valTimeSpec );

	return	valEpicsTime;
}

// For accessing calibrated ticks per second from C code
extern "C" double  	HiResTicksPerSecond( )
{
	return CalibrateHiResTicksPerSec::HiResTicksPerSecond();
}


/* Register GetHiResTicks */
static const iocshFuncDef   GetHiResTicksFuncDef	= { "GetHiResTicks", 0, NULL };
static int  GetHiResTicksCallFunc( const iocshArgBuf * args )
{
	t_HiResTime			hiResTicks	= GetHiResTicks( );
#ifdef linux
	printf( "%lld\n", hiResTicks );
#endif
	return static_cast<int>( hiResTicks );
}
void GetHiResTicksRegister(void)
{
	iocshRegister( &GetHiResTicksFuncDef, reinterpret_cast<iocshCallFunc>(GetHiResTicksCallFunc) );
}
epicsRegisterFunction( GetHiResTicks );


/* Register HiResTicksToSeconds */
static const iocshArg		HiResTicksToSecondsArg0		= { "numTicks", iocshArgInt };
static const iocshArg	*	HiResTicksToSecondsArgs[1]	= { &HiResTicksToSecondsArg0 };
static const iocshFuncDef   HiResTicksToSecondsFuncDef	= { "HiResTicksToSeconds", 1, HiResTicksToSecondsArgs };
static double  HiResTicksToSecondsCallFunc( const iocshArgBuf * args )
{
	double seconds = HiResTicksToSeconds( static_cast<t_HiResTime>( args[0].ival ) );
#ifdef linux
	printf( "%.7e\n", seconds );
#endif
	return seconds;
}
void HiResTicksToSecondsRegister(void)
{
	iocshRegister( &HiResTicksToSecondsFuncDef, reinterpret_cast<iocshCallFunc>(HiResTicksToSecondsCallFunc) );
}
epicsRegisterFunction( HiResTicksToSeconds );

extern "C"
{
	epicsExportRegistrar( GetHiResTicksRegister );
	epicsExportRegistrar( HiResTicksToSecondsRegister );
	epicsExportAddress( int, DEBUG_HI_RES_TIME );
}
