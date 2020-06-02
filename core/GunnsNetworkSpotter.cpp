/**
@file
@brief     GUNNS Network Spotter implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o))
*/

#include "GunnsNetworkSpotter.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
//TODO this is included for the GUNNS_NAME_ERREX macro.  Replace with GunnsMacro.hh.
#include "GunnsBasicNode.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name  (--)  Instance name for self-identification in messages.
///
/// @details  Default constructs this GUNNS Network Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsNetworkSpotterConfigData::GunnsNetworkSpotterConfigData(const std::string& name)
    :
    mName(name)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Network Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsNetworkSpotterConfigData::~GunnsNetworkSpotterConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Network Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsNetworkSpotterInputData::GunnsNetworkSpotterInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Network Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsNetworkSpotterInputData::~GunnsNetworkSpotterInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Network Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsNetworkSpotter::GunnsNetworkSpotter()
    :
    mName    (),
    mInitFlag(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Network Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsNetworkSpotter::~GunnsNetworkSpotter()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Pointer to configuration data object.
/// @param[in]  inputData   (--)  Pointer to input data object.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Network Spotter with its config and input data.  This method
///           should be called by the derived class initialize method, which is in turned called by
///           the GUNNS network class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsNetworkSpotter::initialize(const GunnsNetworkSpotterConfigData* configData,
                                     const GunnsNetworkSpotterInputData*  inputData)
{
    /// - Reset the initialization complete flag.
    mInitFlag = false;

    if (!configData) {
        TsHsMsg msg(TS_HS_ERROR, TS_HS_GUNNS);
        msg << "throwing " << "TsInitializationException" << " " << "Invalid Configuration Data" << " - " << "Null config data pointer."  << '\n' << tsStackTrace();
        hsSendMsg(msg);
        throw TsInitializationException("Invalid Configuration Data", "GunnsNetworkSpotter", "Null config data pointer.");
    }

    if (!inputData) {
        TsHsMsg msg(TS_HS_ERROR, TS_HS_GUNNS);
        msg << "throwing " << "TsInitializationException" << " " << "Invalid Input Data" << " - " << "Null input data pointer."  << '\n' << tsStackTrace();
        hsSendMsg(msg);
        throw TsInitializationException("Invalid Input Data", "GunnsNetworkSpotter", "Null input data pointer.");
    }

    /// - Initialize instance name from the argument.
    GUNNS_NAME_ERREX("GunnsNetworkSpotter", configData->mName);

    /// - Set the initialization complete flag.
    mInitFlag = true;
}
