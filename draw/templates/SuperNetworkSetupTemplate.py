#!/usr/bin/python
# @copyright Copyright 2022 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, March 2019, --, Initial implementation.}
# @revs_end
#
# This implements a templated output of the Trick input file to set up a GUNNS super-network.
class SuperNetworkSetupTemplate:

  data = {}

  def __init__(self, data):
    self.data = data
    return

  def render(self):
    r = (
      '\'\'\'\n'
      '####################################################################################################\n')
    for notice in self.data['doxNotices']:
      r = r + notice
    for copyright in self.data['doxCopyrights']:
      r = r + copyright
    for license in self.data['doxLicenses']:
      r = r + license
    for dox in self.data['doxData'][:-1]:
      r = r + dox + '\n'
    for dox in self.data['doxData'][-1:]:
      r = r + dox + '\n\n'
    for reference in self.data['doxReferences'][:-1]:
      r = r + reference
    for reference in self.data['doxReferences'][-1:]:
      r = r + reference + '\n'
    if len(self.data['doxAssumptions']) > 0:
      r = r + ('@details ASSUMPTIONS AND LIMITATIONS:\n')
      for assumption in self.data['doxAssumptions'][:-1]:
        r = r + assumption + '\n'
      for assumption in self.data['doxAssumptions'][-1:]:
        r = r + assumption + '\n\n'
    r = r + (self.data['revline'] + '\n'
      '####################################################################################################\n'
      '\'\'\'\n'
      '\n'
      'import trick\n'
      '\n'
      '# Add the given sub-networks to the given super-network.  The super-network is not finalized, and\n'
      '# link port connections are not changed.\n'
      'def ' + self.data['functionName'] + 'AddNetworks(superNet,    # the super-network\n')
    functionNameIndent = ' ' * (len(self.data['functionName']) + len('AddNetworks') + 1)
    for subNet in self.data['subNets']:
      r = r + (
      '    ' + functionNameIndent + subNet[0] + ',' + subNet[3] + '    # sub-network type ' + subNet[2] + '\n')
    r = r + (
      '    ' + functionNameIndent + '):\n')
    for subNet in self.data['subNets']:
      r = r + (
      '    superNet.addSubNetwork(' + subNet[0] + ')\n')
    r = r + (
      '\n'
      '# Finalize the super-network with its nodes and solver configuration data.\n'
      'def ' + self.data['functionName'] + 'Finalize(superNet):\n'
      '    superNet.registerSuperNodes()\n'
      '    superNet.netSolverConfig.mConvergenceTolerance      = ' + self.data['solverConfig'][1] + '\n'
      '    superNet.netSolverConfig.mMinLinearizationPotential = ' + self.data['solverConfig'][2] + '\n'
      '    superNet.netSolverConfig.mMinorStepLimit            = ' + self.data['solverConfig'][3] + '\n'
      '    superNet.netSolverConfig.mDecompositionLimit        = ' + self.data['solverConfig'][4] + '\n'
      '    superNet.netSolver.setIslandMode(trick.Gunns.SOLVE)\n'
      '\n'
      '# Change the link port assignments to connect them between sub-networks.\n'
      '# NOTE: sub-networks must be added to the super-network before this is called.\n'
      'def ' + self.data['functionName'] + 'MoveLinks(')
    subNet0 = self.data['subNets'][0]
    r = r + (subNet0[0] + ',' + subNet0[3] + '    # sub-network type ' + subNet0[2] + '\n')
    functionNameIndent = ' ' * (len(self.data['functionName']) + len('MoveLinks') + 1)
    for subNet in self.data['subNets'][1:]:
      r = r + (
      '    ' + functionNameIndent + subNet[0] + ',' + subNet[3] + '    # sub-network type ' + subNet[2] + '\n')
    r = r + (
      '    ' + functionNameIndent + '):\n'
      '    # Get super-node number offset for each sub-network.\n'
      '    superNetwork = ' + subNet0[0] + '.getSuperNetwork()\n'
      '    super_ground = superNetwork.netNodeList.mNumNodes - 1\n')
    for subNet in self.data['subNets']:
      r = r + (
      '    offset_' + subNet[0] + subNet[3] + ' = ' + subNet[0] + '.getNodeOffset()\n')
    r = r + (
      '\n'
      '    # Override initial link node mapping.\n'
      '    #\n')
    for link in self.data['links']:
      r = r + (
      '    # Link ' + link.subNetName + '.' + link.name + '\n'
      '    map_' + link.subNetName + '_' + link.name + ' = trick.alloc_type(' + str(len(link.ports) + len(link.superPorts)) + ', "int")\n'
      '    map_' + link.subNetName + '_' + link.name + ' = ' + link.portMap + '\n'
      '    ' + link.subNetName + '.netInput.' + link.name + '.mInitialNodeMap = map_' + link.subNetName + '_' + link.name + '\n'
      '\n')
    r = r + (
      '\n'
      '# This is the all-in-one call to configure and finalize this super-network and updated the link\n'
      '# connections.  This should only be used if this super-network is not going to be added to a\n'
      '# higher-level super-network.\n'
      'def ' + self.data['functionName'] + 'Setup(superNet,    # the super-network\n')
    functionNameIndent = ' ' * (len(self.data['functionName']) + len('Setup') + 1)
    for subNet in self.data['subNets']:
      r = r + (
      '    ' + functionNameIndent + subNet[0] + ',' + subNet[3] + '    # sub-network type ' + subNet[2] + '\n')
    r = r + (
      '    ' + functionNameIndent + '):\n'
      '    # Add the sub-networks.\n'
      '    ' + self.data['functionName'] + 'AddNetworks(superNet,    # the super-network\n')
    functionNameIndent = ' ' * (len(self.data['functionName']) + len('AddNetworks') + 1)
    for subNet in self.data['subNets']:
      r = r + (
      '    ' + functionNameIndent + subNet[0] + ',' + subNet[3] + '    # sub-network type ' + subNet[2] + '\n')
    r = r + (
      '    ' + functionNameIndent + ')\n'
      '\n'
      '    # Configure the super-network.\n'
      '    ' + self.data['functionName'] + 'Finalize(superNet)\n'
      '\n'
      '    # Change link connections.\n'

      '    ' + self.data['functionName'] + 'MoveLinks(')
    subNet0 = self.data['subNets'][0]
    r = r + (subNet0[0] + ',' + subNet0[3] + '    # sub-network type ' + subNet0[2] + '\n')
    functionNameIndent = ' ' * (len(self.data['functionName']) + len('MoveLinks') + 1)
    for subNet in self.data['subNets'][1:]:
      r = r + (
      '    ' + functionNameIndent + subNet[0] + ',' + subNet[3] + '    # sub-network type ' + subNet[2] + '\n')
    r = r + (
      '    ' + functionNameIndent + ')\n'
      '\n')
    return r
