#ifndef GunnsMultiLinkConnector_EXISTS
#define GunnsMultiLinkConnector_EXISTS

/**
@file
@brief     GUNNS Multiple Links Connector Spotter declarations

@defgroup  TSM_GUNNS_CORE_MULTI_LINKS_CONNECTOR   GUNNS Multiple Links Connector Spotter
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:  (Provides the classes for the GUNNS Multiple Links Connector Spotter.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsMultiLinkConnector.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2017-07) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsNetworkSpotter.hh"
#include "core/network/GunnsNetworkBase.hh"
#include <vector>

// Forward-declare pointer types.
class GunnsBasicLink;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Multiple Links Connector Spotter Configuration Data
///
/// @details  This class provides a data structure for the GUNNS Multiple Links Connector Spotter
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsMultiLinkConnectorConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        /// @brief  Default constructs this GUNNS Multiple Links Connector Spotter configuration data.
        GunnsMultiLinkConnectorConfigData(const std::string& name);
        /// @brief  Default destructs this GUNNS Multiple Links Connector Spotter configuration data.
        virtual ~GunnsMultiLinkConnectorConfigData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsMultiLinkConnectorConfigData(const GunnsMultiLinkConnectorConfigData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsMultiLinkConnectorConfigData& operator =(const GunnsMultiLinkConnectorConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Multiple Links Connector Spotter Input Data
///
/// @details  This class provides a data structure for the GUNNS Multiple Links Connector Spotter
///           input data.  This is used to define one assignment of a link port to a node.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsMultiLinkConnectorInputData : public GunnsNetworkSpotterInputData
{
    public:
        GunnsBasicLink* mLink; /**< (--) trick_chkpnt_io(**) Pointer to the link that connects. */
        int             mPort; /**< (--) trick_chkpnt_io(**) The link's port number to connect. */
        int             mNode; /**< (--) trick_chkpnt_io(**) The joint network node number to connect to. */
        /// @brief  Default constructs this GUNNS Multiple Links Connector Spotter input data.
        GunnsMultiLinkConnectorInputData(      GunnsBasicLink* link = 0,
                                         const int             port = 0,
                                         const int             node = 0);
        /// @brief  Default destructs this GUNNS Multiple Links Connector Spotter input data.
        virtual ~GunnsMultiLinkConnectorInputData();
        /// @brief  Commands the given link to attach its given port to the given node.
        void     connect(int offset);

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsMultiLinkConnectorInputData(const GunnsMultiLinkConnectorInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsMultiLinkConnectorInputData& operator =(const GunnsMultiLinkConnectorInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Multiple Links Connector Spotter Class.
///
/// @details  This spotter is used to command multiple links to change their node connections at the
///           same time.  It holds a list of "connections" (actually a config data object): each
///           connection has a pointer to a link, the link's port number to command, and the network
///           node number to connect the port to.  The connect method loops through these
///           connections and sends the command to each link.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsMultiLinkConnector : public GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(GunnsMultiLinkConnector);
    public:
        /// @brief  Default Constructor
        GunnsMultiLinkConnector(GunnsNetworkBase* network, const int jointType = 0);
        /// @brief  Default destructor.
        virtual     ~GunnsMultiLinkConnector();
        /// @brief  Initializes the GUNNS Multiple Links Connector Spotter with configuration and
        ///         input data.
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        /// @brief  Steps the GUNNS Multiple Links Connector Spotter prior to the GUNNS solver step.
        virtual void stepPreSolver(const double dt);
        /// @brief  Steps the GUNNS Multiple Links Connector Spotter after the GUNNS solver step.
        virtual void stepPostSolver(const double dt);
        /// @brief  Adds a new connection defined by the given data.
        void         addConnection(GunnsBasicLink* link, int port, int node);
        /// @brief  Activates all connections.
        void         connect(const int offset);
        /// @brief  Returns a pointer to the network this spotter belongs to.
        GunnsNetworkBase* getNetwork() const;
        /// @brief  Returns the joint network type this connector requires.
        int          getJointType() const;
        /// @brief  Returns the joint network index this spotter connects to.
        unsigned int getJointIndex() const;
        /// @brief  Sets the joint network this spotter connects to.
        void         setJointNetwork(GunnsNetworkBase* network);

    protected:
        GunnsNetworkBase&                              mNetwork;      /**< ** (--) trick_chkpnt_io(**) Reference to the network containing this spotter. */
        GunnsNetworkBase*                              mJointNetwork; /**< ** (--) trick_chkpnt_io(**) Pointer to the joint network. */
        int                                            mJointType;    /**< ** (--) trick_chkpnt_io(**) The type of joint network this connector requires. */
        std::vector<GunnsMultiLinkConnectorInputData*> mConnections;  /**< ** (--) trick_chkpnt_io(**) List of connections to make. */
        /// @brief   Validates the supplied configuration data.
        const GunnsMultiLinkConnectorConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        /// @brief   Validates the supplied input data.
        const GunnsMultiLinkConnectorInputData*  validateInput (const GunnsNetworkSpotterInputData* input);

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsMultiLinkConnector(const GunnsMultiLinkConnector& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsMultiLinkConnector& operator =(const GunnsMultiLinkConnector& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] offset (--) Super-network node offset of the sub-network being connected to.
///
/// @details  mNode is the node number to connect to relative to the network's own local nodes, so
///           we must add the given network's node offset since it may be in a super-network.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsMultiLinkConnectorInputData::connect(int offset)
{
    mLink->setPort(mPort, mNode + offset);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsNetworkNase* (--) Pointer to the network this spotter belongs to.
///
/// @details  Returns a pointer to the network this spotter belongs to.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsNetworkBase* GunnsMultiLinkConnector::getNetwork() const
{
    return &mNetwork;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int (--) The joint network type this connector requires.
///
/// @details  Returns the joint network type this connector requires.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsMultiLinkConnector::getJointType() const
{
    return mJointType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  network  (--)  Pointer to the joint network.
///
/// @details  Sets mJointNetwork to the given base network pointer.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsMultiLinkConnector::setJointNetwork(GunnsNetworkBase* network)
{
    mJointNetwork = network;
}

#endif
