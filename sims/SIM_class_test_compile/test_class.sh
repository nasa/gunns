#!/usr/bin/bash
#
# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, May 2021, --, Initial implementation.}
# @revs_end
#
# This will attempt to build this Trick sim containing one instance of the class defined in
# the given class header file.  This assumes the following:
# - the given header file declares a class of the same name,
# - the class is not pure-virtual,
# - the class has a compilable default constructor.
#
# Invoke this script with a gunns/ -relative path to the class header file to test.
# Example:
# $ source test_class.sh aspects/electrical/Switch/GunnsElectUserLoadSwitch.hh
#
make clean
python3 gen_sm.py $1
trick-CP

