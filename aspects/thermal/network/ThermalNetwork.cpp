/************************************** TRICK HEADER ***********************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
(
    (ThermFileParser.o)
    (core/Gunns.o)
    (core/GunnsBasicNode.o)
    (aspects/thermal/GunnsThermalCapacitor.o)
    (core/GunnsBasicConductor.o)
    (aspects/thermal/GunnsThermalRadiation.o)
    (aspects/thermal/GunnsThermalHeater.o)
    (aspects/thermal/GunnsThermalPanel.o)
    (aspects/thermal/GunnsThermalPotential.o)
    (aspects/thermal/GunnsThermalSource.o)
    (core/GunnsBasicIslandAnalyzer.o)
    (units/TS_conversions.o)
    (software/exceptions/TsParseException.o)
)
***************************************************************************************************/
#include "ThermalNetwork.hh"
#include "software/exceptions/TsParseException.hh"
#include "math/UnitConversion.hh"
#include "aspects/thermal/PtcsMacros.hh"
#include "math/MsMath.hh" //needed for MsMath::isInRange() in setHeaterPowers()
/**************************************************************************************************/
/* ConfigData class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  nodeFile  (--)  xml file with node & capacitance link data
/// @param[in]  condFile  (--)  xml file with conduction link data
/// @param[in]  radFile   (--)  xml file with radiation link data
/// @param[in]  htrFile   (--)  xml file with heater link data
/// @param[in]  panFile   (--)  xml file with thermal-panel data
/// @param[in]  etcFile   (--)  xml file with other thermal link data (potentials, sources, etc)
///
/// @details    Constructs this ThermalNetwork configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermalNetworkConfigData::ThermalNetworkConfigData(
        std::string nodeFile,
        std::string condFile,
        std::string radFile,
        std::string htrFile,
        std::string panFile,
        std::string etcFile)
    :
    cNodeFile(nodeFile),
    cCondFile(condFile),
    cRadFile(radFile),
    cHtrFile(htrFile),
    cPanFile(panFile),
    cEtcFile(etcFile)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  source to copy
///
/// @details  Copy constructs this ThermalNetwork configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermalNetworkConfigData::ThermalNetworkConfigData(const ThermalNetworkConfigData& that)
    :
    cNodeFile(that.cNodeFile),
    cCondFile(that.cCondFile),
    cRadFile(that.cRadFile),
    cHtrFile(that.cHtrFile),
    cPanFile(that.cPanFile),
    cEtcFile(that.cEtcFile)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this ThermalNetwork configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermalNetworkConfigData::~ThermalNetworkConfigData()
{
    // nothing to do
}

/**************************************************************************************************/
/* InputData class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  thermInputFile  (--)  xml file with thermal input override data
///
/// @details    Constructs this ThermalNetwork input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermalNetworkInputData::ThermalNetworkInputData(std::string thermInputFile,
                                                 std::string radInputFile)
    :
    iThermInputFile(thermInputFile),
    iInputRadFile(radInputFile)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  source to copy
///
/// @details  Copy constructs this ThermalNetwork input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermalNetworkInputData::ThermalNetworkInputData(const ThermalNetworkInputData& that)
    :
    iThermInputFile(that.iThermInputFile),
    iInputRadFile(that.iInputRadFile)

{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this ThermalNetwork input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermalNetworkInputData::~ThermalNetworkInputData()
{
    // nothing to do
}

/**************************************************************************************************/
/* ThermalNetwork class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default ThermalNetwork constructor. Defaults all pointers to zero.
///             Stores the name of the network.  The base network is constructed with zero node
///             count and null pointer to the nodes array -- these must be set during network
///             initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermalNetwork::ThermalNetwork(const std::string& name)
    :
    GunnsNetworkBase(name, 0, 0),
    mConfig(),
    mInput(),
    mCapacitanceLinks(0),
    mConductionLinks(0),
    mRadiationLinks (0),
    mHeaters(0),
    mPanels(0),
    mPotentialLinks(0),
    mSources(0),
    mMalfHtrMiswireFlag(false),
    mMalfHtrIndexValue(0),
    mHtrPowerElectrical(0),
    parser(name + ".parser"),
    netIslandAnalyzer(netNodeList),
    numCapEditGroups(0),
    mCapEditScaleFactor(0),
    mCapEditScalePrev(0),
    pNodes(0),
    indexSpaceNode(0),
    numLinksCap(0),
    numLinksCond(0),
    numLinksRad(0),
    numLinksHtr(0),
    numLinksPan(0),
    numLinksPot(0),
    numLinksSrc(0),
    mCapacitanceConfigData(0),
    mConductionConfigData(0),
    mRadiationConfigData(0),
    mHeaterConfigData(0),
    mPanelConfigData(0),
    mPotentialConfigData(0),
    mSourceConfigData(0),
    mCapacitanceInputData(0),
    mConductionInputData(0),
    mRadiationInputData(0),
    mHeaterInputData(0),
    mPanelInputData(0),
    mPotentialInputData(0),
    mSourceInputData(0)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructor, deletes allocated arrays and config/input objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermalNetwork::~ThermalNetwork()
{
    cleanUp();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name  (--)  string representing the name of this thermal network
///
/// @details  Tells the network's ThermFileParser object to parse the configuration files and
///           populate its vectors with the data found.  The appropriate space on the heap is
///           allocated based on the number of links of each type.  Config/Input data objects are
///           constructed with the corresponding data from the config-files.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::initNodes(const std::string& name)
{
    /// - For backward compatibility with old sims that don't pass the network name in the
    ///   constructor, we reset the name to this method's argument.  This argument only overrides
    ///   the network name if it was previously empty.
    if ("" == mName and "" != name) {
        mName = name;
    }

    /// - Clear any allocated memory.
    cleanUp();

    /// - Set file names in the ThermFileParser object.
    parser.mNodeFile          = mConfig.cNodeFile;
    parser.mCondFile          = mConfig.cCondFile;
    parser.mRadFile           = mConfig.cRadFile;
    parser.mHtrFile           = mConfig.cHtrFile;
    parser.mPanFile           = mConfig.cPanFile;
    parser.mEtcFile           = mConfig.cEtcFile;
    parser.mThermInputFile    = mInput.iThermInputFile;
    parser.mThermInputFileRad = mInput.iInputRadFile;

    /// - Populate the parser's C++ vectors with data from the config-files.
    parser.initialize(mName + ".parser");

    /// - Set the number of capacitance-edit groups.
    numCapEditGroups = parser.vCapEditGroupList.size();

    /// - Allocate and initialize the GunnsBasicNode pointer and all the node objects.
    buildNodes();

    /// - Use TS_NEW macros to allocate the links and config/input data.
    allocate();

    /// - Validate initial state.
    validate();

    /// - Construct the config/input data objects with data from the config-files.
    buildConfig(mName);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initializes the links with their node map assignments and Config/Input data, and
///           initializes the built-in network island analyzer spotter.  If this is not a sub-
///           network in a super-network the network solver is initialized with default Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::initNetwork()
{
    /// - Initialize the link objects at their ports with correct config/input data.
    buildLinks();

    /// - Initialize the island analyzer spotter.
    GunnsBasicIslandAnalyzerConfigData config(mName + ".netIslandAnalyzer");
    GunnsBasicIslandAnalyzerInputData input;
    netIslandAnalyzer.initialize(&config, &input);

    if (not netIsSubNetwork) {
        /// - Construct the Gunns network config.
        GunnsConfigData NetworkConfig(mName, // network name
                                      1.0,   // convergenceTolerance
                                      1.0,   // minLinearizationP
                                      1,     // minorStepLimit
                                      1);    // decompositionLimit

        /// - Initialize the solver after all link objects have been initialized.
        netSolver.initializeNodes(netNodeList);
        netSolver.initialize(NetworkConfig, netLinks);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Performs "pre" logic prior to loading a checkpoint.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::preloadCheckpoint()
{
    try
    {
        /// - Perform capacitor group edits.
        preloadCapacitanceGroups();

    } catch(...)
    {
        /// - Report error.
        TS_PTCS_ERROR("Other Exception while preloading for checkpoint.");
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Restarts the ThermalNetwork by calling the GUNNS restart() function.
///
/// @note   Super-networks will not call this function when they restart, so the only way to get the
///         restartCapacitanceGroups() function to run is to add a separate restart job for each
///         ThermalNetwork sub-network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::restart()
{
    try
    {
        if (not netIsSubNetwork) {
            /// - In a sub-network, this network's solver restart is skipped since this solver isn't
            ///   used.
            netSolver.restart();
        }
        restartCapacitanceGroups();

    } catch(TsInitializationException& e)
    {
        /// - Report error.
        TS_PTCS_ERROR("TsInitializationException while restarting.");

    } catch(...)
    {
        /// - Report error.
        TS_PTCS_ERROR("Other Exception while restarting.");
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  timeStep  (s)  integration time step
///
/// @details    Updates the pre-solution functions including heater power, capacitor group edits,
///             and spotters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::stepSpottersPre(const double timeStep)
{
    /// - Set the electrical power of each heater in the network.
    setHeaterPowers();

    /// - Perform capacitor group edits.
    editCapacitanceGroups();

    /// - Call the island analyzer pre-solultion update.
    netIslandAnalyzer.stepPreSolver(timeStep);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  timeStep  (s)  integration time step
///
/// @details    Updates the post-solution functions including spotters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::stepSpottersPost(const double timeStep)
{
    /// - Call the island analyzer post-solultion update.
    netIslandAnalyzer.stepPostSolver(timeStep);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Allocates and initializes the network's nodes objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::buildNodes()
{
    /// - Store the number of nodes from the ThermFileParser object.
    netNumLocalNodes = parser.numNodes;

    if (not netIsSubNetwork) {
        /// - Allocate the pNodes array with the correct size.
        TS_NEW_CLASS_ARRAY_EXT(pNodes, netNumLocalNodes, GunnsBasicNode, (), std::string(mName) + ".pNodes");

        /// - Set the nodeList attributes.
        netNodeList.mNumNodes = netNumLocalNodes;
        netNodeList.mNodes    = pNodes;
    }
    indexSpaceNode = netNodeList.mNumNodes - 1;

    /// - Loop through all the nodes.
    for(int i = 0; i < netNumLocalNodes; ++i)
    {
        /// - Call initialize() on every GunnsBasicNode.
        netNodeList.mNodes[i + netSuperNodesOffset].initialize( parser.vNodeNames.at(i) );
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Allocates arrays for each link-object type.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::allocate()
{
    /// - Set the link counters. It's important that this happens here, since the cleanUp() method
    ///   relies on the counters being nonzero as an indication that the arrays have been allocated.
    numLinksCap  = parser.numLinksCap;
    numLinksCond = parser.numLinksCond;
    numLinksRad  = parser.numLinksRad;
    numLinksHtr  = parser.numLinksHtr;
    numLinksPan  = parser.numLinksPan;
    numLinksPot  = parser.numLinksPot;
    numLinksSrc  = parser.numLinksSrc;

    /// - Capacitance Links
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (numLinksCap > 0)
    {
        /// - Allocate links array.
        TS_NEW_CLASS_ARRAY_EXT(mCapacitanceLinks, numLinksCap, GunnsThermalCapacitor, (),
                               std::string(mName) + ".mCapacitanceLinks");

        /// - Allocate ConfigData pointer array.
        mCapacitanceConfigData = new GunnsThermalCapacitorConfigData*[numLinksCap];

        /// - Allocate InputData pointer array.
        mCapacitanceInputData = new GunnsThermalCapacitorInputData*[numLinksCap];

        /// - Throw an exception if null pointer to the Network's Capacitors.
        TS_PTCS_IF_ERREX(0 == mCapacitanceLinks, TsInitializationException,
                "allocation error,", "Null pointer to CapacitanceLinks.");
        /// - Throw an exception if null pointer to the Network's CapacitanceConfigData.
        TS_PTCS_IF_ERREX(0 == mCapacitanceConfigData, TsInitializationException,
                "allocation error,", "Null pointer to CapacitanceConfigData.");
        /// - Throw an exception if null pointer to the Network's CapacitanceInputData.
        TS_PTCS_IF_ERREX(0 == mCapacitanceInputData, TsInitializationException,
                "allocation error,", "Null pointer to CapacitanceInputData.");

        /// - Allocate & initialize capacitance edit group controls.
        if (0 < numCapEditGroups)
        {
            TS_NEW_PRIM_ARRAY_EXT(mCapEditScaleFactor, numCapEditGroups, double,
                                  std::string(mName) + ".mCapEditScaleFactor");
            TS_NEW_PRIM_ARRAY_EXT(mCapEditScalePrev,   numCapEditGroups, double,
                                  std::string(mName) + ".mCapEditScalePrev");
            for(int i = 0; i < numCapEditGroups; ++i)
            {
                mCapEditScaleFactor[i] = 1.0;
                mCapEditScalePrev[i]   = 1.0;
            }
        }
        /// - Initialize each Config and Input Data pointer to null.
        for(int i = 0; i < numLinksCap; ++i)
        {
            mCapacitanceConfigData[i] = 0;
            mCapacitanceInputData[i] = 0;
        }
    }
    /// - Conduction Links
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (numLinksCond > 0)
    {
        /// - Allocate links array.
        TS_NEW_CLASS_ARRAY_EXT(mConductionLinks, numLinksCond, GunnsBasicConductor, (),
                               std::string(mName) + ".mConductionLinks");

        /// - Allocate ConfigData array.
        mConductionConfigData = new GunnsBasicConductorConfigData*[numLinksCond];

        /// - Allocate InputData array.
        mConductionInputData = new GunnsBasicConductorInputData*[numLinksCond];

        /// - Throw an exception if null pointer to the Network's Conductors.
        TS_PTCS_IF_ERREX(0 == mConductionLinks, TsInitializationException,
                "allocation error,", "Null pointer to ConductionLinks.");
        /// - Throw an exception if null pointer to the Network's ConductionConfigData.
        TS_PTCS_IF_ERREX(0 == mConductionConfigData, TsInitializationException,
                "allocation error,", "Null pointer to ConductionConfigData.");
        /// - Throw an exception if null pointer to the Network's ConductionInputData.
        TS_PTCS_IF_ERREX(0 == mConductionInputData, TsInitializationException,
                "allocation error,", "Null pointer to ConductionInputData.");

        /// - Initialize each Config and Input Data pointer to null.
        for(int i = 0; i < numLinksCond; ++i)
        {
            mConductionConfigData[i] = 0;
            mConductionInputData[i] = 0;
        }
    }
    /// - Radiation Links
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (numLinksRad > 0)
    {
        /// - Allocate links array.
        TS_NEW_CLASS_ARRAY_EXT(mRadiationLinks, numLinksRad, GunnsThermalRadiation, (),
                               std::string(mName) + ".mRadiationLinks");

        /// - Allocate ConfigData pointer array.
        mRadiationConfigData = new GunnsThermalRadiationConfigData*[numLinksRad];

        /// - Allocate InputData pointer array.
        mRadiationInputData = new GunnsThermalRadiationInputData*[numLinksRad];

        /// - Throw an exception if null pointer to the Network's Radiation links.
        TS_PTCS_IF_ERREX(0 == mRadiationLinks, TsInitializationException,
                "allocation error,", "Null pointer to RadiationLinks.");
        /// - Throw an exception if null pointer to the Network's RadiationConfigData.
        TS_PTCS_IF_ERREX(0 == mRadiationConfigData, TsInitializationException,
                "allocation error,", "Null pointer to RadiationConfigData.");
        /// - Throw an exception if null pointer to the Network's RadiationInputData.
        TS_PTCS_IF_ERREX(0 == mRadiationInputData, TsInitializationException,
                "allocation error,", "Null pointer to RadiationInputData.");

        /// - Initialize each Config and Input Data pointer to null.
        for(int i = 0; i < numLinksRad; ++i)
        {
            mRadiationConfigData[i] = 0;
            mRadiationInputData[i] = 0;
        }
    }
    /// - Heaters
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (numLinksHtr > 0)
    {
        /// - Allocate links array.
        TS_NEW_CLASS_ARRAY_EXT(mHeaters, numLinksHtr, GunnsThermalHeater, (),
                               std::string(mName) + ".mHeaters");

        /// - Allocate ConfigData pointer array.
        mHeaterConfigData = new GunnsThermalHeaterConfigData*[numLinksHtr];

        /// - Allocate InputData pointer array.
        mHeaterInputData = new GunnsThermalHeaterInputData*[numLinksHtr];

        /// - Allocate heater power array.
        TS_NEW_PRIM_ARRAY_EXT(mHtrPowerElectrical, numLinksHtr, double,
                              std::string(mName) + ".mHtrPowerElectrical");

        /// - Allocate heater malfunction index array.
        TS_NEW_PRIM_ARRAY_EXT(mMalfHtrIndexValue, numLinksHtr, int,
                              std::string(mName) + ".mMalfHtrIndexValue");

        /// - Throw an exception if null pointer to the Network's Heater Objects.
        TS_PTCS_IF_ERREX(0 == mHeaters, TsInitializationException,
                "allocation error,", "Null pointer to Heaters.");
        /// - Throw an exception if null pointer to the Network's HeaterConfigData.
        TS_PTCS_IF_ERREX(0 == mHeaterConfigData, TsInitializationException,
                "allocation error,", "Null pointer to HeaterConfigData.");
        /// - Throw an exception if null pointer to the Network's HeaterInputData.
        TS_PTCS_IF_ERREX(0 == mHeaterInputData, TsInitializationException,
                "allocation error,", "Null pointer to HeaterInputData.");

        for(int i = 0; i < numLinksHtr; ++i)
        {
            /// - Initialize each Config and Input Data pointer to null.
            mHeaterConfigData[i] = 0;
            mHeaterInputData[i] = 0;
            /// - Initialize heater power to zero.
            mHtrPowerElectrical[i] = 0.0;

            /// - The MalfHtrIndex is used to cross up heater indices. They are initialized
            ///   to be in proper order.
            mMalfHtrIndexValue[i] = i;
        }
    }
    /// - Thermal Panels
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (numLinksPan > 0)
    {
        /// - Allocate links array.
        TS_NEW_CLASS_ARRAY_EXT(mPanels, numLinksPan, GunnsThermalPanel, (),
                               std::string(mName) + ".mPanels");

        /// - Allocate ConfigData pointer array.
        mPanelConfigData = new GunnsThermalPanelConfigData*[numLinksPan];

        /// - Allocate InputData pointer array.
        mPanelInputData = new GunnsThermalPanelInputData*[numLinksPan];

        /// - Throw an exception if null pointer to the Network's Panel Objects.
        TS_PTCS_IF_ERREX(0 == mPanels, TsInitializationException,
                "allocation error,", "Null pointer to Panels.");
        /// - Throw an exception if null pointer to the Network's PanelConfigData.
        TS_PTCS_IF_ERREX(0 == mPanelConfigData, TsInitializationException,
                "allocation error,", "Null pointer to PanelConfigData.");
        /// - Throw an exception if null pointer to the Network's PanelInputData.
        TS_PTCS_IF_ERREX(0 == mPanelInputData, TsInitializationException,
                "allocation error,", "Null pointer to PanelInputData.");

        /// - Initialize each Config and Input Data pointer to null.
        for(int i = 0; i < numLinksPan; ++i)
        {
            mPanelConfigData[i] = 0;
            mPanelInputData[i] = 0;
        }
    }
    /// - Potential Links
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (numLinksPot > 0)
    {
        /// - Allocate links array.
        TS_NEW_CLASS_ARRAY_EXT(mPotentialLinks, numLinksPot, GunnsThermalPotential, (),
                               std::string(mName) + ".mPotentialLinks");

        /// - Allocate ConfigData array.
        mPotentialConfigData = new GunnsThermalPotentialConfigData*[numLinksPot];

        /// - Allocate InputData array.
        mPotentialInputData = new GunnsThermalPotentialInputData*[numLinksPot];

        /// - Throw an exception if null pointer to the Network's Potential Links.
        TS_PTCS_IF_ERREX(0 == mPotentialLinks, TsInitializationException,
                "allocation error,", "Null pointer to PotentialLinks.");
        /// - Throw an exception if null pointer to the Network's PotentialConfigData.
        TS_PTCS_IF_ERREX(0 == mPotentialConfigData, TsInitializationException,
                "allocation error,", "Null pointer to PotentialConfigData.");
        /// - Throw an exception if null pointer to the Network's PotentialInputData.
        TS_PTCS_IF_ERREX(0 == mPotentialInputData, TsInitializationException,
                "allocation error,", "Null pointer to PotentialInputData.");

        /// - Initialize each Config and Input Data pointer to null.
        for(int i = 0; i < numLinksPot; ++i)
        {
            mPotentialConfigData[i] = 0;
            mPotentialInputData[i] = 0;
        }
    }
    /// - Basic Sources
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (numLinksSrc > 0)
    {
        /// - Allocate links array.
        TS_NEW_CLASS_ARRAY_EXT(mSources, numLinksSrc, GunnsThermalSource, (),
                               std::string(mName) + ".mSources");

        /// - Allocate ConfigData pointer array.
        mSourceConfigData = new GunnsThermalSourceConfigData*[numLinksSrc];

        /// - Allocate InputData pointer array.
        mSourceInputData = new GunnsThermalSourceInputData*[numLinksSrc];

        /// - Throw an exception if null pointer to the Network's Source Objects.
        TS_PTCS_IF_ERREX(0 == mSources, TsInitializationException,
                "allocation error,", "Null pointer to Sources.");
        /// - Throw an exception if null pointer to the Network's SourceConfigData.
        TS_PTCS_IF_ERREX(0 == mSourceConfigData, TsInitializationException,
                "allocation error,", "Null pointer to SourceConfigData.");
        /// - Throw an exception if null pointer to the Network's SourceInputData.
        TS_PTCS_IF_ERREX(0 == mSourceInputData, TsInitializationException,
                "allocation error,", "Null pointer to SourceInputData.");

        /// - Initialize each Config and Input Data pointer to null.
        for(int i = 0; i < numLinksSrc; ++i)
        {
            mSourceConfigData[i] = 0;
            mSourceInputData[i] = 0;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds config/input data arrays for each link-type. The data stored in the network's
///           ThermFileParser object is passed into the constructor of each link's
///           config or input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::buildConfig(const std::string& networkName)
{
    /// - Build Configuration and Input data for Capacitance links
    ////////////////////////////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < numLinksCap; ++i)
    {
        /// - Read data from the parser's vectors.
        std::stringstream is;
        is << i;
        const std::string name = networkName + ".mCapacitanceLinks_" + is.str() + "_"
                               + parser.vCapNames.at(i);
        const double cap       = parser.vCapCapacitances.at(i);
        const double temp      = parser.vCapTemperatures.at(i);
        const int groupId      = parser.vCapEditGroupIdentifiers.at(i);

        /// - Construct ConfigData.
        mCapacitanceConfigData[i] = new GunnsThermalCapacitorConfigData(
                                  name,       /// name of Gunns link
                                  &netNodeList, /// nodes list for this network
                                  groupId);   /// identifier for capacitance edit grouping
        /// - Construct InputData.
        mCapacitanceInputData[i] = new GunnsThermalCapacitorInputData(
                                  false,      /// malfBlockageFlag
                                  0.0,        /// malfBlockageValue
                                  cap,        /// capacitance
                                  temp);      /// potential
    }
    /// - Build Configuration and Input data for Conduction links
    ////////////////////////////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < numLinksCond; ++i)
    {
        /// - Read data from the parser's vectors.
        std::stringstream is;
        is << i;
        const std::string name = networkName + ".mConductionLinks_" + is.str() + "_"
                               + parser.vCondNames.at(i);
        const double cond      = parser.vCondConductivities.at(i);

        /// - Construct ConfigData.
        mConductionConfigData[i] = new GunnsBasicConductorConfigData(
                                  name,       /// name of Gunns link
                                  &netNodeList, /// nodes list for this network
                                  cond);      /// conductivity
        /// - Construct InputData.
        mConductionInputData[i] = new GunnsBasicConductorInputData(
                                  false,      /// malfBlockageFlag
                                  0.0);       /// malfBlockageValue
    }
    /// - Build Configuration and Input data for Radiation links
    ////////////////////////////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < numLinksRad; ++i)
    {
        /// - Read data from the parser's vectors.
        std::stringstream is;
        is << i;
        const std::string name = networkName + ".mRadiationLinks_" + is.str() + "_"
                               + parser.vRadNames.at(i);
        const double cond      = parser.vRadCoefficients.at(i) * UnitConversion::STEFAN_BOLTZMANN_CONST_SI;

        /// - Construct ConfigData.
        mRadiationConfigData[i] = new GunnsThermalRadiationConfigData(
                                  name,       /// name of Gunns link
                                  &netNodeList, /// nodes list for this network
                                  cond);      /// conductivity

        /// - If the optional radiation thermal input XML file has been used then it has read view
        ///   scalars into a vector.  Otherwise use a view scalar of 1.  This vector will throw an
        ///   exception if we try to read from it beyond its size.
        double scalar = 1.0;
        if (parser.vRadViewScalars.size() > 0) {
            scalar = parser.vRadViewScalars.at(i);
        }

        /// - Construct InputData.
        mRadiationInputData[i] = new GunnsThermalRadiationInputData(
                                  false,      /// malfBlockageFlag
                                  0.0,        /// malfBlockageValue
                                  scalar);    /// viewScalar
    }
    /// - Build Configuration and Input data for heaters
    ////////////////////////////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < numLinksHtr; ++i)
    {
        /// - Read data from the parser's vectors.
        std::stringstream is;
        is << i;
        const std::string name     = networkName + ".mHeaters_" + is.str() + "_"
                                   + parser.vHtrNames.at(i);
        const double scalar        =  parser.vHtrScalars.at(i);
        std::vector<double>* fracs = &parser.vHtrFracs.at(i);

        /// - Construct ConfigData.
        mHeaterConfigData[i] = new GunnsThermalHeaterConfigData(
                             name,          /// name of Gunns link of Gunns link
                             &netNodeList,    /// nodes list for this network
                             scalar,        /// heater electrical-to-thermal tuning scalar
                             fracs);        /// fraction of power distributed to each node
        /// - Construct InputData.
        mHeaterInputData[i] = new GunnsThermalHeaterInputData(
                                  false,      /// malfBlockageFlag
                                  0.0,        /// malfBlockageValue
                                  0.0);       /// demandedFlux
    }
    /// - Build Configuration and Input data for ThermalPanels
    ////////////////////////////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < numLinksPan; ++i)
    {
        /// - Read data from the parser's vectors.
        std::stringstream is;
        is << i;
        const std::string name     = networkName + ".mPanels_" + is.str() + "_"
                                   + parser.vPanNames.at(i);
        std::vector<double>* fracs = &parser.vPanFracs.at(i);
        const double scalar        = 1.0;
        const double alpha         = parser.vPanAbsorptivities.at(i);
        const double area          = parser.vPanAreas.at(i);

        /// - Construct ConfigData.
        mPanelConfigData[i] = new GunnsThermalPanelConfigData(
                                  name,       /// name of Gunns link
                                  &netNodeList, /// nodes list for this network
                                  scalar,     /// tuning scalar, defaulted to 1.0
                                  fracs,      /// fraction of power distributed to each node
                                  alpha,      /// absorptivity
                                  area);      /// surface area (m2)
        /// - Construct InputData.
        mPanelInputData[i] = new GunnsThermalPanelInputData(
                                  false,  /// malfBlockageFlag
                                  0.0,    /// malfBlockageValue
                                  0.0,    /// initial demandedFlux
                                  false,  /// malfFluxOverrideFlag
                                  0.0);   /// malfFluxOverrideValue
    }
    /// - Build Configuration and Input data for Potential links
    ////////////////////////////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < numLinksPot; ++i)
    {
        /// - Read data from the parser's vectors.
        std::stringstream is;
        is << i;
        const std::string name = networkName + ".mPotentialLinks_" + is.str() + "_"
                               + parser.vPotNames.at(i);
        const double temp      = parser.vPotTemperatures.at(i);
        const double cond      = parser.vPotConductivities.at(i);

        /// - Construct ConfigData.
        mPotentialConfigData[i] = new GunnsThermalPotentialConfigData(
                                  name,       /// name of Gunns link
                                  &netNodeList, /// nodes list
                                  cond);      /// conductivity
        /// - Construct InputData.
        mPotentialInputData[i] = new GunnsThermalPotentialInputData(
                                  false,      /// malfBlockageFlag
                                  0.0,        /// malfBlockageValue
                                  temp);      /// sourcePotential, temperature the link will hold
    }
    /// - Build Configuration and Input data for BasicSources
    ////////////////////////////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < numLinksSrc; ++i)
    {
        /// - Read data from the parser's vectors.
        std::stringstream is;
        is << i;
        const std::string name     = networkName + ".mSources_" + is.str() + "_"
                                   + parser.vSrcNames.at(i);
        const double initValue     = parser.vSrcInits.at(i);
        const double scalar        = parser.vSrcScalars.at(i);
        std::vector<double>* fracs = &parser.vSrcFracs.at(i);

        /// - Construct ConfigData.
        mSourceConfigData[i] = new GunnsThermalSourceConfigData(
                                name,          /// name of Gunns link of Gunns link
                                &netNodeList,    /// nodes list for this network
                                scalar,        /// tuning scalar
                                fracs);        /// fraction of power distributed to each node
        /// - Construct InputData.
        mSourceInputData[i] = new GunnsThermalSourceInputData(
                                false,      /// malfBlockageFlag
                                0.0,        /// malfBlockageValue
                                initValue); /// initial demandedFlux
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initializes arrays for links of each type. A Gunns object is created for every link
///           listed in the config-files. Each object is then initialized with its appropriate
///           port numbers and config/input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::buildLinks()
{
    /// - Call initialize() on each Capacitance Link.
    buildLinkType("Capacitance", numLinksCap, &ThermalNetwork::initializeCap);

    /// - Call initialize() on each Conduction Link.
    buildLinkType("Conduction", numLinksCond, &ThermalNetwork::initializeCond);

    /// - Call initialize() on each Radiation Link.
    buildLinkType("Radiation", numLinksRad, &ThermalNetwork::initializeRad);

    /// - Call initialize() on each Heater.
    buildLinkType("Heater", numLinksHtr, &ThermalNetwork::initializeHtr);

    /// - Call initialize() on each Panel.
    buildLinkType("Panel", numLinksPan, &ThermalNetwork::initializePan);

    /// - Call initialize() on each Potential Link.
    buildLinkType("Potential", numLinksPot, &ThermalNetwork::initializePot);

    /// - Call initialize() on each Source.
    buildLinkType("Source", numLinksSrc, &ThermalNetwork::initializeSrc);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  linkType  (--)  name of the given link-type
/// @param[in]  total     (--)  total number of links in the network of the given type
/// @param[in]  pf        (--)  A pointer to a void function that takes an int. Used to represent
///                             one of the seven initializeLink() methods.
///
/// @details  Uses a function pointer to call the initialize sub-function for each link type.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::buildLinkType(const char* linkType,             // name of link type
                                   const int total,                  // number of links of this type
                                   void (ThermalNetwork::*pf) (int)  // pointer to member function
                                  )
{
    /// - Loop through the given number of links.
    for(int i = 0; i < total; ++i)
    {
        try
        {
            /// - Call the method provided by the function-pointer "pf".
            ///   This function is of the form: void initialize________Link(int i).
            (this->*pf)(i);

        } catch (TsInitializationException& e)
        {
            /// - Send warning report on link initialization failure.
            TS_PTCS_WARNING(linkType << " link " << i << ": Exception while initializing.");
        }
    }

}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  i  (--)  index of specific link within Capacitance arrays
///
/// @details  Sub-function to initialize a specific Capacitance link
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::initializeCap(int i)
{
    /// - Get the node number of the link's port0.
    const int port0 = parser.vCapPorts.at(i) + netSuperNodesOffset;

    /// - Capacitance links have a port1 always at SPACE (the very last node).
    const int port1 = indexSpaceNode;

    /// - Initialize the link with its corresponding data.
    mCapacitanceLinks[i].initialize(*mCapacitanceConfigData[i],
                                    *mCapacitanceInputData[i],
                                    netLinks,
                                    port0,
                                    port1);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  i  (--)  index of specific link within Conduction arrays
///
/// @details  Sub-function to initialize a specific Conduction link
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::initializeCond(int i)
{
    /// - Get the node number of the link's port0 and port1.
    const int port0 = parser.vCondPorts0.at(i) + netSuperNodesOffset;
    const int port1 = parser.vCondPorts1.at(i) + netSuperNodesOffset;

    /// - Initialize the link with its corresponding data.
    mConductionLinks[i].initialize(*mConductionConfigData[i],
                                   *mConductionInputData[i],
                                   netLinks,
                                   port0,
                                   port1);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  i  (--)  index of specific link within Radiation arrays
///
/// @details  Sub-function to initialize a specific Radiation link
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::initializeRad(int i)
{
    /// - Get the node number of the link's port0 and port1.
    const int port0 = parser.vRadPorts0.at(i) + netSuperNodesOffset;
    const int port1 = parser.vRadPorts1.at(i) + netSuperNodesOffset;

    /// - Initialize the link with its corresponding data.
    mRadiationLinks[i].initialize(*mRadiationConfigData[i],
                                  *mRadiationInputData[i],
                                  netLinks,
                                  port0,
                                  port1);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  i  (--)  index of specific link within Heater arrays
///
/// @details  Sub-function to initialize a specific Heater link. As an argument to its initialize()
///           method, a GunnsThermalHeater takes a pointer to a vector of ints. This vector should
///           contain the numbers of all the nodes to which the heater is connected. The size of
///           the vector should match the size of the power-draw fraction loaded into config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::initializeHtr(int i)
{
    /// - Create a temporary ports vector from the heater port numbers at index i.
    std::vector<int>* ports = &parser.vHtrPorts.at(i);
    for (unsigned int j=0; j<parser.vHtrPorts.at(i).size(); ++j) {
        parser.vHtrPorts.at(i).at(j) += netSuperNodesOffset;
    }

    /// -Initialize the link with its corresponding data.
    mHeaters[i].initialize(*mHeaterConfigData[i],
                           *mHeaterInputData[i],
                           netLinks,
                           ports);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  i  (--)  index of specific link within Panel arrays
///
/// @details  Sub-function to initialize a specific Panel link
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::initializePan(int i)
{
    /// - Create a temporary ports vector from the panel port numbers at index i.
    std::vector<int>* ports = &parser.vPanPorts.at(i);
    for (unsigned int j=0; j<parser.vPanPorts.at(i).size(); ++j) {
        parser.vPanPorts.at(i).at(j) += netSuperNodesOffset;
    }

    /// - Initialize the link with its corresponding data.
    mPanels[i].initialize(*mPanelConfigData[i],
                          *mPanelInputData[i],
                          netLinks,
                          ports);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  i  (--)  index of specific link within Potential arrays
///
/// @details  Sub-function to initialize a specific Potential link
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::initializePot(int i)
{
    /// - Potential links have a port0 always at SPACE (the very last node).
    const int port0 = indexSpaceNode;

    /// - Get the node number of the link's port1.
    const int port1 = parser.vPotPorts.at(i) + netSuperNodesOffset;

    /// - Initialize the link with its corresponding data.
    mPotentialLinks[i].initialize(*mPotentialConfigData[i],
                                  *mPotentialInputData[i],
                                  netLinks,
                                  port0,
                                  port1);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  i  (--)  index of specific link within Source arrays
///
/// @details  Sub-function to initialize a specific Source link
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::initializeSrc(int i)
{
    /// - Create a temporary ports vector from the heater port numbers at index i.
    std::vector<int>* ports = &parser.vSrcPorts.at(i);
    for (unsigned int j=0; j<parser.vSrcPorts.at(i).size(); ++j) {
        parser.vSrcPorts.at(i).at(j) += netSuperNodesOffset;
    }

    /// - Initialize the link with its corresponding data.
    mSources[i].initialize(*mSourceConfigData[i],
                           *mSourceInputData[i],
                           netLinks,
                           ports);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throw    TsInitializationException if there is a null pointer
///
/// @details  Validates this Thermal Network initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::validate()
{
    /// - Throw an exception if numberOf nodes is not > 0.
    TS_PTCS_IF_ERREX(netNodeList.mNumNodes <= 0, TsInitializationException,
            "invalid init data,", "netNodeList.mNumNodes <= 0.");

    /// - Throw an exception if null pointer to the Basic Node object.
    TS_PTCS_IF_ERREX(0 == netNodeList.mNodes, TsInitializationException,
                "allocation error,", "Null pointer to BasicNode array.");

    /// - Throw an exception if numberOf nodes is not > 0.
    TS_PTCS_IF_ERREX(netNumLocalNodes <= 0, TsInitializationException,
            "invalid init data,", "netNumLocalNodes <= 0.");

    /// - Throw an exception if numCapEditGroups is not >= 0.
    TS_PTCS_IF_ERREX(numCapEditGroups < 0, TsInitializationException,
            "invalid init data,", "numCapEditGroups < 0.");
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Loops through each heater, setting its electrical power. If the HtrMiswire malf is
///           active, a heater might receive power from the incorrect electrical circuit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::setHeaterPowers()
{
    /// - Loop through each heater in the network.
    for(int i = 0; i < numLinksHtr; ++i)
    {
        /// - Default index
        int index = i;

        if (true == mMalfHtrMiswireFlag)
        {
            if (true == MsMath::isInRange(0, mMalfHtrIndexValue[i], numLinksHtr-1) )
            {
                /// - Overwrite index with the cross-up value from the mMalfHtrIndexValue array.
                index = mMalfHtrIndexValue[i];

            } else
            {
                TS_PTCS_WARNING("- Cannot re-wire heater to out-of-range index.");
                TS_PTCS_WARNING("- Request: " << mMalfHtrIndexValue[i]
                                  << ", must be between 0 and " << numLinksHtr-1);
                mMalfHtrIndexValue[i] = i;
            }
        }
        /// - Set the heater's electrical power.
        mHeaters[i].setPowerElectrical( mHtrPowerElectrical[index] );
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  For each capacitance edit group, detects a change in the capacitance scale factor
///           control, and for each capacitor link in that group, scales its capacitance by the
///           ratio of the new control value over the old value.  This allows entire groups of
///           capacitors to be edited simultaneously.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::editCapacitanceGroups()
{
    for (int group = 0; group < numCapEditGroups; ++group)
    {
        /// - Protect for divide by zeros below.
        mCapEditScaleFactor[group] = std::max(mCapEditScaleFactor[group], DBL_EPSILON);
        mCapEditScalePrev[group]   = std::max(mCapEditScalePrev[group],   DBL_EPSILON);

        if ( DBL_EPSILON < fabs(mCapEditScalePrev[group] - mCapEditScaleFactor[group]) )
        {
            const double ratio = mCapEditScaleFactor[group] / mCapEditScalePrev[group];
            mCapEditScalePrev[group] = mCapEditScaleFactor[group];

            applyCapacitanceGroups(group, ratio);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  For each capacitance edit group, revert the applicable capacitance links back to
///           default capacitances prior to loading checkpoint.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::preloadCapacitanceGroups()
{
    for (int group = 0; group < numCapEditGroups; ++group)
    {
        /// - Protect for divide by zeros below.
        mCapEditScaleFactor[group] = std::max(mCapEditScaleFactor[group], DBL_EPSILON);
        /// - Reverse the current scale
        const double ratio = 1.0 / mCapEditScaleFactor[group];
        /// - Apply ratio
        applyCapacitanceGroups(group, ratio);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  For each capacitance edit group, Apply current scale factor to capacitive links
///           after loading checkpoint.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::restartCapacitanceGroups()
{
    for (int group = 0; group < numCapEditGroups; ++group)
    {
        /// - Scale factor cannot be zero.
        mCapEditScaleFactor[group] = std::max(mCapEditScaleFactor[group], DBL_EPSILON);
        /// - Apply capacitance scale
        applyCapacitanceGroups(group, mCapEditScaleFactor[group]);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  For the given capacitance edit group, apply capacitance ratio to capacitance of
///           applicable links
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::applyCapacitanceGroups(int group, double ratio)
{
        for (int i = 0; i < numLinksCap; ++i)
        {
            if (mCapacitanceLinks[i].getEditCapacitanceGroup() == group)
            {
                /// - Apply ratio to capacitance
                mCapacitanceLinks[i].setCapacitance(ratio * mCapacitanceLinks[i].getCapacitance());
            }
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes allocated arrays and config/input objects
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::cleanUp()
{
    /// - Delete Heaters.
    for(int i = numLinksHtr-1; i >= 0; --i)
    {
        delete mHeaterInputData[i];
        mHeaterInputData[i] = 0;
        delete mHeaterConfigData[i];
        mHeaterConfigData[i] = 0;
    }
    TS_DELETE_ARRAY(mMalfHtrIndexValue);
    TS_DELETE_ARRAY(mHtrPowerElectrical);
    delete[] mHeaterInputData;
    mHeaterInputData = 0;
    delete[] mHeaterConfigData;
    mHeaterConfigData = 0;
    TS_DELETE_ARRAY(mHeaters);

    /// - Delete Panels.
    for(int i = numLinksPan-1; i >= 0; --i)
    {
        delete mPanelInputData[i];
        mPanelInputData[i] = 0;
        delete mPanelConfigData[i];
        mPanelConfigData[i] = 0;
    }
    delete[] mPanelInputData;
    mPanelInputData = 0;
    delete[] mPanelConfigData;
    mPanelConfigData = 0;
    TS_DELETE_ARRAY(mPanels);

    /// - Delete Sources.
    for(int i = numLinksSrc-1; i >= 0; --i)
    {
        delete mSourceInputData[i];
        mSourceInputData[i] = 0;
        delete mSourceConfigData[i];
        mSourceConfigData[i] = 0;
    }
    delete[] mSourceInputData;
    mSourceInputData = 0;
    delete[] mSourceConfigData;
    mSourceConfigData = 0;
    TS_DELETE_ARRAY(mSources);

    /// - Delete Potential links.
    for(int i = numLinksPot-1; i >= 0; --i)
    {
        delete mPotentialInputData[i];
        mPotentialInputData[i] = 0;
        delete mPotentialConfigData[i];
        mPotentialConfigData[i] = 0;
    }
    delete[] mPotentialInputData;
    mPotentialInputData = 0;
    delete[] mPotentialConfigData;
    mPotentialConfigData = 0;
    TS_DELETE_ARRAY(mPotentialLinks);

    /// - Delete Radiation links.
    for(int i = numLinksRad-1; i >= 0; --i)
    {
        delete mRadiationInputData[i];
        mRadiationInputData[i] = 0;
        delete mRadiationConfigData[i];
        mRadiationConfigData[i] = 0;
    }
    delete[] mRadiationInputData;
    mRadiationInputData = 0;
    delete[] mRadiationConfigData;
    mRadiationConfigData = 0;
    TS_DELETE_ARRAY(mRadiationLinks);

    /// - Delete Conduction links.
    for(int i = numLinksCond-1; i >= 0 ; --i)
    {
        delete mConductionInputData[i];
        mConductionInputData[i] = 0;
        delete mConductionConfigData[i];
        mConductionConfigData[i] = 0;
    }
    delete[] mConductionInputData;
    mConductionInputData = 0;
    delete[] mConductionConfigData;
    mConductionConfigData = 0;
    TS_DELETE_ARRAY(mConductionLinks);

    /// - Delete Capacitance links.
    TS_DELETE_ARRAY(mCapEditScalePrev);
    TS_DELETE_ARRAY(mCapEditScaleFactor);
    for(int i = numLinksCap-1; i >= 0; --i)
    {
        delete mCapacitanceInputData[i];
        mCapacitanceInputData[i] = 0;
        delete mCapacitanceConfigData[i];
        mCapacitanceConfigData[i] = 0;
    }
    delete[] mCapacitanceInputData;
    mCapacitanceInputData = 0;
    delete[] mCapacitanceConfigData;
    mCapacitanceConfigData = 0;
    TS_DELETE_ARRAY(mCapacitanceLinks);

    /// - Delete nodes pointer.
    TS_DELETE_ARRAY(pNodes);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
/// @param[in] index (--) Array of heater wiring indices during active malfunction.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermalNetwork::setMalfHtrMiswire(const bool flag, const int* index)
{
    mMalfHtrMiswireFlag = flag;

    if (index) {
        /// - If an array pointer supplied, copy the supplied array values into our index array.
        for(int i = 0; i < numLinksHtr; ++i)
        {
            mMalfHtrIndexValue[i] = index[i];
        }
    } else {
        /// - If the array pointer is NULL, restore our malf index array to the normal sequence.
        for(int i = 0; i < numLinksHtr; ++i)
        {
            mMalfHtrIndexValue[i] = i;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsParseException
///
/// @details  This calls the file parser to do a pre-count of the number of nodes, stores it in
///           the base network's number of nodes, and finally returns the value.
////////////////////////////////////////////////////////////////////////////////////////////////////
int ThermalNetwork::getNumLocalNodes()
{
    parser.mNodeFile = mConfig.cNodeFile;
    parser.preCountNodes();
    netNumLocalNodes = parser.numNodes;
    return netNumLocalNodes;
}
