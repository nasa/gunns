/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
    (Provides the classes for modeling the GUNNS Electrical Resistive Load.)

 REQUIREMENTS:
    ()

 REFERENCE:
    ()

 ASSUMPTIONS AND LIMITATIONS:
    ()
LIBRARY DEPENDENCY:
    (
     (aspects/electrical/UserLoad/UserLoadBase.o)
     (simulation/hs/TsHsMsg.o)
     (software/exceptions/TsInitializationException.o)
     (software/exceptions/TsNumericalException.o)
    )

 PROGRAMMERS:
    (
     (Shailaja Janapati) (L3) (10-2011) (Initial Prototype)
    )
 **************************************************************************************************/

#include <cmath>
#include <cfloat>
#include "aspects/electrical/UserLoad/ResistiveLoad.hh"
#include "aspects/electrical/UserLoad/UserLoadBase.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsNumericalException.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "math/MsMath.hh"

//=================================================================================================
//
// BEGIN CLASS ResistiveLoadConfigData
//
//=================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name              (--)  user load name
/// @param[in] loadType          (--)  type of user load resistive or constant power
/// @param[in] underVoltageLimit (V)   lower limit for the voltage at which the voltage trips
/// @param[in] resistanceNormal  (ohm) resistance value for this load when operating in Normal or ON mode
/// @param[in] resistanceStandby (ohm) resistance value for this load when operating in Standby mode
/// @param[in] fuseCurrentLimit  (amp) Current above which the fuse blows.
///
/// @details  Default User Load constant resistor load Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
ResistiveLoadConfigData::ResistiveLoadConfigData(const std::string& name,
                                                 const int          loadType,
                                                 const double       underVoltageLimit,
                                                 const double       resistanceNormal,
                                                 const double       resistanceStandby,
                                                 const double       fuseCurrentLimit)
    :
    UserLoadBaseConfigData(name, loadType, underVoltageLimit, fuseCurrentLimit),
    mResistanceNormal(resistanceNormal),
    mResistanceStandby(resistanceStandby)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    that  --  Object to copy
///
/// @details  Default User Load constant resistor load Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
ResistiveLoadConfigData::ResistiveLoadConfigData(const ResistiveLoadConfigData& that)
    :
    UserLoadBaseConfigData(that),
    mResistanceNormal(that.mResistanceNormal),
    mResistanceStandby(that.mResistanceStandby)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default User Load constant resistor load Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
ResistiveLoadConfigData::~ResistiveLoadConfigData()
{
    // nothing to do
}


//=================================================================================================
//
// BEGIN CLASS ResistiveLoadInputData
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
ResistiveLoadInputData::ResistiveLoadInputData(const bool   lMalfOverrideCurrentFlag,
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
ResistiveLoadInputData::ResistiveLoadInputData(const ResistiveLoadInputData& that)
    :
    UserLoadBaseInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @details  Default User Load constant resistor load Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
ResistiveLoadInputData::~ResistiveLoadInputData()
{
    // nothing to do
}


//=================================================================================================
//
// BEGIN CLASS ResistiveLoad
//
//=================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Default Constant Resistance User Load Electrical Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
ResistiveLoad::ResistiveLoad()
    :UserLoadBase(),
     mResistanceNormal(1.0E6),
     mResistanceStandby(1.0E8),
         mPrintMessageOnce(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Constant Resistance User Load Electrical Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
ResistiveLoad::~ResistiveLoad()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    configData    --  Resistive Load Electrical Config Data
/// @param    networkLoads  --  vector of all user loads to which this object will be added.
/// @param    cardID        --  ID of the load switch card
/// @param    loadSwitchId  --  ID of the current user load object
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Resistive User Load Electrical with configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ResistiveLoad::initialize(const ResistiveLoadConfigData& configData,
        const ResistiveLoadInputData& inputData,
        std::vector<UserLoadBase*>&     networkLoads,
        const int                       cardId,
        const int                       loadSwitchId)
{
    /// Reset init flag.
    mInitFlag = false;

    /// Validate initialization data.
    validate(configData);

    mLoadSwitchId = loadSwitchId;

    /// - Initialize and validate parent
    UserLoadBase::initialize(configData, inputData, networkLoads, cardId);

    /// - set the resistance data
    mResistanceNormal = configData.mResistanceNormal;
    mResistanceStandby = configData.mResistanceStandby;

    /// Set the init flag on successful initialization.
    mInitFlag = true;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    configData  --  Constant Resistance User Load Electric Config Data
///
/// @throws   TsInitializationException
///
/// @details  Validates this Constant Resistance User Load Electrical initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ResistiveLoad::validate(const ResistiveLoadConfigData& configData)
{

    /// @throws Throw exception on range < 0 or > max.
    if (configData.mResistanceNormal < 0.0 || configData.mResistanceNormal > MAXIMUM_RESISTANCE) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "EPS", "ResistiveLoad::validate - Resistance for Normal Load is < 0 or > maximum resistance.",
                TsInitializationException, "Invalid Initialization Data", mNameLoad);
    }

    /// @throws Throw exception on range < 0 or > max.
    if (configData.mResistanceStandby < 0.0 || configData.mResistanceStandby > MAXIMUM_RESISTANCE) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "EPS", "ResistiveLoad::validate - Resistance for Standby Load is < 0 or > maximum resistance.",
                        TsInitializationException, "Invalid Initialization Data", mNameLoad);
    }

}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   voltage  (v)  per Integration time step
///
/// @details Method to update the user load during a time step. For a constant resistance load
/// the input parameter is not "dt" but the voltage passed by the load switch class.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
void ResistiveLoad::step(const double voltage)
{
    /// Call the parent step function. Base class step function sets the
    /// current, actual power, and equivalent resistance values to the initial
    /// conditions.
    UserLoadBase::step(voltage);

    /// -- update load values
    updateLoad();

    try {
        /// calculate the actual load for the current step
        calculateResistiveLoad();

    } catch (TsNumericalException &e) {
        // send the message to the HS log
        TS_HS_EXCEPTION(TS_HS_ERROR, "EPS", e.getMessage().c_str(),
                        TsNumericalException, "ResistiveLoad::step", mNameLoad);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @details Method to calculate the user load during a time step. For a constant resistance load
/// the input parameter is the voltage from the load switch class.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
void ResistiveLoad::calculateResistiveLoad()
{
    ///  Calculate load only when the power is valid
    if (!mPowerValid) {
        return;
    }

    /// -- If malfunction overwrite the power flag is set get the overwrite power value
    if (mMalfOverrideCurrentFlag) {
        if (0.0 <= mMalfOverrideCurrentValue) { // check to eliminate potential divide by zero error
            if (0.0 == mMalfOverrideCurrentValue) {
                mEquivalentResistance = MAXIMUM_RESISTANCE;
            } else {
                mEquivalentResistance = mVoltage /  mMalfOverrideCurrentValue;
            }
            computeActualPower();
        }else{ // if current value is malfunctioned to < 0.0 then set resistance to maximum, shouldn't come here, otherwise  throw an exception
            TS_HS_EXCEPTION(TS_HS_ERROR, "EPS", " Tried to set override Current less than 0.0, expects > 0.0.",
                    TsNumericalException, "ResistiveLoad::calculateResistiveLoad", mNameLoad);
        }
    } else if (mMalfOverridePowerFlag) {
        if (0.0 <= mMalfOverridePower) { // check to eliminate potential divide by zero error
            if (0.0 == mMalfOverridePower) {
                mEquivalentResistance = MAXIMUM_RESISTANCE;
            } else {
                mEquivalentResistance = (mVoltage*mVoltage) /mMalfOverridePower;
            }
            computeActualPower();
        }else{
            TS_HS_EXCEPTION(TS_HS_ERROR, "EPS", " Tried to set override Power less than 0.0, expects > 0.0.",
                    TsNumericalException, "ResistiveLoad::calculateResistiveLoad", mNameLoad);
        }
    } else if (LoadOFF != mLoadOperMode) { /// -- Load operation mode is not OFF
        switch(mLoadOperMode) {
        case LoadON:
            if (0.0 < mResistanceNormal ) {
               mEquivalentResistance = MsMath::limitRange(MINIMUM_RESISTANCE, mResistanceNormal, MAXIMUM_RESISTANCE);
            } else {
               mEquivalentResistance = MAXIMUM_RESISTANCE;
               if (!mPrintMessageOnce){
                  TsHsMsg msg(TS_HS_WARNING, TS_HS_EPS);
                  msg << mNameLoad << " Resistance requested is Zero";
                  hsSendMsg(msg);
                  mPrintMessageOnce = true;
               }
            }
            break;
        case LoadSTANDBY:
            if (0.0 < mResistanceStandby ) {
               mEquivalentResistance = MsMath::limitRange(MINIMUM_RESISTANCE, mResistanceStandby, MAXIMUM_RESISTANCE);
            } else {
                mEquivalentResistance = MAXIMUM_RESISTANCE;
               if (!mPrintMessageOnce){
                  TsHsMsg msg(TS_HS_WARNING, TS_HS_EPS);
                  msg << mNameLoad << " Resistance requested is Zero";
                  hsSendMsg(msg);
                  mPrintMessageOnce = true;
               }
            }
            break;
        }
        computeActualPower();
    }
}



/////////////////////////////////////////////////////////////////////////////////////////////////
///
///// @details Method: setPower - calculate power for the Equivalent resistance.
/////////////////////////////////////////////////////////////////////////////////////////////////
void ResistiveLoad::computeActualPower() {
    if (0.0 <= mEquivalentResistance) {  // check to eliminate potential divide by zero error

        if (0.0 == mEquivalentResistance) {
//            mEquivalentResistance = 1.0/MAXIMUM_RESISTANCE;
//            mEquivalentResistance = MsMath::limitRange(MINIMUM_RESISTANCE, mEquivalentResistance, MAXIMUM_RESISTANCE);
              mEquivalentResistance = DEFAULT_RESISTANCE;
              if (!mPrintMessageOnce){
                 TsHsMsg msg(TS_HS_INFO, TS_HS_EPS);
                  msg << mNameLoad << " Resistance requested is Zero";
                  hsSendMsg(msg);
                  mPrintMessageOnce = true;
              }
        }
        mCurrent = mVoltage / mEquivalentResistance;
        mActualPower = (mVoltage*mVoltage) / mEquivalentResistance;
    } else if (0.0 > mEquivalentResistance) { // if resistance is less than zero throw an exception. This condition should never be reached.
        TS_HS_EXCEPTION(TS_HS_ERROR, "EPS", "Equivalent Resistance value less than 0.0, expects > 0.0.",
                        TsNumericalException, "ResistiveLoad::computeActualPower", mNameLoad);
    }
}



/////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @details Method: updateLoad - method to be overwritten by the derived classes. Keep this method
///    empty so derived classes can add functionality as needed.
/////////////////////////////////////////////////////////////////////////////////////////////////
void ResistiveLoad::updateLoad(){
    //update resistance.
}
