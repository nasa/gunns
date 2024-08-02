# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
import os
INT_TESTER_HOME = os.environ["GUNNS_HOME"]+"/test/utils/intTester/"
#===============================================================================================
# Integration test includes
#===============================================================================================
execfile(INT_TESTER_HOME+"TestSuite.py")
execfile("int_tests/TestOverflow1.py")
execfile("int_tests/TestOverflow2.py")
execfile("int_tests/TestOverflow3.py")
execfile("int_tests/TestOverflow4.py")
execfile("int_tests/TestOverflow5.py")
execfile("int_tests/TestOverflow6.py")
execfile("int_tests/TestOverflow7.py")
execfile("int_tests/TestOverflow8.py")
execfile("int_tests/TestOverflow9.py")
execfile("int_tests/TestOverflow10.py")
execfile("int_tests/TestOverflow11.py")
execfile("int_tests/TestOverflow12.py")
execfile("int_tests/TestOverflow12err.py")
execfile("int_tests/TestOverflow13.py")
execfile("int_tests/TestOverflow14.py")
execfile("int_tests/TestOverflow14err.py")
execfile("int_tests/TestOverflow15.py")
execfile("int_tests/TestOverflow16.py")
execfile("int_tests/TestOverflow17.py")
execfile("int_tests/TestOverflow18.py")
execfile("int_tests/TestOverflow19.py")
execfile("int_tests/TestOverflow20.py")
execfile("int_tests/TestOverflow21.py")
execfile("int_tests/TestOverflow22.py")
execfile("int_tests/TestOverflow23.py")
execfile("int_tests/TestOverflow24.py")
execfile("int_tests/TestOverflow25.py")
execfile("int_tests/TestOverflow26.py")
execfile("int_tests/TestOverflow27.py")
execfile("int_tests/TestOverflow28.py")
execfile("int_tests/TestOverflow29.py")
execfile("int_tests/TestOverflow30.py")
execfile("int_tests/TestOverflow31.py")
execfile("int_tests/TestOverflow32.py")
execfile("int_tests/TestOverflow33.py")
execfile("int_tests/TestOverflow34.py")
execfile("int_tests/TestOverflow35.py")
execfile("int_tests/TestOverflow36.py")
execfile("int_tests/TestOverflow37.py")
execfile("int_tests/TestOverflow38.py")
execfile("int_tests/TestOverflow39.py")
execfile("int_tests/TestOverflow40.py")
execfile("int_tests/TestOverflow41.py")
execfile("int_tests/TestOverflow42.py")
execfile("int_tests/TestOverflow43.py")
execfile("int_tests/TestOverflow44.py")
execfile("int_tests/TestOverflow45.py")
execfile("int_tests/TestOverflow45over.py")
execfile("int_tests/TestOverflow46.py")
execfile("int_tests/TestOverflow47.py")
execfile("int_tests/TestOverflow48.py")

#===============================================================================================
# Test Header
#===============================================================================================
def printTestSuiteHeader():
    """The standard test header"""
    print ("""
=================================================================================================
 Test Suite Title    GUNNS SIM_mass_overflow - Mass overflow tests
 Authors             Jason Harvey
 Date                05/29/2019
-------------------------------------------------------------------------------------------------
 Test Type           This tests conservation of mass & energy in fluid networks.  All fluid links
                     are tested flowing to/from overflowing nodes.
 Subsystems Tested   gunns/core, gunns/aspects/fluid
 Verif Method        Event Based
-------------------------------------------------------------------------------------------------
 Items under test    Networks: sims/networks/fluid/test/overflow/TestOverflow
                               sims/networks/fluid/test/overflow/TestOverflow2
                               sims/networks/fluid/test/overflow/TestOverflow3
                               sims/networks/fluid/test/overflow/TestOverflow4
                               sims/networks/fluid/test/overflow/TestOverflow5
 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) networks at default initial conditions from the GunnDraw drawings
                     2) overrides from RUN_test/input.py.
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
tearDownTime = 0.1
simTestSuiteRunner = TestSuite(tearDownTime)

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest1 = TestOverflow1(
# Test Name String
"""GUNNS Mass Overflow Test 1""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 1       Mass Overflow Test 1
 Author              Jason Harvey
 Date                05/28/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow
                     core/GunnsFluidCapacitor
                     core/GunnsFluidConductor

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest2 = TestOverflow2(
# Test Name String
"""GUNNS Mass Overflow Test 2""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 2       Mass Overflow Test 2
 Author              Jason Harvey
 Date                05/28/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow2
                     core/GunnsFluidSource

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest3 = TestOverflow3(
# Test Name String
"""GUNNS Mass Overflow Test 3""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 3       Mass Overflow Test 3
 Author              Jason Harvey
 Date                05/29/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow3
                     aspects/fluid/conductor/GunnsFluidValve
                     aspects/fluid/conductor/GunnsFluidPipe

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest4 = TestOverflow4(
# Test Name String
"""GUNNS Mass Overflow Test 4""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 4       Mass Overflow Test 4
 Author              Jason Harvey
 Date                05/29/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow4
                     aspects/fluid/conductor/GunnsFluidHeatExchanger
                     aspects/fluid/conductor/GunnsFluidSensor

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest5 = TestOverflow5(
# Test Name String
"""GUNNS Mass Overflow Test 5""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 5       Mass Overflow Test 5
 Author              Jason Harvey
 Date                05/29/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow5
                     aspects/fluid/conductor/GunnsFluidSimpleQd
                     aspects/fluid/conductor/GunnsFluidLeak

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest6 = TestOverflow6(
# Test Name String
"""GUNNS Mass Overflow Test 6""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 6       Mass Overflow Test 6
 Author              Jason Harvey
 Date                06/06/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow6
                     aspects/fluid/conductor/GunnsFluid3WayValve

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest7 = TestOverflow7(
# Test Name String
"""GUNNS Mass Overflow Test 7""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 7       Mass Overflow Test 7
 Author              Jason Harvey
 Date                06/06/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow7
                     aspects/fluid/conductor/GunnsFluid3WayCheckValve

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest8 = TestOverflow8(
# Test Name String
"""GUNNS Mass Overflow Test 8""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 8       Mass Overflow Test 8
 Author              Keaton Dodd
 Date                06/11/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow8
                     aspects/fluid/conductor/GunnsFluidCheckValve
                     aspects/fluid/conductor/GunnsFluidHatch

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest9 = TestOverflow9(
# Test Name String
"""GUNNS Mass Overflow Test 9""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 9       Mass Overflow Test 9
 Author              Keaton Dodd
 Date                06/12/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow9
                     aspects/fluid/conductor/GunnsFluidCondensingHxSeparator
                     aspects/fluid/conductor/GunnsCondensingHx

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

overflowTest10 = TestOverflow10(
"""GUNNS Mass Overflow Test 10""",
# Test Start Message
"""
=================================================================================================
 Test Number 10      Mass Overflow Test 10
 Author              Keaton Dodd
 Date                06/13/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow10
                     aspects/fluid/conductor/GunnsFluidPhaseChangeConductor

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

overflowTest13 = TestOverflow13(
"""GUNNS Mass Overflow Test 13""",
# Test Start Message
"""
=================================================================================================
 Test Number 13      Mass Overflow Test 13
 Author              Jason Harvey
 Date                06/13/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow13
                     aspects/fluid/conductor/GunnsFluidPressureSensitiveValve
                     aspects/fluid/conductor/GunnsFluidRegulatorValve
                     aspects/fluid/conductor/GunnsFluidReliefValve

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest14 = TestOverflow14(
# Test Name String
"""GUNNS Mass Overflow Test 14""",
 # Test Start Message
 """
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 14      Mass Overflow Test 14
 Author              Jason Harvey
 Date                06/13/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow14
                     core/GunnsFluidPotential

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest14err = TestOverflow14err(
# Test Name String
"""GUNNS Mass Overflow Test 14 Expected Conservation Errors""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 14err   Mass Overflow Test 14 Expected Conservation Errors
 Author              Jason Harvey
 Date                06/13/2019
--------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow14
                     core/GunnsFluidPotential

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in this
                        network do not match between run start and run end, because of expected
                        conservation errors due to the design limitation when there is a closed loop
                        of links and overflowing nodes.
--------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
--------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest11 = TestOverflow11(
# Test Name String
"""GUNNS Mass Overflow Test 11""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 11   Mass Overflow Test 11
 Author              Jason Harvey
 Date                06/14/2019
--------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow11
                     aspects/fluid/conductor/GunnsFluidBalancedPrv
                     aspects/fluid/conductor/GunnsFluidLiquidWaterSensor

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
                        overflows during the run
--------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
--------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest12 = TestOverflow12(
# Test Name String
"""GUNNS Mass Overflow Test 12""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 12      Mass Overflow Test 12
 Author              Jason Harvey
 Date                06/14/2019
--------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow12
                     aspects/fluid/conductor/GunnsFluidHatch

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
                        overflows during the run
--------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
--------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest12err = TestOverflow12err(
# Test Name String
"""GUNNS Mass Overflow Test 12 Expected Conservation Errors""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 12err   Mass Overflow Test 12
 Author              Jason Harvey
 Date                06/14/2019
--------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow12err
                     aspects/fluid/conductor/GunnsFluidHatch

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in this
                        network do not match between run start and run end, because of expected
                        conservation errors due to the design limitation when there is a closed loop
                        of links and overflowing nodes.
--------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
--------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

overflowTest15 = TestOverflow15(
# Test Name String
"""GUNNS Mass Overflow Test 15""",
 # Test Start Message
 """
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 15      Mass Overflow Test 15
 Author              Jason Harvey
 Date                06/17/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow15
                     core/GunnsFluidExternalDemand
                     core/GunnsFluidExternalSupply

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string

overflowTest16 = TestOverflow16(
# Test Name String
"""GUNNS Mass Overflow Test 16""",
 # Test Start Message
 """
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 16      Mass Overflow Test 16
 Author              Jason Harvey
 Date                06/18/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow16
                     aspects/fluid/conductor/GunnsFluidEqConductor

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest17 = TestOverflow17(
# Test Name String
"""GUNNS Mass Overflow Test 17""",
 # Test Start Message
 """
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 17      Mass Overflow Test 17
 Author              Jason Harvey
 Date                06/21/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow17
                     aspects/fluid/capacitor/GunnsFluidTank

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest18 = TestOverflow18(
# Test Name String
"""GUNNS Mass Overflow Test 18""",
 # Test Start Message
 """
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 18      Mass Overflow Test 18
 Author              Jason Harvey
 Date                06/21/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow18
                     aspects/fluid/capacitor/GunnsFluidBalloon

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest19 = TestOverflow19(
# Test Name String
"""GUNNS Mass Overflow Test 19""",
 # Test Start Message
 """
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 19      Mass Overflow Test 19
 Author              Jason Harvey
 Date                06/25/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow19
                     aspects/fluid/conductor/GunnsGasTurbine
                     aspects/fluid/conductor/GunnsSimpleRocket

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest20 = TestOverflow20(
# Test Name String
"""GUNNS Mass Overflow Test 20""",
 # Test Start Message
 """
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 20      Mass Overflow Test 20
 Author              Jason Harvey
 Date                06/27/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow20
                     aspects/fluid/hi-fi/GunnsFluidHiFiOrifice
                     aspects/fluid/hi-fi/GunnsFluidHiFiValve

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest21 = TestOverflow21(
# Test Name String
"""GUNNS Mass Overflow Test 21""",
 # Test Start Message
 """
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 21      Mass Overflow Test 21
 Author              Jason Harvey
 Date                06/26/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow21
                     core/GunnsFluidFlowController

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest22 = TestOverflow22(
# Test Name String
"""GUNNS Mass Overflow Test 22""",
 # Test Start Message
 """
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 22      Mass Overflow Test 22
 Author              Jason Harvey
 Date                06/26/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow22
                     core/GunnsFluidJumper
                     core/GunnsFluidSocket

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest23 = TestOverflow23(
# Test Name String
"""GUNNS Mass Overflow Test 23""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 23      Mass Overflow Test 23
 Author              Jason Harvey
 Date                06/27/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow23
                     aspects/fluid/capacitor/GunnsFluidAccum
                     aspects/fluid/capacitor/GunnsFluidAccumGas

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest24 = TestOverflow24(
# Test Name String
"""GUNNS Mass Overflow Test 24""",
 # Test Start Message
 """
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 24      Mass Overflow Test 24
 Author              Keaton Dodd
 Date                06/28/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow24
                     aspects/fluid/potential/GunnsGasFan
                     aspects/fluid/potential/GunnsLiquidCentrifugalPump

 Test Objectives     1) Verify total mass & energy, constituent masses of all nodes in each
                        network  matches between run start and run end, when there are node
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest25 = TestOverflow25(
# Test Name String
"""GUNNS Mass Overflow Test 25""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 25      Mass Overflow Test 25
 Author              Jason Harvey
 Date                07/02/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow25
                     aspects/fluid/source/GunnsFluidAdsorber
                     aspects/fluid/source/GunnsFluidHotAdsorber

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")


# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest26 = TestOverflow26(
# Test Name String
"""GUNNS Mass Overflow Test 26""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 26      Mass Overflow Test 26
 Author              Keaton Dodd
 Date                07/08/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow26
                     aspects/fluid/source/GunnsFluidEvaporation

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest27 = TestOverflow27(
# Test Name String
"""GUNNS Mass Overflow Test 27""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 27      Mass Overflow Test 27
 Author              Keaton Dodd
 Date                07/09/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow27
                     aspects/fluid/source/GunnsFluidReactor
                     aspects/fluid/source/GunnsFluidHotReactor

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest28 = TestOverflow28(
# Test Name String
"""GUNNS Mass Overflow Test 28""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 28      Mass Overflow Test 28
 Author              Keaton Dodd
 Date                07/09/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow28
                     aspects/fluid/source/GunnsFluidMetabolics

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest29 = TestOverflow29(
# Test Name String
"""GUNNS Mass Overflow Test 29""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 29      Mass Overflow Test 29
 Author              Keaton Dodd
 Date                07/10/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow29
                     aspects/fluid/source/GunnsFluidHeater

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")


# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest30 = TestOverflow30(
# Test Name String
"""GUNNS Mass Overflow Test 30""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 30      Mass Overflow Test 30
 Author              Keaton Dodd
 Date                07/11/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow30
                     aspects/fluid/source/GunnsFluidMetabolic2

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest31 = TestOverflow31(
# Test Name String
"""GUNNS Mass Overflow Test 31""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 31      Mass Overflow Test 31
 Author              Keaton Dodd
 Date                07/12/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow31
                     aspects/fluid/source/GunnsFluidFireSource

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")


# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest32 = TestOverflow32(
# Test Name String
"""GUNNS Mass Overflow Test 32""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 32      Mass Overflow Test 32
 Author              Keaton Dodd
 Date                07/12/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow32
                     aspects/fluid/source/GunnsFluidSeparatorGas
                     aspects/fluid/source/GunnsFluidGasDisplacementPump

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest33 = TestOverflow33(
# Test Name String
"""GUNNS Mass Overflow Test 33""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 33      Mass Overflow Test 33
 Author              Jason Harvey
 Date                07/12/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow33
                     aspects/fluid/source/GunnsFluidSelectiveMembrane

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")


# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest34 = TestOverflow34(
# Test Name String
"""GUNNS Mass Overflow Test 34""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 34      Mass Overflow Test 34
 Author              Jason Harvey
 Date                07/15/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow34
                     aspects/fluid/source/GunnsFluidLiquidDisplacementPump

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest35 = TestOverflow35(
# Test Name String
"""GUNNS Mass Overflow Test 35""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 35      Mass Overflow Test 35
 Author              Keaton Dodd
 Date                07/17/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow35
                     aspects/fluid/source/GunnsFluidMultiAdsorber

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest36 = TestOverflow36(
# Test Name String
"""GUNNS Mass Overflow Test 36""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 36      Mass Overflow Test 36
 Author              Keaton Dodd
 Date                07/17/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow36
                     aspects/fluid/source/GunnsFluidPhaseChangeSource

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest37 = TestOverflow37(
# Test Name String
"""GUNNS Mass Overflow Test 37""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 37      Mass Overflow Test 37
 Author              Keaton Dodd
 Date                07/18/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow37
                     aspects/fluid/source/GunnsFluidSublimator

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")


# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest38 = TestOverflow38(
# Test Name String
"""GUNNS Mass Overflow Test 38""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 38      Mass Overflow Test 38
 Author              Keaton Dodd
 Date                07/18/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow38
                     aspects/fluid/source/GunnsFluidSourceBoundary

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest39 = TestOverflow39(
# Test Name String
"""GUNNS Mass Overflow Test 39""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 39      Mass Overflow Test 39
 Author              Jason Harvey
 Date                07/22/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow39
                     aspects/fluid/source/GunnsFluidSimpleH2Redox

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest40 = TestOverflow40(
# Test Name String
"""GUNNS Mass Overflow Test 40""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 40      Mass Overflow Test 40
 Author              Jason Harvey
 Date                07/23/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow40
                     nexsys_subsystems/models/gunns/GunnsfluidAdsorberRca

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest41 = TestOverflow41(
# Test Name String
"""GUNNS Mass Overflow Test 41""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 41      Mass Overflow Test 41
 Author              Jason Harvey
 Date                07/24/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow41
                     nexsys_subsystems/models/gunns/GunnsCdraAdsorber

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest42 = TestOverflow42(
# Test Name String
"""GUNNS Mass Overflow Test 42""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 42      Mass Overflow Test 42
 Author              Keaton Dodd
 Date                07/22/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow42
                     aspects/fluid/source/GunnsFluidSeparatorLiquid

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest43 = TestOverflow43(
# Test Name String
"""GUNNS Mass Overflow Test 43""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 43      Mass Overflow Test 43
 Author              Keaton Dodd
 Date                07/22/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow43
                     nexsys_subsystems/models/gunnsGunnsFluidMetabolic4

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest44 = TestOverflow44(
# Test Name String
"""GUNNS Mass Overflow Test 44""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 44      Mass Overflow Test 44
 Author              Keaton Dodd
 Date                07/25/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow44
                     aspects/fluid/conductor/GunnsFluidHxDynHtc

 Test Objectives     1) Verify total mass & energy, constituent masses of all fluids in the
                        network matches between run start and run end, when there are node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest45 = TestOverflow45(
# Test Name String
"""GUNNS Mass Overflow Test 45""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 45      Mass Overflow Test 45
 Author              Jason Harvey
 Date                07/26/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/TestTc1

 Test Objectives     1) Verify total mass and compound masses of all trace compounds in the
                        network matches between run start and run end, when there are no node
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest45over = TestOverflow45over(
# Test Name String
"""GUNNS Mass Overflow Test 45 Overflow Case""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 45over  Mass Overflow Test 45 Overflow Case
 Author              Jason Harvey
 Date                07/26/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/TestTc1

 Test Objectives     1) Verify total mass and compound masses of all trace compounds in the
                        network matches between run start and run end, when there are
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")


# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest46 = TestOverflow46(
# Test Name String
"""GUNNS Mass Overflow Test 46 Overflow Adsorption/Desorption Case""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 46      Mass Overflow Test 46 Overflow Adsorption/Desorption Case
 Author              Keaton Dodd
 Date                08/08/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow46
                     aspects/fluid/source/GunnsFluidAdsorber

 Test Objectives     1) Verify total mass and compound masses of all trace compounds in the
                        network matches between run start and run end, when there are
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")


# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest47 = TestOverflow47(
# Test Name String
"""GUNNS Mass Overflow Test 47 Overflow Adsorption/Desorption Case""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 47      Mass Overflow Test 47 Overflow Adsorption/Desorption Case
 Author              Keaton Dodd
 Date                08/13/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow48
                     aspects/fluid/source/GunnsFluidHotAdsorber

 Test Objectives     1) Verify total mass and compound masses of all trace compounds in the
                        network matches between run start and run end, when there are
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# Construct a test by giving it a name string, a starting message string, and a finished message string
overflowTest48 = TestOverflow48(
# Test Name String
"""GUNNS Mass Overflow Test 48 Overflow Adsorption/Desorption Case""",
# Test Start Message
"""
=================================================================================================
 Test Suite          GUNNS SIM_mass_overflow
 Test Number 48      Mass Overflow Test 48 Overflow Adsorption/Desorption Case
 Author              Keaton Dodd
 Date                08/21/2019
-------------------------------------------------------------------------------------------------
 Items under test    sims/networks/fluid/test/overfow/MassOverflow48
                     aspects/fluid/source/GunnsFluidMultiAdsorber

 Test Objectives     1) Verify total mass and compound masses of all trace compounds in the
                        network matches between run start and run end, when there are
                        overflows during the run
-------------------------------------------------------------------------------------------------
 Initial Conditions  1) network at default initial conditions from the GunnDraw drawing
                     2) overrides from RUN_test/input.py.
-------------------------------------------------------------------------------------------------
 Test Output

""",
# Test finished message (no longer used)
"")

# After constructing your test, register the test with your test suite.
simTestSuiteRunner.registerTest(overflowTest1)
simTestSuiteRunner.registerTest(overflowTest2)
simTestSuiteRunner.registerTest(overflowTest3)
simTestSuiteRunner.registerTest(overflowTest4)
simTestSuiteRunner.registerTest(overflowTest5)
simTestSuiteRunner.registerTest(overflowTest6)
simTestSuiteRunner.registerTest(overflowTest7)
simTestSuiteRunner.registerTest(overflowTest8)
simTestSuiteRunner.registerTest(overflowTest9)
simTestSuiteRunner.registerTest(overflowTest10)
simTestSuiteRunner.registerTest(overflowTest13)
simTestSuiteRunner.registerTest(overflowTest14)
simTestSuiteRunner.registerTest(overflowTest14err)
simTestSuiteRunner.registerTest(overflowTest11)
simTestSuiteRunner.registerTest(overflowTest12)
simTestSuiteRunner.registerTest(overflowTest12err)
simTestSuiteRunner.registerTest(overflowTest15)
simTestSuiteRunner.registerTest(overflowTest16)
simTestSuiteRunner.registerTest(overflowTest17)
simTestSuiteRunner.registerTest(overflowTest18)
simTestSuiteRunner.registerTest(overflowTest19)
simTestSuiteRunner.registerTest(overflowTest20)
simTestSuiteRunner.registerTest(overflowTest21)
simTestSuiteRunner.registerTest(overflowTest22)
simTestSuiteRunner.registerTest(overflowTest23)
simTestSuiteRunner.registerTest(overflowTest24)
simTestSuiteRunner.registerTest(overflowTest25)
simTestSuiteRunner.registerTest(overflowTest26)
simTestSuiteRunner.registerTest(overflowTest27)
simTestSuiteRunner.registerTest(overflowTest28)
simTestSuiteRunner.registerTest(overflowTest29)
simTestSuiteRunner.registerTest(overflowTest30)
simTestSuiteRunner.registerTest(overflowTest31)
simTestSuiteRunner.registerTest(overflowTest32)
simTestSuiteRunner.registerTest(overflowTest33)
simTestSuiteRunner.registerTest(overflowTest34)
simTestSuiteRunner.registerTest(overflowTest35)
simTestSuiteRunner.registerTest(overflowTest36)
simTestSuiteRunner.registerTest(overflowTest37)
simTestSuiteRunner.registerTest(overflowTest38)
simTestSuiteRunner.registerTest(overflowTest39)
simTestSuiteRunner.registerTest(overflowTest40)
simTestSuiteRunner.registerTest(overflowTest41)
simTestSuiteRunner.registerTest(overflowTest42)
simTestSuiteRunner.registerTest(overflowTest43)
simTestSuiteRunner.registerTest(overflowTest44)
simTestSuiteRunner.registerTest(overflowTest45)
simTestSuiteRunner.registerTest(overflowTest45over)
simTestSuiteRunner.registerTest(overflowTest46)
simTestSuiteRunner.registerTest(overflowTest47)
simTestSuiteRunner.registerTest(overflowTest48)

# To run all registered tests within the test suite, add a read job to call runAllTests().
trick.add_read(0.0, """simTestSuiteRunner.runAllTests()""" )
