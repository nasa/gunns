/**
@file  DrawElect.cpp
@brief DrawElect GUNNS Basic Network implementation.

@copyright  Copyright 2022 United States Government as represented by the Administrator of the
            National Aeronautics and Space Administration. All Rights Reserved.

LIBRARY DEPENDENCY:
(
  (core/GunnsSensorAnalogWrapper.o)
  (aspects/electrical/Batt/GunnsElectBattery.o)
  (aspects/electrical/ConstantPowerLoad/EpsConstantPowerLoad.o)
  (aspects/electrical/Converter/GunnsElectConverterInput.o)
  (aspects/electrical/Converter/GunnsElectConverterOutput.o)
  (aspects/electrical/IPS/GunnsElectIps.o)
  (aspects/electrical/Switch/GunnsElectSelector.o)
  (aspects/electrical/resistive/GunnsElectShort.o)
  (core/GunnsBasicCapacitor.o)
  (core/GunnsBasicConductor.o)
  (core/GunnsBasicPotential.o)
  (core/network/GunnsNetworkBase.o)
)

PROGRAMMERS:
  ((Auto-generated by the GunnsDraw netexport script version 19.4.0) (2023-05-04 05:29:41.381618))
*/

#include "DrawElect.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

// Tables data
const double DrawElect::batterySocVocSoC[3] = {0.0, 0.5, 1.0};
const double DrawElect::batterySocVocVoc[3] = {0.0, 100.0, 120.0};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name    (--) Name of the network for H&S messages.
/// @param[in] network (--) Pointer to the main network object.
///
/// @details  Default constructs the DrawElect Network Config Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawElectConfigData::DrawElectConfigData(const std::string& name, DrawElect* network)
    :
    netSolver(name + ".netSolver", 0.001, 0.001, 20, 10),
    // Spotter Config Data
    converterSenseVout(name + ".converterSenseVout", 0.0, 200.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0, UnitConversion::NO_CONVERSION, 0.0),
    converterSenseIout(name + ".converterSenseIout", 0.0, 200.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0, UnitConversion::NO_CONVERSION, 0.0),
    converterSenseVin(name + ".converterSenseVin", 0.0, 200.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0, UnitConversion::NO_CONVERSION, 0.0),
    // Link Config Data
    resistor_1(name + ".resistor_1", &network->netNodeList, 0.1),
    capacitor(name + ".capacitor", &network->netNodeList),
    selector(name + ".selector", &network->netNodeList, 1.0, 0),
    battery(name + ".battery", &network->netNodeList, 1, false, 0.01, 0.01, 1.0, &network->batterySocVoc),
    load_3(name + ".load_3", &network->netNodeList, 7.5e-7),
    load_4(name + ".load_4", &network->netNodeList, 1.0e-7),
    testIps(name + ".testIps", &network->netNodeList, false, 25.0, 0.0, 0.0, 56.0, 0.0, 0.5, 1.0, 21.0, 135.0, 1.0e-15, 1, 1, false, 1.0e-4),
    load_2(name + ".load_2", &network->netNodeList, 0.0, 25.0, 50.0),
    short1(name + ".short1", &network->netNodeList),
    converterIn(name + ".converterIn", &network->netNodeList, &network->converterSenseVin, 0, 3, 100.0, 130.0, 0),
    converterOut(name + ".converterOut", &network->netNodeList, GunnsElectConverterOutput::VOLTAGE, 100.0, 0.99, &network->converterSenseVout, &network->converterSenseIout, 2, 122.0, 5.0, &network->converterIn, false, 0.0),
    load_1(name + ".load_1", &network->netNodeList, 1.0e-6)
{
    // Load config data vectors
    {
        const std::string array[] = {"load_1", "load_2"};
        selector.mPortLabels.assign(array, array + sizeof(array) / sizeof(std::string));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs the DrawElect Network Config Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawElectConfigData::~DrawElectConfigData()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] network (--) Pointer to the main network object.
///
/// @details  Default constructs the DrawElect Network Input Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawElectInputData::DrawElectInputData(DrawElect* network)
    :
    // Spotter Input Data
    converterSenseVout(true, 0.0),
    converterSenseIout(true, 0.0),
    converterSenseVin(true, 0.0),
    // Link Input Data
    resistor_1(false, 0.0),
    capacitor(false, 0.0, 1.0e-4, 100.0),
    selector(false, 0.0, 1),
    battery(false, 0.0, 1.0, false, 0.0, 0.0),
    load_3(false, 0.0),
    load_4(false, 0.0),
    testIps(false, 0.0),
    load_2(false, 0.0),
    short1(false, 0.0, 0.0, false, 0.0, false, 0.0, false, 0.0, false, 0.0, true, 1.0e8),
    converterIn(false, 0.0, false, 0.0, 0.0, 0.0),
    converterOut(false, 0.0, false, 0.0, 0.0, 100.0),
    load_1(false, 0.0, 0.0)
{
    // Load input data vectors
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs the DrawElect Network Input Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawElectInputData::~DrawElectInputData()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name (--) Name of the network for H&S messages.
///
/// @details  Default constructs the DrawElect Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawElect::DrawElect(const std::string& name)
    :
    GunnsNetworkBase(name, N_NODES, netNodes),
    netNodes(),
    netConfig(name, this),
    netInput(this),
    // Data Tables
    batterySocVoc(batterySocVocSoC, batterySocVocVoc, 3, 0.0, 1.0),
    // Spotters
    converterSenseVout(),
    converterSenseIout(),
    converterSenseVin(),
    // Links
    resistor_1(),
    capacitor(),
    selector(),
    battery(),
    load_3(),
    load_4(),
    testIps(),
    load_2(),
    short1(),
    converterIn(),
    converterOut(),
    load_1()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Default destructs the DrawElect Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
DrawElect::~DrawElect()
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
void DrawElect::initNodes(const std::string& name)
{
    /// - Initialize the nodes.
    netNodeList.mNodes[0 + netSuperNodesOffset].initialize(name + createNodeName(0 + netSuperNodesOffset), 0.0);
    netNodeList.mNodes[1 + netSuperNodesOffset].initialize(name + createNodeName(1 + netSuperNodesOffset), 0.0);
    netNodeList.mNodes[2 + netSuperNodesOffset].initialize(name + createNodeName(2 + netSuperNodesOffset), 0.0);
    netNodeList.mNodes[3 + netSuperNodesOffset].initialize(name + createNodeName(3 + netSuperNodesOffset), 0.0);
    netNodeList.mNodes[4 + netSuperNodesOffset].initialize(name + createNodeName(4 + netSuperNodesOffset), 0.0);
    netNodeList.mNodes[5 + netSuperNodesOffset].initialize(name + createNodeName(5 + netSuperNodesOffset), 0.0);
    netNodeList.mNodes[6 + netSuperNodesOffset].initialize(name + createNodeName(6 + netSuperNodesOffset), 0.0);
    /// - Only init the Ground node if this is not a sub-network.
    if (!netIsSubNetwork) {
        netNodeList.mNodes[7].initialize(name + ".GROUND");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initializes this network's links, spotters and solver with their config and input data
///           objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawElect::initNetwork()
{
    const int groundOffset = netNodeList.mNumNodes - N_NODES;

    /// - Register sockets with the jumper plugs.

    /// - Initialize the links.
    netLinks.clear();
    resistor_1.initialize(netConfig.resistor_1, netInput.resistor_1, netLinks, Node0 + netSuperNodesOffset, Node1 + netSuperNodesOffset);
    capacitor.initialize(netConfig.capacitor, netInput.capacitor, netLinks, Node2 + netSuperNodesOffset, GROUND + groundOffset);
    {
        const int array[] = {Node2 + netSuperNodesOffset, Node3 + netSuperNodesOffset, Node4 + netSuperNodesOffset, Node5 + netSuperNodesOffset, Node6 + netSuperNodesOffset};
        std::vector<int> vector (array, array + sizeof(array) / sizeof(int));
        selector.initialize(netConfig.selector, netInput.selector, netLinks, &vector);
    }
    battery.initialize(netConfig.battery, netInput.battery, netLinks, GROUND + groundOffset, Node0 + netSuperNodesOffset);
    load_3.initialize(netConfig.load_3, netInput.load_3, netLinks, Node5 + netSuperNodesOffset, GROUND + groundOffset);
    load_4.initialize(netConfig.load_4, netInput.load_4, netLinks, Node6 + netSuperNodesOffset, GROUND + groundOffset);
    {
        const int array[] = {Node5 + netSuperNodesOffset, Node6 + netSuperNodesOffset};
        std::vector<int> vector (array, array + sizeof(array) / sizeof(int));
        testIps.initialize(netConfig.testIps, netInput.testIps, netLinks, &vector);
    }
    load_2.initialize(netConfig.load_2, netInput.load_2, netLinks, Node4 + netSuperNodesOffset, GROUND + groundOffset);
    short1.initialize(netConfig.short1, netInput.short1, netLinks, Node0 + netSuperNodesOffset, Node5 + netSuperNodesOffset);
    converterIn.initialize(netConfig.converterIn, netInput.converterIn, netLinks, Node1 + netSuperNodesOffset);
    converterOut.initialize(netConfig.converterOut, netInput.converterOut, netLinks, Node2 + netSuperNodesOffset);
    load_1.initialize(netConfig.load_1, netInput.load_1, netLinks, Node3 + netSuperNodesOffset, GROUND + groundOffset);

    /// - Initialize the spotters.
    converterSenseVout.initialize(&netConfig.converterSenseVout, &netInput.converterSenseVout);
    converterSenseIout.initialize(&netConfig.converterSenseIout, &netInput.converterSenseIout);
    converterSenseVin.initialize(&netConfig.converterSenseVin, &netInput.converterSenseVin);

    /// - Initialize the solver, only if this is not a sub-network.
    if (!netIsSubNetwork) {
        netSolver.initializeNodes(netNodeList);
        netSolver.initialize(netConfig.netSolver, netLinks);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] timeStep (s) Integration time step.
///
/// @details  Updates this network's spotters before solving the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawElect::stepSpottersPre(const double timeStep)
{
    /// - Step network spotters prior to solver step.
    converterSenseVout.stepPreSolver(timeStep);
    converterSenseIout.stepPreSolver(timeStep);
    converterSenseVin.stepPreSolver(timeStep);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   timeStep (s) Integration time step.
///
/// @details  Updates this network's spotters after solving the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawElect::stepSpottersPost(const double timeStep)
{
    /// - Step network spotters after solver step.
    converterSenseVout.stepPostSolver(timeStep);
    converterSenseIout.stepPostSolver(timeStep);
    converterSenseVin.stepPostSolver(timeStep);
}
