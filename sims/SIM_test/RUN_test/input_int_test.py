# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
#---------------------------------------------
# Call basic setup for integration tests
#---------------------------------------------
f = "RUN_test/input.py"
exec(compile(open(f, "rb").read(), f, 'exec'), globals(), locals())

#---------------------------------------------
# Setup Trick
#---------------------------------------------
f = "RUN_test/int_test_trick_setup.py"
exec(compile(open(f, "rb").read(), f, 'exec'), globals(), locals())

#---------------------------------------------
# Call integration tests
#---------------------------------------------
trick_utest.unit_tests.enable()
trick_utest.unit_tests.set_file_name( "RUN_test/results/SIM_test_int_test_results.xml" )
f = "int_tests/SimTestSuite.py"
exec(compile(open(f, "rb").read(), f, 'exec'), globals(), locals())
