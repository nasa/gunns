#ifndef GunnsNetworkBase_EXISTS
#define GunnsNetworkBase_EXISTS

/**
@file
@brief     GUNNS Base Network declarations

@defgroup  TSM_GUNNS_CORE_NETWORK_BASE    GUNNS Base Network Base Class
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for the GUNNS Base Network.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsNetworkBase.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2016-10) (Initial))

@{
*/

#include <string>
#include <pthread.h>
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/Gunns.hh"
#include "core/GunnsBasicNode.hh"

// Forward declarations for pointer types
class GunnsBasicLink;
class GunnsSuperNetworkBase;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Base Network Base Class.
///
/// @details  Abstract base class for all GUNNS networks.  Defines the interface to the super-
///           network types.
///
///           This is a pure virtual class and can't be instantiated directly.  It is intended for
///           all GunnShow-generated networks, ThermalNetwork and hand-written networks to extend
///           this class and implement its pure virtual functions.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsNetworkBase
{
    TS_MAKE_SIM_COMPATIBLE(GunnsNetworkBase);
    public:
        Gunns                        netSolver;           /**<    (--)                     Network solver object. */
        GunnsNodeList                netNodeList;         /**< *o (--) trick_chkpnt_io(**) Network node list structure. */
        /// @brief  Default constructor.
        GunnsNetworkBase(const std::string& name, int numNodes, GunnsBasicNode* nodes);
        /// @brief  Default destructor.
        virtual     ~GunnsNetworkBase();
        /// @brief  Sets this network's drawing node number offset in the super-network.
        void         setNodeOffset(const int offset, GunnsSuperNetworkBase* superNetwork);
        /// @brief  Replace this network's node list with the given super-network's list.
        void         setNodeList(GunnsNodeList& nodeList);
        /// @brief  Stores the given index in a visibility array of this joint network.
        void         setJointIndex(const unsigned int index);
        /// @brief  Returns this network's links vector to the calling super-network.
        const std::vector<GunnsBasicLink*>* getLinks() const;
        /// @brief  Network initialization task, standalone.
        void         initialize(const std::string& name = "");
        /// @brief  Network restart task.
        virtual void restart();
        /// @brief  Network update task.
        virtual void update(const double timeStep);
        /// @brief  Pure virtual initialize nodes function.
        virtual void initNodes(const std::string& name) = 0;
        /// @brief  Pure virtual initialize remaining network objects function.
        virtual void initNetwork() = 0;
        /// @brief  Update network spotters before the solver solution.
        virtual void stepSpottersPre(const double timeStep);
        /// @brief  Update network spotters after the solver solution.
        virtual void stepSpottersPost(const double timeStep);
        /// @brief  Returns this network's fluid configuration.
        virtual const PolyFluidConfigData* getFluidConfig() const;
        /// @brief  Gets this network's drawing node number offset in the super-network.
        int          getNodeOffset() const;
        /// @brief  Returns this network's name.
        const std::string& getName() const;
        /// @brief  Returns the number of nodes defined in this network.
        virtual int  getNumLocalNodes();
        /// @brief  Returns the super-network this network belongs to, if any.
        GunnsSuperNetworkBase* getSuperNetwork() const;
        /// @brief  Returns the index of this joint network in a visibility array.
        int          getJointIndex() const;
        /// @brief  Returns a pointer to the contained mutex object.
        pthread_mutex_t* getMutex();
        /// @brief  Sets the mutex locking enable flag to the given value.
        void         setMutexEnabled(const bool flag);

    protected:
        std::string                  mName;               /**< ** (--) trick_chkpnt_io(**) Network instance name for H&S messages. */
        std::vector<GunnsBasicLink*> netLinks;            /**< ** (--) trick_chkpnt_io(**) Network links vector. */
        bool                         netIsSubNetwork;     /**< *o (--) trick_chkpnt_io(**) This network belongs to a super-network. */
        int                          netSuperNodesOffset; /**< *o (--) trick_chkpnt_io(**) Offset of this network's drawing nodes in the super-network. */
        int                          netJointIndex;       /**< *o (--) trick_chkpnt_io(**) Index of this joint network in a visibility array. */
        int                          netNumLocalNodes;    /**< ** (--) trick_chkpnt_io(**) Number of nodes defined in this network. */
        GunnsSuperNetworkBase*       netSuperNetwork;     /**< ** (--) trick_chkpnt_io(**) Pointer to the super-network this belongs to, if any. */
        pthread_mutex_t              netMutex;            /**< ** (--) trick_chkpnt_io(**) The mutex. */
        bool                         netMutexEnabled;     /**<    (--) trick_chkpnt_io(**) When true, mutex locking is enabled. */
        /// @brief  Returns a network node name containing the node number.
        const std::string createNodeName(const int node) const;

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsNetworkBase(const GunnsNetworkBase& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsNetworkBase& operator =(const GunnsNetworkBase& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  index  (--)  Index of this joint network in a visibility array
///
/// @details  Since the joint index is usually used as an index into an array, it is used and passed
///           here as an unsigned int.  However, we want to use a negative value to denote that this
///           has never been set, and therefore this is not a joint network.  So in this class we
///           store it as a regular int and typecast in here.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsNetworkBase::setJointIndex(const unsigned int index)
{
    netJointIndex = static_cast<int>(index);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  timeStep (s) Integration time step (not used)
///
/// @details  This default implementation does nothing because spotters are optional and this base
///           class has none.  This can be overridden by derived networks to step their spotters.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsNetworkBase::stepSpottersPre(const double timeStep __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  timeStep (s) Integration time step (not used).
///
/// @details  This default implementation does nothing because spotters are optional and this base
///           class has none.  This can be overridden by derived networks to step their spotters.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsNetworkBase::stepSpottersPost(const double timeStep __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  NULL
///
/// @details  The default implementation returns NULL.  This is intended to be overridden by fluid
///           networks to return their internal fluid configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const PolyFluidConfigData* GunnsNetworkBase::getFluidConfig() const
{
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int (--) The offset of this network's original nodes in the super-network.
///
/// @details  This returns the offset of this network's original nodes in the super-network.  When
///           in a super-network, this network's nodes are not used.  This offset term helps the
///           user cross-reference this network's nodes (as defined from a GunnShow drawing, etc)
///           with the actual nodes being used from the super-network.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsNetworkBase::getNodeOffset() const
{
    return netSuperNodesOffset;;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  std::string& (--) The name of this network instance.
///
/// @details  Returns the name of this network instance.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const std::string& GunnsNetworkBase::getName() const
{
    return mName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int (--) The number of nodes defined in this network.
///
/// @details  Returns the number of nodes this network defines, including its ground node.  This may
///           be different than the netNodeList.mNumNodes when this is a sub-network.  This is
///           virtual and non-const to allow derived classes to compute and store their number of
///           nodes if needed.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsNetworkBase::getNumLocalNodes()
{
    return netNumLocalNodes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsSuperNetworkBase*  (--) Returns the super-network this network belongs to, or
///                                        NULL if it is stand-alone.
///
/// @details  Returns the super-network this network belongs to, if any.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsSuperNetworkBase* GunnsNetworkBase::getSuperNetwork() const
{
    return netSuperNetwork;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int  (--)  The index of this joint network in a visibility array.
///
/// @details  Returns the index of this joint network in a visibility array.
///
/// @note     The returned value can be negative, denoting this is not a joint network and there is
///           no visibility array pointing to it.  In this case this value is not usable as an array
///           index and care should be taken by the caller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsNetworkBase::getJointIndex() const
{
    return netJointIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  pthread_mutex_t* (--) Pointer to the contained mutex object.
///
/// @details  Returns the address of the netMutex object.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline pthread_mutex_t* GunnsNetworkBase::getMutex()
{
    return &netMutex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag (--) When true, enabled mutex locking.
///
/// @details  Sets the netMutexEnabled attribute to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsNetworkBase::setMutexEnabled(const bool flag)
{
    netMutexEnabled = flag;
}

#endif
