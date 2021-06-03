#!/usr/bin/python
#
# @copyright Copyright 2021 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, June 2021, --, Initial implementation.}
# @revs_end
#
# This builds and runs the SIM_class_test_compile sim for every C++ class listed in
# gunns/lib/trick_if/S_source.hh, minus those listed in class_ignore_list.py.
#
# For each class to be tested, the following process is performed:
# - make clean the sim,
# - auto-generate the test_class.sh Trick sim object to instantiate the test class in the sim,
# - build the sim and handle & report on build errors.
#
# This requires Trick 19 or later.

# duration?
# on zentraedi, ~40 minutes

import os
import sys

# Make & build the test sim for the given class header file, concatenate any build errors
# to the overall output errors file.
def testtype(classpath):
    os.system('make clean')
    os.system('python gen_sm.py ' + classpath)
    os.system('trick-CP')
    os.system('cat build/MAKE_err >> test_all_output_errors')

###################
# Begin Main Script
###################

#TODO change to the Python3 compatible way:
execfile('class_ignore_list.py')

os.system('rm test_all_output_errors')

#TODO
# read gunns/lib/trick_if/S_source.hh (requires lib/trick to have been built).
with open('../../lib/trick_if/S_source.hh', 'r') as fsources:
    # each line is similar to:
    # #include "aspects/dynamics/GunnsDynEuler321.hh"
    # get the file path from that string by stripping off the leading whitespace and #include
    for line in fsources:
        classpath = line.strip().lstrip('#include ').strip('"')
        
        #skip if the extension is not hh. We're only testing C++ classes
        if classpath is not '':
            if 'hh' == os.path.basename(classpath).split('.')[1]:
                classtype = os.path.basename(classpath).split('.')[0]
            
                #skip of the class type is in the class ignore list
                if classtype not in class_ignore_list:
                    testtype(classpath)
            