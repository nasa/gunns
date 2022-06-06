#!/usr/bin/python
# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, February 2019, --, Initial implementation.}
# @revs_end
#
# This implements a templated output of the network header (.hh) file for GUNNS Basic networks.
class BasicNetworkHeaderTemplate:

  data = {}

  def __init__(self, data):
    self.data = data
    return

  def blockIncludesPostLinks(self):
    return ''

  def blockPreConfig(self):
    return ''

  def blockConfigPreSolver(self):
    return ''

  def blockInputPreSpotter(self):
    return ''

  def blockDeclarationsPreSpotters(self):
    return ''

  def blockDeclarationsEndPublic(self):
    return ''

  def blockPostClass(self):
    return ''

  def render(self):
    r =('#ifndef ' + self.data['networkName'] + '_EXISTS\n'
        '#define ' + self.data['networkName'] + '_EXISTS\n'
        '\n'
        '/**\n')
    for notice in self.data['doxNotices']:
      r = r + notice + '\n'
    r = r + (
        '@file  ' + self.data['networkName'] + '.hh\n'
        '@brief ' + self.data['networkName'] + ' GUNNS ' + self.data['networkType'] + ' Network declarations.\n'
        '\n')
    for copyright in self.data['doxCopyrights']:
      r = r + copyright + '\n'
    for license in self.data['doxLicenses']:
      r = r + license + '\n'
    for dox in self.data['doxData']:
      r = r + dox + '\n'
    r = r + (
        '\n'
        '@details\n'
        'PURPOSE: (Provides classes for the ' + self.data['networkName'] + ' GUNNS ' + self.data['networkType'] + ' Network.)\n'
        '\n'
        'REFERENCES:\n'
        '  (')
    for reference in self.data['doxReferences'][:1]:
      r = r + reference
    for reference in self.data['doxReferences'][1:]:
      r = r + '\n   ' + reference
    r = r + (')\n'
        '\n'
        'ASSUMPTIONS AND LIMITATIONS:\n'
        '  (')
    for assumption in self.data['doxAssumptions'][:1]:
      r = r + assumption
    for assumption in self.data['doxAssumptions'][1:]:
      r = r + '\n   ' + assumption
    r = r + (')\n'
        '\n'
        'LIBRARY DEPENDENCY:\n'
        '  ((' + self.data['networkName'] + '.o))\n'
        '\n'
        'PROGRAMMERS:\n'
        + self.data['revline'] + '\n'
        '\n'
        '@{\n'
        '*/\n'
        '\n'
        '#include "software/SimCompatibility/TsSimCompatibility.hh"\n')
    if len(self.data['dataTables']) > 0:
      r = r + ('#include "math/approximation/TsLinearInterpolator.hh"\n')
    r = r + (
        '#include "core/network/GunnsNetworkBase.hh"\n')
    for path in self.data['spotterSourcePaths']:
      r = r + ('#include "' + path + '.hh"\n')
    for path in self.data['linkSourcePaths']:
      r = r + ('#include "' + path + '.hh"\n')
    r = r + self.blockIncludesPostLinks()
    if len(self.data['socketLists']) > 0:
      r = r + ('#include "core/Gunns' + self.data['networkType'] + 'JumperPlug.hh"\n')
    # TODO namespace statement
    r = r + (
        '\n'
        '// Forward-declare the main network class for use in the config data.\n'
        'class ' + self.data['networkName'] + ';\n'
        '\n')
    r = r + self.blockPreConfig()
    for socketList in self.data['socketLists']:
      className = self.data['networkName'] + '_' + socketList[0]
      namespaceForMacro = ''
      if '' != self.data['networkNamespace']:
        namespaceForMacro = self.data['networkNamespace'].rstrip(':') + '__'
      r = r + (
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @brief    ' + className + ' Jumper Plug definitions \n'
        '///\n'
        '/// @details  Defines a jumper plug class for a socket list in this network.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'class ' + className + ' : public Gunns' + self.data['networkType'] + 'JumperPlug\n'
        '{\n'
        '    TS_MAKE_SIM_COMPATIBLE(' + namespaceForMacro + className + ');\n'
        '    public:\n'
        '        /// @brief  Enumeration of the socket options for this plug.\n'
        '        enum ' + socketList[0] + 'Enum {\n')
      socketNum = 0
      for socket in socketList[2:]:
        r = r + ('            ' + socket + ' = ' + str(socketNum) + ',\n')
        socketNum = socketNum + 1
      r = r + (
        '            NONE = ' + str(socketNum) + '\n'
        '        };\n'
        '        ' + socketList[0] + 'Enum mActiveConnection;     /**< *o (--) trick_chkpnt_io(*io) Currently connected socket     */\n'
        '        ' + socketList[0] + 'Enum mLastDisconnection;    /**< *o (--) trick_chkpnt_io(*io) Socket last disconnected from  */\n'
        '        ' + socketList[0] + 'Enum mConnectionRequest;    /**<    (--) trick_chkpnt_io(**)  Connect to socket command      */\n'
        '        ' + socketList[0] + 'Enum mDisconnectionRequest; /**<    (--) trick_chkpnt_io(**)  Disconnect form socket command */\n'
        '        /// @brief  Default constructs this jumper plug.\n'
        '        ' + className + '(const std::string name)\n'
        '            :\n'
        '            GunnsFluidJumperPlug(name, static_cast <int> (NONE)),\n'
        '            mActiveConnection    (NONE),\n'
        '            mLastDisconnection   (NONE),\n'
        '            mConnectionRequest   (NONE),\n'
        '            mDisconnectionRequest(NONE) {}\n'
        '        /// @brief Default desctructs this jumper plug.\n'
        '        virtual ~' + className + '() {}\n'
        '        /// @brief Sets the active connection of this plug to the given socket index.\n'
        '        /// @param[in] index (--) Index to the socket list enumeration.\n'
        '        virtual void setActiveConnection    (const int index) {mActiveConnection     = static_cast <' + socketList[0] + 'Enum> (index);}\n'
        '        /// @brief Sets the last disconnected socket of this plug to the given socket index.\n'
        '        /// @param[in] index (--) Index to the socket list enumeration.\n'
        '        virtual void setLastDisconnection   (const int index) {mLastDisconnection    = static_cast <' + socketList[0] + 'Enum> (index);}\n'
        '        /// @brief Sets the command to connected to the given socket index.\n'
        '        /// @param[in] index (--) Index to the socket list enumeration.\n'
        '        virtual void setConnectionRequest   (const int index) {mConnectionRequest    = static_cast <' + socketList[0] + 'Enum> (index);}\n'
        '        /// @brief Sets the command to disconnect to the given socket index.\n'
        '        /// @param[in] index (--) Index to the socket list enumeration.\n'
        '        virtual void setDisconnectionRequest(const int index) {mDisconnectionRequest = static_cast <' + socketList[0] + 'Enum> (index);}\n'
        '        /// @brief Returns the index of the currently connected socket.\n'
        '        /// @return int (--) Index to the socket list enumeration.\n'
        '        virtual int  getActiveConnection()     {return static_cast <int> (mActiveConnection);}\n'
        '        /// @brief Returns the index of the socket last disconnected from.\n'
        '        /// @return int (--) Index to the socket list enumeration.\n'
        '        virtual int  getLastDisconnection()    {return static_cast <int> (mLastDisconnection);}\n'
        '        /// @brief Returns the index of the socket the plug is commanded to connect to.\n'
        '        /// @return int (--) Index to the socket list enumeration.\n'
        '        virtual int  getConnectionRequest()    {return static_cast <int> (mConnectionRequest);}\n'
        '        /// @brief Returns the index of the socket the plug is commanded to disconnect from.\n'
        '        /// @return int (--) Index to the socket list enumeration.\n'
        '        virtual int  getDisconnectionRequest() {return static_cast <int> (mDisconnectionRequest);}\n'
        '        /// @brief Returns the index of no socket, the last index in the list.\n'
        '        /// @return int (--) Index to the socket list enumeration.\n'
        '        virtual int  getNoConnection()         {return static_cast <int> (NONE);}\n'
        '};\n'
        '\n')
    r = r + (
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @brief    ' + self.data['networkName'] + ' GUNNS Network Config Data\n'
        '///\n'
        '/// @details  Configuration data class for the ' + self.data['networkName'] + ' Network.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'class ' + self.data['networkName'] + 'ConfigData\n'
        '{\n'
        '    public:\n')
    r = r + self.blockConfigPreSolver()
    r = r + (
        '        // Solver configuration data\n'
        '        GunnsConfigData netSolver;    /**< (--) trick_chkpnt_io(**) Network solver config data. */ \n'
        '        // Spotters configuration data\n')
    for spotter in self.data['spotters']:
      r = r + ('        ' + spotter[0] + 'ConfigData ' + spotter[1] + ';    /**< (--) trick_chkpnt_io(**) ' + spotter[1] + ' config data. */\n')
    r = r + (
        '        // Links configuration data\n')
    for link in self.data['links']:
      r = r + ('        ' + link[0] + 'ConfigData ' + link[1] + ';    /**< (--) trick_chkpnt_io(**) ' + link[1] + ' config data. */\n')
    r = r + (
        '        /// @brief  Default constructs this network configuration data.\n'
        '        ' + self.data['networkName'] + 'ConfigData(const std::string& name, ' + self.data['networkName'] + '* network);\n'
        '        /// @brief  Default destructs this network configuration data.\n'
        '        virtual ~' + self.data['networkName'] + 'ConfigData();\n'
        '\n'
        '    private:\n'
        '        /// @details  Copy constructor unavailable since declared private and not implemented.\n'
        '        ' + self.data['networkName'] + 'ConfigData(const ' + self.data['networkName'] + 'ConfigData&);\n'
        '        /// @details  Assignment operator unavailable since declared private and not implemented.\n'
        '        ' + self.data['networkName'] + 'ConfigData& operator =(const ' + self.data['networkName'] + 'ConfigData&);\n'
        '};\n'
        '\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @brief    ' + self.data['networkName'] + ' GUNNS Network Input Data\n'
        '///\n'
        '/// @details  Input data class for the ' + self.data['networkName'] + ' Network.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'class ' + self.data['networkName'] + 'InputData\n'
        '{\n'
        '    public:\n')
    r = r + self.blockInputPreSpotter()
    r = r + (
        '        // Spotters input data\n')
    for spotter in self.data['spotters']:
      r = r + ('        ' + spotter[0] + 'InputData ' + spotter[1] + ';    /**< (--) trick_chkpnt_io(**) ' + spotter[1] + ' input data. */\n')
    r = r + (
        '        // Links input data\n')
    for link in self.data['links']:
      r = r + ('        ' + link[0] + 'InputData ' + link[1] + ';    /**< (--) trick_chkpnt_io(**) ' + link[1] + ' input data. */\n')
    r = r + (
        '        /// @brief  Default constructs this network input data.\n'
        '        ' + self.data['networkName'] + 'InputData(' + self.data['networkName'] + '* network);\n'
        '        /// @brief  Default destructs this network input data.\n'
        '        virtual ~' + self.data['networkName'] + 'InputData();\n'
        '\n'
        '    private:\n'
        '        /// @details  Copy constructor unavailable since declared private and not implemented.\n'
        '        ' + self.data['networkName'] + 'InputData(const ' + self.data['networkName'] + 'InputData&);\n'
        '        /// @details  Assignment operator unavailable since declared private and not implemented.\n'
        '        ' + self.data['networkName'] + 'InputData& operator =(const ' + self.data['networkName'] + 'InputData&);\n'
        '};\n'
        '\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @brief    ' + self.data['networkName'] + ' GUNNS Network\n'
        '///\n'
        '/// @details  Main class for the ' + self.data['networkName'] + ' Network.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'class ' + self.data['networkName'] + ' : public GunnsNetworkBase\n'
        '{\n'
        '    TS_MAKE_SIM_COMPATIBLE(' + self.data['networkName'] + ');\n'
        '    public:\n'
        '        /// @brief  Enumeration of the ' + self.data['networkName'] + ' Network nodes.\n'
        '        enum Nodes\n'
        '        {\n')
    for node in self.data['nodes']:
      r = r + ('            Node' + node[0] + ' = ' + node[0] + ',    ///< Node ' + node[0] + '\n')
    r = r + (
        '            GROUND = ' + str(self.data['numNodes']) + ',    ///< Ground Node\n'
        '            N_NODES = ' + str(self.data['numNodes'] + 1) + '    ///< Number of nodes including Ground\n'
        '        };\n'
        '        // Network declarations\n'
        '        Gunns' + self.data['networkType'] + 'Node netNodes[' + self.data['networkName'] + '::N_NODES];    /**< (--) Network nodes array. */\n'
        '        ' + self.data['networkNamespace'] + '' + self.data['networkName'] + 'ConfigData netConfig;    /**< (--) trick_chkpnt_io(**) Network config data. */\n'
        '        ' + self.data['networkNamespace'] + '' + self.data['networkName'] + 'InputData netInput;    /**< (--) trick_chkpnt_io(**) Network input data. */\n')
    r = r + self.blockDeclarationsPreSpotters()
    r = r + (
        '        // Data Tables\n')
    for table in self.data['dataTables']:
      name = table[0][1]
      size   = str(int(table[1][1]) - 1)
      axis_x = table[2][0]
      axis_y = table[2][1]
      r = r + ('        static const double ' + name + axis_x + '[' + size + '];    /**< (--) ' + name + ' ' + axis_x + ' (x) axis. */\n'
               '        static const double ' + name + axis_y + '[' + size + '];    /**< (--) ' + name + ' ' + axis_y + ' (y) axis. */\n'
               '        TsLinearInterpolator ' + name + ';    /**< (--) ' + name + ' linear interpolator. */\n')
    r = r + (
        '        // Spotters\n')
    for spotter in self.data['spotters']:
      r = r + ('        ' + spotter[0] + ' ' + spotter[1] + ';    /**< (--) ' + spotter[1] + ' instance. */\n')
    if len(self.data['jumperPlugs']) > 0:
      r = r + (
        '        // Jumper Plugs\n')
      for jumperPlug in self.data['jumperPlugs']:
        r = r + (
        '        ' + jumperPlug[0] + ' ' + jumperPlug[1] + '; /**< (--) ' + jumperPlug[1] + ' instance. */\n')
    r = r + (
        '        // Links\n')
    for link in self.data['links']:
      r = r + ('        ' + link[0] + ' ' + link[1] + ';    /**< (--) ' + link[1] + ' instance. */\n')
    r = r + (
        '        /// @brief  Default constructs this network.\n'
        '        ' + self.data['networkName'] + '(const std::string& name = "");\n'
        '        /// @brief  Default destructs this network.\n'
        '        virtual ~' + self.data['networkName'] + '();\n'
        '        /// @brief  Network nodes initialization task.\n'
        '        virtual void initNodes(const std::string& name);\n'
        '        /// @brief  Network links & spotters initialization task.\n'
        '        virtual void initNetwork();\n'
        '        /// @brief  Update network spotters before the solver solution.\n'
        '        virtual void stepSpottersPre(const double timeStep);\n'
        '        /// @brief  Update network spotters after the solver solution.\n'
        '        virtual void stepSpottersPost(const double timeStep);\n')
    r = r + self.blockDeclarationsEndPublic()
    r = r + (
        '\n'
        '    private:\n'
        '        /// @details  Copy constructor unavailable since declared private and not implemented.\n'
        '        ' + self.data['networkName'] + '(const ' + self.data['networkName'] + '&);\n'
        '        /// @details  Assignment operator unavailable since declared private and not implemented.\n'
        '        ' + self.data['networkName'] + '& operator =(const ' + self.data['networkName'] + '&);\n'
        '};\n'
        '\n'
        '/// @}  \n'
        '\n')
    r = r + self.blockPostClass()
    r = r + (
      # TODO end namespace
        '#endif\n')

    return r.encode('ascii', 'ignore').decode('ascii')
