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

# extract class type from the given path:
classType = os.path.basename(sys.argv[1]).split('.')[0]

with open('ClassTest.sm', 'w') as fout:
    fout.write('//Include the code under test:\n')
    fout.write('##include "' + sys.argv[1] + '"\n')
    fout.write('\n')
    fout.write('class ClassTestSimObject: public Trick::SimObject\n')
    fout.write('{\n')
    fout.write('    public:\n')
    fout.write('        ' + classType + ' code; /**< (1) code under test */\n')
    fout.write('        ClassTestSimObject()\n')
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
