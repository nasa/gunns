/**
@file  DrawFluid.cpp
@brief DrawFluid GUNNS Fluid Network implementation.

@copyright  Copyright 2019 United States Government as represented by the Administrator of the
            National Aeronautics and Space Administration. All Rights Reserved.

LIBRARY DEPENDENCY:
(
  (core/GunnsFluidVolumeMonitor.o)
  (core/GunnsFluidCapacitor.o)
  (core/GunnsFluidConductor.o)
  (core/GunnsFluidPotential.o)
  (core/network/GunnsNetworkBase.o)
)

PROGRAMMERS:
  ((Auto-generated by the GunnsDraw netexport script version 19.5.0) (2024-09-23 07:57:33.044271))
*/

#include "DrawFluid.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

/// @details  Loads constituent fluid types into the network's internal fluid types array.
FluidProperties::FluidType DrawFluidConfigData::netInternalFluidTypes[DrawFluidConfigData::N_INTERNAL_FLUIDS] =
{
    FluidProperties::GUNNS_N2,
    FluidProperties::GUNNS_O2,
    FluidProperties::GUNNS_WATER
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name    (--) Name of the network for H&S messages.
/// @param[in] network (--) Pointer to the main network object.
///
/// @details  Default constructs the DrawFluid Network Config Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawFluidConfigData::DrawFluidConfigData(const std::string& name, DrawFluid* network)
    :
    netSolver(name + ".netSolver", 0.001, 1.0e-6, 1, 1),
    // Spotter Config Data
    tankVolumeMonitor(name + ".tankVolumeMonitor"),
    // Link Config Data
    pipe(name + ".pipe", &network->netNodeList, 1.0e-6, 0.0),
    tank(name + ".tank", &network->netNodeList, 0.0),
    ambient(name + ".ambient", &network->netNodeList, 1.0, 0.0)
{
    // Load config data vectors
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs the DrawFluid Network Config Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawFluidConfigData::~DrawFluidConfigData()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] network (--) Pointer to the main network object.
///
/// @details  Default constructs the DrawFluid Network Input Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawFluidInputData::DrawFluidInputData(DrawFluid* network)
    :
    fluidWaterFractions(0.0, 0.0, 1.0),
    fluidWater(294.261, 101.325, 0.0, 0.0, fluidWaterFractions.array, 0),
    fluidAirFractions(0.8, 0.2, 0.0),
    fluidAir(294.261, 101.325, 0.0, 0.0, fluidAirFractions.array, 0),
    // Spotter Input Data
    tankVolumeMonitor(0),
    // Link Input Data
    pipe(false, 0.0),
    tank(false, 0.0, 1.0, &fluidAir),
    ambient(false, 0.0, 10.0)
{
    // Load input data vectors
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs the DrawFluid Network Input Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawFluidInputData::~DrawFluidInputData()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name (--) Name of the network for H&S messages.
///
/// @details  Default constructs the DrawFluid Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawFluid::DrawFluid(const std::string& name)
    :
    GunnsNetworkBase(name, N_NODES, netNodes),
    netNodes(),
    netConfig(name, this),
    netInput(this),
    netFluidProperties(),
    netInternalFluidConfig(&netFluidProperties, netConfig.netInternalFluidTypes, DrawFluidConfigData::N_INTERNAL_FLUIDS),
    // Data Tables
    // Spotters
    tankVolumeMonitor(netNodeList, tank),
    // Links
    pipe(),
    tank(),
    ambient()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Default destructs the DrawFluid Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawFluid::~DrawFluid()
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
void DrawFluid::initNodes(const std::string& name)
{
    /// - Initialize the nodes.
    GunnsFluidNode* nodes = static_cast<GunnsFluidNode*>(netNodeList.mNodes);
    nodes[0 + netSuperNodesOffset].initialize(name + createNodeName(0 + netSuperNodesOffset), &netInternalFluidConfig, &netInput.fluidAir);
    nodes[1 + netSuperNodesOffset].initialize(name + createNodeName(1 + netSuperNodesOffset), &netInternalFluidConfig, &netInput.fluidAir);
    /// - Only init the Ground node if this is not a sub-network.
    if (!netIsSubNetwork) {
        nodes[2].initialize(name + ".GROUND", &netInternalFluidConfig, 0);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initializes this network's links, spotters and solver with their config and input data
///           objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawFluid::initNetwork()
{
    const int groundOffset = netNodeList.mNumNodes - N_NODES;

    /// - Register sockets with the jumper plugs.

    /// - Initialize the links.
    netLinks.clear();
    pipe.initialize(netConfig.pipe, netInput.pipe, netLinks, Node0 + netSuperNodesOffset, Node1 + netSuperNodesOffset);
    tank.initialize(netConfig.tank, netInput.tank, netLinks, Node0 + netSuperNodesOffset, GROUND + groundOffset);
    ambient.initialize(netConfig.ambient, netInput.ambient, netLinks, GROUND + groundOffset, Node1 + netSuperNodesOffset);

    /// - Initialize the spotters.
    tankVolumeMonitor.initialize(&netConfig.tankVolumeMonitor, &netInput.tankVolumeMonitor);

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
void DrawFluid::stepSpottersPre(const double timeStep)
{
    /// - Step network spotters prior to solver step.
    tankVolumeMonitor.stepPreSolver(timeStep);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   timeStep (s) Integration time step.
///
/// @details  Updates this network's spotters after solving the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawFluid::stepSpottersPost(const double timeStep)
{
    /// - Step network spotters after solver step.
    tankVolumeMonitor.stepPostSolver(timeStep);
}
