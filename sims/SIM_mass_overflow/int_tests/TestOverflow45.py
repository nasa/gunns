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
# GUNNS SIM_mass_overflow Test 45
#===============================================================================================
class TestOverflow45(Test):
    """ A network test class that derives from the Test base class.
        The typical use is to derive from the Test base class, override setup and possibly
        tearDown. Be sure to call registerEventBasedTest in the setup function to schedule when
        your test functions are called. See example below.

        The Test base class also has utility test functions that you can call to make comparisons.
        There are several examples below. See Test.py for more comparisons.
    """
    total_tc_mass_CO  = 0.0
    total_tc_mass_CH4 = 0.0

    def __init__(self, testName, testStartMessage, testFinishMessage):
       """ Class constructor that overrides its parent class constructor"""
       # Invokes the class constructor of the parent class #
       super(TestOverflow45, self).__init__(testName, testStartMessage, testFinishMessage)

    def setup(self):
       """ Test setup function. Called before activating the test event.
           This is where you setup your test environment prior to running all tests.
           This is also where you want to schedule when your test functions are called.
       """
       # Register all test events and schedule the times they are run at
       self.registerEventBasedTest("Nodes Initial State Check", 0.0125, self.noOp, 0.0125, self.checkNodesInitState)
       self.registerEventBasedTest("Nodes Final State Check",   12.0,   self.noOp, 12.0,   self.checkNodesFinalState)

       # Call the parent class setup to complete the test setup
       super(TestOverflow45, self).setup()

    # This stores total fluid quantities of all nodes for comparison against in the final test.
    # This isn't actually testing any values, rather just storing values for later test.
    def checkNodesInitState(self):
       for node in range(0,self.getNumNodes()):
          mass_CO  = self.nodeTcMass(node, trick.ChemicalCompound.CO)
          mass_CH4 = self.nodeTcMass(node, trick.ChemicalCompound.CH4)
          self.total_tc_mass_CO  = self.total_tc_mass_CO  + mass_CO
          self.total_tc_mass_CH4 = self.total_tc_mass_CH4 + mass_CH4

    # Tests final total fluid quantities in all nodes against the stored values from the start of
    # the test, for conservation of mass & energy.
    def checkNodesFinalState(self):
       final_total_tc_mass_CO  = 0.0
       final_total_tc_mass_CH4 = 0.0
       for node in range(0,self.getNumNodes()):
          mass_CO  = self.nodeTcMass(node, trick.ChemicalCompound.CO)
          mass_CH4 = self.nodeTcMass(node, trick.ChemicalCompound.CH4)
          final_total_tc_mass_CO  = final_total_tc_mass_CO  + mass_CO
          final_total_tc_mass_CH4 = final_total_tc_mass_CH4 + mass_CH4

       print("-------------------------------------------------------------------------------------------------")
       # Comparing relative error (final - start / start) to tolerance
       self.testNear(final_total_tc_mass_CO  - self.total_tc_mass_CO,  0.0, (1.0E-16 + tolerance * self.total_tc_mass_CO),  " mass CO error fraction ::")
       self.testNear(final_total_tc_mass_CH4 - self.total_tc_mass_CH4, 0.0, (1.0E-16 + tolerance * self.total_tc_mass_CH4), " mass CH4 error fraction ::")

    """ This is where you setup all your getters/setters for the parameters you need for int testing.
    """
    # Getter for number of nodes
    def getNumNodes(self):
        return massOverflow.fluid45.getNumLocalNodes() - 1
    # Getter for node
    def node(self,node):
        return massOverflow.fluid45.netNodes[node]
    # Getter for node fluid
    def nodeTc(self,node):
        return self.node(node).getContent().getTraceCompounds()
    # Getter for node TC Mass
    def nodeTcMass(self,node,compound):
        return self.nodeTc(node).getMass(compound)
