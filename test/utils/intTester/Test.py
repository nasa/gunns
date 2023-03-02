# Copyright 2023 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
from math            import fabs
from pprint          import pprint
from sys             import path
from time            import sleep
from trick.unit_test import *
import csv

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
                          your test run only once you reach the appropriate circumstance.
       testLogVariables   An optional list of sim variable names that are to be recorded by Trick
                          data logging for this test.  This should be set by the derived test class.
       testLogFileName    Stores the Trick data log path/filename so this test can open the log file
                          and test values within.  This is set automatically by the test suite.
       testLogData        A table of the Trick data log values, loaded from the data log file.  This
                          is updated automatically.
       testLogDataNumRows The number of data rows (recorded sim times) in the Trick data log, including
                          the header row.  This is updated automatically.
       testLogDataNumCols The number of data columns (recorded sim variables) in the Trick data log,
                          updated automatically.
    """

    name = "unnamed test"
    testStartMessage = "start unnamed test"
    testFinishMessage = "finish unnamed test"
    testLogVariables = []
    testLogFileName = ""
    testLogData = None
    testLogDataNumRows = 0
    testLogDataNumCols = 0
    def __init__(self, name, testStartMessage, testFinishMessage):
        """ Test class constructor """
        self.testName = name
        self.testStartMessage = testStartMessage
        self.testFinishMessage = testFinishMessage
        self.testPassed = True
        self.testLogVariables = [] # loaded by derived test class
        self.testLogFileName = ""
        self.testLogData = None
        self.testLogDataNumRows = 0
        self.testLogDataNumCols = 0

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

    def setLogFileName(self, filename):
        self.testLogFileName = filename
        
    def loadLogFile(self):
        """Loads self.testLogData with a matrix (list of lists) by [column][row] of
           the log file's data, including log headers in the 0th row.  Does nothing,
           leaving self.testLogData as None, if there is no log file or it can't be read."""
        try:
            with open(self.testLogFileName, 'r') as csvfile:
                csvreader = csv.reader(csvfile, delimiter=',')
                self.testLogDataNumRows = 0
                self.testLogDataNumCols = 0
                rows = []
                for row in csvreader:
                    self.testLogDataNumRows += 1
                    self.testLogDataNumCols = len(row)
                    rows.append(row)
                self.testLogData = [[0 for x in range(self.testLogDataNumRows)] for y in range(self.testLogDataNumCols)]
                for row in range(0, self.testLogDataNumRows):
                    for col in range(0, self.testLogDataNumCols):
                        self.testLogData[col][row] = rows[row][col]
        except IOError:
            pass
        
    def tearDownChecks(self):
        """Overidden by the derived test class to perform checks at the end of the test.  This is the best place
           to do tests on the logged data.  Logged data checks have some advantages over the normal Trick event-based
           checks:
           - since the data recorder can access protected model variables, we can test proected model variables
             via the data log
           - test can check how a variable trends over time, instead of just checking the value at a specific time.
        """
        pass
    
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
        self.loadLogFile()
        self.tearDownChecks()
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
        print("Scheduling setup of : " +eventName+" for time : " + str(startTime) + " seconds.")
        setupEvent = TestEvent(self.testName + eventName + "Setup")
        setupEvent.action = setupFunction
        setupEvent.registerEvent(startTime)

        print("Scheduling evaluation of : " +eventName+" for time : " + str(finishTime) + " seconds.")
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

    def getLogVariables(self):
        return self.testLogVariables
    
    def lookupLogColumn(self, name):
        """Returns the column number containing data for the given variable name, else None."""
        for col in range(0, self.testLogDataNumCols):
            if name in self.testLogData[col][0]:
                 return col
        return None
    
    def lookupLogRow(self, time):
        """Returns the first row with time value >= the given time, else None."""
        for row in range(1, self.testLogDataNumRows):
            if float(self.testLogData[0][row]) >= time:
                return row
        return None
    
    def lookupLog(self, name, time):
        """Combines lookupLogRow and lookupLogColumn and returns the data value as a float, else None."""
        col = self.lookupLogColumn(name)
        row = self.lookupLogRow(time)
        if col is not None and row is not None:
            return self.testLogData[col][row]
        return None
    
    # Set of test comparison operations that wrap Trick's built in unit-test
    # functionality. They output an xml test report that can be read by
    # CI. These require Trick13.
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

    def testLogTrue(self, value, time, testCase):
        """This tests that value1 at the time in the log data is true (or not zero).
           Fails if the given time does not fall within the log data range."""
        lookupValue = self.lookupLog(value, time)
        if lookupValue is not None:
            self.testTrue(lookupValue, testCase)
        else:
            self.testFalse(True, testCase)

    def testLogFalse(self, value, time, testCase):
        """This tests that value1 at the time in the log data is false (or zero).
           Fails if the given time does not fall within the log data range."""
        lookupValue = self.lookupLog(value, time)
        if lookupValue is not None:
            self.testFalse(lookupValue, testCase)
        else:
            self.testFalse(True, testCase)

    def testLogEqual(self, value1, value2, time, testCase):
        """This tests that value1 at the time in the log data is equal to value2.
           Fails if the given time does not fall within the log data range."""
        lookupValue1 = float(self.lookupLog(value1, time))
        if lookupValue1 is not None:
            self.testEqual(lookupValue1, value2, testCase)
        else:
            self.testFalse(True, testCase)

    def testLogNotEqual(self, value1, value2, time, testCase):
        """This tests that value1 at the time in the log data is not equal to value2.
           Fails if the given time does not fall within the log data range."""
        lookupValue1 = float(self.lookupLog(value1, time))
        if lookupValue1 is not None:
            self.testNotEqual(lookupValue1, value2, testCase)
        else:
            self.testFalse(True, testCase)

    def testLogLT(self, value1, value2, time, testCase):
        """This tests that value1 at the time in the log data is less than value2.
           Fails if the given time does not fall within the log data range."""
        lookupValue1 = float(self.lookupLog(value1, time))
        if lookupValue1 is not None:
            self.testLT(lookupValue1, value2, testCase)
        else:
            self.testFalse(True, testCase)

    def testLogLE(self, value1, value2, time, testCase):
        """This tests that value1 at the time in the log data is less than or equal to value2.
           Fails if the given time does not fall within the log data range."""
        lookupValue1 = float(self.lookupLog(value1, time))
        if lookupValue1 is not None:
            self.testLE(lookupValue1, value2, testCase)
        else:
            self.testFalse(True, testCase)

    def testLogGT(self, value1, value2, time, testCase):
        """This tests that value1 at the time in the log data is greater than value2.
           Fails if the given time does not fall within the log data range."""
        lookupValue1 = float(self.lookupLog(value1, time))
        if lookupValue1 is not None:
            self.testGT(lookupValue1, value2, testCase)
        else:
            self.testFalse(True, testCase)

    def testLogGE(self, value1, value2, time, testCase):
        """This tests that value1 at the time in the log data is greater than or equal to value2.
           Fails if the given time does not fall within the log data range."""
        lookupValue1 = float(self.lookupLog(value1, time))
        if lookupValue1 is not None:
            self.testGE(lookupValue1, value2, testCase)
        else:
            self.testFalse(True, testCase)

    def testLogNear(self, value, target, tolerance, time, testCase):
        """This tests that value at the time in the log data is within the tolerance of the target.
           Fails if the given time does not fall within the log data range."""
        lookupValue = float(self.lookupLog(value, time))
        if lookupValue is not None:
            self.testNear(lookupValue, target, tolerance, testCase)
        else:
            self.testFalse(True, testCase)

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
