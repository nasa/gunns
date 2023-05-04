/**
@file  DrawFluidExtras.cpp
@brief DrawFluidExtras GUNNS Fluid Network implementation.

@copyright  Copyright 2019 United States Government as represented by the Administrator of the
            National Aeronautics and Space Administration. All Rights Reserved.

LIBRARY DEPENDENCY:
(
  (core/GunnsFluidVolumeMonitor.o)
  (aspects/fluid/source/GunnsFluidReactor.o)
  (core/GunnsFluidCapacitor.o)
  (core/GunnsFluidJumper.o)
  (core/GunnsFluidPotential.o)
  (core/GunnsFluidSocket.o)
  (gunns/GunnsFluidMetabolic4.o)
  (core/GunnsFluidJumperPlug.o)
  (properties/ChemicalReaction.o)
  (properties/ChemicalCompound.o)
  (core/network/GunnsNetworkBase.o)
)

PROGRAMMERS:
  ((Auto-generated by the GunnsDraw netexport script version 19.4.0) (2023-05-04 05:30:09.351365))
*/

#include "DrawFluidExtras.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

/// @details  Loads constituent fluid types into the network's internal fluid types array.
FluidProperties::FluidType DrawFluidExtrasConfigData::netInternalFluidTypes[DrawFluidExtrasConfigData::N_INTERNAL_FLUIDS] =
{
    FluidProperties::GUNNS_N2,
    FluidProperties::GUNNS_O2,
    FluidProperties::GUNNS_H2O,
    FluidProperties::GUNNS_CO2,
    FluidProperties::GUNNS_CH4,
    FluidProperties::GUNNS_H2
};

/// @details  Loads chemical compounds types into the trace compounds config array.
ChemicalCompound::Type DrawFluidExtrasConfigData::netTcConfigTypes[DrawFluidExtrasConfigData::N_NETTCCONFIG] =
{
    ChemicalCompound::NH3,
    ChemicalCompound::CO
};

/// @details  Loads chemical reaction types into the reactor chemical reactions array.
ChemicalReaction::Type DrawFluidExtrasConfigData::reactorReactions[DrawFluidExtrasConfigData::N_REACTORREACTIONS] =
{
    ChemicalReaction::H2_REMOVAL,
    ChemicalReaction::CH4_REMOVAL
};

/// @details  Loads chemical compound types into the reactor chemical compounds array.
ChemicalCompound::Type DrawFluidExtrasConfigData::reactorCompounds[DrawFluidExtrasConfigData::N_REACTORCOMPOUNDS] =
{
    ChemicalCompound::CH4,
    ChemicalCompound::O2,
    ChemicalCompound::CO2,
    ChemicalCompound::H2,
    ChemicalCompound::H2O
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name    (--) Name of the network for H&S messages.
/// @param[in] network (--) Pointer to the main network object.
/// 
/// @details  Default constructs the DrawFluidExtras Network Config Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawFluidExtrasConfigData::DrawFluidExtrasConfigData(const std::string& name, DrawFluidExtras* network)
    :
    netTcConfig(netTcConfigTypes, DrawFluidExtrasConfigData::N_NETTCCONFIG, name + ".netTcConfig"),
    netReactions(),
    netCompounds(),
    netSolver(name + ".netSolver", 0.001, 1.0e-6, 1, 1),
    // Spotter Config Data
    tankVolumeMonitor(name + ".tankVolumeMonitor"),
    // Link Config Data
    ambient(name + ".ambient", &network->netNodeList, 1.0, 0.0),
    tank(name + ".tank", &network->netNodeList, 0.0),
    reactor(name + ".reactor", &network->netNodeList, 1.0e-6, 0.0, &netReactions, reactorReactions, 2, &netCompounds, reactorCompounds, 5, 0.0, 0.0, 0.0),
    socket1(name + ".socket1", &network->netNodeList, 1.0e-6, 0.0, true),
    socket2(name + ".socket2", &network->netNodeList, 1.0e-6, 0.0, true),
    crew(name + ".crew", &network->netNodeList),
    jumper(name + ".jumper", &network->netNodeList, 1.0e-6, 0.0, 0, &network->jumperPlug1, false, true)
{
    // Load config data vectors
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs the DrawFluidExtras Network Config Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawFluidExtrasConfigData::~DrawFluidExtrasConfigData()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] network (--) Pointer to the main network object.
///
/// @details  Default constructs the DrawFluidExtras Network Input Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawFluidExtrasInputData::DrawFluidExtrasInputData(DrawFluidExtras* network)
    :
    tcDefaultValues(5.0e-7, 1.0e-6),
    tcDefault(tcDefaultValues.array),
    tcAirValues(1.0e-6, 5.0e-7),
    tcAir(tcAirValues.array),
    fluidDefaultFractions(0.8, 0.2, 0.0, 0.0, 0.0, 0.0),
    fluidDefault(294.261, 101.325, 0.0, 0.0, fluidDefaultFractions.array, &tcDefault),
    fluidAirFractions(0.79, 0.2, 0.006, 0.002, 0.001, 0.001),
    fluidAir(294.261, 101.325, 0.0, 0.0, fluidAirFractions.array, &tcAir),
    reactorCompoundsMasses(0.0, 0.0, 0.0, 0.0, 0.0),
    // Spotter Input Data
    tankVolumeMonitor(0),
    // Link Input Data
    ambient(false, 0.0, 10.0),
    tank(false, 0.0, 1.0, &fluidAir),
    reactor(false, 0.0, reactorCompoundsMasses.array, 294.261),
    socket1(false, 0.0, false, 0.0),
    socket2(false, 0.0, false, 0.0),
    crew(false, 0.0, 0.0, 0.0, 0.0, 0.0),
    jumper(false, 0.0, false, 0.0, -1, DrawFluidTc_sockets::NONE)
{
    // Load input data vectors
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs the DrawFluidExtras Network Input Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawFluidExtrasInputData::~DrawFluidExtrasInputData()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name (--) Name of the network for H&S messages.
///
/// @details  Default constructs the DrawFluidExtras Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawFluidExtras::DrawFluidExtras(const std::string& name)
    :
    GunnsNetworkBase(name, N_NODES, netNodes),
    netNodes(),
    netConfig(name, this),
    netInput(this),
    netFluidProperties(),
    netInternalFluidConfig(&netFluidProperties, netConfig.netInternalFluidTypes, DrawFluidExtrasConfigData::N_INTERNAL_FLUIDS, &netConfig.netTcConfig),
    // Data Tables 
    // Spotters
    tankVolumeMonitor(netNodeList, tank),
    // Jumper Plugs
    jumperPlug1(name + ".jumperPlug1"),
    // Links
    ambient(),
    tank(),
    reactor(),
    socket1(),
    socket2(),
    crew(),
    jumper()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Default destructs the DrawFluidExtras Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawFluidExtras::~DrawFluidExtras()
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
void DrawFluidExtras::initNodes(const std::string& name)
{
    /// - Initialize the nodes.
    GunnsFluidNode* nodes = static_cast<GunnsFluidNode*>(netNodeList.mNodes);
    nodes[0 + netSuperNodesOffset].initialize(name + createNodeName(0 + netSuperNodesOffset), &netInternalFluidConfig, &netInput.fluidAir);
    nodes[1 + netSuperNodesOffset].initialize(name + createNodeName(1 + netSuperNodesOffset), &netInternalFluidConfig, &netInput.fluidAir);
    nodes[2 + netSuperNodesOffset].initialize(name + createNodeName(2 + netSuperNodesOffset), &netInternalFluidConfig, &netInput.fluidAir);
    nodes[3 + netSuperNodesOffset].initialize(name + createNodeName(3 + netSuperNodesOffset), &netInternalFluidConfig, &netInput.fluidAir);
    /// - Only init the Ground node if this is not a sub-network.
    if (!netIsSubNetwork) {
        nodes[4].initialize(name + ".GROUND", &netInternalFluidConfig, 0);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initializes this network's links, spotters and solver with their config and input data
///           objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawFluidExtras::initNetwork()
{
    const int groundOffset = netNodeList.mNumNodes - N_NODES;

    /// - Register sockets with the jumper plugs.
    jumperPlug1.addSocket(socket1);
    jumperPlug1.addSocket(socket2);

    /// - Initialize the links.
    netLinks.clear();
    ambient.initialize(netConfig.ambient, netInput.ambient, netLinks, GROUND + groundOffset, Node1 + netSuperNodesOffset);
    tank.initialize(netConfig.tank, netInput.tank, netLinks, Node0 + netSuperNodesOffset, GROUND + groundOffset);
    reactor.initialize(netConfig.reactor, netInput.reactor, netLinks, Node0 + netSuperNodesOffset, Node1 + netSuperNodesOffset);
    socket1.initialize(netConfig.socket1, netInput.socket1, netLinks, Node2 + netSuperNodesOffset, GROUND + groundOffset);
    socket2.initialize(netConfig.socket2, netInput.socket2, netLinks, Node3 + netSuperNodesOffset, GROUND + groundOffset);
    crew.initialize(netConfig.crew, netInput.crew, netLinks, GROUND + groundOffset, Node3 + netSuperNodesOffset);
    jumper.initialize(netConfig.jumper, netInput.jumper, netLinks, Node0 + netSuperNodesOffset, GROUND + groundOffset);

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
void DrawFluidExtras::stepSpottersPre(const double timeStep)
{
    /// - Step network spotters prior to solver step.
    tankVolumeMonitor.stepPreSolver(timeStep);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   timeStep (s) Integration time step.
///
/// @details  Updates this network's spotters after solving the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawFluidExtras::stepSpottersPost(const double timeStep)
{
    /// - Step network spotters after solver step.
    tankVolumeMonitor.stepPostSolver(timeStep);
}
