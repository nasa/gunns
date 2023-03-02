# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
import socket
import datetime

from trick.unit_test import *

#===============================================================================================
# Integration test includes
#===============================================================================================
f = "int_tests/FluidTypes.py"
exec(compile(open(f, "rb").read(), f, 'exec'), globals(), locals())
f = "int_tests/FluidNetworkConstants.py"
exec(compile(open(f, "rb").read(), f, 'exec'), globals(), locals())

#===============================================================================================
# GUNNS SIM_test Simple Fluid Network Test Class
#===============================================================================================
class TestFluidNetwork(Test):
    """ A network test class that derives from the Test base class.
        The typical use is to derive from the Test base class, override setup and possibly
        tearDown. Be sure to call registerEventBasedTest in the setup function to schedule when
        your test functions are called. See example below.

        The Test base class also has utility test functions that you can call to make comparisons.
        There are several examples below. See Test.py for more comparisons.
    """
    def __init__(self, testName, testStartMessage, testFinishMessage):
       """ Class constructor that overrides its parent class constructor"""
       # Invokes the class constructor of the parent class
       super(TestFluidNetwork, self).__init__(testName, testStartMessage, testFinishMessage)
       # Add sim variables to be Trick data logged for the data log tests
       self.testLogVariables = ['testSimObject.fluid.netSolver.mAvgDecompositionCount',
                                'testSimObject.fluid.netNodes[0].mContent.mPressure',
                                'testSimObject.fluid.netNodes[1].mContent.mPressure',]

    def setup(self):
       """ Test setup function. Called before activating the test event.
           This is where you setup your test environment prior to running all tests.
           This is also where you want to schedule when your test functions are called.
       """
       # Register all test events and schedule the times they are run at
       self.registerEventBasedTest("Nodes Initial State Check", 0.0125, self.noOp, 0.0125, self.checkNodesInitState)
       self.registerEventBasedTest("Links Initial State Check", 0.0125, self.noOp, 0.0125, self.checkLinksInitState)
       self.registerEventBasedTest("Nodes Final State Check",   3.0,    self.noOp, 3.0,    self.checkNodesFinalState)
       self.registerEventBasedTest("Links Final State Check",   3.0,    self.noOp, 3.0,    self.checkLinksFinalState)

       # Call the parent class setup to complete the test setup
       super(TestFluidNetwork, self).setup()

    # Check function
    def checkNodesInitState(self):
       nodeNameKeys = sorted(nodeNames.keys())

       for nodeName in nodeNameKeys:
          print("-------------------------------------------------------------------------------------------------")
          self.testNear(self.nodePressure(nodeName), initP[nodeNames[nodeName]], tolP, " "+nodeName+" initial pressure ::")
          self.testNear(self.nodeTemperature(nodeName), initT[nodeNames[nodeName]],  tolT, " "+nodeName+" initial temperature ::")
          self.testNear(self.nodeMassFraction(nodeName,"GUNNS_H2" ), initMfH2,  tolMf, " "+nodeName+" initial H2  mass fraction ::")
          self.testNear(self.nodeMassFraction(nodeName,"GUNNS_CH4"), initMfCH4, tolMf, " "+nodeName+" initial CH4 mass fraction ::")
          self.testNear(self.nodeMassFraction(nodeName,"GUNNS_CO2"), initMfCO2, tolMf, " "+nodeName+" initial CO2 mass fraction ::")
          self.testNear(self.nodeMassFraction(nodeName,"GUNNS_H2O"), initMfH2O, tolMf, " "+nodeName+" initial H2O mass fraction ::")
          self.testNear(self.nodeMassFraction(nodeName,"GUNNS_O2" ), initMfO2,  tolMf, " "+nodeName+" initial O2  mass fraction ::")
          self.testNear(self.nodeMassFraction(nodeName,"GUNNS_N2" ), initMfN2,  tolMf, " "+nodeName+" initial N2  mass fraction ::")

    # Check function
    def checkLinksInitState(self):
       print("-------------------------------------------------------------------------------------------------")
       self.testNear(testSimObject.fluid.conductor1.getFlowRate(), initConductor1Mdot, tolMdot, " conductor1 initial flow rate ::")
       self.testNear(testSimObject.fluid.leak.getFlowRate(),       initLeakMdot,       tolMdot, " leak       initial flow rate ::")

    # Check function
    def checkNodesFinalState(self):
       nodeNameKeys = sorted(nodeNames.keys())

       for nodeName in nodeNameKeys:
          print("-------------------------------------------------------------------------------------------------")
          self.testNear(self.nodePressure(nodeName), finalP[nodeNames[nodeName]], tolP, " "+nodeName+" final pressure ::")
          self.testNear(self.nodeTemperature(nodeName), finalT[nodeNames[nodeName]],  tolT, " "+nodeName+" final temperature ::")
          self.testNear(self.nodeMassFraction(nodeName,"GUNNS_H2" ), finalMfH2,  tolMf, " "+nodeName+" final H2  mass fraction ::")
          self.testNear(self.nodeMassFraction(nodeName,"GUNNS_CH4"), finalMfCH4, tolMf, " "+nodeName+" final CH4 mass fraction ::")
          self.testNear(self.nodeMassFraction(nodeName,"GUNNS_CO2"), finalMfCO2, tolMf, " "+nodeName+" final CO2 mass fraction ::")
          self.testNear(self.nodeMassFraction(nodeName,"GUNNS_H2O"), finalMfH2O, tolMf, " "+nodeName+" final H2O mass fraction ::")
          self.testNear(self.nodeMassFraction(nodeName,"GUNNS_O2" ), finalMfO2,  tolMf, " "+nodeName+" final O2  mass fraction ::")
          self.testNear(self.nodeMassFraction(nodeName,"GUNNS_N2" ), finalMfN2,  tolMf, " "+nodeName+" final N2  mass fraction ::")

    # Check function
    def checkLinksFinalState(self):
       print("-------------------------------------------------------------------------------------------------")
       self.testNear(testSimObject.fluid.conductor1.getFlowRate(), finalConductor1Mdot, tolMdot, " conductor1 final flow rate ::")
       self.testNear(testSimObject.fluid.leak.getFlowRate(),       finalLeakMdot,       tolMdot, " leak       final flow rate ::")

    # Check log data function
    def checkLogData(self):
        self.testLogNear('testSimObject.fluid.netSolver.mAvgDecompositionCount', 1.0, 0.1, 3.0, " Data log average decomposition count ::")
        self.testLogNear('testSimObject.fluid.netNodes[0].mContent.mPressure', initP[nodeNames["Node 0"]], tolP, 0.0, " Data log Node 0 initial pressure ::")
        self.testLogNear('testSimObject.fluid.netNodes[1].mContent.mPressure', initP[nodeNames["Node 1"]], tolP, 0.0, " Data log Node 1 initial pressure ::")
        self.testLogNear('testSimObject.fluid.netNodes[0].mContent.mPressure', finalP[nodeNames["Node 0"]], tolP, 2.9, " Data log Node 0 final pressure ::")
        self.testLogNear('testSimObject.fluid.netNodes[1].mContent.mPressure', finalP[nodeNames["Node 1"]], tolP, 2.9, " Data log Node 1 final pressure ::")
    
    def tearDownChecks(self):
        """Overrides base class, calls log data check functions"""
        self.checkLogData()
        
    """ This is where you setup all your getters/setters for the parameters you need for int testing.
    """
    # Getter for node 
    def node(self,nodeName):
        return testSimObject.fluid.netNodes[nodeNames[nodeName]]
    # Getter for node fluid
    def nodeFluid(self,nodeName):
        return self.node(nodeName).getContent()
    # Getter for node Pressure
    def nodePressure(self,nodeName):
        return self.nodeFluid(nodeName).getPressure()
    # Getter for node Temperature
    def nodeTemperature(self,nodeName):
        return self.nodeFluid(nodeName).getTemperature()
    # Getter for node Mole Fraction
    def nodeMoleFraction(self,nodeName,fluidType):
        return self.nodeFluid(nodeName).getMoleFraction(fluidTypes[fluidType])
    # Getter for node Mass Fraction
    def nodeMassFraction(self,nodeName,fluidType):
        return self.nodeFluid(nodeName).getMassFraction(fluidTypes[fluidType])
