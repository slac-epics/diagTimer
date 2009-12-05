TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library

LIBRARY_IOC += diagTimer

# Install these header files
INC	+=	HiResTime.h
INC	+=	ContextTimer.h
INC	+=	ContextTimerMax.h


# diagTimer.dbd will be created and installed
#DBD += diagTimer.dbd

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
#diagTimer_SRCS += diagTimer_registerRecordDeviceDriver.cpp
diagTimer_SRCS_vxWorks += -nil-

# Add locally compiled object code
diagTimer_SRCS += HiResTime.cpp
diagTimer_SRCS += ContextTimer.cpp
diagTimer_SRCS += ContextTimerMax.cpp

diagTimerLib_SRCS	+= $(diagTimer_SRCS)

#diagTimer_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

