/**
@file
@brief    GUNNS Fluid Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Source File for GUNNS Fluid Link)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
    (
     (GunnsBasicLink.o)
     (GunnsFluidNode.o)
     (software/exceptions/TsInitializationException.o)
     (software/exceptions/TsOutOfBoundsException.o)
    )

 PROGRAMMERS:
    (
     (Jason Harvey) (L3) (2011-02) (Initial Prototype))
     (Kevin Supak)  (L3) (2011-02) (Updated to Coding Standards))
    )
*/

#include <cmath>
#include <cfloat>

#include "GunnsFluidLink.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name     (--) Link name
/// @param[in] nodeList (--) Network nodes structure
///
/// @details  Default constructs this Fluid Link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLinkConfigData::GunnsFluidLinkConfigData(const std::string& name,
                                                   GunnsNodeList*     nodeList)
    :
    GunnsBasicLinkConfigData(name, nodeList)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Fluid Link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLinkConfigData::GunnsFluidLinkConfigData(const GunnsFluidLinkConfigData& that)
    :
    GunnsBasicLinkConfigData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLinkConfigData::~GunnsFluidLinkConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
///
/// @details  Default constructs this Fluid Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLinkInputData::GunnsFluidLinkInputData(const bool   malfBlockageFlag,
                                                 const double malfBlockageValue)
    :
    GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Fluid Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLinkInputData::GunnsFluidLinkInputData(const GunnsFluidLinkInputData& that)
    :
    GunnsBasicLinkInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLinkInputData::~GunnsFluidLinkInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] numPorts (--) The number of ports the link has
///
/// @details  Constructs the Fluid Link
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLink::GunnsFluidLink(const int numPorts):
    GunnsBasicLink(numPorts),
    mInternalFluid(0),
    mFlowRate(0.0),
    mVolFlowRate(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Fluid Link
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLink::~GunnsFluidLink()
{
    TS_DELETE_OBJECT(mInternalFluid);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData (--) Configuration data
/// @param[in,out] inputData  (--) Input data
/// @param[in]     links      (--) Link vector
/// @param[in]     ports      (--) Port map indices
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Fluid link with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLink::initialize(const GunnsFluidLinkConfigData& configData,
                                const GunnsFluidLinkInputData&  inputData,
                                std::vector<GunnsBasicLink*>&   links,
                                const int*                      ports)
{
    /// - First initialize and validate parent.
    GunnsBasicLink::initialize(configData, inputData, links, ports);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Initialize state data.
    mFlowRate    = 0.0;
    mVolFlowRate = 0.0;

    /// - Validate child initialization.
    validate();

    /// - Set initialization status flag on successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Validates this Fluid link initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLink::validate() const
{
    /// - There is currently nothing to validate.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLink::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] forcedOutflow (--) Indicates that this flow is leaving the node, but changes the
///                               mixture of the node (by only flowing out certain constituents), so
///                               this flow must be collected as an influx with negative flowrate,
///                               rather than a normal outflux.  Normal outfluxes cannot change the
///                               node mixture.
/// @param[in] fromPort      (--) The port the fluid is transported out of (defaults to 0)
/// @param[in] toPort        (--) The port the fluid is transported into (default to 1)
///
/// @details  This function adds the link's flow to the node's collection terms.
///
///           Flow is defined positive going from the fromPort to the toPort, so a positive flow
///           will add mass to port 1 and remove from port 0 (using default port arguments).
///
///           If mInternalFluid is not NULL, then this link is intended to modify the fluid as it
///           passes through, so we pass this fluid's address to the receiving node, as
///           mInternalFluid represents the exit properties of the link.  If mInternalFluid is NULL,
///           then we just pass the source node's fluid to the receiving node.
///
/// @note     No flux is transported if any of the ports don't exist.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLink::transportFluid(const bool forcedOutflow,
                                    const int  fromPort,
                                    const int  toPort)
{
    if (checkValidPortNumber(toPort) && checkValidPortNumber(fromPort)) {

        /// - If the link has declared that it modifies the fluid passing through it, then pass its
        ///   properties to the downstream node.
        if (mInternalFluid) {

            /// - Minimum mass flow rate for transport.  Flow rates in links below this value are
            ///   not transported between nodes.  Math faults in the PolyFluid class can result when
            ///   mass or molar flow rates less than DBL_EPSILON are attempted, and this limit's
            ///   value keeps both mass and molar flow rates above DBL_EPSILON.
            if (mFlowRate > m100EpsilonLimit) {

                // Remove the flow from the fromPort.  If this flow is designated by the caller as a
                // forced outflow, then collect it as an influx with negative flow rate.  Otherwise,
                // it is a normal outflow.
                if (forcedOutflow) {
                    mNodes[fromPort]->collectInflux(-mFlowRate, mInternalFluid);
                } else {
                    mNodes[fromPort]->collectOutflux(mFlowRate);
                }

                // Add the flow to the toPort.
                mNodes[toPort]->collectInflux(mFlowRate, mInternalFluid);

            } else if(mFlowRate < -m100EpsilonLimit) {

                // Remove the flow from toPort.
                if (forcedOutflow) {
                    mNodes[toPort]->collectInflux(mFlowRate, mInternalFluid);
                } else {
                    mNodes[toPort]->collectOutflux(-mFlowRate);
                }

                // Add the flow to fromPort.
                mNodes[fromPort]->collectInflux(-mFlowRate, mInternalFluid);
            }

        /// - The link doesn't use the internal fluid, so pass the source node's fluid to the
        ///   receiving node based on flow direction.
        } else if (mFlowRate > m100EpsilonLimit) {
            mNodes[fromPort]->collectOutflux(mFlowRate);
            mNodes[toPort]->collectInflux(mFlowRate,
                    mNodes[determineSourcePort(mFlowRate, fromPort, toPort)]->getOutflow());

        } else if (mFlowRate < -m100EpsilonLimit) {
            mNodes[toPort]->collectOutflux(-mFlowRate);
            mNodes[fromPort]->collectInflux(-mFlowRate,
                    mNodes[determineSourcePort(mFlowRate, fromPort, toPort)]->getOutflow());
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws  TsInitializationException
///
/// @details  This method creates the link's internal fluid by copy-constructing one of the attached
///           node's content fluid.  However, we can't copy-construct the Vacuum node's contents,
///           so this method will throw an exception if all of the link ports are mapped to the
///           Vacuum node or otherwise have not been mapped.  This means that to create an internal
///           fluid, the link must be mapped to at least one non-Vacuum node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLink::createInternalFluid()
{
    /// - Find a port that doesn't have the vacuum node on it.  We can't copy-construct the vacuum
    ///   node's fluid because it has zero temperature, which MonoFluid::validate doesn't like.
    int port = mNumPorts - 1;
    for (int i = 0; i < mNumPorts; ++i) {
        if (mNodeMap[i] < mNodeMap[port]) {
            port = i;
        }
    }

    /// - Throw an exception if all ports are on the vacuum node or invalid node #'s
    if (mNodeMap[port] >= getGroundNodeIndex()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Link has all nodes uninitialized or vacuum.");
    }

    /// - Copy construct the internal fluid with the valid port node's outflow fluid.  Zero out the
    ///   flow rate just in case the node's fluid content has a garbage flow rate in it.  If the
    ///   fluid already exists, clean up the lower-level dynamically allocated arrays and delete
    ///   before allocating.
    if (mInternalFluid) {
        mInternalFluid->cleanup();
        TS_DELETE_OBJECT(mInternalFluid);
    }
    TS_NEW_PRIM_OBJECT_EXT(mInternalFluid, PolyFluid,
                           (*(mNodes[port]->getOutflow()), std::string(mName) + ".mInternalFluid"),
                           std::string(mName) + ".mInternalFluid");
    mInternalFluid->setFlowRate(0.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] initialFluidState (--) The desired initial properties of the internal fluid
///
/// @throws  TsInitializationException
///
/// @details  This overloaded method creates the link's internal fluid and then initializes its
///           properties with the given fluid input data.  The createInternalFluid() method is used,
///           so its port mapping limitations also apply.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLink::createInternalFluid(const PolyFluidInputData& initialFluidState)
{
    /// - Create the internal fluid using the default method, which copies the fluid properties of
    ///   the source node.  Then, set the fluid properties to the given input data.
    createInternalFluid();
    mInternalFluid->setPressure(initialFluidState.mPressure);
    mInternalFluid->setFlowRate(initialFluidState.mFlowRate);
    mInternalFluid->setMassAndMassFractions(initialFluidState.mMass,
            initialFluidState.mMassFraction);
    mInternalFluid->setTemperature(initialFluidState.mTemperature);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) Link port number to access
///
/// @return  PolyFluid* Returns a pointer to the specified port node content fluid, or NULL if the
///                     port is invalid
///
/// @throws  TsOutOfBoundsException
///
/// @details Returns a pointer to the port Node fluid content.  If the user calls an out of index
///          value it will throw a TsOutOfBoundsException.
////////////////////////////////////////////////////////////////////////////////////////////////////
const PolyFluid* GunnsFluidLink::getNodeContent(const int port) const
{
    const PolyFluid* result = 0;

    if (port >= 0 && port < mNumPorts) {
        result = mNodes[port]->getContent();
    }
    else {
        GUNNS_ERROR(TsOutOfBoundsException, "Input Argument Out Of Range",
                    "Link requesting node content on invalid port.");
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port   (--) The port to be updated
///
/// @details  Sets the port node pointer to the address of the node object number from the network
///           node list indicated by the port's node map.  This ensures the node pointed to always
///           matches the node map value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLink::updateNodePointer(const int port)
{
    GunnsFluidNode* fluidNodes = static_cast<GunnsFluidNode*> (mNodeList->mNodes);
    mNodes[port] = &fluidNodes[mNodeMap[port]];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flow     (--) Mass flow or flux rate, only the sign (direction) is used
/// @param[in] fromPort (--) Link port that positive flow flows from
/// @param[in] toPort   (--) Link port that positive flow flows to
///
/// @returns   int      (--) Port whose node should supply the fluid properties for the flow,
///                          defaults to fromPort if flow = 0.
///
/// @details  Normally the node actually supplying the flow should supply its fluid properties as
///           the properties of the flow.  However if that node is the ground node, it has no valid
///           fluid properties, so instead use the other port's node.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsFluidLink::determineSourcePort(const double flow,
                                        const int fromPort,
                                        const int toPort) const
{
    int result = fromPort;

    if ( (flow < 0.0 && mNodeMap[toPort]   != getGroundNodeIndex()) ||
         (flow > 0.0 && mNodeMap[fromPort] == getGroundNodeIndex()) ) {
        result = toPort;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (unused).
///
/// @throws   TsInitializationException
///
/// @details  This default implementation throws an exception, as all derived fluid links should
///           override this.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLink::transportFlows(const double dt __attribute__((unused)))
{
    GUNNS_ERROR(TsInitializationException, "Invalid Implementation",
                "class has not implemented this method.");
}
