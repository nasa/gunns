# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
import socket
import datetime

from trick.unit_test import *

#===============================================================================================
# Integration test includes
#===============================================================================================
execfile("int_tests/FluidTypes.py")
execfile("int_tests/FluidNetworkConstants.py")

#===============================================================================================
# GUNNS SIM_mass_overflow Test 15
#===============================================================================================
class TestOverflow15(Test):
    """ A network test class that derives from the Test base class.
        The typical use is to derive from the Test base class, override setup and possibly
        tearDown. Be sure to call registerEventBasedTest in the setup function to schedule when
        your test functions are called. See example below.

        The Test base class also has utility test functions that you can call to make comparisons.
        There are several examples below. See Test.py for more comparisons.
    """
    total_mass     = 0.0
    total_enthalpy = 0.0
    total_mass_N2  = 0.0
    total_mass_O2  = 0.0

    def __init__(self, testName, testStartMessage, testFinishMessage):
       """ Class constructor that overrides its parent class constructor"""
       # Invokes the class constructor of the parent class #
       super(TestOverflow15, self).__init__(testName, testStartMessage, testFinishMessage)

    def setup(self):
       """ Test setup function. Called before activating the test event.
           This is where you setup your test environment prior to running all tests.
           This is also where you want to schedule when your test functions are called.
       """
       # Register all test events and schedule the times they are run at
       self.registerEventBasedTest("Nodes Initial State Check", 0.0125, self.noOp, 0.0125, self.checkNodesInitState)
       self.registerEventBasedTest("Nodes Middle State Check",  5.9,    self.noOp, 5.9,    self.checkNodesMiddleState)
       self.registerEventBasedTest("Nodes Final State Check",   12.0,   self.noOp, 12.0,   self.checkNodesFinalState)

       # Call the parent class setup to complete the test setup
       super(TestOverflow15, self).setup()

    # This stores total fluid quantities of all nodes for comparison against in the final test.
    # This isn't actually testing any values, rather just storing values for later test.
    def checkNodesInitState(self):
       for node in range(0,self.getNumNodes()):
          mass     = self.nodeMass(node)
          enthalpy = mass * self.nodeEnthalpy(node)
          mass_N2  = mass * self.nodeMassFraction(node, "GUNNS_N2")
          mass_O2  = mass * self.nodeMassFraction(node, "GUNNS_O2")

          self.total_mass     = self.total_mass     + mass
          self.total_enthalpy = self.total_enthalpy + enthalpy
          self.total_mass_N2  = self.total_mass_N2  + mass_N2
          self.total_mass_O2  = self.total_mass_O2  + mass_O2

    # Tests middle-of-test total fluid quantities in all nodes against the stored values from the start of
    # the test, for conservation of mass & energy.
    def checkNodesMiddleState(self):
       final_total_mass     = 0.0
       final_total_enthalpy = 0.0
       final_total_mass_N2  = 0.0
       final_total_mass_O2  = 0.0
       final_total_mass_H2O = 0.0
       final_total_mass_CO2 = 0.0
       for node in range(0,self.getNumNodes()):
          mass     = self.nodeMass(node)
          enthalpy = mass * self.nodeEnthalpy(node)
          mass_N2  = mass * self.nodeMassFraction(node, "GUNNS_N2")
          mass_O2  = mass * self.nodeMassFraction(node, "GUNNS_O2")

          final_total_mass     = final_total_mass     + mass
          final_total_enthalpy = final_total_enthalpy + enthalpy
          final_total_mass_N2  = final_total_mass_N2  + mass_N2
          final_total_mass_O2  = final_total_mass_O2  + mass_O2

       print("-------------------------------------------------------------------------------------------------")
       # Comparing relative error (final - start / start) to tolerance
       # This shows that the External Supply/Demand links conserve in the reverse (demand to supply)
       # flow direction.
       self.testNear(final_total_mass     - self.total_mass,     0.0, (1.0E-16 + tolerance * self.total_mass),     " mass error fraction ::")
       self.testNear(final_total_enthalpy - self.total_enthalpy, 0.0, (1.0E-16 + tolerance * self.total_enthalpy), " enthalpy error fraction ::")
       self.testNear(final_total_mass_N2  - self.total_mass_N2,  0.0, (1.0E-16 + tolerance * self.total_mass_N2),  " mass N2 error fraction ::")
       self.testNear(final_total_mass_O2  - self.total_mass_O2,  0.0, (1.0E-16 + tolerance * self.total_mass_O2),  " mass O2 error fraction ::")

    # Tests final total fluid quantities in all nodes against the stored values from the start of
    # the test, for conservation of mass & energy.
    def checkNodesFinalState(self):
       final_total_mass     = 0.0
       final_total_enthalpy = 0.0
       final_total_mass_N2  = 0.0
       final_total_mass_O2  = 0.0
       final_total_mass_H2O = 0.0
       final_total_mass_CO2 = 0.0
       for node in range(0,self.getNumNodes()):
          mass     = self.nodeMass(node)
          enthalpy = mass * self.nodeEnthalpy(node)
          mass_N2  = mass * self.nodeMassFraction(node, "GUNNS_N2")
          mass_O2  = mass * self.nodeMassFraction(node, "GUNNS_O2")

          final_total_mass     = final_total_mass     + mass
          final_total_enthalpy = final_total_enthalpy + enthalpy
          final_total_mass_N2  = final_total_mass_N2  + mass_N2
          final_total_mass_O2  = final_total_mass_O2  + mass_O2

       print("-------------------------------------------------------------------------------------------------")
       # Comparing relative error (final - start / start) to tolerance
       # The External Fluid Supply & Demand links don't conserve energy & constituent mass in the
       # forward flow direction (supply to demand) when enthalpy & mixture are changing in the
       # supply node, because of lag.  This creates a small amount of error that we allow for in our
       # larger tolerance.
       self.testNear(final_total_mass     - self.total_mass,     0.0, (1.0E-16 + 1.4E-6 * self.total_mass),  " mass error fraction ::")
       self.testNear(final_total_enthalpy - self.total_enthalpy, 0.0, (1.0E-16 + 2.4E-5 * self.total_enthalpy), " enthalpy error fraction ::")
       self.testNear(final_total_mass_N2  - self.total_mass_N2,  0.0, (1.0E-16 + 2.5E-4 * self.total_mass_N2),  " mass N2 error fraction ::")
       self.testNear(final_total_mass_O2  - self.total_mass_O2,  0.0, (1.0E-16 + 6.6E-5 * self.total_mass_O2),  " mass O2 error fraction ::")

    """ This is where you setup all your getters/setters for the parameters you need for int testing.
    """
    # Getter for number of nodes
    def getNumNodes(self):
        return massOverflow.fluid15.getNumLocalNodes() - 1
    # Getter for node
    def node(self,node):
        return massOverflow.fluid15.netNodes[node]
    # Getter for node fluid
    def nodeFluid(self,node):
        return self.node(node).getContent()
    # Getter for node mass
    def nodeMass(self,node):
        return self.nodeFluid(node).getMass()
    # Getter for node enthalpy
    def nodeEnthalpy(self,node):
        return self.nodeFluid(node).getSpecificEnthalpy()
    # Getter for node Mass Fraction
    def nodeMassFraction(self,node,fluidType):
        return self.nodeFluid(node).getMassFraction(fluidTypes[fluidType])
