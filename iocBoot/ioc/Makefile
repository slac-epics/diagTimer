# Makefile to install the st.cmd into the correctly location.
# Author: Jason Tiller

TOP = ../..
include $(TOP)/configure/CONFIG
ARCH = linux-x86_64
TARGETS = envPaths

# See RULES.copy for more information on these variables.
COPY_ONLY_IF_NON_LOCAL=1
COPY_SUBDIR=iocBoot/iocdiagTimer
COPY_FILES=st.cmd

include $(TOP)/configure/RULES.ioc
# Don't really need this for our development usage
# include $(TOP)/configure/RULES.copy
