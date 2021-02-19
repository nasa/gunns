/**
@file
@brief    GUNNS Fluid Jumper Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Provides the classes for modeling the GUNNS Fluid Jumper.)

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
#include "GunnsFluidJumper.hh"
#include "GunnsFluidSocket.hh"
#include "GunnsFluidJumperPlug.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                 (--) Link name.
/// @param[in] nodes                (--) Network nodes array.
/// @param[in] maxConductivity      (m2) Maximum possible effective conductivity of the link.
/// @param[in] expansionScaleFactor (--) Scale factor for isentropic cooling across the link (0-1).
/// @param[in] plug0                (--) Pointer to jumper plug for port 0.
/// @param[in] plug1                (--) Pointer to jumper plug for port 1.
/// @param[in] selfSealing0         (--) Plug 0 self-seals when un-plugged.
/// @param[in] selfSealing1         (--) Plug 1 self-seals when un-plugged.
///
/// @details  Constructs the Fluid Jumper Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidJumperConfigData::GunnsFluidJumperConfigData(const std::string&    name,
                                                       GunnsNodeList*        nodes,
                                                       const double          maxConductivity,
                                                       const double          expansionScaleFactor,
                                                       GunnsFluidJumperPlug* plug0,
                                                       GunnsFluidJumperPlug* plug1,
                                                       const bool            selfSealing0,
                                                       const bool            selfSealing1)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, expansionScaleFactor),
    mPlug0       (plug0),
    mPlug1       (plug1),
    mSelfSealing0(selfSealing0),
    mSelfSealing1(selfSealing1)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from.
///
/// @details  Copy Constructs the Fluid Jumper Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidJumperConfigData::GunnsFluidJumperConfigData(const GunnsFluidJumperConfigData& that)
    :
    GunnsFluidConductorConfigData(that),
    mPlug0       (that.mPlug0),
    mPlug1       (that.mPlug1),
    mSelfSealing0(that.mSelfSealing0),
    mSelfSealing1(that.mSelfSealing1)
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Fluid Jumper Config Data Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidJumperConfigData::~GunnsFluidJumperConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--)   Blockage malfunction flag.
/// @param[in] malfBlockageValue (--)   Blockage malfunction fractional value (0-1).
/// @param[in] malfLeakRateFlag  (--)   Leak-thru malfunction flag.
/// @param[in] malfLeakRateValue (kg/s) Leak-thru malfunction value.
/// @param[in] connection0       (--)   Index of the initial plug 0 socket.
/// @param[in] connection1       (--)   Index of the initial plug 1 socket.
///
/// @details  Default constructs this Fluid Jumper input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidJumperInputData::GunnsFluidJumperInputData(const bool   malfBlockageFlag,
                                                     const double malfBlockageValue,
                                                     const bool   malfLeakRateFlag,
                                                     const double malfLeakRateValue,
                                                     const int    connection0,
                                                     const int    connection1)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mMalfLeakRateFlag (malfLeakRateFlag),
    mMalfLeakRateValue(malfLeakRateValue),
    mConnection0      (connection0),
    mConnection1      (connection1)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Fluid Jumper input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidJumperInputData::GunnsFluidJumperInputData(const GunnsFluidJumperInputData& that)
    :
    GunnsFluidConductorInputData(that),
    mMalfLeakRateFlag (that.mMalfLeakRateFlag),
    mMalfLeakRateValue(that.mMalfLeakRateValue),
    mConnection0      (that.mConnection0),
    mConnection1      (that.mConnection1)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Jumper input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidJumperInputData::~GunnsFluidJumperInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Fluid Jumper Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidJumper::GunnsFluidJumper()
    :
    GunnsFluidConductor(),
    mMalfLeakRateFlag(false),
    mMalfLeakRateValue(0.0),
    mSelfSealing0(false),
    mSelfSealing1(false),
    mPreviousLeakRate(0.0),
    mLeakConductivity(0.0)
{
    mPlug[0] = 0;
    mPlug[1] = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Fluid Jumper Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidJumper::~GunnsFluidJumper()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data.
/// @param[in]     inputData    (--) Reference to Link Input Data.
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector.
/// @param[in]     port0        (--) Port 0 Mapping.
/// @param[in]     port1        (--) Port 1 Mapping.
///
/// @throws   TsInitializationException
///
/// @details  Initializes the Fluid Jumper with config and input data.
//////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidJumper::initialize(const GunnsFluidJumperConfigData& configData,
                                  const GunnsFluidJumperInputData&  inputData,
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
    mPlug[0]           = configData.mPlug0;
    mPlug[1]           = configData.mPlug1;
    mSelfSealing0      = configData.mSelfSealing0;
    mSelfSealing1      = configData.mSelfSealing1;

    /// - Initialize from input data.
    mMalfLeakRateFlag  = inputData.mMalfLeakRateFlag;
    mMalfLeakRateValue = inputData.mMalfLeakRateValue;

    /// - Initialize the plugs if they are present, and the resulting node connections.
    const int nodes[2]       = {port0, port1};
    const int connections[2] = {inputData.mConnection0, inputData.mConnection1};
    for (int port = 0; port < 2; ++port) {
        if (mPlug[port]) {
            setPort(port, mPlug[port]->initialize(nodes[port], connections[port]));
        }
    }

    /// - Initialize remaining state data.
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
/// @details  Validates the jumper input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidJumper::validate(const GunnsFluidJumperInputData& inputData) const
{
    /// - Throw an exception if leak-thru malfunction value is < 0.
    if (inputData.mMalfLeakRateValue < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Malfunction leak rate < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidJumper::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();

    /// - Update the plugs during restart.  Even though this happens again first-thing when going to
    ///   run, we also do it here so that a socket or node mapping conflict is identified early.  If
    ///   the plug has a connection in the checkpoint, disconnect then re-connect to that socket.
    ///   If the plug does not have a connection in the checkpoint, disconnect from the socket it
    ///   was last disconnected from in the checkpoint.  These actions ensure that the link port
    ///   ends up mapped to the correct node.
    for (int port = 0; port < 2; ++port) {
        if (mPlug[port]) {
            if (mPlug[port]->getNoConnection() != mPlug[port]->getActiveConnection()) {
                mPlug[port]->setDisconnectionRequest(mPlug[port]->getActiveConnection());
                mPlug[port]->setConnectionRequest   (mPlug[port]->getActiveConnection());
            } else {
                mPlug[port]->setDisconnectionRequest(mPlug[port]->getLastDisconnection());
                mPlug[port]->setConnectionRequest   (mPlug[port]->getNoConnection());
            }
        }
    }
    updatePlugs();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @throws   TsOutOfBoundsException
///
/// @details  Updates the jumper's conductivity based on the plug connection states and whether the
///           plugs are self-sealing.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidJumper::updateState(const double)
{
    updatePlugs();

    /// - If either of the jumper ends have plugs that are disconnected and self-sealing, then the
    ///   entire jumper is considered sealed and blocks flow.
    bool sealed =
            ( (mPlug[0] and mSelfSealing0 and
               mPlug[0]->getNoConnection() == mPlug[0]->getActiveConnection()) ) or
            ( (mPlug[1] and mSelfSealing1 and
               mPlug[1]->getNoConnection() == mPlug[1]->getActiveConnection()) );

    /// - We force jumper conductivity to zero if both ports are on the same node.  This is
    ///   necessary since the jumper bypasses the normal port rule of not allowing multiple ports
    ///   connected to the same node, and placing a conductivity between the same node corrupts the
    ///   admittance matrix.
    if (mNodeMap[0] == mNodeMap[1]) {
        mEffectiveConductivity = 0.0;

    /// - When not sealed, the jumper acts just like a regular fluid conductor.
    } else if (sealed) {

        /// - Determine new conductivity of the leak-thru when the malfunction rate value changes.
        if (mPreviousLeakRate != mMalfLeakRateValue) {
            mLeakConductivity  = GunnsFluidUtils::predictConductivity(
                                     mMalfLeakRateValue, mMinLinearizationPotential,
                                     mNodes[0]->getContent(), mNodes[1]->getContent());
            mPreviousLeakRate  = mMalfLeakRateValue;
        }

        /// - When the jumper is sealed, conductivity will be zero unless the leak malfunction is
        ///   active.  The leak conductivity is limited to the normal hose max.
        if (mMalfLeakRateFlag) {
            mEffectiveConductivity = std::min(mLeakConductivity, mMaxConductivity);
        } else {
            mEffectiveConductivity = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @throws   TsOutOfBoundsException
///
/// @details  Updates the plugs and maps link ports to the connected nodes if they have changed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidJumper::updatePlugs()
{
    for (int port = 0; port < 2; ++port) {
        if (mPlug[port]) {
            int node = mPlug[port]->update(mNodeMap[port]);
            if (node != mNodeMap[port]) {
                setPort(port, node);
            }
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
void GunnsFluidJumper::setMalfLeakRate(const bool flag, const double value)
{
    mMalfLeakRateFlag  = flag;
    mMalfLeakRateValue = value;
}
