/**
@file
@brief     GUNNS Base Network implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o)
   (core/Gunns.o))
*/

#include <sstream>
#include "GunnsNetworkBase.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name     (--) Name of this network instance for H&S messages.
/// @param[in]  numNodes (--) Number of node objects in this network.
/// @param[in]  nodes    (--) Pointer to the network nodes array.
///
/// @details  Default constructs this GUNNS Base Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsNetworkBase::GunnsNetworkBase(const std::string& name, int numNodes, GunnsBasicNode* nodes)
    :
    netSolver(),
    netNodeList(numNodes, nodes),
    mName(name),
    netLinks(),
    netIsSubNetwork(false),
    netSuperNodesOffset(0),
    netJointIndex(-1),
    netNumLocalNodes(numNodes),
    netSuperNetwork(0),
    netMutex(),
    netMutexEnabled(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Base Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsNetworkBase::~GunnsNetworkBase()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  offset       (--) The offset of this network drawing's nodes in the super-network.
/// @param[in]  superNetwork (--) Pointer to the super-network.
///
/// @details  This is useful as a reference for users to convert between the numbered nodes in the
///           drawing from which this network was generated and the nodes in the super-network.  It
///           is added to the default node assignments for the links initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsNetworkBase::setNodeOffset(const int offset, GunnsSuperNetworkBase* superNetwork)
{
    netSuperNodesOffset = offset;
    netSuperNetwork     = superNetwork;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  nodeList (--) The super-network's node list.
///
/// @details  This overrides this network's node list with the given super-network's node list and
///           sets a flag indicating that this network is now a sub-network in a super-network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsNetworkBase::setNodeList(GunnsNodeList& nodeList)
{
    // This is using the default assignment operator of the GunnsNodeList struct.
    netNodeList     = nodeList;
    netIsSubNetwork = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @returns  std::vector<GunnsBasicLink*>* (--) Pointer to this network's links vector.
///
/// @details  Returns the address of this network's links vector.  This should only be called by a
///           super-network after this sub-network has been initialized.  This method will throw an
///           exception if it hasn't been set as a sub network.
////////////////////////////////////////////////////////////////////////////////////////////////////
const std::vector<GunnsBasicLink*>* GunnsNetworkBase::getLinks() const
{
    if (!netIsSubNetwork) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data", "isn't a sub-network.");
    }
    return &netLinks;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name (--) Optional override value for the network name for backwards compatibility.
///
/// @details  Initializes this GUNNS Base Network.  This should be called by the Trick
///           initialization job.  The node, links, spotters & solver are initialized with their
///           config and input data objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsNetworkBase::initialize(const std::string& name)
{
    /// - This only functions when the network is a standalone network.  When this is a sub-network
    ///   in a super-network, initialization functions are called separately by the super so this
    ///   method does nothing.
    if (netIsSubNetwork) return;

    /// - Catch exceptions and send a fatal H&S error.
    try {
        if (name.empty()) {
            initNodes(mName);
        } else {
            initNodes(name);
        }
        initNetwork();
        pthread_mutex_init(&netMutex, NULL);

    } catch (TsInitializationException& e) {
        /// - Send an H&S fatal message and return on TsInitializationException
        TsHsMsg msg(TS_HS_FATAL, "GUNNS");
        msg << mName << " caught initialization exception." << '\n' << tsStackTrace();
        hsSendMsg(msg);
    } catch (const std::exception& e) {
        /// - Send an H&S fatal message and return on unexpected exception derived from std::exception.
        TsHsMsg msg(TS_HS_FATAL, "GUNNS");
        msg << mName << " caught unexpected exception." << '\n' << e.what() << tsStackTrace();
        hsSendMsg(msg);
    } catch (...) {
        /// - Send an h&s fatal message and return on unexpected exception not derived from std::exception.
        TsHsMsg msg(TS_HS_FATAL, "GUNNS");
        msg << mName << " caught unexpected exception." << '\n' << tsStackTrace();
        hsSendMsg(msg);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Restarts this GUNNS Base Network.  This should be called by the Trick restart job.
///           This is intended to be called after load from a checkpoint.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsNetworkBase::restart()
{
    /// - In a super-network, this method returns without doing anything, since the only thing
    ///   restarted is the solver, and this solver isn't used.
    if (netIsSubNetwork) return;

    /// - Catch exceptions and send a non-fatal H&S error.
    try {
        netSolver.restart();

    } catch (const std::exception& e) {
        /// - Send an H&S error message and return on unexpected exception derived from std::exception.
        TsHsMsg msg(TS_HS_ERROR, "GUNNS");
        msg << mName << " caught unexpected exception." << '\n' << e.what() << tsStackTrace();
        hsSendMsg(msg);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  timeStep (s) Integration time step.
///
/// @details  Propagates the network state in run-time.  This should be called by the Trick
///           scheduled job.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsNetworkBase::update(const double timeStep)
{
    /// - In a super-network, this method returns without doing anything, since the spotters are
    ///   already updated by the super-network, and this solver isn't used.
    if (netIsSubNetwork) return;

    if (netMutexEnabled) {
        pthread_mutex_lock(&netMutex);
    }

    /// - Catch exceptions and send a non-fatal H&S error.
    try {
        stepSpottersPre(timeStep);
        netSolver.step(timeStep);
        stepSpottersPost(timeStep);

    } catch (const std::exception& e) {
        /// - Send an H&S error message and return on unexpected exception derived from std::exception.
        TsHsMsg msg(TS_HS_ERROR, "GUNNS");
        msg << mName << " caught unexpected exception." << '\n' << e.what() << tsStackTrace();
        hsSendMsg(msg);
    } catch (...) {
        /// - Send an H&S error message and return on unexpected exception not derived from std::exception.
        TsHsMsg msg(TS_HS_ERROR, "GUNNS");
        msg << mName << " caught unexpected exception." << '\n' << tsStackTrace();
        hsSendMsg(msg);
    }

    pthread_mutex_unlock(&netMutex);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  node (--) The node's number.
///
/// @returns  std::string (--) The name string with node number appended.
///
/// @details  Returns ".Node_<node>" where <node> is the input argument converted to decimal form.
////////////////////////////////////////////////////////////////////////////////////////////////////
const std::string GunnsNetworkBase::createNodeName(const int node) const
{
    std::ostringstream stream;
    stream << ".Node_" << node;
    return stream.str();
}
