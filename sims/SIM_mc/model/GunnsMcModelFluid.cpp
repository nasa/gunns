/**
@file  GunnsMcModelFluid.cpp
@brief GunnsMcModelFluid GUNNS Fluid Network implementation.

@copyright  Copyright 2023 United States Government as represented by the Administrator of the
            National Aeronautics and Space Administration. All Rights Reserved.

LIBRARY DEPENDENCY:
(
  (aspects/fluid/conductor/GunnsFluidValve.o)
  (core/GunnsFluidConductor.o)
  (core/GunnsFluidPotential.o)
  (core/network/GunnsNetworkBase.o)
)

PROGRAMMERS:
  ((Auto-generated by the GunnsDraw netexport script version 19.3.8) (2023-01-23 12:06:03.586420))
*/

#include "GunnsMcModelFluid.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

/// @details  Loads constituent fluid types into the network's internal fluid types array.
FluidProperties::FluidType GunnsMcModelFluidConfigData::netInternalFluidTypes[GunnsMcModelFluidConfigData::N_INTERNAL_FLUIDS] =
{
    FluidProperties::GUNNS_N2,
    FluidProperties::GUNNS_O2
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name    (--) Name of the network for H&S messages.
/// @param[in] network (--) Pointer to the main network object.
/// 
/// @details  Default constructs the GunnsMcModelFluid Network Config Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMcModelFluidConfigData::GunnsMcModelFluidConfigData(const std::string& name, GunnsMcModelFluid* network)
    :
    netSolver(name + ".netSolver", 0.001, 1.0e-8, 1, 1),
    // Spotter Config Data
    // Link Config Data
    pressureSource(name + ".pressureSource", &network->netNodeList, 1.0e6, 0.0),
    conductor1(name + ".conductor1", &network->netNodeList, 0.001, 0.0),
    conductor2(name + ".conductor2", &network->netNodeList, 0.002, 0.0),
    valve1(name + ".valve1", &network->netNodeList, 0.0005, 0.0, 0.0, 0.0, 0.0),
    valve2(name + ".valve2", &network->netNodeList, 0.00025, 0.0, 0.0, 0.0, 0.0)
{
    // Load config data vectors
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs the GunnsMcModelFluid Network Config Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMcModelFluidConfigData::~GunnsMcModelFluidConfigData()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] network (--) Pointer to the main network object.
///
/// @details  Default constructs the GunnsMcModelFluid Network Input Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMcModelFluidInputData::GunnsMcModelFluidInputData(GunnsMcModelFluid* network)
    :
    fluidDefaultFractions(0.8, 0.2),
    fluidDefault(294.261, 101.325, 0.0, 0.0, fluidDefaultFractions.array, 0),
    // Spotter Input Data
    // Link Input Data
    pressureSource(false, 0.0, 101.325),
    conductor1(false, 0.0),
    conductor2(false, 0.0),
    valve1(false, 0.0, 1.0, false, 0.0, 0.0),
    valve2(false, 0.0, 1.0, false, 0.0, 0.0)
{
    // Load input data vectors
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs the GunnsMcModelFluid Network Input Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMcModelFluidInputData::~GunnsMcModelFluidInputData()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name (--) Name of the network for H&S messages.
///
/// @details  Default constructs the GunnsMcModelFluid Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMcModelFluid::GunnsMcModelFluid(const std::string& name)
    :
    GunnsNetworkBase(name, N_NODES, netNodes),
    netNodes(),
    netConfig(name, this),
    netInput(this),
    netFluidProperties(),
    netInternalFluidConfig(&netFluidProperties, netConfig.netInternalFluidTypes, GunnsMcModelFluidConfigData::N_INTERNAL_FLUIDS),
    // Data Tables 
    // Spotters
    // Links
    pressureSource(),
    conductor1(),
    conductor2(),
    valve1(),
    valve2()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Default destructs the GunnsMcModelFluid Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMcModelFluid::~GunnsMcModelFluid()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name (--) Name of the network for H&S messages.
///
/// @details  Initializes the nodes with their config and input data objects.  The nodes are
///           initialized through the node list object, which may point to this network's nodes
///           (when this is a standalone network), or the super-network's nodes (as a sub-network).
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMcModelFluid::initNodes(const std::string& name)
{
    /// - Initialize the nodes.
    GunnsFluidNode* nodes = static_cast<GunnsFluidNode*>(netNodeList.mNodes);
    nodes[0 + netSuperNodesOffset].initialize(name + createNodeName(0 + netSuperNodesOffset), &netInternalFluidConfig, &netInput.fluidDefault);
    nodes[1 + netSuperNodesOffset].initialize(name + createNodeName(1 + netSuperNodesOffset), &netInternalFluidConfig, &netInput.fluidDefault);
    nodes[2 + netSuperNodesOffset].initialize(name + createNodeName(2 + netSuperNodesOffset), &netInternalFluidConfig, &netInput.fluidDefault);
    /// - Only init the Ground node if this is not a sub-network.
    if (!netIsSubNetwork) {
        nodes[3].initialize(name + ".GROUND", &netInternalFluidConfig, 0);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initializes this network's links, spotters and solver with their config and input data
///           objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMcModelFluid::initNetwork()
{
    const int groundOffset = netNodeList.mNumNodes - N_NODES;

    /// - Register sockets with the jumper plugs.

    /// - Initialize the links.
    netLinks.clear();
    pressureSource.initialize(netConfig.pressureSource, netInput.pressureSource, netLinks, GROUND + groundOffset, Node0 + netSuperNodesOffset);
    conductor1.initialize(netConfig.conductor1, netInput.conductor1, netLinks, Node0 + netSuperNodesOffset, Node1 + netSuperNodesOffset);
    conductor2.initialize(netConfig.conductor2, netInput.conductor2, netLinks, Node0 + netSuperNodesOffset, Node1 + netSuperNodesOffset);
    valve1.initialize(netConfig.valve1, netInput.valve1, netLinks, Node1 + netSuperNodesOffset, Node2 + netSuperNodesOffset);
    valve2.initialize(netConfig.valve2, netInput.valve2, netLinks, Node2 + netSuperNodesOffset, GROUND + groundOffset);

    /// - Initialize the spotters.

    /// - Initialize the solver, only if this is not a sub-network.
    if (!netIsSubNetwork) {
        netSolver.initializeFluidNodes(netNodeList);
        netSolver.initialize(netConfig.netSolver, netLinks);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] timeStep (s) Integration time step.
///
/// @details  Updates this network's spotters before solving the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMcModelFluid::stepSpottersPre(const double timeStep)
{
    /// - Step network spotters prior to solver step.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   timeStep (s) Integration time step.
///
/// @details  Updates this network's spotters after solving the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMcModelFluid::stepSpottersPost(const double timeStep)
{
    /// - Step network spotters after solver step.
}
