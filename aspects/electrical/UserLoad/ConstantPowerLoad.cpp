/**
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
     (aspects/electrical/UserLoad/UserLoadBase.o)
     (aspects/electrical/UserLoad/ConstantPowerLoad.o)
     (simulation/hs/TsHsMsg.o)
     (software/exceptions/TsInitializationException.o)
     (software/exceptions/TsNumericalException.o)
    )
*/

#include <cmath>
#include <cfloat>
#include <ostream>
#include "aspects/electrical/UserLoad/ConstantPowerLoad.hh"
#include "aspects/electrical/UserLoad/UserLoadBase.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsNumericalException.hh"


//=================================================================================================
//
// BEGIN CLASS ConstantPowerLoadConfigData
//
//=================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name              (--)  user load name
/// @param[in] loadType          (--)  type of user load constant resistance or constant power
/// @param[in] underVoltageLimit (V)   lower limit for the voltage at which the voltage trips
/// @param[in] powerNormal       (W)   power for this user load when operating in the normal or ON mode
/// @param[in] powerStandby      (W)   power for this user load when operating in the Standby mode
/// @param[in] loadOperMode      (--)  flag to verify if the load is set to ON
/// @param[in] fuseCurrentLimit  (amp) Current above which the fuse blows.
///
/// @details  Default User Load constant power load Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
ConstantPowerLoadConfigData::ConstantPowerLoadConfigData(const std::string& name,
                                                         const int          loadType,
                                                         const double       underVoltageLimit,
                                                         const double       powerNormal,
                                                         const double       powerStandby,
                                                         const double       fuseCurrentLimit)
     :
     UserLoadBaseConfigData(name, loadType, underVoltageLimit, fuseCurrentLimit),
     mPowerNormal(powerNormal),
     mPowerStandby(powerStandby)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    that  --  Object to copy
///
/// @details  Default User Load constant power load Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
ConstantPowerLoadConfigData::ConstantPowerLoadConfigData (const ConstantPowerLoadConfigData& that)
:UserLoadBaseConfigData(that),
 mPowerNormal(that.mPowerNormal),
 mPowerStandby(that.mPowerStandby)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @details  Default User Load constant resistor load Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
ConstantPowerLoadConfigData::~ConstantPowerLoadConfigData()
{
    // nothing to do
}

//=================================================================================================
//
// BEGIN CLASS ConstantPowerLoadInputData
//
//=================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] lOverwritePowerFlag (--) Flag to overwrite the Load power value
/// @param[in] lOverwritePower     (--) Overwrite power value
/// @param[in] loadOperMode        (--) user load is set to one of these modes - ON/OFF/STANDBY
/// @param[in] initialVoltage      (v)  Initial input voltage to the user load from the power supply.
///
/// @details  Default User Load constant resistor load Input Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
ConstantPowerLoadInputData::ConstantPowerLoadInputData(const bool   lMalfOverrideCurrentFlag,
                                                       const double lMalfOverrideCurrentValue,
                                                       const int    loadOperMode,
                                                       const double initialVoltage)
    :
    UserLoadBaseInputData(lMalfOverrideCurrentFlag, lMalfOverrideCurrentValue,
                          loadOperMode, initialVoltage)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    that  --  Object to copy
///
/// @details  Default User Load constant resistor load Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
ConstantPowerLoadInputData::ConstantPowerLoadInputData(const ConstantPowerLoadInputData& that)
    :
    UserLoadBaseInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @details  Default User Load constant resistor load Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
ConstantPowerLoadInputData::~ConstantPowerLoadInputData()
{
    // nothing to do
}


//=================================================================================================
//
// BEGIN CLASS ConstantPowerLoad
//
//=================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @details   Default Constant Power User Load Electrical Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
ConstantPowerLoad::ConstantPowerLoad():
		 UserLoadBase(),
		 mPowerNormal(0.0),
		 mPowerStandby(0.0)
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @details  Default Constant Power User Load Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
ConstantPowerLoad::~ConstantPowerLoad()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    configData    --  Constant Resistance User Load Electric Config Data
/// @param[in]    networkLoads  --  User load vector to collect all the user loads
/// @param[in]    cardId         --  Switch Card ID on which this user load is configured
/// @param[in]    loadSwitchId   --  User Load ID.
///
/// @return   void
/// @details  Initializes this Constant Power User Load Electrical with configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConstantPowerLoad::initialize(const ConstantPowerLoadConfigData& configData,
        const ConstantPowerLoadInputData& inputData,
        std::vector<UserLoadBase *>&  networkLoads,
        const int   cardId,
        const int   loadSwitchId)
{
    /// - Reset init flag.
    mInitFlag = false;

    /// - Validate initialization data.
    validate(configData);
    mLoadSwitchId = loadSwitchId;

    /// - Initialize and validate parent
    UserLoadBase::initialize(configData, inputData, networkLoads, cardId);

    // set power data
    mPowerNormal = configData.mPowerNormal;
    mPowerStandby = configData.mPowerStandby;

    /// - Set init flag on successful initialization.
    mInitFlag = true;
}




////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    configData  --  Constant Power Load Electric Config Data
///
/// @throws   TsInitializationException
///
/// @details  Validates this Constant Power Load Electrical initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConstantPowerLoad::validate(const ConstantPowerLoadConfigData& configData)
{
    /// - Throw exception on range < 0 or > max.
    if (configData.mPowerNormal < 0.0 || configData.mPowerNormal > 1.0E7 ) {
        std::ostringstream msg;
        msg << "ConstantPowerLoad::validate - Power of "<< configData.mPowerNormal << " for Normal Load is < 0 or > maximum power.";
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_EPS, msg.str(),
                        TsInitializationException, "Invalid Initialization Data", mNameLoad);
    }

    /// - Throw exception on range < 0 or > max.
    if (configData.mPowerStandby < 0.0 || configData.mPowerStandby > 1.0E7 ) {
        std::ostringstream msg;
        msg << "ConstantPowerLoad::validate - Power of "<< configData.mPowerStandby << " for Standby Load is < 0 or > maximum power.";
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_EPS, msg.str(),
                        TsInitializationException, "Invalid Initialization Data", mNameLoad);
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   voltage  (v)  voltage at each time step
///
/// @details Method to update the user load during a time step. For a constant power load
/// the input parameter is not "dt" but the voltage at which the load operates.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConstantPowerLoad::step(const double voltage)
{
    /// call the parent step function
    UserLoadBase::step(voltage);

    /// --  update load values
    updateLoad();

    try {
        // calculate the load due to this resistive load
        calculateConstantPowerLoad();
    } catch (TsNumericalException &e) {
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_EPS, e.getMessage().c_str(),
                        TsNumericalException, "ConstantPowerLoad::step", mNameLoad);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @details Method to compute the current and resistance based on the voltage supplied.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConstantPowerLoad::calculateConstantPowerLoad()
{
    if (!mPowerValid) {
        return;
    }

    if (mMalfOverrideCurrentFlag) {
        if (0.0 <= mMalfOverrideCurrentValue) {
            mActualPower = mVoltage * mMalfOverrideCurrentValue;
            computeActualPower();
        } else { // current value can't be less than zero.
            TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_EPS, " Tried to set override Current less than 0.0, expects > 0.0.",
                                TsNumericalException, "ConstantPowerLoad::calculateConstantPowerLoad", mNameLoad);
        }
    } else if (mMalfOverridePowerFlag) {
        if (0.0 <= mMalfOverridePower) {
            mActualPower = mMalfOverridePower;
            computeActualPower();
        } else {
            TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_EPS, " Tried to set override Power less than 0.0, expects > 0.0.",
                                TsNumericalException, "ConstantPowerLoad::calculateConstantPowerLoad", mNameLoad);
        }
    } else if (mLoadOperMode > LoadOFF){
        switch(mLoadOperMode) {
        case LoadON:  // In Normal Operation
            mActualPower = mPowerNormal;
            break;
        case LoadSTANDBY:  // In Standby Operation
            mActualPower = mPowerStandby;
            break;
        }
        computeActualPower();
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////
///
///// @details Method: setPower - calculate power for the Equivalent resistance.
/////////////////////////////////////////////////////////////////////////////////////////////////
void ConstantPowerLoad::computeActualPower() {
    // voltage value is greater than zero - powerValid flag is true.
    mCurrent = mActualPower / mVoltage;

    if (0.0 < mActualPower) { // check to eliminate potential divide by zero error
        const double limitVoltage = std::max(mVoltage, 0.99 * mUnderVoltageLimit);
        mEquivalentResistance = limitVoltage*limitVoltage /mActualPower;

    } else if (0.0 > mActualPower){ // if the power is less than zero set resistance to maximum value
        mEquivalentResistance = MAXIMUM_RESISTANCE;
        std::ostringstream msg;
        msg << "Actual power value " <<  mActualPower  << " is less than zero, expected a value greater than or equal to zero.";
        TS_HS_EXCEPTION(TS_HS_ERROR, "EPS", msg.str(),
                TsNumericalException, "ConstantPowerLoad::calculateConstantPowerLoad", mNameLoad);
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @details Method: updateLoad - method to be overwritten by the derived classes. Keep this method
///    empty so derived classes can add functionality as needed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConstantPowerLoad::updateLoad(){
    //update power.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @details Method: setPowerNormal - method to set the power for normal ops.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConstantPowerLoad::setPowerNormal(const double power){
    mPowerNormal = fmax(0.0, power);
}





