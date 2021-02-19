/**
@file
@brief     GUNNS Multiple Links Connector Spotter implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((core/GunnsNetworkSpotter.o)
   (core/network/GunnsNetworkBase.o)
   (simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o))
*/

#include "GunnsMultiLinkConnector.hh"
#include "core/GunnsBasicLink.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name       (--)  Instance name for self-identification in messages.
///
/// @details  Default constructs this GUNNS Multiple Links Connector Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMultiLinkConnectorConfigData::GunnsMultiLinkConnectorConfigData(const std::string& name)
    :
    GunnsNetworkSpotterConfigData(name)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Multiple Links Connector Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMultiLinkConnectorConfigData::~GunnsMultiLinkConnectorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  link  (--)  Pointer to the link that connects.
/// @param[in]  port  (--)  The link's port number to connect.
/// @param[in]  node  (--)  The joint network node number to connect to.
///
/// @details  Default constructs this GUNNS Multiple Links Connector Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMultiLinkConnectorInputData::GunnsMultiLinkConnectorInputData(      GunnsBasicLink* link,
                                                                   const int             port,
                                                                   const int             node)
    :
    GunnsNetworkSpotterInputData(),
    mLink(link),
    mPort(port),
    mNode(node)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Multiple Links Connector Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMultiLinkConnectorInputData::~GunnsMultiLinkConnectorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] network   (--) Pointer to the network containing this spotter.
/// @param[in] jointType (--) Optional type of joint network this connector requires.
///
/// @details Default constructs this GUNNS Multiple Links Connector Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMultiLinkConnector::GunnsMultiLinkConnector(GunnsNetworkBase* network, const int jointType)
    :
    GunnsNetworkSpotter(),
    mNetwork(*network),
    mJointNetwork(0),
    mJointType(jointType),
    mConnections()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Multiple Links Connector Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMultiLinkConnector::~GunnsMultiLinkConnector()
{
    for(unsigned int i=0; i<mConnections.size(); ++i){
        delete mConnections[i];
        mConnections[i] = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
/// @param[in]  inputData   (--)  Instance input data.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Multiple Links Connector Spotter with its configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMultiLinkConnector::initialize(const GunnsNetworkSpotterConfigData* configData,
                                         const GunnsNetworkSpotterInputData*  inputData)
{
    /// - Initialize the base class.
    GunnsNetworkSpotter::initialize(configData, inputData);

    /// - Reset the init flag.
    mInitFlag = false;

    /// - Validate & initialize from config & input data.
    validateConfig(configData);
    validateInput(inputData);

    /// - Set the init flag.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
///
/// @returns  GunnsMultiLinkConnectorConfigData (--) Type-casted and validated config data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base config data class pointer to this spotter's config data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsMultiLinkConnectorConfigData* GunnsMultiLinkConnector::validateConfig(const GunnsNetworkSpotterConfigData* config)
{
    const GunnsMultiLinkConnectorConfigData* result = dynamic_cast<const GunnsMultiLinkConnectorConfigData*>(config);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Bad config data pointer type.");
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputData  (--)  Instance input data.
///
/// @returns  GunnsMultiLinkConnectorInputData (--) Type-casted and validated input data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base input data class pointer to this spotter's input data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsMultiLinkConnectorInputData* GunnsMultiLinkConnector::validateInput(const GunnsNetworkSpotterInputData* input)
{
    const GunnsMultiLinkConnectorInputData* result = dynamic_cast<const GunnsMultiLinkConnectorInputData*>(input);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Bad input data pointer type.");
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (not used).
///
/// @details  This method does nothing because this spotter has no function prior to the network
///           solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMultiLinkConnector::stepPreSolver(const double dt __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (not used).
///
/// @details  This method does nothing because this spotter has no function after the network
///           solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMultiLinkConnector::stepPostSolver(const double dt __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  link  (--)  Pointer to the link that connects.
/// @param[in]  port  (--)  The link's port number to connect.
/// @param[in]  node  (--)  The joint network node number to connect to.
///
/// @details  Creates a new config data describing a link connection from the given arguments, and
///           adds it to the connections vector.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMultiLinkConnector::addConnection(GunnsBasicLink* link, int port, int node)
{
    GunnsMultiLinkConnectorInputData* connection = new GunnsMultiLinkConnectorInputData(link, port, node);
    mConnections.push_back(connection);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  offset  (--)  Super-network nodes offset of the network being connected to.
///
/// @details  Calls each connection object's connect method to activate the link connection
///           commands.  Passes the super-network node offset of the network being connected to, in
///           case we are in a sub-network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMultiLinkConnector::connect(const int offset)
{
    for (unsigned int i=0; i<mConnections.size(); ++i) {
        mConnections[i]->connect(offset);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int (--) Index of the joint network in its visibility array.
///
/// @throws   TsInitializationException
///
/// @details  Returns the index of the joint network in its visibility array.  This should only be
///           called after the joint network has been created and a visibility array pointed to it,
///           otherwise this will throw an exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int GunnsMultiLinkConnector::getJointIndex() const
{
    if (not mJointNetwork) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "joint network pointer is missing.");
    }
    int result = mJointNetwork->getJointIndex();

    if (result < 0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "joint network is missing visibility array index.");
    }
    return static_cast<unsigned int>(result);
}
