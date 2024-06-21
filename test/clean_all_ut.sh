#!/bin/tcsh -f

# Copyright 2024 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
#TODO Clean this up and improve it to work on a list of test folders....

# TODO these lines are the same for each test folder so a loop would be good...
set FOLDER = core/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = core/network/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/dynamics/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/fluid/capacitor/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/fluid/conductor/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/fluid/fluid/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/fluid/hi-fi/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/fluid/potential/test
cd $GUNNS_HOME/$FOLDER; make clean;
 
set FOLDER = aspects/fluid/source/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/thermal/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/thermal/network/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/electrical/Batt/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/electrical/ConstantPowerLoad/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/electrical/Converter/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/electrical/Diode/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/electrical/IPS/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/electrical/PowerBus/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/electrical/resistive/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/electrical/sar/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/electrical/SolarArray/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/electrical/Switch/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/electrical/SwitchCard/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = aspects/electrical/UserLoad/test
cd $GUNNS_HOME/$FOLDER; make clean;

set FOLDER = math/test
cd $MS_UTILS_HOME/$FOLDER; make clean;

set FOLDER = math/approximation/test
cd $MS_UTILS_HOME/$FOLDER; make clean;

set FOLDER = math/elementary_functions/test
cd $MS_UTILS_HOME/$FOLDER; make clean;

set FOLDER = math/linear_algebra/test
cd $MS_UTILS_HOME/$FOLDER; make clean;

set FOLDER = math/linear_algebra/cuda/test
cd $MS_UTILS_HOME/$FOLDER; make clean;

set FOLDER = math/root_finding/test
cd $MS_UTILS_HOME/$FOLDER; make clean;

set FOLDER = math/time/test
cd $MS_UTILS_HOME/$FOLDER; make clean;

set FOLDER = parsing/test
cd $MS_UTILS_HOME/$FOLDER; make clean;

set FOLDER = properties/test
cd $MS_UTILS_HOME/$FOLDER; make clean;

set FOLDER = software/SimBus/test
cd $MS_UTILS_HOME/$FOLDER; make clean;

set FOLDER = software/SimCompatibility/test
cd $MS_UTILS_HOME/$FOLDER; make clean;

set FOLDER = fileSearch/test
cd $MS_UTILS_HOME/$FOLDER; make clean;

set FOLDER = aspects/signal/effectors/thermostat/test
cd $TS_MODELS_HOME/$FOLDER; make clean;

set FOLDER = common/controllers/fluid/test
cd $TS_MODELS_HOME/$FOLDER; make clean;

set FOLDER = common/controllers/valveAssemblies/test
cd $TS_MODELS_HOME/$FOLDER; make clean;

set FOLDER = common/effectors/mechanical/motor/test
cd $TS_MODELS_HOME/$FOLDER; make clean;

set FOLDER = common/sensors/test
cd $TS_MODELS_HOME/$FOLDER; make clean;

# clean the test compiled lib
cd $GUNNS_HOME/lib/test; make clean;

