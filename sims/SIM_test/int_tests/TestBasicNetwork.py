# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
import socket
import datetime

from trick.unit_test import *

#===============================================================================================
# Integration test includes
#===============================================================================================
f = "int_tests/BasicNetworkConstants.py"
exec(compile(open(f, "rb").read(), f, 'exec'), globals(), locals())

#===============================================================================================
# GUNNS SIM_test Simple Basic Network Test Class
#===============================================================================================
class TestBasicNetwork(Test):
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
       super(TestBasicNetwork, self).__init__(testName, testStartMessage, testFinishMessage)
       # Add sim variables to be Trick data logged for the data log tests
       self.testLogVariables = ['testSimObject.elect.netSolver.mConvergenceFailCount',
                                'testSimObject.elect.netSolver.mAvgDecompositionCount',]

    def setup(self):
       """ Test setup function. Called before activating the test event.
           This is where you setup your test environment prior to running all tests.
           This is also where you want to schedule when your test functions are called.
       """
       # Register all test events and schedule the times they are run at
       self.registerEventBasedTest("Nodes Initial State Check", 0.0125, self.noOp, 0.0125, self.checkNodesInitState)
       self.registerEventBasedTest("Basic Links Initial State Check", 0.0125, self.noOp, 0.0125, self.checkLinksInitState)
       self.registerEventBasedTest("Nodes Final State Check",   3.0,    self.noOp, 3.0,    self.checkNodesFinalState)
       self.registerEventBasedTest("Basic Links Final State Check",   3.0,    self.noOp, 3.0,    self.checkLinksFinalState)

       # Call the parent class setup to complete the test setup
       super(TestBasicNetwork, self).setup()

    # Check function
    def checkNodesInitState(self):
       nodeNameKeys = sorted(basicNodeNames.keys())

       for nodeName in nodeNameKeys:
          print("-------------------------------------------------------------------------------------------------")
          self.testNear(self.nodePotential(nodeName), basicInitP[basicNodeNames[nodeName]], basicTolP, " "+nodeName+" initial potential ::")

    # Check function
    def checkLinksInitState(self):
       print("-------------------------------------------------------------------------------------------------")
       self.testNear(testSimObject.elect.SimpleVoltageSource.getFlux(), flux, tolFlux, " SimpleVoltageSource initial flux ::")
       self.testNear(testSimObject.elect.Resistor1.getFlux(), flux, tolFlux, " Resistor1 initial flux ::")
       self.testNear(testSimObject.elect.Resistor2.getFlux(), flux, tolFlux, " Resistor2 initial flux ::")

    # Check function
    def checkNodesFinalState(self):
       nodeNameKeys = sorted(nodeNames.keys())

       for nodeName in nodeNameKeys:
          print("-------------------------------------------------------------------------------------------------")
          self.testNear(self.nodePotential(nodeName), basicFinalP[basicNodeNames[nodeName]], basicTolP, " "+nodeName+" final nodePotential ::")

    # Check function
    def checkLinksFinalState(self):
       print("-------------------------------------------------------------------------------------------------")
       self.testNear(testSimObject.elect.SimpleVoltageSource.getFlux(), flux, tolFlux, " SimpleVoltageSource final flux ::")
       self.testNear(testSimObject.elect.Resistor1.getFlux(), flux, tolFlux, " Resistor1 final flux ::")
       self.testNear(testSimObject.elect.Resistor2.getFlux(), flux, tolFlux, " Resistor2 final flux ::")

    # Check log data function
    def checkLogData(self):
        self.testLogEqual('testSimObject.elect.netSolver.mConvergenceFailCount', 0, 3.0, " Data log convergence fail count ::")
        self.testLogLT('testSimObject.elect.netSolver.mAvgDecompositionCount', 0.5, 3.0, " Data log average decomposition count ::")

    def tearDownChecks(self):
        """Overrides base class, calls log data check functions"""
        self.checkLogData()

    """ This is where you setup all your getters/setters for the parameters you need for int testing.
    """
    # Getter for node
    def node(self,nodeName):
        return testSimObject.elect.netNodes[basicNodeNames[nodeName]]
    # Getter for node potential
    def nodePotential(self,nodeName):
        return self.node(nodeName).getPotential()
