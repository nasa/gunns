# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
# Omit these files because they use Trick message_publish:
SOURCES := $(filter-out %/TsHsConsolePlugin.cpp,$(SOURCES))
SOURCES := $(filter-out %/TsHsMsgStdFilter.cpp,$(SOURCES))
SOURCES := $(filter-out %/TsHsStdMngr.cpp,$(SOURCES))
SOURCES := $(filter-out %/TsHsTextPlugin.cpp,$(SOURCES))

# Omit these becaue they use Trick memory manager:
SOURCES := $(filter-out %/TsMemoryManager.cpp,$(SOURCES))

# Omit these because they aren't needed for Trick-less and would complicate the makefile:
SOURCES := $(filter-out %/TS_timer.c,$(SOURCES))
SOURCES := $(filter-out %/TS_conversions.c,$(SOURCES))
