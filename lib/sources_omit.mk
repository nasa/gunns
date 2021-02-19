# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
# Omit these files because they're broken or not finished:
SOURCES := $(filter-out %/GunnsGasCooledTurbine.cpp,$(SOURCES))
SOURCES := $(filter-out %/FetSwitch.cpp,$(SOURCES))
SOURCES := $(filter-out %/FetSwitchElect.cpp,$(SOURCES))

# Omit because obsolete, nobody uses it, and doesn't play nice with trickified lib:
SOURCES := $(filter-out %/TS_hs_msg.cpp,$(SOURCES))
SOURCES := $(filter-out %/TS_sgmt_conversion.c,$(SOURCES))

# Omit because we are disabling dependencies on sqlite3:
SOURCES := $(filter-out %/TsHsSqlitePlugin.cpp,$(SOURCES))
