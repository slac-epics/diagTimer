TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# build an ioc application

PROD_IOC = testApp

DBD += testApp.dbd

# testApp.dbd will be made up from these files:
testApp_DBD += base.dbd
#include definitions for any other support applications needed
testApp_DBD += runTest.dbd
testApp_DBD += diagTimer.dbd

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
testApp_SRCS += testApp_registerRecordDeviceDriver.cpp
testApp_SRCS += diagTimerTest.c
testApp_SRCS_DEFAULT += testAppMain.cpp
testApp_SRCS_vxWorks += -nil-

#The following adds support from base/src/vxWorks
testApp_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

#add a definition for each support application used by this application
testApp_LIBS += diagTimer

testApp_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

