#!/bin/tcsh -f

# Copyright 2024 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
#TODO Clean this up and improve it to work on a list of test folders....

set OUT = `pwd`/make_all_ut.out
echo ' ' > $OUT
echo '**** make_all_ut summary ****' >> $OUT

# clean & re-build the test compiled lib
cd $GUNNS_HOME/lib/test; make clean; make;

# test core
set FOLDER = core/test

# TODO these lines are the same for each test folder so a loop would be good...
cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

set FOLDER = core/network/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

set FOLDER = core/optimization/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif


########################################################
# test dynamics aspect
########################################################
set FOLDER = aspects/dynamics/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif


########################################################
# test fluid aspect
########################################################
set FOLDER = aspects/fluid/capacitor/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/fluid/conductor/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/fluid/fluid/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/fluid/hi-fi/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/fluid/potential/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/fluid/source/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif


########################################################
# test thermal aspect
########################################################
set FOLDER = aspects/thermal/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/thermal/network/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif


########################################################
# test electrical aspect
########################################################
set FOLDER = aspects/electrical/Batt/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/electrical/ConstantPowerLoad/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/electrical/Converter/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/electrical/Diode/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
#set FOLDER = aspects/electrical/FetSwitch/test
#
#cd $GUNNS_HOME/$FOLDER; make clean; make;
#if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
#else
#    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
#endif

#
set FOLDER = aspects/electrical/IPS/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/electrical/PowerBus/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/electrical/resistive/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/electrical/sar/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/electrical/SolarArray/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/electrical/Switch/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/electrical/SwitchCard/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = aspects/electrical/UserLoad/test

cd $GUNNS_HOME/$FOLDER; make clean; make;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif


########################################################
# test relevent folders in ms-utils
########################################################
set FOLDER = math/test

cd $MS_UTILS_HOME/$FOLDER; make clean; make;
if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = math/approximation/test

cd $MS_UTILS_HOME/$FOLDER; make clean; make;
if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = math/elementary_functions/test

cd $MS_UTILS_HOME/$FOLDER; make clean; make;
if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = math/linear_algebra/test

cd $MS_UTILS_HOME/$FOLDER; make clean; make;
if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = math/root_finding/test

cd $MS_UTILS_HOME/$FOLDER; make clean; make;
if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = math/time/test

cd $MS_UTILS_HOME/$FOLDER; make clean; make;
if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = parsing/test

cd $MS_UTILS_HOME/$FOLDER; make clean; make;
if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = properties/test

cd $MS_UTILS_HOME/$FOLDER; make clean; make;
if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

# Can't be unit-tested because it has Trick dependencies
#set FOLDER = simulation/hs/test
#
#cd $MS_UTILS_HOME/$FOLDER; make clean; make;
#if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
#else
#    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
#endif

# Can't be unit-tested because it has Trick dependencies
#set FOLDER = software/exceptions/test
#
#cd $MS_UTILS_HOME/$FOLDER; make clean; make;
#if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
#else
#    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
#endif

#
set FOLDER = software/SimBus/test

cd $MS_UTILS_HOME/$FOLDER; make clean; make;
if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = software/SimCompatibility/test

cd $MS_UTILS_HOME/$FOLDER; make clean; make;
if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = fileSearch/test

cd $MS_UTILS_HOME/$FOLDER; make clean; make;
if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = strings/test

cd $MS_UTILS_HOME/$FOLDER; make clean; make;
if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

########################################################
# test gunns-ts-models
########################################################
set FOLDER = aspects/signal/effectors/thermostat/test

cd $TS_MODELS_HOME/$FOLDER; make clean; make;
if ( -f $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = common/controllers/fluid/test

cd $TS_MODELS_HOME/$FOLDER; make clean; make;
if ( -f $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = common/controllers/generic/test

cd $TS_MODELS_HOME/$FOLDER; make clean; make;
if ( -f $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = common/controllers/valveAssemblies/test

cd $TS_MODELS_HOME/$FOLDER; make clean; make;
if ( -f $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = common/effectors/mechanical/motor/test

cd $TS_MODELS_HOME/$FOLDER; make clean; make;
if ( -f $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

#
set FOLDER = common/sensors/test

cd $TS_MODELS_HOME/$FOLDER; make clean; make;
if ( -f $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif


########################################################
# Summarize results to screen
########################################################
echo ' ' >> $OUT
tail -n 100 $OUT
cd $GUNNS_HOME

