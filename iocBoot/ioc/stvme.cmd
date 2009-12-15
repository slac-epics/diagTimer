# RTEMS startup script
cd( "../.." )

ld( "bin/RTEMS-beatnik/testApp.obj" )

## Register all support components
dbLoadDatabase("dbd/diagTimer.dbd",0,0)
dbLoadDatabase("dbd/testApp.dbd",0,0)
testApp_registerRecordDeviceDriver(pdbbase) 

## Load record instances
#dbLoadRecords("db/dbExample1.db","user=bhillHost")

## Set this to see messages from mySub
#var mySubDebug 1

# No need to call iocInit() since this test program
# doesn't load any db databases
# iocInit()

# To run the diag timer tests, type
# runTest( 0 )    for timer accuracy test
# runTest( 1 )    for timer overhead test

