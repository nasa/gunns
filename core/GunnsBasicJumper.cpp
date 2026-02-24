/**
@file
@brief    GUNNS Basic Jumper Link implementation

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
    (Provides the classes for modeling the GUNNS Basic Jumper.)

 REQUIREMENTS:
    ()

 REFERENCE:
    ()

 ASSUMPTIONS AND LIMITATIONS:
    ()

LIBRARY DEPENDENCY:
    ((GunnsBasicJumperPlug.o))

 PROGRAMMERS:
    ((Jason Harvey) (L-3 Communications) (2012-08) (Initial))
*/
#include "GunnsBasicJumper.hh"
#include "GunnsBasicSocket.hh"
#include "GunnsBasicJumperPlug.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                (--) Link name.
/// @param[in] nodes               (--) Network nodes array.
/// @param[in] defaultConductivity (--) Default conductivity of the link.
/// @param[in] plug0               (--) Pointer to jumper plug for port 0.
/// @param[in] plug1               (--) Pointer to jumper plug for port 1.
/// @param[in] selfSealing0        (--) Plug 0 self-seals when un-plugged.
/// @param[in] selfSealing1        (--) Plug 1 self-seals when un-plugged.
///
/// @details  Constructs the Basic Jumper Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicJumperConfigData::GunnsBasicJumperConfigData(const std::string&    name,
                                                       GunnsNodeList*        nodes,
                                                       const double          defaultConductivity,
                                                       GunnsBasicJumperPlug* plug0,
                                                       GunnsBasicJumperPlug* plug1,
                                                       const bool            selfSealing0,
                                                       const bool            selfSealing1)
    :
    GunnsBasicConductorConfigData(name, nodes, defaultConductivity),
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
/// @details  Copy Constructs the Basic Jumper Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicJumperConfigData::GunnsBasicJumperConfigData(const GunnsBasicJumperConfigData& that)
    :
    GunnsBasicConductorConfigData(that),
    mPlug0       (that.mPlug0),
    mPlug1       (that.mPlug1),
    mSelfSealing0(that.mSelfSealing0),
    mSelfSealing1(that.mSelfSealing1)
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Jumper Config Data Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicJumperConfigData::~GunnsBasicJumperConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag.
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1).
/// @param[in] connection0       (--) Index of the initial plug 0 socket.
/// @param[in] connection1       (--) Index of the initial plug 1 socket.
///
/// @details  Default constructs this Basic Jumper input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicJumperInputData::GunnsBasicJumperInputData(const bool   malfBlockageFlag,
                                                     const double malfBlockageValue,
                                                     const int    connection0,
                                                     const int    connection1)
    :
    GunnsBasicConductorInputData(malfBlockageFlag, malfBlockageValue),
    mConnection0(connection0),
    mConnection1(connection1)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Basic Jumper input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicJumperInputData::GunnsBasicJumperInputData(const GunnsBasicJumperInputData& that)
    :
    GunnsBasicConductorInputData(that),
    mConnection0(that.mConnection0),
    mConnection1(that.mConnection1)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Basic Jumper input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicJumperInputData::~GunnsBasicJumperInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Basic Jumper Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicJumper::GunnsBasicJumper()
    :
    GunnsBasicConductor(),
    mSelfSealing0(false),
    mSelfSealing1(false)
{
    mPlug[0] = 0;
    mPlug[1] = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Jumper Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicJumper::~GunnsBasicJumper()
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
/// @details  Initializes the Basic Jumper with config and input data.
//////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicJumper::initialize(const GunnsBasicJumperConfigData& configData,
                                  const GunnsBasicJumperInputData&  inputData,
                                  std::vector<GunnsBasicLink*>&     networkLinks,
                                  const int                         port0,
                                  const int                         port1)
{
    /// - Initialize the parent class.
    GunnsBasicConductor::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag     = false;

    /// - Initialize from config data.
    mPlug[0]      = configData.mPlug0;
    mPlug[1]      = configData.mPlug1;
    mSelfSealing0 = configData.mSelfSealing0;
    mSelfSealing1 = configData.mSelfSealing1;

    /// - Initialize the plugs if they are present, and the resulting node connections.
    const int nodes[2]       = {port0, port1};
    const int connections[2] = {inputData.mConnection0, inputData.mConnection1};
    for (int port = 0; port < 2; ++port) {
        if (mPlug[port]) {
            setPort(port, mPlug[port]->initialize(nodes[port], connections[port]));
        }
    }

    /// - Set init flag on successful validation.
    mInitFlag     = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicJumper::restartModel()
{
    /// - Reset the base class.
    GunnsBasicConductor::restartModel();

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
void GunnsBasicJumper::updateState(const double dt __attribute__((unused)))
{
    updatePlugs();

    /// - If either of the jumper ends have plugs that are disconnected and self-sealing, then the
    ///   entire jumper is considered sealed and blocks flow.
    bool sealed =
            ( (mPlug[0] and mSelfSealing0 and
               mPlug[0]->getNoConnection() == mPlug[0]->getActiveConnection()) ) or
            ( (mPlug[1] and mSelfSealing1 and
               mPlug[1]->getNoConnection() == mPlug[1]->getActiveConnection()) );

    /// - We also force jumper conductivity to zero if both ports are on the same node.  This is
    ///   necessary since the jumper bypasses the normal port rule of not allowing multiple ports
    ///   connected to the same node, and placing a conductivity between the same node corrupts the
    ///   admittance matrix.
    if (sealed or (mNodeMap[0] == mNodeMap[1])) {
        mEffectiveConductivity = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @throws   TsOutOfBoundsException
///
/// @details  Updates the plugs and maps link ports to the connected nodes if they have changed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicJumper::updatePlugs()
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
