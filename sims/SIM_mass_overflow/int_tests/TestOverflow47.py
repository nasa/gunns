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
# GUNNS SIM_mass_overflow Test 47
#===============================================================================================
class TestOverflow47(Test):
    """ A network test class that derives from the Test base class.
        The typical use is to derive from the Test base class, override setup and possibly
        tearDown. Be sure to call registerEventBasedTest in the setup function to schedule when
        your test functions are called. See example below.

        The Test base class also has utility test functions that you can call to make comparisons.
        There are several examples below. See Test.py for more comparisons.
    """
    total_mass_ad      = 0.0
    total_mass_N2_ad   = 0.0
    total_mass_O2_ad   = 0.0
    total_mass_H2O_ad  = 0.0
    total_mass_CO2_ad  = 0.0
    temperature_ad     = 0.0
    saver = 0.0

    total_mass_des     = 0.0
    total_mass_N2_des  = 0.0
    total_mass_O2_des  = 0.0
    total_mass_H2O_des = 0.0
    total_mass_CO2_des = 0.0
    temperature_des    = 0.0

    def __init__(self, testName, testStartMessage, testFinishMessage):
       """ Class constructor that overrides its parent class constructor"""
       # Invokes the class constructor of the parent class #
       super(TestOverflow47, self).__init__(testName, testStartMessage, testFinishMessage)

    def setup(self):
       """ Test setup function. Called before activating the test event.
           This is where you setup your test environment prior to running all tests.
           This is also where you want to schedule when your test functions are called.
       """
       # Register all test events and schedule the times they are run at
       self.registerEventBasedTest("Nodes Initial State Check", 0.0125, self.noOp, 0.0125, self.checkNodesInitState)
       self.registerEventBasedTest("Nodes Final State Check",   12.0,   self.noOp, 12.0,   self.checkNodesFinalState)

       # Call the parent class setup to complete the test setup
       super(TestOverflow47, self).setup()

    # This stores total fluid quantities of all nodes for comparison against in the final test.
    # This isn't actually testing any values, rather just storing values for later test.
    def checkNodesInitState(self):
       sorb12adH2O_mass  = float (massOverflow.fluid47.hsorb12ad.getAdsorbedMass())
       sorb45deH2O_mass  = float (massOverflow.fluid47.hsorb45de.getAdsorbedMass())

       self.total_mass_ad      = self.total_mass_ad     + sorb12adH2O_mass
       self.total_mass_H2O_ad  = self.total_mass_H2O_ad + sorb12adH2O_mass
       self.total_mass_CO2_ad  = self.total_mass_CO2_ad
       self.temperature_ad     = self.nodeTemperature(0)

       self.total_mass_des     = self.total_mass_des     + sorb45deH2O_mass
       self.total_mass_H2O_des = self.total_mass_H2O_des + sorb45deH2O_mass
       self.total_mass_CO2_des = self.total_mass_CO2_des
       self.temperature_des    = self.nodeTemperature(4)

       for node in range(0,self.getNumNodes()-3):
          mass_ad     = self.nodeMass(node)
          mass_N2_ad  = mass_ad * self.nodeMassFraction(node, "GUNNS_N2")
          mass_O2_ad  = mass_ad * self.nodeMassFraction(node, "GUNNS_O2")
          mass_H2O_ad = mass_ad * self.nodeMassFraction(node, "GUNNS_H2O")
          mass_CO2_ad = mass_ad * self.nodeMassFraction(node, "GUNNS_CO2")

          self.total_mass_ad     = self.total_mass_ad     + mass_ad
          self.total_mass_N2_ad  = self.total_mass_N2_ad  + mass_N2_ad
          self.total_mass_O2_ad  = self.total_mass_O2_ad  + mass_O2_ad
          self.total_mass_H2O_ad = self.total_mass_H2O_ad + mass_H2O_ad
          self.total_mass_CO2_ad = self.total_mass_CO2_ad + mass_CO2_ad

       for node in range(4,self.getNumNodes()):
          mass_des     = self.nodeMass(node)
          mass_N2_des  = mass_des * self.nodeMassFraction(node, "GUNNS_N2")
          mass_O2_des  = mass_des * self.nodeMassFraction(node, "GUNNS_O2")
          mass_H2O_des = mass_des * self.nodeMassFraction(node, "GUNNS_H2O")
          mass_CO2_des = mass_des * self.nodeMassFraction(node, "GUNNS_CO2")

          self.total_mass_des     = self.total_mass_des     + mass_des
          self.total_mass_N2_des  = self.total_mass_N2_des  + mass_N2_des
          self.total_mass_O2_des  = self.total_mass_O2_des  + mass_O2_des
          self.total_mass_H2O_des = self.total_mass_H2O_des + mass_H2O_des
          self.total_mass_CO2_des = self.total_mass_CO2_des + mass_CO2_des

    # Tests final total fluid quantities in all nodes against the stored values from the start of
    # the test, for conservation of mass & energy.
    def checkNodesFinalState(self):
       sorb12adH2O_mass  = float (massOverflow.fluid47.hsorb12ad.getAdsorbedMass())
       sorb45deH2O_mass  = float (massOverflow.fluid47.hsorb45de.getAdsorbedMass())

       final_total_mass_ad      = sorb12adH2O_mass
       final_total_mass_N2_ad   = 0.0
       final_total_mass_O2_ad   = 0.0
       final_total_mass_H2O_ad  = sorb12adH2O_mass
       final_total_mass_CO2_ad  = 0.0
       final_temperature_ad     = self.nodeTemperature(0)

       final_total_mass_des     = sorb45deH2O_mass
       final_total_mass_N2_des  = 0.0
       final_total_mass_O2_des  = 0.0
       final_total_mass_H2O_des = sorb45deH2O_mass
       final_total_mass_CO2_des = 0.0
       final_temperature_des    = self.nodeTemperature(4)

       for node in range(0,self.getNumNodes()-3):
          mass_ad     = self.nodeMass(node)
          mass_N2_ad  = mass_ad * self.nodeMassFraction(node, "GUNNS_N2")
          mass_O2_ad  = mass_ad * self.nodeMassFraction(node, "GUNNS_O2")
          mass_H2O_ad = mass_ad * self.nodeMassFraction(node, "GUNNS_H2O")
          mass_CO2_ad = mass_ad * self.nodeMassFraction(node, "GUNNS_CO2")

          final_total_mass_ad     = final_total_mass_ad     + mass_ad
          final_total_mass_N2_ad  = final_total_mass_N2_ad  + mass_N2_ad
          final_total_mass_O2_ad  = final_total_mass_O2_ad  + mass_O2_ad
          final_total_mass_H2O_ad = final_total_mass_H2O_ad + mass_H2O_ad
          final_total_mass_CO2_ad = final_total_mass_CO2_ad + mass_CO2_ad

       for node in range(4,self.getNumNodes()):
          mass_des     = self.nodeMass(node)
          mass_N2_des  = mass_des * self.nodeMassFraction(node, "GUNNS_N2")
          mass_O2_des  = mass_des * self.nodeMassFraction(node, "GUNNS_O2")
          mass_H2O_des = mass_des * self.nodeMassFraction(node, "GUNNS_H2O")
          mass_CO2_des = mass_des * self.nodeMassFraction(node, "GUNNS_CO2")

          final_total_mass_des     = final_total_mass_des     + mass_des
          final_total_mass_N2_des  = final_total_mass_N2_des  + mass_N2_des
          final_total_mass_O2_des  = final_total_mass_O2_des  + mass_O2_des
          final_total_mass_H2O_des = final_total_mass_H2O_des + mass_H2O_des
          final_total_mass_CO2_des = final_total_mass_CO2_des + mass_CO2_des

       print("-------------------------------------------------------------------------------------------------")
       # Comparing relative error (final - start / start) to tolerance
       # It is too much effort to test conservation of energy by checking enthalpy, because the
       # desorbed mass takes the link's internal fluid temperature, which varies over this test.
       # Next best thing we can do is check that temperature remains constant.
       self.testNear(final_total_mass_ad      - self.total_mass_ad,      0.0, (1.0E-16 + tolerance * self.total_mass_ad),      " Nodes 0-3 Adsorbed mass error fraction ::")
       self.testNear(final_total_mass_N2_ad   - self.total_mass_N2_ad,   0.0, (1.0E-16 + tolerance * self.total_mass_N2_ad),   " Nodes 0-3 Adsorbed mass N2 error fraction ::")
       self.testNear(final_total_mass_O2_ad   - self.total_mass_O2_ad,   0.0, (1.0E-16 + tolerance * self.total_mass_O2_ad),   " Nodes 0-3 Adsorbed mass O2 error fraction ::")
       self.testNear(final_total_mass_H2O_ad  - self.total_mass_H2O_ad,  0.0, (1.0E-16 + tolerance * self.total_mass_H2O_ad),  " Nodes 0-3 Adsorbed mass H2O error fraction ::")
       self.testNear(final_total_mass_CO2_ad  - self.total_mass_CO2_ad,  0.0, (1.0E-16 + tolerance * self.total_mass_CO2_ad),  " Nodes 0-3 Adsorbed mass CO2 error fraction ::")
       self.testNear(final_temperature_ad     - self.temperature_ad,     0.0, (1.0E-16 + tolerance * self.temperature_ad),     " Nodes 0-3 Adsorbed temperature error fraction ::")
       print(" ")
       self.testNear(final_total_mass_des     - self.total_mass_des,     0.0, (1.0E-16 + tolerance * self.total_mass_des),     " Nodes 4-6 Desorbed mass error fraction ::")
       self.testNear(final_total_mass_N2_des  - self.total_mass_N2_des,  0.0, (1.0E-16 + tolerance * self.total_mass_N2_des),  " Nodes 4-6 Desorbed mass N2 error fraction ::")
       self.testNear(final_total_mass_O2_des  - self.total_mass_O2_des,  0.0, (1.0E-16 + tolerance * self.total_mass_O2_des),  " Nodes 4-6 Desorbed mass O2 error fraction ::")
       self.testNear(final_total_mass_H2O_des - self.total_mass_H2O_des, 0.0, (1.0E-16 + tolerance * self.total_mass_H2O_des), " Nodes 4-6 Desorbed mass H2O error fraction ::")
       self.testNear(final_total_mass_CO2_des - self.total_mass_CO2_des, 0.0, (1.0E-16 + tolerance * self.total_mass_CO2_des), " Nodes 4-6 Desorbed mass CO2 error fraction ::")
       self.testNear(final_temperature_des    - self.temperature_des,    0.0, (1.0E-16 + tolerance * self.temperature_des),    " Nodes 4-6 Desorbed temperature error fraction ::")
    """ This is where you setup all your getters/setters for the parameters you need for int testing.
    """
    # Getter for number of nodes
    def getNumNodes(self):
        return massOverflow.fluid47.getNumLocalNodes() - 1
    # Getter for node
    def node(self,node):
        return massOverflow.fluid47.netNodes[node]
    # Getter for node fluid
    def nodeFluid(self,node):
        return self.node(node).getContent()
    # Getter for node mass
    def nodeMass(self,node):
        return self.nodeFluid(node).getMass()
    # Getter for node Mass Fraction
    def nodeMassFraction(self,node,fluidType):
        return self.nodeFluid(node).getMassFraction(fluidTypes[fluidType])
    # Getter for node temperature
    def nodeTemperature(self,node):
        return self.nodeFluid(node).getTemperature()
