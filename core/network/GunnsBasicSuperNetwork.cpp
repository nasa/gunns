/**
@file
@brief     GUNNS Basic Super-Network implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o)
   (core/network/GunnsSuperNetworkBase.o)
   (core/GunnsBasicNode.o))
*/

#include "GunnsBasicSuperNetwork.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name (--) Name of this instance for H&S messages.
///
/// @details  Default constructs this GUNNS Basic Super-Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSuperNetwork::GunnsBasicSuperNetwork(const std::string& name)
    :
    GunnsSuperNetworkBase(name),
    netNodes(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Basic Super-Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSuperNetwork::~GunnsBasicSuperNetwork()
{
    freeNodes();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Allocates the dynamic array of basic nodes for the super-network, registers the
///           memory with Trick, and sets the super-network node list nodes pointer to this array.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicSuperNetwork::allocateNodes()
{
    TS_NEW_CLASS_ARRAY_EXT(netNodes, netNodeList.mNumNodes, GunnsBasicNode, (), mName + ".netNodes");
    netNodeList.mNodes = netNodes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes the netNodes array.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicSuperNetwork::freeNodes()
{
    TS_DELETE_ARRAY(netNodes);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  True if the nodes array is NULL, false otherwise.
///
/// @details  Returns whether the nodes array has been allocated.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsBasicSuperNetwork::isNullNodes() const
{
    return (0 == netNodes);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initializes the super-network basic Ground node to zero potential.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicSuperNetwork::initGroundNode()
{
    netNodes[netNodeList.mNumNodes-1].initialize(mName + ".GROUND", 0.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initializes the super-network GUNNS solver with the basic nodes array.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicSuperNetwork::initSolver()
{
    netSolver.initializeNodes(netNodeList);
    netSolver.initialize(netSolverConfig, mLinks);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type  (--)  Not used.
///
/// @throws   TsInitializationException
///
/// @details  This implementation just throws an exception.  It should never be called since this
///           class can't create a joint network.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsNetworkBase* GunnsBasicSuperNetwork::createJointNetwork(const int type __attribute__((unused)))
{
    GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
            "this class cannot create sub-network joints.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This implementation does nothing since this class doesn't create any joint networks to
///           register.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicSuperNetwork::registerJoints()
{
    // nothing to do
}
