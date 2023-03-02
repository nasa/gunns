# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
import socket
import datetime

from trick.unit_test import *
from subprocess import call

#===============================================================================================
# Integration test includes
#===============================================================================================
#

#===============================================================================================
# GUNNS SIM_test Health & Status Log Test Class
#===============================================================================================
class TestHsLog(Test):
    """ A network test class that derives from the Test base class.
        The typical use is to derive from the Test base class, override setup and possibly
        tearDown. Be sure to call registerEventBasedTest in the setup function to schedule when
        your test functions are called. See example below.

        The Test base class also has utility test functions that you can call to make comparisons.
        There are several examples below. See Test.py for more comparisons.
    """
    def __init__(self, testName, testStartMessage, testFinishMessage):
       """ Class constructor that overrides its parent class constructor"""
       # Invokes the class constructor of the parent class #
       super(TestHsLog, self).__init__(testName, testStartMessage, testFinishMessage)

    def setup(self):
       """ Test setup function. Called before activating the test event.
           This is where you setup your test environment prior to running all tests.
           This is also where you want to schedule when your test functions are called.
       """
       # Register all test events and schedule the times they are run at
       self.registerEventBasedTest("H&S Log Check",   3.0,    self.noOp, 3.0,    self.checkHsLogFinalState)

       # Call the parent class setup to complete the test setup
       super(TestHsLog, self).setup()

    # Check function
    def checkHsLogFinalState(self):
       print("-------------------------------------------------------------------------------------------------")
       correctLineCount = False
       # Open the H&S log file
       with open('logs/TS_Health_and_Status.out', 'r') as logFile:
          # Verify the first 12 lines are successful network init messages
          self.testTrue(next(logFile).rstrip().endswith("testSimObject.fluid.netSolver initialized with 3 links, 3 nodes, solver: NORMAL, islands: OFF, run: RUN."), " fluid network initialization ::")
          self.testTrue(next(logFile).rstrip().endswith("testSimObject.elect.netSolver initialized with 3 links, 3 nodes, solver: NORMAL, islands: OFF, run: RUN."), " elect network initialization ::")
          self.testTrue(next(logFile).rstrip().endswith("testSimObject.thermal.netSolver initialized with 9 links, 14 nodes, solver: NORMAL, islands: OFF, run: RUN."), " thermal network initialization ::")
          self.testTrue(next(logFile).rstrip().endswith("testSimObject.superFluid.netSolver initialized with 6 links, 5 nodes, solver: NORMAL, islands: OFF, run: RUN."), " fluid super-network initialization ::")
          self.testTrue(next(logFile).rstrip().endswith("testSimObject.superFluid   has sub-network: testSimObject.subFluid1."), " fluid super-network initialization ::")
          self.testTrue(next(logFile).rstrip().endswith("testSimObject.superFluid   has sub-network: testSimObject.subFluid2."), " fluid super-network initialization ::")
          self.testTrue(next(logFile).rstrip().endswith("testSimObject.superElect.netSolver initialized with 6 links, 5 nodes, solver: NORMAL, islands: OFF, run: RUN."), " elect super-network initialization ::")
          self.testTrue(next(logFile).rstrip().endswith("testSimObject.superElect   has sub-network: testSimObject.subElect1."), " elect super-network initialization ::")
          self.testTrue(next(logFile).rstrip().endswith("testSimObject.superElect   has sub-network: testSimObject.subElect2."), " elect super-network initialization ::")
          self.testTrue(next(logFile).rstrip().endswith("testSimObject.superThermal.netSolver initialized with 18 links, 27 nodes, solver: NORMAL, islands: OFF, run: RUN."), " thermal super-network initialization ::")
          self.testTrue(next(logFile).rstrip().endswith("testSimObject.superThermal   has sub-network: testSimObject.subThermal1."), " thermal super-network initialization ::")
          self.testTrue(next(logFile).rstrip().endswith("testSimObject.superThermal   has sub-network: testSimObject.subThermal2."), " thermal super-network initialization ::")

          # Verify the log file only has the above 12 lines in it
          try:
             next(logFile)
          except StopIteration:
             correctLineCount = True

       self.testTrue(correctLineCount, " no run messages from networks ::")

    """ This is where you setup all your getters/setters for the parameters you need for int testing.
    """
#

