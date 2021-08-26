/*
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (See UserLoadBase.hh)

 REQUIREMENTS:
 ()

 REFERENCE:
 ()

 ASSUMPTIONS AND LIMITATIONS:
 ()

 LIBRARY DEPENDENCY:
 (
     (UserLoadBase.o)
     (simulation/hs/TsHsMsg.o)
     (software/exceptions/TsInitializationException.o)
 )

 PROGRAMMERS:
 (
     (Shailaja Janapati) (L-3) (Initial) (October 2011)
 )
 */
#include "UserLoadBase.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include <iostream>
#include <cstring>
#include <string>
#include <cfloat>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Max Resistance
////////////////////////////////////////////////////////////////////////////////////////////////////
const double UserLoadBase::MAXIMUM_RESISTANCE = 1.0E8;
const double UserLoadBase::MINIMUM_RESISTANCE = 0.1;
const double UserLoadBase::DEFAULT_RESISTANCE = 1000000.0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name              (--)  Name of the User Load object for fault messaging
/// @param[in] loadType          (--)  Type of the user load.
/// @param[in] underVoltageLimit (V)   Minimum Under voltage limit set.
/// @param[in] fuseCurrentLimit  (amp) Current above which the fuse blows.
///
/// @details  Constructs the UserLoadBaseConfigData.
////////////////////////////////////////////////////////////////////////////////////////////////////
UserLoadBaseConfigData::UserLoadBaseConfigData(const std::string &name,
                                               const int         loadType,
                                               const double      underVoltageLimit,
                                               const double      fuseCurrentLimit)
    :
    mName(name),
    mUserLoadType(loadType),
    mUnderVoltageLimit(underVoltageLimit),
    mFuseCurrentLimit(fuseCurrentLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    that  --  Object to copy
///
/// @details  Copy constructs the UserLoadBaseConfigData.
////////////////////////////////////////////////////////////////////////////////////////////////////
UserLoadBaseConfigData::UserLoadBaseConfigData (const UserLoadBaseConfigData& that)
    :
    mName(that.mName),
    mUserLoadType(that.mUserLoadType),
    mUnderVoltageLimit(that.mUnderVoltageLimit),
    mFuseCurrentLimit(that.mFuseCurrentLimit)
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the UserLoadBase Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
UserLoadBaseConfigData::~UserLoadBaseConfigData()
{
    // nothing to do
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] lOverwritePowerFlag (--) Flag to overwrite the Load power value
/// @param[in] lOverwritePower     (--) Overwrite power value
/// @param[in] loadOperMode        (--) user load is set to one of these modes - ON/OFF/STANDBY
/// @param[in] initialVoltage      (V)  Initial input voltage to the user load from the power supply.
///
/// @details  Constructs the UserLoadBase Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
UserLoadBaseInputData::UserLoadBaseInputData(const bool   lMalfOverrideCurrentFlag,
                                             const double lMalfOverrideCurrentValue,
                                             const int    loadOperMode,
                                             const double initialVoltage)
    :
    mMalfOverrideCurrentFlag(lMalfOverrideCurrentFlag),
    mMalfOverrideCurrentValue(lMalfOverrideCurrentValue),
    mLoadOperMode(loadOperMode),
    mInitialVoltage(initialVoltage)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    that  --  Object to copy
///
/// @details  Default User Load constant resistor load Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UserLoadBaseInputData::UserLoadBaseInputData (const UserLoadBaseInputData& that)
    :
    mMalfOverrideCurrentFlag(that.mMalfOverrideCurrentFlag),
    mMalfOverrideCurrentValue(that.mMalfOverrideCurrentValue),
    mLoadOperMode(that.mLoadOperMode),
    mInitialVoltage(that.mInitialVoltage)
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the UserLoadBase Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
UserLoadBaseInputData::~UserLoadBaseInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    mName                 --  Name of the User Load object for fault messaging
/// @param    mCurrent          -- The total current calculated
/// @param    mActualPower      --  The Actual Power through the load
/// @param    mEquivalentResistance    --  The actual resistance for the load
/// @param    mUnderVoltage     --  Minimum Under voltage limit set.
/// @param    mLoadOperMode         --  enum to verify the mode of operation of the load.
/// @param    mUserLoadType     --  Type of the user load.
///
/// @details  Default constructs this UserLoadBase object with default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
UserLoadBase::UserLoadBase():
                            mMalfOverrideCurrentFlag(false),
                            mMalfOverrideCurrentValue(0.0),
                            mMalfOverridePowerFlag(false),
                            mMalfOverridePower(0.0),
                            mMalfBlowFuse(false),
                            mMagicPowerFlag(false),
                            mMagicPowerValue(120.123),
                            mNameLoad("Load"),
                            mPrettyNameLoad("Load"),
                            mCardId(0),
                            mLoadSwitchId(0),
                            mCurrent(0),
                            mActualPower(0),
                            mLoadOperMode(LoadON),
                            mUserLoadType(RESISTIVE_LOAD),
                            mEquivalentResistance(MAXIMUM_RESISTANCE),
                            mVoltage(0),
                            mUnderVoltageLimit(98),
                            mFuseCurrentLimit(0.0),
                            mFuseIsBlown(false),
                            mPowerValid(true),
                            mInitFlag(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this UserLoadBase object.
////////////////////////////////////////////////////////////////////////////////////////////////////
UserLoadBase::~UserLoadBase()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param  &theStringVector  [out]  vector of the split up string
/// @param  &theString        [in]   the string to be split up
/// @param  &theDelimiter     [in]   the character to split the string up by
///
/// @details utility function to tokenize a string based on an arbitrary delimiter
////////////////////////////////////////////////////////////////////////////////////////////////////
void UserLoadBase::tokenize(std::vector<std::string>& theStringVector,
                            const std::string&        theString,
                            const std::string&        theDelimiter) {
    size_t start = 0, end = 0;

    while (end != std::string::npos) {
        end=theString.find(theDelimiter, start);

        theStringVector.push_back(theString.substr(start, (end == std::string::npos) ? std::string::npos : end - start));

        start = ((end > std::string::npos - theDelimiter.size()) ? std::string::npos : end + theDelimiter.size());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    configData  -- Input configuration data
/// @param    loadsVector -- Input network loads vector. The loads object is pushed to the vector stack.
/// @param    cardId      -- The Load Switch card on which this User Load is part of
/// @param    loadSwitchId -- The id for this user load.
///
///
/// @details  Initializes this UserLoadBase object with configuration data, under voltage trip limit.
///           Allocates memory for the load vectors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UserLoadBase::initialize(const UserLoadBaseConfigData &configData,
        const UserLoadBaseInputData &inputData,
        std::vector<UserLoadBase*>&  networkLoads,
        const int cardId)
{
    // validate the config data before initialization
    validate(configData, inputData);

    /// - Initialize from config data.
    mUserLoadType = configData.mUserLoadType;
    mUnderVoltageLimit = configData.mUnderVoltageLimit;
    mFuseCurrentLimit = configData.mFuseCurrentLimit;

    //TODO all this mCardId, mNameLoad and mPrettyNameLoad stuff is very TS21-
    // specific and should be removed....

    // Initialize load card number and switch number.
    // This data is set from the config data. It is
    // used by the client of the class - eg: LoadSwitchCard class
    mCardId = cardId;
   // mLoadSwitchId = loadSwitchId;

    /// - Initialize load name from config data- (we must do this to checkpoint the name)
    //TS_STRDUP(mName, const_cast<char *>(configData.mName.c_str()));

    mNameLoad= configData.mName;

    // split up the name by underscores
    std::vector<std::string> lParsedName;
    tokenize(lParsedName, mNameLoad, "_");

    // now piece together everything after the 1st 5 parts
    std::string lShortName;

    for (unsigned int i=5; i < lParsedName.size(); i++) {
        lShortName = lShortName + lParsedName[i] + " ";
    }

    // quick and dirty test to try to catch unassigned RPCs and blank out the name.  For appearances.
    if (lShortName == "Undefined ") {
        lShortName = " ";
    }

    mPrettyNameLoad = lShortName;

    // Initialize input data
    mMalfOverrideCurrentFlag  = inputData.mMalfOverrideCurrentFlag;
    mMalfOverrideCurrentValue = inputData.mMalfOverrideCurrentValue;
    mLoadOperMode             = inputData.mLoadOperMode;
    mVoltage                  = inputData.mInitialVoltage;
    mPowerValid               = mVoltage >= mUnderVoltageLimit;

    // Initialize the current & power values to zero
    mCurrent = 0.0;
    mEquivalentResistance = MAXIMUM_RESISTANCE;
    mActualPower = 0.0;

    /// - Add this load to the network loads.
    networkLoads.push_back(this);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    configData  -- Input configuration data
///
///
/// @details  Validates this UserLoadBase configuration data for load name, operation mode and load type.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
void UserLoadBase::validate(const UserLoadBaseConfigData &configData,
                            const UserLoadBaseInputData &inputData)
{
    std::string exceptionName("Invalid Initialization Data");
    std::string cause("UserLoadBase::validate");

    /// @throws Throw exception if user load name is invalid
    if ("" == configData.mName) {
        std::string msg = "Load: name is not set.";

        hsSendMsg(TS_HS_ERROR, "EPS", msg.c_str());
        throw TsInitializationException(msg.c_str(), exceptionName, cause); // correct signature of this object
    }

    /// @throws Throw exception if load type set incorrectly.
    if ((RESISTIVE_LOAD != configData.mUserLoadType) && (CONSTANT_POWER_LOAD != configData.mUserLoadType)) {
        std::string msg = "Load: " + configData.mName + " has invalid load type.";

        hsSendMsg(TS_HS_ERROR, "EPS", msg.c_str());
        throw TsInitializationException(msg.c_str(), // message
                exceptionName, // thrower
                cause);  // root cause
    }

    /// @throws Throw exception if load operation mode set incorrectly.
    if ((LoadON != inputData.mLoadOperMode) && (LoadSTANDBY != inputData.mLoadOperMode)
            && (LoadOFF != inputData.mLoadOperMode)) {
        std::string msg = "Load: " + configData.mName + "Operating Mode is unset.";

        hsSendMsg(TS_HS_ERROR, "EPS", msg.c_str());
        throw TsInitializationException(exceptionName, cause, msg.c_str());
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    voltage  -- Input voltage data for the step method
///
///
/// @details  Step this UserLoadBase object. Assign the voltage and power valid values.
///           Check for under voltage limit. Initialize the current, actual power and
///           equivalent resistance members.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UserLoadBase::step(double voltage) {

    // Initialize the current and resistance values
    mCurrent = 0.0;
    mActualPower = 0.0;
    mEquivalentResistance = MAXIMUM_RESISTANCE;

    if (mMalfBlowFuse) {
        mFuseIsBlown = true;
    }

    // factor in magic power override
    if (mMagicPowerFlag) {
        mVoltage = mMagicPowerValue;
    } else if (mFuseIsBlown) {
        mVoltage = 0.0;
    } else {
        mVoltage = voltage;
    }

    // set power valid to true if voltage is > than underVoltage limit
    mPowerValid = mVoltage > mUnderVoltageLimit;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--)  Malfunction activation flag, true activates.
/// @param[in] value (amp) Malfunction current value.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UserLoadBase::setMalfOverrideCurrent(const bool flag, const double value)
{
    mMalfOverrideCurrentFlag  = flag;
    mMalfOverrideCurrentValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
/// @param[in] value (W)  Malfunction power value.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UserLoadBase::setMalfOverridePower(const bool flag, const double value)
{
    mMalfOverridePowerFlag = flag;
    mMalfOverridePower     = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] voltage (V) Input voltage to the load.
///
/// @returns  bool (--) True if the fuse has blown during this update.
///
/// @details  Determines whether the given input voltage causes the fuse to blow, based on the
///           equivalent load resistance as I = V/R.  Sets the fuse blown state if the fuse limit
///           is exceeded, and returns whether the fuse has blown during this update.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool UserLoadBase::updateFuse(const double voltage)
{
    if (mFuseCurrentLimit > 0.0 and not mFuseIsBlown) {
        const double current = voltage / std::max(mEquivalentResistance, DBL_EPSILON);
        if (current > mFuseCurrentLimit) {
            mFuseIsBlown = true;
            return true;
        }
    }
    return false;
}
