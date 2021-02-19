#!/usr/bin/python
# @copyright Copyright 2019 United States Government as represented by the Administrator of the
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

# TODO add some comments up top, revline, etc.
  def render(self):
    r = (
      'import trick\n'
      '\n'
      'def ' + self.data['functionName'] + '(superNet,    # the super-network\n')
    functionNameIndent = ' ' * (len(self.data['functionName']) + 1)
    for subNet in self.data['subNets']:
      r = r + (
      '    ' + functionNameIndent + subNet[0] + ',' + subNet[3] + '    # sub-network type ' + subNet[2] + '\n')
    r = r + (
      '    ' + functionNameIndent + '):\n'
      '\n'
      '    # Add sub-networks to the super-network.\n')
    for subNet in self.data['subNets']:
      r = r + (
      '    superNet.addSubNetwork(' + subNet[0] + ')\n')
    r = r + (
      '\n'
      '    # Configure the super-network.\n'
      '    superNet.registerSuperNodes()\n'
      '    superNet.netSolverConfig.mConvergenceTolerance      = ' + self.data['solverConfig'][1] + '\n'
      '    superNet.netSolverConfig.mMinLinearizationPotential = ' + self.data['solverConfig'][2] + '\n'
      '    superNet.netSolverConfig.mMinorStepLimit            = ' + self.data['solverConfig'][3] + '\n'
      '    superNet.netSolverConfig.mDecompositionLimit        = ' + self.data['solverConfig'][4] + '\n'
      '    superNet.netSolver.setIslandMode(trick.Gunns.SOLVE)\n'
      '\n'
      '    # Get super-node number offset for each sub-network.\n'
      '    super_ground = superNet.netNodeList.mNumNodes - 1\n')
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

    return r
