#include "iostream"
#include "diagTimerTest.h"
#include "HiResTime.h"
#include "ContextTimer.h"

using namespace	std;

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
	default:
		cout << "diagTimerTest: Unsupported test number " << testNum << endl;
		break;
	}

	return -1;
}
