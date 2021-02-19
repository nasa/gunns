# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
import os
INT_TESTER_HOME = os.environ["GUNNS_HOME"]+"/test/utils/intTester/"
execfile(INT_TESTER_HOME+"Test.py")

class TestSuite:
    """Test suite class. This class is instantiated inside of an integration test file. It is
       responsible for running all tests and verifying pass/fail status. It manages a list of
       test objects (see Test.py).

       Attributes
       testList         The list of tests managed by this test suite
       testTearDownTime The amount of time added to the last scheduled test event time before
                        shutting down the entire sim.
    """

    def __init__(self, tearDownTime):
       """TestSuite class constructor"""
       self.testList = []
       self.testTearDownTime = tearDownTime

    def registerTest(self, testToRegister):
        """Function to add a test to the testList attribute"""
        self.testList.append(testToRegister)

    def runAllTests(self):
        """Function to loop through the test list and call each test's setup function,
           activate the test event, check its pass/fail status and update the failed and
           passed tests list, call the test's teardown function, and finally deactivate
           the test event.
        """
        for test in self.testList:
            self.runTest(test)
            endOfTestSuiteTime = test.lastTestTime
        print "Scheduling sim to stop at : " + str(endOfTestSuiteTime+self.testTearDownTime) + " seconds."
        self.scheduleSimShutdown(endOfTestSuiteTime+self.testTearDownTime)

    def runTest(self, testToRun):
        """Function to run a given test. All that needs to be done is call the test setup.
        """
        testToRun.setup()

    def scheduleSimShutdown(self, shutdownTime):
        trick.add_read(shutdownTime, """trick.stop()""")
