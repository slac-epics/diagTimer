#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "epicsThread.h"
#include "iocsh.h"
#include "diagTimerTest.h"
#include <epicsExit.h>
#include <epicsExport.h>
#include <registryFunction.h>

int main(int argc,char *argv[])
{
    if(argc>=2)
	{
        iocsh(argv[1]);
        epicsThreadSleep(.2);
    }

    iocsh(NULL);
	epicsExit(0);
    return(0);
}


extern "C" int runTest( unsigned int testNum )
{
	return diagTimerTest( testNum );
}


/* Register runTest */
static const iocshArg		runTestArg0		= { "testNum", iocshArgInt };
static const iocshArg	*	runTestArgs[1]	= { &runTestArg0 };
static const iocshFuncDef   runTestFuncDef	= { "runTest", 1, runTestArgs };
static void  runTestCallFunc( const iocshArgBuf * args )
{
	runTest( args[0].ival );
}
static void runTestRegister(void)
{
	iocshRegister( &runTestFuncDef, runTestCallFunc );
}

epicsExportRegistrar( runTestRegister );

