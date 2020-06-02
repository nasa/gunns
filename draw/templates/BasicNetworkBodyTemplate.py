#!/usr/bin/python
# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, February 2019, --, Initial implementation.}
# @revs_end
#
# This implements a templated output of the network body (.cpp) file for GUNNS Basic networks.
class BasicNetworkBodyTemplate:

  data = {}

  def __init__(self, data):
    self.data = data
    return

  def blockDepsPostLinks(self):
    return ''

  def blockPreConfig(self):
    return ''

  def blockConfigPreSolver(self):
    return ''

  def blockInputPreSpotter(self):
    return ''

  def blockConstructorPreSpotter(self):
    return ''

  def blockInitNodes(self):
    r = ''
    for node in self.data['nodes']:
      r = r+('    netNodeList.mNodes[' + node[0] + ' + netSuperNodesOffset].initialize(name + createNodeName(' + node[0] + ' + netSuperNodesOffset), ' + node[1] + ');\n')
    r = r+('    /// - Only init the Ground node if this is not a sub-network.\n'
        '    if (!netIsSubNetwork) {\n'
        '        netNodeList.mNodes[' + str(self.data['numNodes']) + '].initialize(name + ".GROUND");\n'
        '    }\n')
    return r

  def blockSolverInitializeNodes(self):
    r = ('        netSolver.initializeNodes(netNodeList);\n')
    return r

  def render(self):
    r =('/**\n')
    for notice in self.data['doxNotices']:
      r = r + notice + '\n'
    r = r+(
        '@file  ' + self.data['networkName'] + '.cpp\n'
        '@brief ' + self.data['networkName'] + ' GUNNS ' + self.data['networkType'] + ' Network implementation.\n'
        '\n')
    for copyright in self.data['doxCopyrights']:
      r = r + copyright + '\n'
    for license in self.data['doxLicenses']:
      r = r + license + '\n'
    r = r+(
        'LIBRARY DEPENDENCY:\n'
        '(\n')
    for path in self.data['spotterSourcePaths']:
      r =r +('  (' + path + '.o)\n')
    for path in self.data['linkSourcePaths']:
      r =r +('  (' + path + '.o)\n')
    if len(self.data['socketLists']) > 0:
      r =r +('  (core/Gunns' + self.data['networkType'] + 'JumperPlug.o)\n')
    r = r + self.blockDepsPostLinks()
    r = r+('  (core/network/GunnsNetworkBase.o)\n'
        ')\n'
        '\n'
        'PROGRAMMERS:\n'
        + self.data['revline'] + '\n'
        '*/\n'
        '\n'
        '#include "' + self.data['networkName'] + '.hh"\n'
        '#include "simulation/hs/TsHsMsg.hh"\n'
        '#include "software/exceptions/TsInitializationException.hh"\n'
        '\n')
# TODO namespace #}
    if len(self.data['dataTables']) > 0:
      r = r+('// Tables data\n')
      for table in self.data['dataTables']:
        name   = table[0][1]
        size   = str(int(table[1][1]) - 1)
        x_vals = ''
        y_vals = ''
        for row in table[3:-1]:
          x_vals = x_vals + row[0] + ', '
          y_vals = y_vals + row[1] + ', '
        x_vals = x_vals + table[-1][0]
        y_vals = y_vals + table[-1][1]
        r = r+('const double ' + self.data['networkName'] + '::' + name + table[2][0] + '[' + size + '] = {' + x_vals + '};\n'
               'const double ' + self.data['networkName'] + '::' + name + table[2][1] + '[' + size + '] = {' + y_vals + '};\n')
      r = r+('\n')
    r = r + self.blockPreConfig()
    r = r+(
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @param[in] name    (--) Name of the network for H&S messages.\n'
        '/// @param[in] network (--) Pointer to the main network object.\n'
        '/// \n'
        '/// @details  Default constructs the ' + self.data['networkName'] + ' Network Config Data.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '' + self.data['networkName'] + 'ConfigData::' + self.data['networkName'] + 'ConfigData(const std::string& name, ' + self.data['networkName'] + '* network)\n'
        '    :\n')
    r = r + self.blockConfigPreSolver()
    r = r+('    netSolver(name + ".netSolver"' + self.data['solverConfig'] + '),\n'
        '    // Spotter Config Data\n')
    for spotter in self.data['spotters']:
      r = r+('    ' + spotter[1] + '(name + ".' + spotter[1] + '"' + spotter[2] + '),\n')
    r = r+('    // Link Config Data\n')
    for link in self.data['links'][:-1]:
      r = r+('    ' + link[1] + '(name + ".' + link[1] + '", &network->netNodeList' + link[2] + '),\n')
    for link in self.data['links'][-1:]:
      r = r+('    ' + link[1] + '(name + ".' + link[1] + '", &network->netNodeList' + link[2] + ')\n')
    r = r+('{\n'
        '    // Load config data vectors\n')
    for link in self.data['links']:
      r = r + link[5]
    r = r+('}\n'
        '\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @details  Default destructs the ' + self.data['networkName'] + ' Network Config Data.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '' + self.data['networkName'] + 'ConfigData::~' + self.data['networkName'] + 'ConfigData()\n'
        '{\n'
        '    // Nothing to do\n'
        '}\n'
        '\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @param[in] network (--) Pointer to the main network object.\n'
        '///\n'
        '/// @details  Default constructs the ' + self.data['networkName'] + ' Network Input Data.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '' + self.data['networkName'] + 'InputData::' + self.data['networkName'] + 'InputData(' + self.data['networkName'] + '* network)\n'
        '    :\n')
    r = r + self.blockInputPreSpotter()
    r = r+('    // Spotter Input Data\n')
    for spotter in self.data['spotters']:
      r = r+('    ' + spotter[1] + '(' + spotter[3] + '),\n')
    r = r+('    // Link Input Data\n')
    for link in self.data['links'][:-1]:
      r = r+('    ' + link[1] + '(' + link[3] + '),\n')
    for link in self.data['links'][-1:]:
      r = r+('    ' + link[1] + '(' + link[3] + ')\n')
    r = r+('{\n'
        '    // Load input data vectors\n')
    for link in self.data['links']:
      r = r + link[6]
    r = r+('}\n'
        '\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @details  Default destructs the ' + self.data['networkName'] + ' Network Input Data.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '' + self.data['networkName'] + 'InputData::~' + self.data['networkName'] + 'InputData()\n'
        '{\n'
        '    // Nothing to do\n'
        '}\n'
        '\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @param[in] name (--) Name of the network for H&S messages.\n'
        '///\n'
        '/// @details  Default constructs the ' + self.data['networkName'] + ' Network.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '' + self.data['networkName'] + '::' + self.data['networkName'] + '(const std::string& name)\n'
        '    :\n'
        '    GunnsNetworkBase(name, N_NODES, netNodes),\n'
        '    netNodes(),\n'
        '    netConfig(name, this),\n'
        '    netInput(this),\n')
    r = r + self.blockConstructorPreSpotter()
    r = r+('    // Data Tables \n')
    for table in self.data['dataTables']:
      name   = table[0][1]
      size   = str(int(table[1][1]) - 1)
      x_axis = name + table[2][0]
      y_axis = name + table[2][1]
      x_min  = table[3][0]
      x_max  = table[-1][0]
      r = r+('    ' + name + '(' + x_axis + ', ' + y_axis + ', ' + size + ', ' + x_min + ', ' + x_max + '),\n')
    r = r+('    // Spotters\n')
    for spotter in self.data['spotters']:
      r = r+('    ' + spotter[1] + '(' + spotter[4] + '),\n')
    if len(self.data['jumperPlugs']) > 0:
      r = r + (
        '    // Jumper Plugs\n')
      for jumperPlug in self.data['jumperPlugs']:
        r = r + (
        '    ' + jumperPlug[1] + '(name + ".' + jumperPlug[1] + '"),\n')
    r = r+('    // Links\n')
    for link in self.data['links'][:-1]:
      r = r+('    ' + link[1] + '(),\n')
    for link in self.data['links'][-1:]:
      r = r+('    ' + link[1] + '()\n')
    r = r+('{\n'
        '    // Nothing to do\n'
        '}\n'
        '\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @details Default destructs the ' + self.data['networkName'] + ' Network.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '' + self.data['networkName'] + '::~' + self.data['networkName'] + '()\n'
        '{\n'
        '    // Nothing to do\n'
        '}\n'
        '\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @param[in] name (--) Name of the network for H&S messages.\n'
        '///\n'
        '/// @details  Initializes the nodes with their config and input data objects.  The nodes are\n'
        '///           initialized through the node list object, which may point to this network\'s nodes\n'
        '///           (when this is a standalone network), or the super-network\'s nodes (as a sub-network).\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'void ' + self.data['networkName'] + '::initNodes(const std::string& name)\n'
        '{\n'
        '    /// - Initialize the nodes.\n')
    r = r + self.blockInitNodes()
    r = r+('}\n'
        '\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @details  Initializes this network\'s links, spotters and solver with their config and input data\n'
        '///           objects.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'void ' + self.data['networkName'] + '::initNetwork()\n'
        '{\n'
        '    const int groundOffset = netNodeList.mNumNodes - N_NODES;\n'
        '\n'
        '    /// - Register sockets with the jumper plugs.\n')
    for jumperPlug in self.data['jumperPlugs']:
      socketListName = jumperPlug[0][len(self.data['networkName'] + '_'):]
      for socketList in self.data['socketLists']:
        if socketList[0] == socketListName:
          for socket in socketList[2:]:
            r = r + ('    ' + jumperPlug[1] + '.addSocket(' + socket + ');\n')
    r = r + (
        '\n'
        '    /// - Initialize the links.\n'
        '    netLinks.clear();\n')
    for link in self.data['links']:
      r = r+link[4]
    r = r+('\n'
        '    /// - Initialize the spotters.\n')
    for spotter in self.data['spotters']:
      r = r+('    ' + spotter[1] + '.initialize(&netConfig.' + spotter[1] + ', &netInput.' + spotter[1] + ');\n')
    r = r+('\n'
        '    /// - Initialize the solver, only if this is not a sub-network.\n'
        '    if (!netIsSubNetwork) {\n')
    r = r + self.blockSolverInitializeNodes()
    r = r+('        netSolver.initialize(netConfig.netSolver, netLinks);\n'
        '    }\n'
        '}\n'
        '\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @param[in] timeStep (s) Integration time step.\n'
        '///\n'
        '/// @details  Updates this network\'s spotters before solving the network.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'void ' + self.data['networkName'] + '::stepSpottersPre(const double timeStep)\n'
        '{\n'
        '    /// - Step network spotters prior to solver step.\n')
    for spotter in self.data['spotters']:
      r = r+('    ' + spotter[1] + '.stepPreSolver(timeStep);\n')
    r = r+('}\n'
        '\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @param[in]   timeStep (s) Integration time step.\n'
        '///\n'
        '/// @details  Updates this network\'s spotters after solving the network.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'void ' + self.data['networkName'] + '::stepSpottersPost(const double timeStep)\n'
        '{\n'
        '    /// - Step network spotters after solver step.\n')
    for spotter in self.data['spotters']:
      r = r+('    ' + spotter[1] + '.stepPostSolver(timeStep);\n')
    r = r+('}\n')
# TODO end namespace #}
    return r
