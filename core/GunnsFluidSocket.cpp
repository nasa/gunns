/**
@file
@brief    GUNNS Fluid Socket Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Provides the classes for modeling the GUNNS Fluid Socket.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     ()

LIBRARY DEPENDENCY:
    ((GunnsFluidJumperPlug.o))

 PROGRAMMERS:
    ((Jason Harvey) (L-3 Communications) (2012-08) (Initial))
*/
#include "GunnsFluidSocket.hh"
#include "GunnsFluidJumperPlug.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsInvalidStateTransitionException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                 (--) Link name.
/// @param[in] nodes                (--) Network nodes array.
/// @param[in] maxConductivity      (m2) Maximum possible effective conductivity of the link.
/// @param[in] expansionScaleFactor (--) Scale factor for isentropic cooling across the link (0-1).
/// @param[in] selfSealing          (--) Self-seals when disconnected.
///
/// @details  Constructs the Fluid Socket Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSocketConfigData::GunnsFluidSocketConfigData(const std::string& name,
                                                       GunnsNodeList*     nodes,
                                                       const double       maxConductivity,
                                                       const double       expansionScaleFactor,
                                                       const bool         selfSealing)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, expansionScaleFactor),
    mSelfSealing(selfSealing)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from.
///
/// @details  Copy Constructs the Fluid Socket Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSocketConfigData::GunnsFluidSocketConfigData(const GunnsFluidSocketConfigData& that)
    :
    GunnsFluidConductorConfigData(that),
    mSelfSealing(that.mSelfSealing)
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Fluid Socket Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSocketConfigData::~GunnsFluidSocketConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--)   Blockage malfunction flag.
/// @param[in] malfBlockageValue (--)   Blockage malfunction fractional value (0-1).
/// @param[in] malfLeakRateFlag  (--)   Leak-thru malfunction flag.
/// @param[in] malfLeakRateValue (kg/s) Leak-thru malfunction value.
///
/// @details  Default constructs this Fluid Socket input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSocketInputData::GunnsFluidSocketInputData(const bool   malfBlockageFlag,
                                                     const double malfBlockageValue,
                                                     const bool   malfLeakRateFlag,
                                                     const double malfLeakRateValue)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mMalfLeakRateFlag (malfLeakRateFlag),
    mMalfLeakRateValue(malfLeakRateValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Fluid Socket input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSocketInputData::GunnsFluidSocketInputData(const GunnsFluidSocketInputData& that)
    :
    GunnsFluidConductorInputData(that),
    mMalfLeakRateFlag (that.mMalfLeakRateFlag),
    mMalfLeakRateValue(that.mMalfLeakRateValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Socket input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSocketInputData::~GunnsFluidSocketInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Fluid Socket Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSocket::GunnsFluidSocket()
    :
    GunnsFluidConductor(),
    mMalfLeakRateFlag(false),
    mMalfLeakRateValue(0.0),
    mSelfSealing(false),
    mConnection(0),
    mHasConnection(false),
    mPreviousLeakRate(0.0),
    mLeakConductivity(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Fluid Socket Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSocket::~GunnsFluidSocket()
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
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Initializes the Fluid Socket with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSocket::initialize(const GunnsFluidSocketConfigData& configData,
                                  const GunnsFluidSocketInputData&  inputData,
                                  std::vector<GunnsBasicLink*>&     networkLinks,
                                  const int                         port0,
                                  const int                         port1)
{
    /// - Initialize the parent class.
    GunnsFluidConductor::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag          = false;

    /// - Validate input data.
    validate(inputData);

    /// - Initialize from config data.
    mSelfSealing       = configData.mSelfSealing;

    /// - Initialize from input data.
    mMalfLeakRateFlag  = inputData.mMalfLeakRateFlag;
    mMalfLeakRateValue = inputData.mMalfLeakRateValue;

    /// - Initialize state data.
    mConnection        = 0;
    mHasConnection     = false;
    mPreviousLeakRate  = 0.0;
    mLeakConductivity  = 0.0;

    /// - Set init flag on successful validation.
    mInitFlag          = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] inputData (--) Link input data.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Validates the socket input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSocket::validate(const GunnsFluidSocketInputData& inputData) const
{
    /// - Throw an exception if leak-thru malfunction value is < 0.
    if (inputData.mMalfLeakRateValue < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Malfunction leak rate < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSocket::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  During load initialization, there is an order dependency between GunnsFluidSocket and
///           GunnsFluidJumper objects.  The socket links must be initialized before the jumper
///           links, so that the sockets can return the proper nodes when the jumpers make their
///           initial socket requests.  To ensure that the order is correct, requestConnection and
///           requestDisconnection methods call this method before proceeding.  This method checks
///           the link's initialization flag and if it hasn't been set, an exception is thrown.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSocket::checkInitialization() const
{
    if (!mInitFlag) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Invalid Initialization Data",
                TsInitializationException,
                "attempted processing before initialization was completed.", "Unknown Socket");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] plug (--) Pointer to the plug requesting the connection.
///
/// @returns  (--) Index of the Node to connect to.
///
/// @throws   TsInvalidStateTransitionException
///
/// @details  Processes a request from a GUNNS jumper plug to connect to the GUNNS Basic Node this
///           GUNNS Fluid Socket link is connected to on port 0.  If the request is granted (there
///           is not another plug already connected), the index of the port 0 Node is returned, and
///           the socket stores the address of the plug as its active connection, otherwise an
///           exception is thrown which will cause the jumper to fail to connect to the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsFluidSocket::requestConnection(const GunnsFluidJumperPlug* plug)
{
    checkInitialization();

    int result = mNodeMap[0];

    if (mConnection) {
        GUNNS_WARNING("rejected " << plug->getName() << "'s connection request because "
                << mConnection->getName() << " is already connected here.");
        throw TsInvalidStateTransitionException("Invalid Connection Request",
                                                "GunnsFluidSocket::requestConnection",
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
///           this GUNNS Fluid Socket link is connected to on port 0.  The index of the ambient node
///           is always returned, so an unconnected plug may be moved from one ambient node to
///           another.  The socket's connection pointer is only reset to null if the plug was
///           the one actually connected.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsFluidSocket::requestDisconnection(const GunnsFluidJumperPlug* plug)
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
/// @details  Updates the socket's conductivity based on connection state, leak-thru malfunction and
///           whether it is self-sealing.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSocket::updateState(const double)
{
    /// - If a connection to a jumper is made, or if there is no connection but the socket is self-
    ///   sealing, then the socket is considered sealed and blocks flow.  When not sealed, the
    ///   socket acts just like a regular fluid conductor.
    if (mConnection or mSelfSealing) {

        /// - Determine new conductivity of the leak-thru when the malfunction rate value changes.
        if (mPreviousLeakRate != mMalfLeakRateValue) {
            mLeakConductivity  = GunnsFluidUtils::predictConductivity(
                                     mMalfLeakRateValue, mMinLinearizationPotential,
                                     mNodes[0]->getContent(), mNodes[1]->getContent());
            mPreviousLeakRate  = mMalfLeakRateValue;
        }

        /// - When the socket is sealed, conductivity will be zero unless the leak malfunction is
        ///   active.  The leak conductivity is limited to the normal hose max.
        if (mMalfLeakRateFlag) {
            mEffectiveConductivity = std::min(mLeakConductivity, mMaxConductivity);
        } else {
            mEffectiveConductivity = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--)   Malfunction activation flag, true activates
/// @param[in] value (kg/s) Initial malfunction leak rate
///
/// @details  Sets the malf leak rate flag and value to given parameters.  Calling this method with
///           default arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSocket::setMalfLeakRate(const bool flag, const double value)
{
    mMalfLeakRateFlag  = flag;
    mMalfLeakRateValue = value;
}
