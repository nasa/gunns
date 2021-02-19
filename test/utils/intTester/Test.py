# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
from   math    import fabs
from   pprint  import pprint
from   sys     import path
from   time    import sleep

class Test(object):
    """Test class. This class is sub-classed for each individual test to be run in an
       integration test. The basic procedure is to derive from this class, implement the
       setup, testProcedure, and teardown methods, and then either activate and manage
       the test on your own, or use the TestSuite class (see TestSuite.py) to manage
       several individual tests within an integration test.

       Attributes
       testName           A name for this test.  This should be sim-unique.
       testStartMessage   A message that is printed out during the test's setup routine
       testFinishMessage  A message that is printed out during the test's teardown routine
       testPassed         Flag that indicated pass/fail status of this test. Should be updated
                          within the derived class's testProcedure method.
                          true by default, but if you want to have a test that only runs
                          in a certain circumstance, then you can set this flag to false by default
                          in the derived class, and then add functionality to turn it on, and have
                          your test run only once you reach the appropriate cicrumstance.
    """

    name = "unnamed test"
    testStartMessage = "start unnamed test"
    testFinishMessage = "finish unnamed test"
    def __init__(self, name, testStartMessage, testFinishMessage):
        """ Test class constructor """
        self.testName = name
        self.testStartMessage = testStartMessage
        self.testFinishMessage = testFinishMessage
        self.testPassed = True

    def setup(self):
        """A test setup function that is intended to be run before
            each test event is activated. This function should be used to
            setup your test environment prior to executing your test.
            To be implemented by the derived class.
        """
        # Register the Tear Down Event so the test tear down function gets called after
        # all other event based tests have been run.
        tearDownEvent = TestEvent(self.testName+"TearDown")
        tearDownEvent.action = self.tearDown
        tearDownEvent.registerEvent(self.lastTestTime+0.1)
        # print setup messages
        print("\n"+self.testName + " has been initiated.")
        print(self.testStartMessage)
        # Test assumed to pass until this flag is set otherwise. This should
        # be set appropriately by the derived classes testProcedure function.
        self.testPassed = True

    def tearDown(self):
        """A test tear down function that is intended to be run after
           each test procedure has been run. This function should be used to
           clean up your test environment prior to executing another test.
           To be implemented by the derived class.
        """
        # Make sure this function only executes after the last test has completed.
        if (self.currentSimTime() < self.lastTestTime):
            return

        print("-------------------------------------------------------------------------------------------------")
#        print("Checking all tests conditions")
#        # Loop through all test conditions and report pass/fail status
#        for testResult in TestResults.keys():
#            if (TestResults[testResult]):
#                print(testResult + " passed.")
#            else:
#                print(testResult + " failed.")
#                # One conditional failed. Overall test fails.
#                self.testPassed = False
#
#        print("-------------------------------------------------------------------------------------------------")
#        print("Overall test result")
#        # Display test completion message
#        if (self.testPassed):
#            print(self.testName + " has passed.")
#        else:
#            print(self.testName + " has failed.")
#        print(self.testFinishMessage)

    #--- define useful test utilities
    def registerEventBasedTest(self, eventName, startTime, setupFunction, finishTime, evaluationFunction):
        print "Scheduling setup of : " +eventName+" for time : " + str(startTime) + " seconds."
        setupEvent = TestEvent(self.testName + eventName + "Setup")
        setupEvent.action = setupFunction
        setupEvent.registerEvent(startTime)

        print "Scheduling evaluation of : " +eventName+" for time : " + str(finishTime) + " seconds."
        evaluateEvent = TestEvent(self.testName + eventName + "Evaluate")
        evaluateEvent.action = evaluationFunction
        evaluateEvent.registerEvent(finishTime)
        self.lastTestTime = finishTime

    def setTestCondition(self, conditionalLabel, passFailStatus):
        TestResults[conditionalLabel] = passFailStatus

    # Useful if you have to register an event based test that requires either no setup function or evaluate function.
    def noOp(self):
        pass

    def currentSimTime(self):
        """get the current simulation time from Trick"""
        return ( trick.exec_get_sim_time() )


    # Set of test comparison operations that wrap Trick's built in unit-test
    # functionality. They output an xml test report that can be read by
    # Jenkins. These require Trick13.
    from trick.unit_test import *
    def testTrue(self, value, testCase):
        """Test for true"""
        TRICK_EXPECT_TRUE( value , self.testName , testCase)

    def testFalse(self, value, testCase):
        """Test for false"""
        TRICK_EXPECT_FALSE( value , self.testName , testCase)

    def testEqual(self, value1, value2, testCase):
        """Test for value1 = to value2"""
        TRICK_EXPECT_EQ( value1 , value2, self.testName , testCase)

    def testNotEqual(self, value1, value2, testCase):
        """Test for value1 != to value2"""
        TRICK_EXPECT_NE( value1 , value2, self.testName , testCase)

    def testLT(self, value1, value2, testCase):
        """Test for value1 less than value2"""
        TRICK_EXPECT_LT( value1 , value2, self.testName , testCase)

    def testLE(self, value1, value2, testCase):
        """Test for value1 less than or equal to value2"""
        TRICK_EXPECT_LE( value1 , value2, self.testName , testCase)

    def testGT(self, value1, value2, testCase):
        """Test for value1 greater than value2"""
        TRICK_EXPECT_GT( value1 , value2, self.testName , testCase)

    def testGE(self, value1, value2, testCase):
        """Test for value1 greater than or equal to value2"""
        TRICK_EXPECT_GE( value1 , value2, self.testName , testCase)

    def testNear(self, value, target, tolerance, testCase):
        """Test that both values are within a tolerance of each other"""
        TRICK_EXPECT_NEAR( value , target , tolerance , self.testName , testCase)



EventBasedTests = {}
TestResults = {}
class TestEvent(object):
    """Test class. This class is sub-classed for each individual test to be run in an
       integration test. The basic procedure is to derive from this class, implement the
       setup, testProcedure, and teardown methods, and then either activate and manage
       the test on your own, or use the TestSuite class (see TestSuite.py) to manage
       several individual tests within an integration test.

       Attributes
       testName           A name for this test.  Note that Trick uses this name to distinquish
                          between events, so this name must be unique among all events in the Trick
                          sim.  The caller should make sure to supply a sim-unique name because
                          name collisions in Trick's event dictionary cause unpredictable results.
       testStartMessage   A message that is printed out during the test's setup routine
       testFinishMessage  A message that is printed out during the test's teardown routine
       testPassed         Flag that indicated pass/fail status of this test. Should be updated
                          within the derived class's testProcedure method.
                          true by default, but if you want to have a test that only runs
                          in a certain circumstance, then you can set this flag to false by default
                          in the derived class, and then add functionality to turn it on, and have
                          your test run only once you reach the appropriate cicrumstance.
    """

    def __init__(self, eventName):
        """ Test Event class constructor """
        self.action = ""
        self.eventName = eventName


    def registerEvent(self, eventTime):
        """A test setup function that is intended to be run before
        """
        EventBasedTests[self.eventName] = self
        trick.add_read(eventTime, """EventBasedTests[\""""+self.eventName+"""\"].action()""" )
