# Copyright 2024 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
# All source folders containing run-time code to be compiled in libraries
# should be included in this list.  We grab all .cpp's by default, and
# specific classes can be omitted later.
SOURCES := \
 $(wildcard $(GUNNS_HOME)/aspects/dynamics/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/electrical/Batt/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/electrical/Converter/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/electrical/Diode/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/electrical/ConstantPowerLoad/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/electrical/IPS/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/electrical/PowerBus/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/electrical/FetSwitch/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/electrical/SolarArray/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/electrical/Switch/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/electrical/SwitchCard/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/electrical/UserLoad/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/electrical/resistive/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/electrical/sar/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/electrical/TripLogic/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/fluid/capacitor/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/fluid/conductor/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/fluid/fluid/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/fluid/hi-fi/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/fluid/potential/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/fluid/source/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/thermal/*.cpp) \
 $(wildcard $(GUNNS_HOME)/aspects/thermal/network/*.cpp) \
 $(wildcard $(GUNNS_HOME)/core/*.cpp) \
 $(wildcard $(GUNNS_HOME)/core/network/*.cpp) \
 $(wildcard $(GUNNS_HOME)/core/optimization/*.cpp) \
 $(wildcard $(GUNNS_HOME)/gunns-ts-models/aspects/signal/effectors/thermostat/*.cpp) \
 $(wildcard $(GUNNS_HOME)/gunns-ts-models/common/controllers/fluid/*.cpp) \
 $(wildcard $(GUNNS_HOME)/gunns-ts-models/common/controllers/generic/*.cpp) \
 $(wildcard $(GUNNS_HOME)/gunns-ts-models/common/controllers/valveAssemblies/*.cpp) \
 $(wildcard $(GUNNS_HOME)/gunns-ts-models/common/effectors/mechanical/motor/*.cpp) \
 $(wildcard $(GUNNS_HOME)/gunns-ts-models/common/sensors/*.cpp) \
 $(wildcard $(GUNNS_HOME)/ms-utils/properties/*.cpp) \
 $(wildcard $(GUNNS_HOME)/ms-utils/software/SimBus/*.cpp) \
 $(wildcard $(GUNNS_HOME)/ms-utils/software/SimCompatibility/*.cpp) \
 $(wildcard $(GUNNS_HOME)/ms-utils/software/exceptions/*.cpp) \
 $(wildcard $(GUNNS_HOME)/ms-utils/math/*.cpp) \
 $(wildcard $(GUNNS_HOME)/ms-utils/math/approximation/*.cpp) \
 $(wildcard $(GUNNS_HOME)/ms-utils/math/elementary_functions/*.cpp) \
 $(wildcard $(GUNNS_HOME)/ms-utils/math/linear_algebra/*.cpp) \
 $(wildcard $(GUNNS_HOME)/ms-utils/math/root_finding/*.cpp) \
 $(wildcard $(GUNNS_HOME)/ms-utils/math/time/*.cpp) \
 $(wildcard $(GUNNS_HOME)/ms-utils/fileSearch/*.cpp) \
 $(wildcard $(GUNNS_HOME)/ms-utils/parsing/*.cpp) \
 $(wildcard $(GUNNS_HOME)/ms-utils/parsing/tinyxml/*.cpp) \
 $(wildcard $(GUNNS_HOME)/ms-utils/simulation/hs/*.cpp) \
 $(wildcard $(GUNNS_HOME)/ms-utils/simulation/timer/*.c) \
 $(wildcard $(GUNNS_HOME)/ms-utils/units/*.c)
