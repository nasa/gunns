#ifndef GunnsSuperNetworkBase_EXISTS
#define GunnsSuperNetworkBase_EXISTS

/**
@file
@brief     GUNNS Super-Network Base declarations

@defgroup  TSM_GUNNS_CORE_SUPER_NETWORK_BASE    GUNNS Super-Network Base Class
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for the GUNNS Super-Network Base.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsSuperNetworkBase.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2016-10) (Initial))

@{
*/

#include <string>
#include <vector>
#include <map>
#include <pthread.h>
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/Gunns.hh"
#include "core/GunnsFluidNode.hh"

// Forward declarations for pointer types
class GunnsNetworkBase;
class GunnsMultiLinkConnector;
class GunnsBasicLink;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Sub-Network Joint Class.
///
/// @details  This class stores information about a joint between two connector spotters in
///           different sub-networks, and a joint sub-network that they both connect to.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsSubNetworkJoint
{
    public:
        GunnsNetworkBase*        network;   /**< ** (--) Pointer to the joint network. */
        GunnsMultiLinkConnector* location1; /**< ** (--) Pointer to the 1st location spotter. */
        GunnsMultiLinkConnector* location2; /**< ** (--) Pointer to the 2nd location spotter. */
        /// @brief  Default constructor.
        GunnsSubNetworkJoint(GunnsNetworkBase*        net,
                             GunnsMultiLinkConnector* loc1,
                             GunnsMultiLinkConnector* loc2);
        /// @brief  Default destructor.
        virtual ~GunnsSubNetworkJoint();
        /// @brief  Connects the location connector spotters to the joint network.
        void     join(const int offset);

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsSubNetworkJoint(const GunnsSubNetworkJoint& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsSubNetworkJoint& operator =(const GunnsSubNetworkJoint& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Super-Network Base Class.
///
/// @details  This aggregates multiple GUNNS networks into a combined node set and system of
///           equations.  This allows easy combination of various separate networks into a single
///           system via input file, rather than having to create new GunnShow drawings of the
///           various combined systems.
///
///           The GUNNS networks that are combined in this way are called "sub-networks".  To
///           qualify, a sub-network must extend the GunnsNetworkBase class, which defines the
///           interface to this super-network class.
///
///           Super-networks can be nested -- a super-network can be added as a sub-network to
///           another super-network, and so on.  When a super-network is sub'd to another super-
///           network, it passes up its own sub-networks to the parent, and thereafter does nothing.
///           This is merely a model organizational aid.
///
///           Link connections between sub-networks can be created in the input file by overriding
///           their initial node assignment.  Also, pre-defined connection patterns between sub-
///           networks can be created by using "joint" networks.  This class is extended as needed
///           to dynamically create joint sub-networks as interfaces between GunnsMultiLinkConnector
///           spotters in other sub-networks.  This allows certain connection patterns between sub-
///           networks to be automated & simplified in the input file.
///
///           Sim Object structure:
///           - The sub-networks can have their Trick jobs defined (initialization, restart,
///             scheduled) in their home sim objects as usual.  This is optional.  Defining them
///             allows the option of running them standalone (not in a super-network) when needed.
///             Their jobs do not interfere with the super-network.  When added as a sub-network to
///             a super-network, their jobs do nothing and the super-network takes over execution of
///             the sub-network.
///           - The super-network should have the same usual Trick jobs defined, preferably in the
///             same sim object as the sub-networks.  Order doesn't matter.
///           - Note that the super-network takes over execution of the sub-network's links &
///             spotters.  To avoid timing problems with the Sim Bus, the super-network should be
///             updated in the same Sim Bus thread as its sub-networks.
///
///           Input file steps should proceed in this order:
///           1. In the input file, call this super-network's addSubNetwork method with each of the
///              desired sub-network's names, like so:
///                mySimObject.superNetwork.addSubNetwork(mySimObject.subNetworkA)
///                mySimObject.superNetwork.addSubNetwork(mySimObject.subNetworkB)
///           2. To use joints, call this super-network's joinLocations method for each pair of
///              spotters in different sub-networks to join, like so:
///                mySimObject.superNetowrk.joinLocations(mySimObject.subNetworkA.jointLocation,
///                                                       mySimObject.subNetworkB.jointLocation)
///           3. In the input file, call the super-network's registerSuperNodes method, like so:
///                mySimObject.superNetwork.registerSuperNodes()
///           4. Steps 1-3 should be completed for each super-network before adding it to a higher
///              level super-network.
///           5. In the input file, override the super-network solver's config data as needed.  Only
///              top-level super-networks execute; mid-level super-networks in a nesting do not, and
///              their solver config can be omitted.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsSuperNetworkBase
{
    TS_MAKE_SIM_COMPATIBLE(GunnsSuperNetworkBase);

    public:
        Gunns                               netSolver;       /**<    (--)                     Network solver object. */
        GunnsConfigData                     netSolverConfig; /**< *o (--) trick_chkpnt_io(**) Network solver configuration data. */
        GunnsNodeList                       netNodeList;     /**< *o (--) trick_chkpnt_io(**) Network node list structure. */
        /// @brief  Default constructor.
        GunnsSuperNetworkBase(const std::string& name);
        /// @brief  Default destructor.
        virtual ~GunnsSuperNetworkBase();
        /// @brief  Adds the given base network as a sub-network to this super-network.
        void     addSubNetwork(GunnsNetworkBase* network);
        /// @brief  Adds the given super-network as a sub-network to this super-network.
        void     addSubNetwork(GunnsSuperNetworkBase* network);
        /// @brief  Allocates the nodes array and updates sub-networks with the super node list.
        void     registerSuperNodes();
        /// @brief  Super-network initialization task.
        void     initialize();
        /// @brief  Super-network restart task.
        void     restart();
        /// @brief  Super-network update task.
        void     update(const double timeStep);
        /// @brief  Returns this network's name.
        const std::string& getName() const;
        /// @brief  Returns whether the given sub-network is already added to this super-network.
        bool     isSubNetworkAdded(const GunnsNetworkBase* network) const;
        /// @brief  Returns the joint network the given location spotter is to connect to.
        GunnsNetworkBase* getLocationJoint(const GunnsMultiLinkConnector* location);
        /// @brief  Returns the joint network index of the location spotter with the given name.
        unsigned int getJointIndex(const std::string& locationName);
        /// @brief  Connects two sub-network locations via a joint sub-network.
        void     joinLocations(GunnsMultiLinkConnector* location1,
                               GunnsMultiLinkConnector* location2);
        /// @brief  Returns a pointer to the contained mutex object.
        pthread_mutex_t* getMutex();
        /// @brief  Sets the mutex locking enable flag to the given value.
        void     setMutexEnabled(const bool flag);

    protected:
        std::string                         mName;           /**< ** (--) trick_chkpnt_io(**) Network instance name for H&S messages. */
        std::vector<GunnsBasicLink*>        mLinks;          /**< ** (--) trick_chkpnt_io(**) Network links vector. */
        std::vector<GunnsNetworkBase*>      mSubnets;        /**< ** (--) trick_chkpnt_io(**) Sub-networks vector. */
        std::vector<GunnsNetworkBase*>      mJointNetworks;  /**< ** (--) trick_chkpnt_io(**) Joint networks vector (only for deletion). */
        std::vector<GunnsSubNetworkJoint*>  mJoints;         /**< ** (--) trick_chkpnt_io(**) Joints vector. */
        std::map<std::string, unsigned int> mJointsIndeces;  /**< ** (--) trick_chkpnt_io(**) Map of location spotter name to joint network index. */
        unsigned int                        numJoints;       /**< *o (--) trick_chkpnt_io(**) Number of network joints. */
        pthread_mutex_t                     netMutex;        /**< ** (--) trick_chkpnt_io(**) The mutex. */
        bool                                netMutexEnabled; /**<    (--) trick_chkpnt_io(**) When true, mutex locking is enabled. */
        /// @brief  Pure virtual method to create dynamic nodes array of the derived aspect type.
        virtual void allocateNodes() = 0;
        /// @brief  Pure virtual method to delete the dynamic nodes array.
        virtual void freeNodes() = 0;
        /// @brief  Pure virtual method to return whether nodes array has been allocated.
        virtual bool isNullNodes() const = 0;
        /// @brief  Pure virtual method to initialize the super-network Ground node.
        virtual void initGroundNode() = 0;
        /// @brief  Pure virtual method to initialize the super-network solver.
        virtual void initSolver() = 0;
        /// @brief  Pure virtual method to create a derived joint network type.
        virtual GunnsNetworkBase* createJointNetwork(const int type) = 0;
        /// @brief  Pure virtual method to allocate array of the derived joint network type.
        virtual void registerJoints() = 0;
        /// @brief  Initialize super-network spotters.
        virtual void initSpotters();
        /// @brief  Update super-network spotters before the solver solution.
        virtual void stepSpottersPre(const double timeStep);
        /// @brief  Update super-network spotters after the solver solution.
        virtual void stepSpottersPost(const double timeStep);
        /// @brief  Removes contained objects from this super-network.
        void         empty();
        /// @brief  Returns the vector of sub-networks.
        std::vector<GunnsNetworkBase*>& getSubnets();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsSuperNetworkBase(const GunnsSuperNetworkBase& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsSuperNetworkBase& operator =(const GunnsSuperNetworkBase& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  std::string& (--) The name of this network instance.
///
/// @details  Returns the name of this network instance.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const std::string& GunnsSuperNetworkBase::getName() const
{
    return mName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  network  (--)  Pointer to the network to check for in this super-network.
///
/// @returns  bool  (--)  True if this sub-network is already added, False if not.
///
/// @details  Returns whether the given sub-network is already added to this super-network.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsSuperNetworkBase::isSubNetworkAdded(const GunnsNetworkBase* network) const
{
    for (unsigned int i=0; i<mSubnets.size(); ++i) {
        if (network == mSubnets[i]) {
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This default implementation does nothing because generic super-networks have no
///           spotters.  This can be overridden by custom super-networks to init their spotters.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsSuperNetworkBase::initSpotters()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  timeStep (s) Integration time step (not used)
///
/// @details  This default implementation does nothing because generic super-networks have no
///           spotters.  This can be overridden by custom super-networks to step their spotters.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsSuperNetworkBase::stepSpottersPre(const double timeStep __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  timeStep (s) Integration time step (not used).
///
/// @details  This default implementation does nothing because generic super-networks have no
///           spotters.  This can be overridden by custom super-networks to step their spotters.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsSuperNetworkBase::stepSpottersPost(const double timeStep __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes any allocated nodes, clears out the node list and sub-networks vector.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsSuperNetworkBase::empty()
{
    netNodeList.mNumNodes = 0;
    netNodeList.mNodes = 0;
    freeNodes();
    mSubnets.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  std::vector<GunnsNetworkBase*>* (--) Pointer to this sub-networks vector.
///
/// @details  Returns the address of the vector of sub-networks.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline std::vector<GunnsNetworkBase*>& GunnsSuperNetworkBase::getSubnets()
{
    return mSubnets;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  pthread_mutex_t* (--) Pointer to the contained mutex object.
///
/// @details  Returns the address of the mMutex object.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline pthread_mutex_t* GunnsSuperNetworkBase::getMutex()
{
    return &netMutex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag (--) When true, enabled mutex locking.
///
/// @details  Sets the netMutexEnabled attribute to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsSuperNetworkBase::setMutexEnabled(const bool flag)
{
    netMutexEnabled = flag;
}

#endif
