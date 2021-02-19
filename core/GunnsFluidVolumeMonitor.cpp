/**
@file
@brief     GUNNS Fluid Volume Monitor Spotter implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((GunnsNetworkSpotter.o)
   (core/GunnsFluidNode.o)
   (core/GunnsFluidLink.o)
   (simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o))
*/

#include "GunnsFluidVolumeMonitor.hh"
#include "GunnsFluidNode.hh"
#include "GunnsFluidLink.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name  (--)  Instance name for self-identification in messages.
///
/// @details  Default constructs this GUNNS Fluid Volume Monitor Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidVolumeMonitorConfigData::GunnsFluidVolumeMonitorConfigData(const std::string& name)
    :
    GunnsNetworkSpotterConfigData(name)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Volume Monitor Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidVolumeMonitorConfigData::~GunnsFluidVolumeMonitorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  linkPort (--) Which of the link's port nodes to monitor.
///
/// @details  Default constructs this GUNNS Fluid Volume Monitor Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidVolumeMonitorInputData::GunnsFluidVolumeMonitorInputData(const int linkPort)
    :
    mLinkPort(linkPort)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Volume Monitor Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidVolumeMonitorInputData::~GunnsFluidVolumeMonitorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  nodeList  (--)  Reference to the network node list.
/// @param[in]  link      (--)  Reference to the link of interest.
///
/// @details Default constructs this GUNNS Fluid Volume Monitor Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidVolumeMonitor::GunnsFluidVolumeMonitor(const GunnsNodeList&  nodeList,
                                                 const GunnsFluidLink& link)
    :
    GunnsNetworkSpotter  (),
    mNodeList            (nodeList),
    mLink                (link),
    mLinkPort            (0),
    mNumFluidConstituents(0),
    mNodeMass            (0.0),
    mNodeMassFractions   (0),
    mNodeMoleFractions   (0),
    mNodeVolume          (0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Volume Monitor Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidVolumeMonitor::~GunnsFluidVolumeMonitor()
{
    cleanup();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes allocated memory objects
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidVolumeMonitor::cleanup()
{
    TS_DELETE_ARRAY(mNodeMoleFractions);
    TS_DELETE_ARRAY(mNodeMassFractions);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
/// @param[in]  inputData   (--)  Instance input data.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Volume Monitor Spotter with its configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidVolumeMonitor::initialize(const GunnsNetworkSpotterConfigData* configData,
                                         const GunnsNetworkSpotterInputData*  inputData)
{
    /// - Initialize the base class.
    GunnsNetworkSpotter::initialize(configData, inputData);

    /// - Reset the init flag.
    mInitFlag = false;

    /// - Validate & initialize from config & input data.
    validateConfig(configData);
    const GunnsFluidVolumeMonitorInputData* input = validateInput(inputData);
    mLinkPort = input->mLinkPort;

    /// - Allocate dynamic memory.
    mNumFluidConstituents = mLink.getNodeContent(0)->getNConstituents();
    TS_NEW_PRIM_ARRAY_EXT(mNodeMassFractions, mNumFluidConstituents, double, configData->mName + ".mNodeMassFractions");
    TS_NEW_PRIM_ARRAY_EXT(mNodeMoleFractions, mNumFluidConstituents, double, configData->mName + ".mNodeMoleFractions");

    /// - Step to initialize the outputs.
    stepPostSolver(0.0);

    /// - Set the init flag.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
///
/// @returns  GunnsFluidVolumeMonitorConfigData (--) Type-casted and validated config data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base config data class pointer to this spotter's config data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsFluidVolumeMonitorConfigData* GunnsFluidVolumeMonitor::validateConfig(const GunnsNetworkSpotterConfigData* config)
{
    const GunnsFluidVolumeMonitorConfigData* result = dynamic_cast<const GunnsFluidVolumeMonitorConfigData*>(config);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Bad config data pointer type.");
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputData  (--)  Instance input data.
///
/// @returns  GunnsFluidVolumeMonitorInputData (--) Type-casted and validated input data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base input data class pointer to this spotter's input data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsFluidVolumeMonitorInputData* GunnsFluidVolumeMonitor::validateInput(const GunnsNetworkSpotterInputData* input)
{
    const GunnsFluidVolumeMonitorInputData* result = dynamic_cast<const GunnsFluidVolumeMonitorInputData*>(input);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Bad input data pointer type.");
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (not used).
///
/// @details  This method does nothing because this spotter has no function prior to the network
///           solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidVolumeMonitor::stepPreSolver(const double dt __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (not used).
///
/// @details  Copies desired properties from the node.  This is done post-solution so we'll see the
///           most recent balanced node properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidVolumeMonitor::stepPostSolver(const double dt __attribute__((unused)))
{
    /// - Limit the user-supplied link port number to the link's valid range.
    mLinkPort = MsMath::limitRange(0, mLinkPort, mLink.getNumberPorts()-1);

    /// - Set up pointers for speed.
    GunnsFluidNode*  nodes = static_cast<GunnsFluidNode*>(mNodeList.mNodes);
    GunnsFluidNode*  node  = &nodes[mLink.getNodeMap()[mLinkPort]];
    const PolyFluid* fluid = node->getContent();

    /// - Copy the node properties.
    for (int i = 0; i < mNumFluidConstituents; ++i) {
        const FluidProperties::FluidType type = fluid->getType(i);
        mNodeMassFractions[i] = fluid->getMassFraction(type);
        mNodeMoleFractions[i] = fluid->getMoleFraction(type);
    }
    mNodeMass = node->getMass();
    mNodeVolume = node->getVolume();

    if (fluid->getTraceCompounds()) {
        mNodeTcMoleFractions = fluid->getTraceCompounds()->getMoleFractions();
    }
}
