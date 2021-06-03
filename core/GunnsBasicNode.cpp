/**
@file
@brief    GUNNS Basic Node implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Source File for GUNNS Basic Node)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((software/exceptions/TsInitializationException.o))

PROGRAMMERS:
   (
    (Jason Harvey) (L3) (2011-02) (Initial Prototype))
    (Eric Ferguson)(L3) (2012-02) (Updates and Maintenance))
   )
*/

#include "GunnsBasicNode.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] numNodes (--) Number of nodes in the array
/// @param[in] nodes    (--) The nodes array
///
/// @details  Default constructs the Gunns node list structure
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsNodeList::GunnsNodeList(int             numNodes,
                             GunnsBasicNode* nodes)
    :
    mNumNodes (numNodes),
    mNodes    (nodes)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the basic node.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicNode::GunnsBasicNode()
    :
    mName                     (),
    mPotential                (0.0),
    mInfluxRate               (0.0),
    mOutfluxRate              (0.0),
    mNetFlux                  (0.0),
    mFluxThrough              (0.0),
    mNetworkCapacitance       (0.0),
    mNetworkCapacitanceRequest(0.0),
    mScheduledOutflux         (0.0),
    mIslandVector             (0),
    mNetCapDeltaPotential     (0),
    mInitFlag                 (false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the basic node.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicNode::~GunnsBasicNode()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name       (--) Node name
/// @param[in]     potential  (--) Initial potential
///
/// @throws   TsInitializationException
///
/// @details  Initializes this basic node with its name and initial potential.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicNode::initialize(const std::string& name, const double potential)
{
    /// - Validate and initialize node name from the name argument.
    GUNNS_NAME_ERREX("GunnsBasicNode", name);

    /// - Initialize state data.
    mPotential                 = potential;
    mInfluxRate                = 0.0;
    mOutfluxRate               = 0.0;
    mNetFlux                   = 0.0;
    mFluxThrough               = 0.0;
    mNetworkCapacitance        = 0.0;
    mNetworkCapacitanceRequest = 0.0;
    mScheduledOutflux          = 0.0;
    mIslandVector              = 0;
    mNetCapDeltaPotential      = 0;
    mInitFlag                  = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicNode::restart()
{
    /// - Reset non-config & non-checkpointed class attributes.
    mInfluxRate                = 0.0;
    mOutfluxRate               = 0.0;
    mNetFlux                   = 0.0;
    mFluxThrough               = 0.0;
    mNetworkCapacitanceRequest = 0.0;
    mScheduledOutflux          = 0.0;
    mIslandVector              = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Validates this Gunns Basic Node initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicNode::validate()
{
    // Reset init flag.
    mInitFlag = false;

    /// - There is currently nothing to validate.

    // Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method calculates the net and through flux terms for model insight.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicNode::integrateFlows(const double)
{
    mNetFlux = mInfluxRate - mOutfluxRate;

    if (mInfluxRate < mOutfluxRate) {
        mFluxThrough = mInfluxRate;
    }
    else {
        mFluxThrough = mOutfluxRate;
    }
}
