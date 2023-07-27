#!/usr/bin/env python3
#
# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, June 2021, --, Initial implementation.}
# @revs_end
#
# This builds and runs the SIM_class_test_compile sim for every C++ class listed in
# the local class_test_list.py file and also present in gunns/lib/trick_if/S_source.hh,
# minus those listed in class_ignore_list.py.
#
# For each class to be tested, the following process is performed:
# - make clean the sim,
# - auto-generate the test_class.sh Trick sim object to instantiate the test class in the sim,
# - build the sim and handle & report on build errors.
#
# This requires Trick 19 or later, and Python 3.
#
# This could be parallelized to run faster, but would take a lot of effort to set up.
# Each core could run a separate class build, but would have to use separate ClassTest.sm
# files and sim folders to avoid stepping on each other.

import os
import sys

# Make & build the test sim for the given class header file, concatenate any build errors
# to the overall output errors file.
def testtype(classpath):
    os.system('make clean')
    os.system('python3 gen_sm.py ' + classpath)
    os.system('trick-CP')
    if os.path.isfile('build/MAKE_err'):
        os.system('cat build/MAKE_err >> test_all_output_errors')

###################
# Begin Main Script
###################

exec(compile(open('class_ignore_list.py', "rb").read(), 'class_ignore_list.py', 'exec'))

outputfile = 'test_all_output_errors'
if os.path.isfile(outputfile):
    os.system('rm ' + outputfile)

# Read class_test_list.py into a python list.
# The file is written as 1 name per line.
testlist = []
with open('class_test_list.py', 'r') as flist:
    for line in flist:
        #the path is relative to gunns/
        classpath = line.strip()

        #skip if the extension is not for C++. We're only testing C++ classes
        if classpath != '':
            if (os.path.basename(classpath).endswith('.hh') or os.path.basename(classpath).endswith('.cpp')):
                testlist.append(os.path.basename(classpath).split('.')[0])

# Read gunns/lib/trick_if/S_source.hh (requires lib/trick to have been built).
anytests = False
with open('../../lib/trick_if/S_source.hh', 'r') as fsources:
    # each line is similar to:
    # #include "aspects/dynamics/GunnsDynEuler321.hh"
    # get the file path from that string by stripping off the leading whitespace and #include
    for line in fsources:
        classpath = line.strip().lstrip('#include ').strip('"')
        
        #skip if the extension is not hh. We're only testing C++ classes
        if classpath != '':
            if 'hh' == os.path.basename(classpath).split('.')[1]:
                classtype = os.path.basename(classpath).split('.')[0]
            
                #skip if the class type is in the class ignore list or
                #is not in the test list
                if (classtype not in class_ignore_list) and (classtype in testlist):
                    print("\n\n-=#=-=#=-=#=-=#=-=#=-")
                    print("Testing ", classpath, ":")
                    anytests = True
                    testtype(classpath)

if anytests:
    # Scan the output file for errors, output overall pass/fail
    outputfile = os.environ["GUNNS_HOME"]+"/sims/SIM_class_test_compile/test_all_output_errors"
    if os.path.isfile(outputfile):
        results = open(outputfile).read()
        print('\n\n-=#=-=#=-=#=-=#=-=#=-\nContents of test_all_output_errors file:\n')
        print(results)
        if 'error' in results or 'undefined' in results or 'unresolved' in results:
            print("-=#=-=#=-=#=-=#=-=#=-")
            print('TEST FAILED')
            sys.exit(1)
    else:
        print("-=#=-=#=-=#=-=#=-=#=-")
        print('test_all_output_errors file is missing!')
        print('TEST FAILED')
        sys.exit(1)
else:
    print("-=#=-=#=-=#=-=#=-=#=-")
    print("There were no classes to test.")
    
print("-=#=-=#=-=#=-=#=-=#=-")
print('TEST PASSED')
sys.exit(0)
