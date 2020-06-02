/**
@file
@brief     GUNNS Fluid Super-Network implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o)
   (core/GunnsFluidNode.o)
   (core/network/GunnsSuperNetworkBase.o)
   (core/network/GunnsNetworkBase.o))
*/

#include "GunnsFluidSuperNetwork.hh"
#include "core/GunnsBasicNode.hh"
#include "core/network/GunnsNetworkBase.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name (--) Name of this instance for H&S messages.
///
/// @details  Default constructs this GUNNS Fluid Super-Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSuperNetwork::GunnsFluidSuperNetwork(const std::string& name)
    :
    GunnsSuperNetworkBase(name),
    netNodes(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Super-Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSuperNetwork::~GunnsFluidSuperNetwork()
{
    freeNodes();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Allocates the dynamic array of fluid nodes for the super-network, registers the
///           memory with Trick, and sets the super-network node list nodes pointer to this array.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSuperNetwork::allocateNodes()
{
    TS_NEW_CLASS_ARRAY_EXT(netNodes, netNodeList.mNumNodes, GunnsFluidNode, (), mName + ".netNodes");
    netNodeList.mNodes = netNodes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes the netNodes array.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSuperNetwork::freeNodes()
{
    TS_DELETE_ARRAY(netNodes);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  True if the nodes array is NULL, false otherwise.
///
/// @details  Returns whether the nodes array has been allocated.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidSuperNetwork::isNullNodes() const
{
    return (0 == netNodes);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Checks all fluid sub-networks are using identical fluid configurations, and
///           initializes the super-network fluid Ground node to NULL fluid state (vacuum).
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSuperNetwork::initGroundNode()
{
    const PolyFluidConfigData* config = mSubnets[0]->getFluidConfig();
    for (unsigned int i=1; i<mSubnets.size(); ++i) {
        if (!compareFluidConfigs(config, mSubnets[i]->getFluidConfig())) {
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "sub-network" + mSubnets[i]->getName() + " does not have an identical fluid config.");
        }
    }

    netNodes[netNodeList.mNumNodes-1].initialize(mName + ".VACUUM", config, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initializes the super-network GUNNS solver with the fluid nodes array.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSuperNetwork::initSolver()
{
    netSolver.initializeFluidNodes(netNodeList);
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
GunnsNetworkBase* GunnsFluidSuperNetwork::createJointNetwork(const int type __attribute__((unused)))
{
    GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
            "this class cannot create sub-network joints.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This implementation does nothing since this class doesn't create any joint networks to
///           register.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSuperNetwork::registerJoints()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  config1 (--) Pointer to the first fluid configuration data object.
/// @param[in]  config2 (--) Pointer to the second fluid configuration data object.
///
/// @returns  bool (--) True if the two configs are identical, false otherwise.
///
/// @details  Checks all of the properties of the given PolyFluidConfigData except for their
///           mProperties pointer.  If any of the other properties are different, returns false,
///           otherwise true.
///
///           We considered implementing this as overriding PolyFluidConfigData's comparison
///           operator==, but in our case we don't want to compare its mProperties member and that
///           would leave the comparison incomplete from PolyFluid's perspective.
///
///           The caller must ensure neither argument is NULL.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidSuperNetwork::compareFluidConfigs(const PolyFluidConfigData* config1,
                                                 const PolyFluidConfigData* config2) const
{
    /// - Fail if the PolyFluidConfigData have different number of fluid types.
    if (config1->mNTypes != config2->mNTypes) {
        return false;
    }

    for (int i=0; i<config1->mNTypes; ++i) {
        /// - Fail if fluid types don't match in the same order.
        if (config1->mTypes[i] != config2->mTypes[i]) {
            return false;
        }
    }

    /// - Trace Compounds are optional, so fail if only one is present.
    if ( (0 != config1->mTraceCompounds) xor (0 != config2->mTraceCompounds) ) {
        return false;
    }

    if (config1->mTraceCompounds and config2->mTraceCompounds) {
        /// - Fail if there are different number of TC types.
        if (config1->mTraceCompounds->mNTypes != config2->mTraceCompounds->mNTypes) {
            return false;
        }

        /// - Fail if TC types don't match and in the same order.
        for (int i=0; i<config1->mTraceCompounds->mNTypes; ++i) {
            if (*(config1->mTraceCompounds->mCompounds[i]) !=
                *(config2->mTraceCompounds->mCompounds[i])) {
                return false;
            }
        }
    }

    return true;
}
