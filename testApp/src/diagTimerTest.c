#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "diagTimerTest.h"
#include "HiResTime.h"
#include "ContextTimer.h"


double timeValToSec( struct timeval timeVal )
{
	double		result	= timeVal.tv_sec;
	result += (double) timeVal.tv_usec / 1000000.0;
	return result;
}


int testTimerFromC( unsigned int nSeconds )
{
	if ( nSeconds == 0 || nSeconds > 60 )
	{
		fprintf( stderr, "testTimerFromC: Invalid test duration %u\n", nSeconds );
		return -1;
	}

	printf( "testTimerFromC: Checking timer accuracy.  Please wait %u seconds ...\n", nSeconds );
	struct timeval		startTimeOfDay;
	gettimeofday( &startTimeOfDay, NULL );
	t_HiResTime		startTime		= GetHiResTicks();
	sleep( nSeconds );
	t_HiResTime		endTime			= GetHiResTicks();
	struct timeval	endTimeOfDay;
	gettimeofday( &endTimeOfDay, NULL );
	double			durHiRes		= HiResTicksToSeconds( endTime )
									- HiResTicksToSeconds( startTime );
	double			durTimeOfDay	= timeValToSec(endTimeOfDay) - timeValToSec(startTimeOfDay);

	printf( "testTimerFromC: startTime=%llu ticks, %f sec\n",
			startTime, HiResTicksToSeconds(startTime) );
	printf( "testTimerFromC:   endTime=%llu ticks, %f sec\n",
			endTime, HiResTicksToSeconds(  endTime) );
	printf( "testTimerFromC: Target dur %f sec, measured HiRes dur %f sec\n",
			durTimeOfDay, durHiRes	);
	return 0;
}

int diagTimerTest( unsigned int testNum )
{
	switch ( testNum )
	{
	case 0:
		return testTimerAccuracy( 10 );
		break;
	case 1:
		return testTimerOverhead( 1000 );
		break;
	case 2:
		return testTimerFromC( 10 );
		break;
	default:
		printf( "diagTimerTest: Unsupported test number %u\n", testNum );
		break;
	}

	return -1;
}
