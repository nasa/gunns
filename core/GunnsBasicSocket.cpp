/**
@file
@brief    GUNNS Basic Socket Link implementation

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Provides the classes for modeling the GUNNS Basic Socket.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     ()

LIBRARY DEPENDENCY:
    ((GunnsBasicJumperPlug.o)
     (GunnsBasicConductor.o))

 PROGRAMMERS:
    ((Jason Harvey) (L-3 Communications) (2012-08) (Initial))
*/
#include "GunnsBasicSocket.hh"
#include "GunnsBasicJumperPlug.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsInvalidStateTransitionException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                (--) Link name.
/// @param[in] nodes               (--) Network nodes array.
/// @param[in] defaultConductivity (--) Default conductivity of the link.
/// @param[in] selfSealing         (--) Self-seals when disconnected.
///
/// @details  Constructs the Basic Socket Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSocketConfigData::GunnsBasicSocketConfigData(const std::string& name,
                                                       GunnsNodeList*     nodes,
                                                       const double       defaultConductivity,
                                                       const bool         selfSealing)
    :
    GunnsBasicConductorConfigData(name, nodes, defaultConductivity),
    mSelfSealing(selfSealing)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from.
///
/// @details  Copy Constructs the Basic Socket Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSocketConfigData::GunnsBasicSocketConfigData(const GunnsBasicSocketConfigData& that)
    :
    GunnsBasicConductorConfigData(that),
    mSelfSealing(that.mSelfSealing)
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Socket Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSocketConfigData::~GunnsBasicSocketConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
///
/// @details  Default constructs this Basic Socket input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSocketInputData::GunnsBasicSocketInputData(const bool   malfBlockageFlag,
                                                     const double malfBlockageValue)
    :
    GunnsBasicConductorInputData(malfBlockageFlag, malfBlockageValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Basic Socket input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSocketInputData::GunnsBasicSocketInputData(const GunnsBasicSocketInputData& that)
    :
    GunnsBasicConductorInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Basic Socket input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSocketInputData::~GunnsBasicSocketInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Basic Socket Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSocket::GunnsBasicSocket()
    :
    GunnsBasicConductor(),
    mSelfSealing(false),
    mConnection(0),
    mHasConnection(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Socket Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSocket::~GunnsBasicSocket()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data
/// @param[in]     inputData    (--) Reference to Link Input Data
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector
/// @param[in]     port0        (--) Port 0 Mapping
/// @param[in]     port1        (--) Port 1 Mapping
///
/// @throws   TsInitializationException
///
/// @details  Initializes the Basic Socket with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicSocket::initialize(const GunnsBasicSocketConfigData& configData,
                                  const GunnsBasicSocketInputData&  inputData,
                                  std::vector<GunnsBasicLink*>&     networkLinks,
                                  const int                         port0,
                                  const int                         port1)
{
    /// - Initialize the parent class.
    GunnsBasicConductor::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag      = false;

    /// - Initialize from config data.
    mSelfSealing   = configData.mSelfSealing;

    /// - Initialize state data.
    mConnection    = 0;
    mHasConnection = false;

    /// - Set init flag on successful validation.
    mInitFlag      = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  During load initialization, there is an order dependency between GunnsBasicSocket and
///           GunnsBasicJumper objects.  The socket links must be initialized before the jumper
///           links, so that the sockets can return the proper nodes when the jumpers make their
///           initial socket requests.  To ensure that the order is correct, requestConnection and
///           requestDisconnection methods call this method before proceeding.  This method checks
///           the link's initialization flag and if it hasn't been set, an exception is thrown.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicSocket::checkInitialization() const
{
    if (!mInitFlag) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Invalid Initialization Data",
                TsInitializationException,
                "attempted processing before initialization was completed.", "Unknown Socket");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicSocket::restartModel()
{
    /// - Reset the base class.
    GunnsBasicConductor::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] plug (--) Pointer to the plug requesting the connection.
///
/// @returns  (--) Index of the Node to connect to.
///
/// @throws   TsInvalidStateTransitionException
///
/// @details  Processes a request from a GUNNS jumper plug to connect to the GUNNS Basic Node this
///           GUNNS Basic Socket link is connected to on port 0.  If the request is granted (there
///           is not another plug already connected), the index of the port 0 Node is returned, and
///           the socket stores the address of the plug as its active connection, otherwise an
///           exception is thrown which will cause the jumper to fail to connect to the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsBasicSocket::requestConnection(const GunnsBasicJumperPlug* plug)
{
    checkInitialization();

    int result = mNodeMap[0];

    if (mConnection) {
        GUNNS_WARNING("rejected " << plug->getName() << "'s connection request because "
                << mConnection->getName() << " is already connected here.");
        throw TsInvalidStateTransitionException("Invalid Connection Request",
                                                "GunnsBasicSocket::requestConnection",
                                                "Another plug is already connected here.");
    } else {
        mConnection    = plug;
        mHasConnection = true;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] plug (--) Pointer to the plug requesting the disconnection.
///
/// @returns  (--) Index of the Node to connect to.
///
/// @details  Processes a request from a GUNNS jumper plug to disconnect from the GUNNS Basic Node
///           this GUNNS Basic Socket link is connected to on port 0.  The index of the ambient node
///           is always returned, so an unconnected plug may be moved from one ambient node to
///           another.  The socket's connection pointer is only reset to null if the plug was
///           the one actually connected.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsBasicSocket::requestDisconnection(const GunnsBasicJumperPlug* plug)
{
    checkInitialization();

    if (plug == mConnection) {
        mConnection    = 0;
        mHasConnection = false;
    }
    return mNodeMap[1];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @details  Updates the socket's conductivity based on connection state and whether it is self-
///           sealing.  Only a non-self-sealing socket that has no connection will have
///           conductivity.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicSocket::updateState(const double dt __attribute__((unused)))
{
    if (mSelfSealing or mConnection) {
        mEffectiveConductivity = 0.0;
    }
}
