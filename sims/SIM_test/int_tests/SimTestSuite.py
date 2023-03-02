# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
import os
INT_TESTER_HOME = os.environ["GUNNS_HOME"]+"/test/utils/intTester/"
#===============================================================================================
# Integration test includes
#===============================================================================================
f = INT_TESTER_HOME+"TestSuite.py"
exec(compile(open(f, "rb").read(), f, 'exec'), globals(), locals())
f = "int_tests/TestFluidNetwork.py"
exec(compile(open(f, "rb").read(), f, 'exec'), globals(), locals())
f = "int_tests/TestBasicNetwork.py"
exec(compile(open(f, "rb").read(), f, 'exec'), globals(), locals())
f = "int_tests/TestThermalNetwork.py"
exec(compile(open(f, "rb").read(), f, 'exec'), globals(), locals())
f = "int_tests/TestHsLog.py"
exec(compile(open(f, "rb").read(), f, 'exec'), globals(), locals())

#===============================================================================================
# Test Header
#===============================================================================================
def printTestSuiteHeader():
    """The standard test header"""
    print ("""
=================================================================================================
 Test Suite Title    GUNNS SIM_test - Simple Network Tests.
 Authors             Jason Harvey
 Date                01/22/2015
-------------------------------------------------------------------------------------------------
 Test Type           This is a simple network test. It verifies that simple GunnShow-generated
                     electrical, thermal and fluid networks can initialize, go to run, and trend
                     properly.
 Subsystems Tested   gunns/core
 Verif Method        Event Based
-------------------------------------------------------------------------------------------------
 Items under test    Networks: sims/networks/basic/test/TestBasicNetwork,
                               sims/networks/fluid/test/TestFluidNetwork,
                               sims/networks/thermal/gunnshowThermalTest/GunnShowThermalTest.
                     Super-Networks: core/network/GunnsBasicSuperNetwork,
                                     core/network/GunnsFluidSuperNetwork
 Test Objectives     1) Verify all network nodes initialize to nominal properties.
                     2) Verify networks initialize with no errors.
                     3) Verify networks go to run with no errors.
                     4) Verify network state and trends after a few seconds in run.
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) networks at default initial conditions from the GunnShow drawings.

-------------------------------------------------------------------------------------------------

=================================================================================================
    """)

#===============================================================================================
# Test execution
#===============================================================================================

#--- give the sim 0 sec then print our header
trick.add_read(0.0, """printTestSuiteHeader()""" )

#===============================================================================================
# Test Suite construction:
# Construct a TestSuite object with a tear down time. This is a test manager class that allows
# you to register tests and then run either a single test within the suite or run all the tests
# in the order they were registered.
#===============================================================================================
 
# tearDownTime : The amount of time added to the last scheduled test event time before shutting
# down the entire sim.
tearDownTime = 3.0   # 3 seconds seems to be a reasonable value for most cases.
simTestSuiteRunner = TestSuite(tearDownTime)

# Construct a test by giving it a name string, a starting message string, and a finished message string
fluidNetworkTest = TestFluidNetwork(
# Test Name String
"""GUNNS SIM_test : Fluid Network Test""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_test
 Test Number 1       Fluid Network Test
 Author              Jason Harvey
 Date                01/22/2015
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/TestFluidNetwork

 Test Objectives     1) Verify all network nodes initialize to nominal properties.
                     2) Verify network initializes with no errors.
                     3) Verify network goes to run with no errors.
                     4) Verify network state and trends after a few seconds in run.
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnShow drawing.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
basicNetworkTest = TestBasicNetwork(
# Test Name String
"""GUNNS SIM_test : Basic Network Test""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_test
 Test Number 2       Basic Network Test
 Author              Jason Harvey
 Date                01/22/2015
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/basic/test/TestBasicNetwork

 Test Objectives     1) Verify all network nodes initialize to nominal properties.
                     2) Verify network initializes with no errors.
                     3) Verify network goes to run with no errors.
                     4) Verify network state and trends after a few seconds in run.
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnShow drawing.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
thermalNetworkTest = TestThermalNetwork(
# Test Name String
"""GUNNS SIM_test : Thermal Network Test""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_test
 Test Number 2       Thermal Network Test
 Author              Jason Harvey
 Date                07/29/2015
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/thermal/gunnshowThermalTest/GunnShowThermalTest

 Test Objectives     1) Verify all network nodes initialize to nominal properties.
                     2) Verify network initializes with no errors.
                     3) Verify network goes to run with no errors.
                     4) Verify network state and trends after a few seconds in run.
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnShow drawing.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
hsLogTest = TestHsLog(
# Test Name String
"""GUNNS SIM_test : Health and Status Log Test""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_test
 Test Number 3       Health and Status Log Test
 Author              Jason Harvey
 Date                01/22/2015
-------------------------------------------------------------------------------------------------
 Items under test    logs/TS_Health_and_Status.out

 Test Objectives     1) Verify all networks initialize and go to run with no errors.
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) Empty HS logs.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# After constructing your test, register the test with your test suite.
simTestSuiteRunner.registerTest(fluidNetworkTest)
simTestSuiteRunner.registerTest(basicNetworkTest)
simTestSuiteRunner.registerTest(thermalNetworkTest)
simTestSuiteRunner.registerTest(hsLogTest)
# Initialize the Trick data logging with the data logging rate.
simTestSuiteRunner.initLog("simTestSuiteRunner", 0.1)

# To run all registered tests within the test suite, add a read job to call runAllTests().
trick.add_read(0.0, """simTestSuiteRunner.runAllTests()""" )
