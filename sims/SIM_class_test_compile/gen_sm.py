#!/usr/bin/python
# @copyright Copyright 2021 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, May 2021, --, Initial implementation.}
# @revs_end
#
# This script auto-generates a Trick Sim Object class code file called ClassTest.sm, that
# instantiates one instance of the class defined in the given source file.  This assumes:
# - the given source header file declares a class of the same name as the filename.
# - the class has a compilable default constructor
import os
import sys

if (len(sys.argv) != 2):
    quit();

# Python 2.7 vs. 3 by feature detection.
try:
    execfile('class_custom_construction.py')
except NameError:
    exec(compile(open('class_custom_construction.py', "rb").read(), 'class_custom_construction.py', 'exec'))

# extract class type from the given path:
classType = os.path.basename(sys.argv[1]).split('.')[0]

if classType in class_custom_construction:
    includeStatement   = class_custom_construction[classType]['include']
    declareStatement   = class_custom_construction[classType]['declare']
    constructStatement = class_custom_construction[classType]['construct']
else:
    # default syntax
    includeStatement   = '##include "' + sys.argv[1] + '"\n'
    declareStatement   = '        ' + classType + ' code; /**< (1) code under test */\n'
    constructStatement = ''

with open('ClassTest.sm', 'w') as fout:
    fout.write('//Include the code under test:\n')
    fout.write(includeStatement)
    fout.write('\n')
    fout.write('class ClassTestSimObject: public Trick::SimObject\n')
    fout.write('{\n')
    fout.write('    public:\n')
    fout.write(declareStatement)
    fout.write('        ClassTestSimObject()\n')
    fout.write(constructStatement)
    fout.write('        {\n')
    fout.write('            // nothing to do\n')
    fout.write('        }\n')
    fout.write('\n')
    fout.write('        virtual ~ClassTestSimObject() { /* nothing to do */ }\n')
    fout.write('\n')
    fout.write('    private:\n')
    fout.write('        // Copy constructor unavailable since declared private and not implemented.\n')
    fout.write('        ClassTestSimObject(const ClassTestSimObject&);\n')
    fout.write('        // Assignment operator unavailable since declared private and not implemented.\n')
    fout.write('        ClassTestSimObject operator = (const ClassTestSimObject&);\n')
    fout.write('};\n')

print('Generated ClassTest.sm for ' + classType + '.')
