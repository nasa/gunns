#!/bin/tcsh -f

# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
#TODO Clean this up and improve it to work on a list of test folders....

set OUT = `pwd`/make_all_ut_roses.out
echo ' ' > $OUT
echo '**** make_all_ut_roses summary ****' >> $OUT

# clean & re-build the test compiled lib
cd $GUNNS_HOME/lib/test; make clean; make roses;

# test core
set FOLDER = core/test

# TODO these lines are the same for each test folder so a loop would be good...
cd $GUNNS_HOME/$FOLDER; make clean; make -f Makefile.roses;
if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

########################################################
# test relevent folders in ms-utils
########################################################
set FOLDER = math/linear_algebra/cuda/test

cd $MS_UTILS_HOME/$FOLDER; make clean; make -f Makefile.roses;
if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
else
    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
endif

########################################################
# Summarize results to screen
########################################################
echo ' ' >> $OUT
tail -n 100 $OUT
cd $GUNNS_HOME

