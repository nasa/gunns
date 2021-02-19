/************************************** TRICK HEADER ***********************************************
@file     HtrControl.cpp
@brief    HtrControl Model implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (aspects/electrical/UserLoad/UserLoadBase.o)
***************************************************************************************************/
#include "HtrControl.hh"
#include "aspects/thermal/PtcsMacros.hh"
#include <string>
#include <iostream> //needed for std::ostringstream in initialize()

/**************************************************************************************************/
/* ConfigData class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Constructs this HtrControl model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
HtrControlConfigData::HtrControlConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  source to copy
///
/// @details    Copy constructs this HtrControl model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
HtrControlConfigData::HtrControlConfigData(const HtrControlConfigData& that __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this HtrControl model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
HtrControlConfigData::~HtrControlConfigData()
{
    // nothing to do
}


/**************************************************************************************************/
/* InputData class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfCmdOnFlag       (--)  Malfunction flag, heater command on
/// @param[in]  malfCmdOffFlag      (--)  Malfunction flag, heater command off
/// @param[in]  isKillSwitchEngaged (--)  Flag to open thermostat switch & kill the heater
///
/// @details    Default constructs this HtrControl model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
HtrControlInputData::HtrControlInputData(const bool malfCmdOnFlag,
                                         const bool malfCmdOffFlag,
                                         const bool isKillSwitchEngaged)
    :
    iMalfCmdOnFlag(malfCmdOnFlag),
    iMalfCmdOffFlag(malfCmdOffFlag),
    iIsKillSwitchEngaged(isKillSwitchEngaged)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  source to copy
///
/// @details    Copy constructs this HtrControl model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
HtrControlInputData::HtrControlInputData(const HtrControlInputData& that)
    :
    iMalfCmdOnFlag(that.iMalfCmdOnFlag),
    iMalfCmdOffFlag(that.iMalfCmdOffFlag),
    iIsKillSwitchEngaged(that.iIsKillSwitchEngaged)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this HtrControl model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
HtrControlInputData::~HtrControlInputData()
{
    // nothing to do
}


/**************************************************************************************************/
/* HtrControl class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default HtrControl constructor. Defaults all pointers and members to zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
HtrControl::HtrControl()
    :
    mMalfCmdOnFlag(false),
    mMalfCmdOffFlag(false),
    mName(),
    mInitialized(false),
    mIsKillSwitchEngaged(false),
    mCommandActual(LoadOFF),
    mCommandNominal(LoadOFF)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructor, deletes allocated arrays.
////////////////////////////////////////////////////////////////////////////////////////////////////
HtrControl::~HtrControl()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  config  (--)  configuration data
/// @param[in]  input   (--)  input data
/// @param[in]  name    (--)  HtrControl name
///
/// @throw      TsInitializationException in validate() method.
///
/// @details    Initializes this HtrControl model from configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void HtrControl::initialize(const HtrControlConfigData& config __attribute__((unused)),
                            const HtrControlInputData& input,
                            const std::string& name)
{
    /// - Reset the initialization complete flag.
    mInitialized = false;

    /// - Validate and initialize object name.
    TS_PTCS_NAME_ERREX(name);

    /// - Validate the input data.
    validate(input);

    /// - Initialize from the validated input data.
    mMalfCmdOnFlag  = input.iMalfCmdOnFlag;
    mMalfCmdOffFlag = input.iMalfCmdOffFlag;
    mIsKillSwitchEngaged = input.iIsKillSwitchEngaged;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  input   (--)  input data
///
/// @throw      TsInitializationException if config/input parameters not valid.
///
/// @details    Validates this HtrControl model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void HtrControl::validate(const HtrControlInputData& input) const
{
    /// - Throw an exception on a conflicting command.
    TS_PTCS_IF_ERREX(true == input.iMalfCmdOnFlag && true == input.iMalfCmdOffFlag,
            TsInitializationException,
            "Invalid input data", "Malf'd to both ON and OFF.")
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throw      TsInitializationException if HtrControl not initialized.
///
/// @details    Checks for initialization. That's all.
////////////////////////////////////////////////////////////////////////////////////////////////////
void HtrControl::verifyInitialization()
{
    /// - Check for initialization complete.
    TS_PTCS_IF_ERREX( 0 == isInitialized(), TsInitializationException,
            "Invalid update() call", "Update() called w/o proper initialization.")
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  commandStatus  (--)  UserLoadMode status without the Command:ON/OFF malf considered.
///
/// @return     UserLoadMode   (--)  LoadON / LoadOFF command status resulting from consideration of
///                                  Command malfunctions.
///
/// @details    Applies the Command:ON/OFF malfunction if active, otherwise returns the
///             input command status.
///
/// @note       The Command:OFF malfunction takes precedence over the Command:ON malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
UserLoadMode HtrControl::considerCommandMalfs(const UserLoadMode commandStatus)
{
    /// - Create temporary UserLoadMode.
    UserLoadMode result = commandStatus;

    /// - If the Command:OFF malfunction is active...
    if (mMalfCmdOffFlag)
    {
        /// - Set heater command to OFF.
        result = LoadOFF;

    /// - If the Command:ON malfunction is active...
    } else if (mMalfCmdOnFlag)
    {
        /// - Set heater command to ON.
        result = LoadON;

    /// - If no command malfs are active, check the kill switch.
    } else if (mIsKillSwitchEngaged)
    {
        /// - Set heater command to OFF.
        result = LoadOFF;
    }

    /// - If no command override is active, return the status as is.
    return result;
}
