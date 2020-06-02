#!/usr/bin/python
# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, February 2019, --, Initial implementation.}
# @revs_end
#
# This implements a templated output of the network body (.cpp) file for GUNNS Fluid networks.
# It extends the basic network template with overrides for fluid-specific output syntax.
from templates.BasicNetworkBodyTemplate import BasicNetworkBodyTemplate

class FluidNetworkBodyTemplate(BasicNetworkBodyTemplate):

  def __init__(self, data):
    BasicNetworkBodyTemplate.__init__(self, data)
    return

  def blockDepsPostLinks(self):
    r = ''
    if len(self.data['reactions']) > 0:
      r = r + ('  (properties/ChemicalReaction.o)\n')
    if len(self.data['intTcConfig']) > 0 or len(self.data['compounds']) > 0:
      r = r + ('  (properties/ChemicalCompound.o)\n')
    return r

  def blockPreConfig(self):
    r = (
      '/// @details  Loads constituent fluid types into the network\'s internal fluid types array.\n'
      'FluidProperties::FluidType ' + self.data['networkNamespace'] + self.data['networkName'] + 'ConfigData::netInternalFluidTypes[' + self.data['networkNamespace'] + self.data['networkName'] + 'ConfigData::N_INTERNAL_FLUIDS] =\n'
      '{\n')
    for constituent in self.data['intFluidConfig'][2:-1]:
      r = r+('    FluidProperties::' + constituent + ',\n')
    for constituent in self.data['intFluidConfig'][-1:]:
      r = r+('    FluidProperties::' + constituent + '\n')
    r = r + (
      '};\n'
      '\n')
    for extConfig in self.data['extFluidConfigs']:
      r = r+(
        '/// @details  Loads constituent fluid types into an external network fluid types array.\n'
        'FluidProperties::FluidType ' + self.data['networkNamespace'] + self.data['networkName'] + 'ConfigData::' + extConfig[0] + 'FluidTypes[' + self.data['networkNamespace'] + self.data['networkName'] + 'ConfigData::N_' + extConfig[0].upper() + '_FLUIDS] =\n'
        '{\n')
      for constituent in extConfig[2:-1]:
        r = r+('    FluidProperties::' + constituent + ',\n')
      for constituent in extConfig[-1:]:
        r = r+('    FluidProperties::' + constituent + '\n')
      r = r + (
        '};\n'
        '\n')
    if len(self.data['intTcConfig']) > 0:
      r = r + (
        '/// @details  Loads chemical compounds types into the trace compounds config array.\n'
        'ChemicalCompound::Type ' + self.data['networkNamespace'] + self.data['networkName'] + 'ConfigData::' + self.data['intTcConfig'][0] + 'Types[' + self.data['networkNamespace'] + self.data['networkName'] + 'ConfigData::N_' + self.data['intTcConfig'][0].upper() + '] =\n'
        '{\n')
      for compound in self.data['intTcConfig'][2:-1]:
        r = r + ('    ChemicalCompound::' + compound + ',\n')
      for compound in self.data['intTcConfig'][-1:]:
        r = r + ('    ChemicalCompound::' + compound + '\n')
      r = r + (
        '};\n'
        '\n')
    for rxnReactions in self.data['reactions']:
      r = r + (
        '/// @details  Loads chemical reaction types into the reactor chemical reactions array.\n'
        'ChemicalReaction::Type ' + self.data['networkNamespace'] + self.data['networkName'] + 'ConfigData::' + rxnReactions[0] + '[' + self.data['networkNamespace'] + self.data['networkName'] + 'ConfigData::N_' + rxnReactions[0].upper() + '] =\n'
        '{\n')
      for reaction in rxnReactions[2:-1]:
        r = r + (
        '    ChemicalReaction::' + reaction + ',\n')
      for reaction in rxnReactions[-1:]:
        r = r + (
        '    ChemicalReaction::' + reaction + '\n')
      r = r + (
        '};\n'
        '\n')
    for rxnCompounds in self.data['compounds']:
      r = r + (
        '/// @details  Loads chemical compound types into the reactor chemical compounds array.\n'
        'ChemicalCompound::Type ' + self.data['networkNamespace'] + self.data['networkName'] + 'ConfigData::' + rxnCompounds[0][1] + '[' + self.data['networkNamespace'] + self.data['networkName'] + 'ConfigData::N_' + rxnCompounds[0][1].upper() + '] =\n'
        '{\n')
      for compound in rxnCompounds[3:-2]:
        r = r + (
        '    ChemicalCompound::' + compound[0] + ',\n')
      for compound in rxnCompounds[-2:-1]:
        r = r + (
        '    ChemicalCompound::' + compound[0] + '\n')
      r = r + (
        '};\n'
        '\n')
    return r

  def blockConfigPreSolver(self):
    r = ''
    for extConfig in self.data['extFluidConfigs']:
      r = r+('    ' + extConfig[0] + '(&network->netFluidProperties, ' + extConfig[0] + 'FluidTypes, ' + self.data['networkName'] + 'ConfigData::N_' + extConfig[0].upper() + '_FLUIDS),\n')
    if len(self.data['intTcConfig']) > 0:
      r = r + (
        '    ' + self.data['intTcConfig'][0] + '(' + self.data['intTcConfig'][0] + 'Types, ' + self.data['networkName'] + 'ConfigData::N_' + self.data['intTcConfig'][0].upper() + ', name + ".' + self.data['intTcConfig'][0] + '"),\n')
    if len(self.data['reactions']) > 0:
      r = r + ('    netReactions(),\n')
    if len(self.data['compounds']) > 0:
      r = r + ('    netCompounds(),\n')
    return r

  def blockInputPreSpotter(self):
    r = ''
    for tcState in self.data['tcStates']:
      r = r + (
        '    ' + tcState[0][1] + 'Values(' + tcState[-1][1] + '),\n'
        '    ' + tcState[0][1] + '(' + tcState[0][1] + 'Values.array),\n')
    for fluidState in self.data['fluidStates']:
      r = r+('    ' + fluidState[0][1] + 'Fractions(' + fluidState[-1][1] + '),\n'
        '    ' + fluidState[0][1] + '(' + fluidState[3][1] + ', ' + fluidState[2][1] + ', 0.0, ' + fluidState[4][1] + ', ' + fluidState[0][1] + 'Fractions.array, ' + fluidState[5][1] + '),\n')
    for rxnCompounds in self.data['compounds']:
      r = r + (
        '    ' + rxnCompounds[0][1] + 'Masses(' + rxnCompounds[-1][1] + '),\n')
    return r

  def blockConstructorPreSpotter(self):
    r = ('    netFluidProperties(),\n')
    internalTcConfigName = ''
    if len(self.data['intTcConfig']) > 0:
      internalTcConfigName = ', &netConfig.' + self.data['intTcConfig'][0]
    r = r + (
      '    netInternalFluidConfig(&netFluidProperties, netConfig.netInternalFluidTypes, ' + self.data['networkName'] + 'ConfigData::N_INTERNAL_FLUIDS' + internalTcConfigName + '),\n')
    return r

  def blockInitNodes(self):
    r =('    GunnsFluidNode* nodes = static_cast<GunnsFluidNode*>(netNodeList.mNodes);\n')
    for node in self.data['nodes']:
      r = r+('    nodes[' + node[0] + ' + netSuperNodesOffset].initialize(name + createNodeName(' + node[0] + ' + netSuperNodesOffset), &netInternalFluidConfig, &netInput.' + node[1] + ');\n')
    r = r+('    /// - Only init the Ground node if this is not a sub-network.\n'
        '    if (!netIsSubNetwork) {\n'
        '        nodes[' + str(self.data['numNodes']) + '].initialize(name + ".GROUND", &netInternalFluidConfig, 0);\n'
        '    }\n')
    return r

  def blockSolverInitializeNodes(self):
    r =('        netSolver.initializeFluidNodes(netNodeList);\n')
    return r

