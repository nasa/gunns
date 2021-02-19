/**
@file
@brief     GUNNS Super-Network Base implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o)
   (software/exceptions/TsOutOfBoundsException.o)
   (core/Gunns.o)
   (core/network/GunnsNetworkBase.o)
   (core/network/GunnsMultiLinkConnector.o)
   (core/GunnsBasicLink.o))
*/

#include "GunnsSuperNetworkBase.hh"
#include "core/network/GunnsNetworkBase.hh"
#include "core/network/GunnsMultiLinkConnector.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  net   (--)  Pointer to the joint network.
/// @param[in]  loc1  (--)  Pointer to the 1st location spotter.
/// @param[in]  loc2  (--)  Pointer to the 2nd location spotter.
///
/// @details  Default constructs this GUNNS Sub-Network Joint.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSubNetworkJoint::GunnsSubNetworkJoint(GunnsNetworkBase*        net,
                                           GunnsMultiLinkConnector* loc1,
                                           GunnsMultiLinkConnector* loc2)
    :
    network(net),
    location1(loc1),
    location2(loc2)
{
    if (network) {
        location1->setJointNetwork(network);
        location2->setJointNetwork(network);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Sub-Network Joint.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSubNetworkJoint::~GunnsSubNetworkJoint()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  offset  (--)  Offset of the joint network in the super-network nodes list.
///
/// @details  Calls each location spotter's connect method so they can make their various
///           connections to the joint network.  We pass the offset of the joint network's nodes in
///           the super-network, so that the connection spotter's connections can be defined
///           relative to the joint network and end up connecting to the correct super-nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSubNetworkJoint::join(const int offset)
{
    location1->connect(offset);
    location2->connect(offset);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name (--) Name of this instance for H&S messages.
///
/// @details  Default constructs this GUNNS Super-Network Base.  We default the solver config data
///           to values that will pass init, but the user should override these as needed via the
///           input file.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSuperNetworkBase::GunnsSuperNetworkBase(const std::string& name)
    :
    netSolver(),
    netSolverConfig(name + ".netSolver", 1.0e-6, 1.0e-6, 1, 1),
    netNodeList(),
    mName(name),
    mLinks(),
    mSubnets(),
    mJointNetworks(),
    mJoints(),
    mJointsIndeces(),
    numJoints(0),
    netMutex(),
    netMutexEnabled(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Super-Network Base.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsSuperNetworkBase::~GunnsSuperNetworkBase()
{
    for(unsigned int i=0; i<mJointNetworks.size(); ++i){
        delete mJointNetworks[i];
        mJointNetworks[i] = 0;
    }
    for(unsigned int i=0; i<mJoints.size(); ++i){
        delete mJoints[i];
        mJoints[i] = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  network (--) Address of a GunnsNetworkBase network.
///
/// @details  This should be called from the input file for each sub-network to be added to this
///           super-network.
///
///           Adds the given sub-network to this super-network's vector of network pointers, adds
///           the sub-network's node count (not counting its Ground node) to this super-network's
///           total node count, and gives to the sub-network the starting index of its nodes in the
///           total node count (it's node offset).
///
///           If the given network is already present in this super-network, then this method will
///           return without doing anything, and won't add it twice.
///
///           If the given network already belongs to another super-network, then that super-network
///           is added to this super-network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSuperNetworkBase::addSubNetwork(GunnsNetworkBase* network)
{
    try {
        if (not network) {
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "attempted to add sub-network with NULL argument.");
        }
        if (netNodeList.mNodes) {
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "attempted to add sub-network " + network->getName() +
                    " after registerSuperNodes() was called.");
        }
        if (not isSubNetworkAdded(network)) {
            GunnsSuperNetworkBase* superNetwork = network->getSuperNetwork();
            if (superNetwork) {
                addSubNetwork(superNetwork);
            } else {
                mSubnets.push_back(network);
                network->setNodeOffset(netNodeList.mNumNodes, this);
                netNodeList.mNumNodes += (network->getNumLocalNodes() - 1);
            }
        }
    } catch (TsInitializationException& e) {
        /// - Send an H&S fatal message and return on TsInitializationException
        TsHsMsg msg(TS_HS_FATAL, "GUNNS");
        msg << mName << " caught initialization exception." << '\n' << tsStackTrace();
        hsSendMsg(msg);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  network (--) Address of a GunnsSuperNetworkBase super-network.
///
/// @details  This should be called from the input file for each super-network to be added to this
///           super-network.
///
///           For each of the sub-networks that currently belong to the given super-network, re-adds
///           them to this super-network instead, then empties out the given super-network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSuperNetworkBase::addSubNetwork(GunnsSuperNetworkBase* network)
{
    try {
        if (netNodeList.mNodes) {
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "attempted to add sub-network " + network->getName() +
                    " after registerSuperNodes() was called.");
        }
        std::vector<GunnsNetworkBase*>& otherSubs = network->getSubnets();
        for (unsigned int i=0; i<otherSubs.size(); ++i) {
            /// - Null out the sub-network's old super-network before adding it to this one, to
            ///   avoid an infinite loop.
            otherSubs[i]->setNodeOffset(0, 0);
            addSubNetwork(otherSubs[i]);
        }
        network->empty();

    } catch (TsInitializationException& e) {
        /// - Send an H&S fatal message and return on TsInitializationException
        TsHsMsg msg(TS_HS_FATAL, "GUNNS");
        msg << mName << " caught initialization exception." << '\n' << tsStackTrace();
        hsSendMsg(msg);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  This must be called from the input file after all sub-networks have been added via
///           the addNetwork method.
///
///           This allocates the array of basic nodes for the super-network and shares the nodes
///           with the sub-networks.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSuperNetworkBase::registerSuperNodes()
{
    try {
        /// - Throw an error if called twice.
        if (netNodeList.mNodes) {
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "attempted to registerSuperNodes() twice.");
        }

        /// - Add a final node for the super-network Ground node.
        netNodeList.mNumNodes++;

        /// - Allocate the nodes array and update the node list.
        allocateNodes();

        /// - Give the finalized node list to the sub-networks and register them as belonging to a
        ///   super-network.
        for (unsigned int i=0; i<mSubnets.size(); ++i) {
            mSubnets[i]->setNodeList(netNodeList);
        }
        registerJoints();

    } catch (TsInitializationException& e) {
        /// - Send an H&S fatal message and return on TsInitializationException
        TsHsMsg msg(TS_HS_FATAL, "GUNNS");
        msg << mName << " caught initialization exception." << '\n' << tsStackTrace();
        hsSendMsg(msg);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  location  (--)  Pointer to the location connector to get the joint network for.
///
/// @returns  GunnsNetworkBase*  (--)  Pointer to the joint network, or NULL if there isn't one.
///
/// @details  Returns a pointer to the joint network that a location connector has been assigned to,
///           otherwise returns NULL if the connector has not yet been assigned a joint network.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsNetworkBase* GunnsSuperNetworkBase::getLocationJoint(const GunnsMultiLinkConnector* location)
{
    for (unsigned int i=0; i<mJoints.size(); ++i) {
        if (location == mJoints[i]->location1 or
            location == mJoints[i]->location2) {
            return mJoints[i]->network;
        }
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  locationName  (--)  Sim variable name of the location spotter to look up the joint
///                                 network index for.
///
/// @returns  unsigned int  (--)  Joint network index of the location spotter with the given name.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Finds the joint that has a location spotter matching the given name, then returns that
///           spotter's joint network index.  Throws exception if none of the joints have a spotter
///           with the given name.
///
///           Since for each locationName there is a unique returned index that never changes, we
///           use a map so we don't have to keep string comparing the location spotter's name every
///           pass.  Maps are slow too, so we don't really know if this saves time.
////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int GunnsSuperNetworkBase::getJointIndex(const std::string& locationName)
{
    /// - If the locationName key is already found in the map, then return its value as the joint
    ///   index.
    std::map<std::string, unsigned int>::iterator it = mJointsIndeces.find(locationName);
    if (it != mJointsIndeces.end()) {
        return it->second;
    } else {

        /// - locationName key is not yet in the map, so find the joint network index for this
        ///   locationName, add a new record for this pair to the map, and return the index.
        for (unsigned int i=0; i<mJoints.size(); ++i) {
            if (locationName == mJoints[i]->location1->getName()) {
                unsigned int result = mJoints[i]->location1->getJointIndex();
                mJointsIndeces.insert(std::pair<std::string, unsigned int>(locationName, result));
                return result;
            } else if (locationName == mJoints[i]->location2->getName()) {
                unsigned int result = mJoints[i]->location2->getJointIndex();
                mJointsIndeces.insert(std::pair<std::string, unsigned int>(locationName, result));
                return result;
            }
        }
    }
    GUNNS_ERROR(TsOutOfBoundsException, "Argument Not In Range",
            "no location spotter found with the given name.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  location1  (--)  Pointer to the 1st sub-network location to join.
/// @param[in]  location2  (--)  Pointer to the 2nd sub-network location to join.
///
/// @details  This should be called from the input file for each pair of connector locations in
///           different sub-networks to join via a joint network.  This causes a joint network to be
///           dynamically created by a derived class implementing a network factory.  We store info
///           about that joint network and the connectors that connect to it in a vector, so that
///           these connections can be made later in the initialize method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSuperNetworkBase::joinLocations(GunnsMultiLinkConnector* location1,
                                          GunnsMultiLinkConnector* location2)
{
    /// - Throw an error if the locations require conflicting types of joint network.
    const int type = location1->getJointType();
    if (type != location2->getJointType()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "locations require conflicting joint network types.");
    }

    /// - Create a new joint & add it to mJointVector.
    GunnsSubNetworkJoint* joint          = 0;
    GunnsNetworkBase*     existingJoint1 = getLocationJoint(location1);
    GunnsNetworkBase*     existingJoint2 = getLocationJoint(location2);

    if (existingJoint1 and existingJoint2) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "both locations have already been added.");

    } else if (existingJoint1) {
        /// - If one location has already been added, then add the new location to the same joint
        ///   network instead of creating a new joint network.  This allows "3-way" connections by
        ///   combining multiple 2-way connections.
        ///
        ///   Note that this will cause the derived class's dynamic array of joint network pointers
        ///   (for visibility) to have duplicate pointers to the same joint network.  Trick will
        ///   still checkpoint the pointed-to joint network just once, even if pointed to multiple
        ///   times.  The only side effect is that when looking at the dynamic joints network
        ///   pointers array in Trick View, some of them are actually the same network.
        joint = new GunnsSubNetworkJoint(existingJoint1, location1, location2);
    } else if (existingJoint2) {
        joint = new GunnsSubNetworkJoint(existingJoint2, location1, location2);
    } else {

        /// - When both locations are being added for the 1st time, create a new joint network for
        ///   them to connect to.
        joint = new GunnsSubNetworkJoint(createJointNetwork(type), location1, location2);
        mJointNetworks.push_back(joint->network);
        joint->network->setJointIndex(mJoints.size());
    }
    mJoints.push_back(joint);
    numJoints = mJoints.size();

    /// - Add the joint network to this super-network.  This relies on addSubNetwork not adding a
    ///   network again if it's already included.
    addSubNetwork(joint->network);

    /// - Add the location's parent networks to this super-network.
    addSubNetwork(location1->getNetwork());
    addSubNetwork(location2->getNetwork());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This initializes this super-network and prepares it for run.  The nodes are
///           initialized by copying their state from the sub-network nodes they replace, the links
///           from the sub-networks are added to this super-netwwork's link vector, and the solver
///           is initialized with the new links vector.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSuperNetworkBase::initialize()
{
    try {
        /// - Allow for the option of having this super-network in the sim without giving it any
        ///   sub-networks to control.  In this case we return without doing anything.
        if (mSubnets.size() < 1) {
            return;

        } else if (isNullNodes()) {
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "netNodes has not been allocated.");
        }

        /// - Loop over all sub-networks and have them initialize our super-network nodes for us
        ///   instead of their own.
        for (unsigned int i=0; i<mSubnets.size(); ++i) {
            mSubnets[i]->initNodes(mName);
        }
        initGroundNode();

        /// - Now that the super-network's nodes have all been initialized, loop again over all
        ///   sub-networks, initialize their links and spotters and append their links to our super-
        ///   network links vector.
        for (unsigned int i=0; i<mSubnets.size(); ++i) {
            mSubnets[i]->initNetwork();
            /// - This will cause the sub-network to throw exception if it hasn't been set as a
            ///   sub-network.  This ensures that we called the sub's input file init methods prior.
            const std::vector<GunnsBasicLink*>* subLinks = mSubnets[i]->getLinks();
            mLinks.insert(mLinks.end(), subLinks->begin(), subLinks->end());
        }
        initSpotters();

        /// - For join sub-networks to joint networks via link connections.
        for (unsigned int i=0; i<numJoints; ++i) {
            mJoints[i]->join(mJoints[i]->network->getNodeOffset());
        }

        /// - Initialize the solver and inform H&S of our final sub-network contents.
        initSolver();
        for (unsigned int i=0; i<mSubnets.size(); ++i) {
            GUNNS_INFO("  has sub-network: " + mSubnets[i]->getName() + ".");
        }

        /// - Initialize the mutex.
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
/// @details  Restarts this super-network.  This should be called by the Trick restart job.
///           This is intended to be called after load from a checkpoint.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSuperNetworkBase::restart()
{
    if (mSubnets.size() < 1) return;

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
/// @param[in]  timeStep  (s)  Integration time step.
///
/// @details  Propagates the super-network state in run-time.  This should be called by the Trick
///           scheduled job.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsSuperNetworkBase::update(const double timeStep)
{
    if (mSubnets.size() < 1) return;

    if (netMutexEnabled) {
        pthread_mutex_lock(&netMutex);
    }

    try {
        for (unsigned int i=0; i<mSubnets.size(); ++i) {
            mSubnets[i]->stepSpottersPre(timeStep);
        }
        stepSpottersPre(timeStep);

        netSolver.step(timeStep);

        for (unsigned int i=0; i<mSubnets.size(); ++i) {
            mSubnets[i]->stepSpottersPost(timeStep);
        }
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
