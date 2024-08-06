# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
#---------------------------------------------
# Call basic setup for integration tests
#---------------------------------------------
execfile( "gitlab_test/input.py")

#---------------------------------------------
# Setup Trick
#---------------------------------------------
execfile("gitlab_test/int_test_trick_setup.py")

#---------------------------------------------
# Call integration tests
#---------------------------------------------
trick_utest.unit_tests.enable()
trick_utest.unit_tests.set_file_name( "gitlab_test/results/SIM_test_int_test_results.xml" )
execfile("int_tests/SimTestSuite.py")
