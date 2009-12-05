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
#include "HiResTime.h"
#include "ContextTimer.h"


using	namespace		std;


// No non-inline functions yet


// shell programs to test ContextTimer


extern "C" int
TestContextTimerOverhead( int nLoops )
{
	t_HiResTime		beginHiRes(		ContextTimer::GetCurTime( )	);
	if ( nLoops == 0 )
		nLoops = 10000;

	cout << "TestContextTimerOverhead: " << nLoops	<< " loops ..." << endl;
	for ( int i = 0; i < nLoops; i++ )
	{
		{
		CONTEXT_TIMER( "ContextTimerTest" );
		}
	}

	t_HiResTime	newHiRes(	ContextTimer::GetCurTime( )	);
	t_HiResTime	cumDur		=	newHiRes - beginHiRes;
	double		cumDurSec	=	HiResTicksToSeconds( cumDur );
	cout	<<	"Cum Dur = " <<	cumDurSec	<<	"sec"	<< endl;
	cout	<<	"Avg for "	 << nLoops
			<<	" loops = "	 << cumDurSec / nLoops << "sec" << endl;
	return 0;
}
