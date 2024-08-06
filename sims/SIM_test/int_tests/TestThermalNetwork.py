# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
import socket
import datetime

from trick.unit_test import *

#===============================================================================================
# Integration test includes
#===============================================================================================
f = "int_tests/ThermalNetworkConstants.py"
exec(compile(open(f, "rb").read(), f, 'exec'), globals(), locals())

#===============================================================================================
# GUNNS SIM_test Simple Thermal Network Test Class
#===============================================================================================
class TestThermalNetwork(Test):
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
       super(TestThermalNetwork, self).__init__(testName, testStartMessage, testFinishMessage)
       # Add sim variables to be Trick data logged for the data log tests
       self.testLogVariables = ['testSimObject.thermal.netSolver.mAvgDecompositionCount',]

    def setup(self):
       """ Test setup function. Called before activating the test event.
           This is where you setup your test environment prior to running all tests.
           This is also where you want to schedule when your test functions are called.
       """
       # Register all test events and schedule the times they are run at
       self.registerEventBasedTest("Nodes Final State Check",   3.0,    self.noOp, 3.0,    self.checkNodesFinalState)
       self.registerEventBasedTest("Thermal Links Final State Check",   3.0,    self.noOp, 3.0,    self.checkLinksFinalState)

       # Call the parent class setup to complete the test setup
       super(TestThermalNetwork, self).setup()

    # Check function
    def checkNodesFinalState(self):
       nodeNameKeys = sorted(thermalNodeNames.keys())

       for nodeName in nodeNameKeys:
          print("-------------------------------------------------------------------------------------------------")
          self.testNear(self.nodePotential(nodeName), thermalFinalP[thermalNodeNames[nodeName]], thermalTolP, " "+nodeName+" final potential ::")

    # Check function
    def checkLinksFinalState(self):
       print("-------------------------------------------------------------------------------------------------")
       self.testNear(testSimObject.thermal.potential.getFlux(), fluxConductor, tolFlux, " potential final flux ::")
       self.testNear(testSimObject.thermal.conductor.getFlux(), fluxConductor, tolFlux, " conductor final flux ::")
       self.testNear(testSimObject.thermal.externalDemand.getFlux(), fluxDemand, tolFlux, " demand final flux ::")

    # Check log data function
    def checkLogData(self):
        self.testLogNear('testSimObject.thermal.netSolver.mAvgDecompositionCount', 1.0, 0.1, 3.0, " Data log average decomposition count ::")

    def tearDownChecks(self):
        """Overrides base class, calls log data check functions"""
        self.checkLogData()

    """ This is where you setup all your getters/setters for the parameters you need for int testing.
    """
    # Getter for node
    def node(self,nodeName):
        return testSimObject.thermal.netNodes[thermalNodeNames[nodeName]]
    # Getter for node potential
    def nodePotential(self,nodeName):
        return self.node(nodeName).getPotential()
