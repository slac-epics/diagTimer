/* ****************************************************************************
 *
 *  ContextTimer.cpp
 *  Created by Bruce Hill, on Fri Nov 5 22:36:36 PDT 2004
 *
 *  Copyright (c) 2004, Euphonix, Inc.
 *
 *  File Contents: Implementation of the ContextTimer component.
 *  Please see ContextTimer.h for full documentation of this system.
 *
 *************************************************************************** */
 
#include <iostream>
#include <iocsh.h>
#include <epicsExport.h>
#include "HiResTime.h"
#include "ContextTimer.h"


using	namespace		std;


// No non-inline functions yet


// shell programs to test ContextTimer


extern "C" int testTimerAccuracy( unsigned int nSeconds )
{
	if ( nSeconds == 0 || nSeconds > 60 )
	{
		cerr << "testTimerAccuracy: Invalid test duration " << nSeconds << endl;
		return -1;
	}

	cout	<< "testTimerAccuracy: Checking timer accuracy.  Please wait "
			<< nSeconds	<< " seconds ..." << endl;
	epicsTime		startTimeOfDay	= epicsTime::getCurrent();
	t_HiResTime		startTime		= GetHiResTicks();
	sleep( nSeconds );
	t_HiResTime		endTime			= GetHiResTicks();
	epicsTime		endTimeOfDay	= epicsTime::getCurrent();
	double			durHiRes		= HiResTicksToSeconds( endTime )
									- HiResTicksToSeconds( startTime );
	double			durTimeOfDay	= endTimeOfDay - startTimeOfDay;

	cout << "testTimerAccuracy: startTime=" << startTime << " ticks, "
		 << HiResTicksToSeconds(startTime) << " sec" << endl;
	cout << "testTimerAccuracy:   endTime=" <<   endTime << " ticks, "
		 << HiResTicksToSeconds(  endTime) << " sec" << endl;
	cout << "testTimerAccuracy: Target dur " << durTimeOfDay
		<<	" sec, measured HiRes dur " << durHiRes	<< " sec" << endl;
	return 0;
}


extern "C" int testTimerOverhead( unsigned int nLoops )
{
	if( nLoops == 0 )
		nLoops = 10000;

	cout << "testTimerOverhead: " << nLoops	<< " loops ..." << endl;
	t_HiResTime		beginHiRes(		ContextTimer::GetCurTime( )	);
	for ( unsigned int i = 0; i < nLoops; i++ )
	{
		{
		CONTEXT_TIMER( "ContextTimerTest" );
		}
	}
	t_HiResTime	endHiRes(	ContextTimer::GetCurTime( )	);

	t_HiResTime	cumDur		=	endHiRes - beginHiRes;
	double		cumDurSec	=	HiResTicksToSeconds( cumDur );
	cout	<<	"Avg for "	 << nLoops
			<<	" loops = "	 << (cumDurSec / nLoops) * 1.0e9 << " ns" << endl;
	return 0;
}


/* Register testTimerOverhead */
static const iocshArg		testTimerOverheadArg0		= { "numLoops", iocshArgInt };
static const iocshArg	*	testTimerOverheadArgs[1]	= { &testTimerOverheadArg0 };
static const iocshFuncDef   testTimerOverheadFuncDef	= { "testTimerOverhead", 1, testTimerOverheadArgs };
static void  testTimerOverheadCallFunc( const iocshArgBuf * args )
{
	testTimerOverhead( args[0].ival );
}
static void testTimerOverheadRegister(void)
{
	iocshRegister( &testTimerOverheadFuncDef, testTimerOverheadCallFunc );
}

epicsExportRegistrar( testTimerOverheadRegister );


/* Register testTimerAccuracy */
static const iocshArg		testTimerAccuracyArg0		= { "numSec", iocshArgInt };
static const iocshArg	*	testTimerAccuracyArgs[1]	= { &testTimerAccuracyArg0 };
static const iocshFuncDef   testTimerAccuracyFuncDef	= { "testTimerAccuracy", 1, testTimerAccuracyArgs };
static void  testTimerAccuracyCallFunc( const iocshArgBuf * args )
{
	testTimerAccuracy( args[0].ival );
}
static void testTimerAccuracyRegister(void)
{
	iocshRegister( &testTimerAccuracyFuncDef, testTimerAccuracyCallFunc );
}

epicsExportRegistrar( testTimerAccuracyRegister );

