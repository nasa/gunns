/*********************** TRICK HEADER *************************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (Purpose of the Class)

 REQUIREMENTS:
 (none)

 REFERENCE:
 (none)

 ASSUMPTIONS AND LIMITATIONS:
 ()

 LIBRARY DEPENDENCY:
  ()

 PROGRAMMERS:
 ((Minh Tran) (L-3) (Date) (Initial Prototype))
 **************************************************************************************************/
#include "FetSwitch.hh"
#include "core/GunnsBasicLink.hh"

const double FetSwitch::mConductanceLimit = 1.0E15;

//===============================================================================================
//
// FetSwitch Configuration Data
//
//===============================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    fetOnResistance                 -- On Resistance
/// @param    fetOffResistance                -- Fet off equivalent resistance
/// @param    diodeReverseResistance          -- Reverse Diode equivalent resistance
/// @param    diodeForwardResistance          -- Forward Diode equivalent resistance
/// @param    overCurrentProtection           -- Fet over current protection
/// @param    overCurrentLimit                -- Over Curent Limit
///
/// @details  Constructs the FetSwitchElect Configuration data
/////////////////////////////////////////////////////////////////////////////////////////////////
FetSwitchConfigData::FetSwitchConfigData(           const double fetOnResistance,
                                                      const double fetOffResistance,
                                                      const double diodeReverseResistance,
                                                      const double diodeForwardResistance,
                                                      const bool overCurrentProtection,
                                                      const double overCurrentLimit)
    :
    mFetOnResistance(fetOnResistance),
    mFetOffResistance(fetOffResistance),
    mDiodeReverseResistance(diodeReverseResistance),
    mDiodeForwardResistance(diodeForwardResistance),
    mOverCurrentProtection(overCurrentProtection),
    mOverCurrentLimit(overCurrentLimit)
{
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the FetSwitch Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
FetSwitchConfigData::~FetSwitchConfigData()
{
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    that  --  Object to copy
///
/// @details  Copy constructs this FetSwitchconfig data.
////////////////////////////////////////////////////////////////////////////////////////////////////
FetSwitchConfigData::FetSwitchConfigData(const FetSwitchConfigData& that)
{
    mFetOnResistance = that.mFetOnResistance;
    mFetOffResistance = that.mFetOffResistance;
    mDiodeReverseResistance = that.mDiodeReverseResistance;
    mDiodeForwardResistance = that.mDiodeForwardResistance;
    mOverCurrentProtection = that.mOverCurrentProtection;
    mOverCurrentLimit = that.mOverCurrentLimit;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    fetOnResistance                 -- On Resistance
/// @param    fetOffResistance                -- Fet off equivalent resistance
/// @param    diodeReverseResistance          -- Reverse Diode equivalent resistance
/// @param    diodeForwardResistance          -- Forward Diode equivalent resistance
/// @param    overCurrentProtection           -- Fet over current protection
/// @param    overCurrentLimit                -- Over Curent Limit
///
/// @brief Fet Switch Configuration Data Initialize
/////////////////////////////////////////////////////////////////////////////////////////////////
void FetSwitchConfigData::init (const double fetOnResistance,
                                const double fetOffResistance,
                                const double diodeReverseResistance,
                                const double diodeForwardResistance,
                                const bool overCurrentProtection,
                                const double overCurrentLimit )
{
    mFetOnResistance = fetOnResistance;
    mFetOffResistance = fetOffResistance;
    mDiodeReverseResistance = diodeReverseResistance;
    mDiodeForwardResistance = diodeForwardResistance;
    mOverCurrentProtection = overCurrentProtection;
    mOverCurrentLimit = overCurrentLimit;
}

//===============================================================================================
//
// FetSwitch Input Data
//
//===============================================================================================
//////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the FetSwitch Input data
//////////////////////////////////////////////////////////////////////////////////////////////
FetSwitchInputData::FetSwitchInputData(const bool switchMalfFlag,
                                       const int switchMalf,
                                       const bool switchCmd)
    :
    mSwitchMalfFlag(switchMalfFlag),
    mSwitchMalf(switchMalf),
    mSwitchCmd(switchCmd)
{
    return;
}

//////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initial the Switch Input data
//////////////////////////////////////////////////////////////////////////////////////////////
void FetSwitchInputData::init (const bool switchMalfFlag,
                               const int switchMalf,
                               const bool switchCmd ) {

    mSwitchMalfFlag = switchMalfFlag;
    mSwitchMalf = switchMalf;
    mSwitchCmd = switchCmd;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the FetSwitch Input Data Object
///////////////////////////////////////////////////////////////////////////////////////////////
FetSwitchInputData::~FetSwitchInputData()
{
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    that  --  Object to copy
///
/// @details  Copy constructs this FetSwitch Input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
FetSwitchInputData::FetSwitchInputData(const FetSwitchInputData& that)
{
    mSwitchCmd = that.mSwitchCmd;
    mSwitchMalf = that.mSwitchMalf;
    mSwitchMalfFlag = that.mSwitchMalfFlag;
}

//===============================================================================================
//
// FetSwitch
//
//===============================================================================================
FetSwitch::FetSwitch():
    mSwitchMalfFlag (false),
    mSwitchMalf (0),
    mCurrent ( 0.0),
    mFetCmd ( true),
    mFetState ( true),
    mDiodeReverseBias ( true),
    mTrip(false),
    mTripReset (false),
    mFetOnResistance ( 0.000001),
    mFetOffResistance ( 1000000.0),
    mDiodeReverseResistance ( 1000000.0),
    mDiodeForwardResistance ( 0.1),
    mOverCurrentProtection ( false),
    mOverCurrentLimit ( 100.0),
    mPowerDissipation(0.0)
{
}



FetSwitch::~FetSwitch() {
}


void FetSwitch::initialize(const FetSwitchConfigData& configData,
                            const FetSwitchInputData& inputData)
{
    mFetOnResistance = configData.mFetOnResistance;
    mFetOffResistance = configData.mFetOffResistance;
    mDiodeReverseResistance = configData.mDiodeReverseResistance;
    mDiodeForwardResistance = configData.mDiodeForwardResistance;
    mOverCurrentProtection = configData.mOverCurrentProtection;
    mOverCurrentLimit = configData.mOverCurrentLimit;

    mSwitchMalf = inputData.mSwitchMalf;
    mSwitchMalfFlag = inputData.mSwitchMalfFlag;
    mFetCmd = inputData.mSwitchCmd;
}


///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method: updateSwitch
///////////////////////////////////////////////////////////////////////////////////////////////
void FetSwitch::updateSwitchState() {

    mFetState = mFetCmd;

    if (mTripReset) {
        mTrip = false;                  // open cmd will reset trip flag;
    }

    int switchMalf = 0;
    if (mSwitchMalfFlag) {
        switchMalf = mSwitchMalf;
    }

    switch (switchMalf) {
        case 0:                            // case normal
            if ((mFetCmd)&&(!mTrip)) {
               mFetState = true;
            } else {
               mFetState = false;
            }
            break;

        case 1:                            // Fet fail opened
            mFetState = false;
            break;

        case 2:                           //  Fet Fail closed
            mFetState = true;
            break;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////
/// @param    current                 -- Fet current
///
/// @details  Method: updateSwitchFlow
///////////////////////////////////////////////////////////////////////////////////////////////
void FetSwitch::updateSwitchFlow(double current) {

    int switchMalf = 0;
    if (mSwitchMalfFlag) {
        switchMalf = mSwitchMalf;
    }

    if ((0 == switchMalf) &&
        (current > mOverCurrentLimit) &&
        (mOverCurrentProtection)){

        mCurrent = 0;
        mFetState = false;
        mTrip = true;

    } else {
        mCurrent = current;
    }

    if (getConductance() > 0) {
         mPowerDissipation = (mCurrent * mCurrent) / getConductance();
    }
}


bool FetSwitch::getFetState()
{
    return mFetState;
}

double FetSwitch::getCurrent()
{
    return mCurrent;
}

bool FetSwitch::getDiodeReverseBias(){
    return mDiodeReverseBias;
}

void FetSwitch::setDiodeReverseBias(bool diodeReverseBias){
    mDiodeReverseBias = diodeReverseBias;
}

double FetSwitch::getConductance(){

    double switchResistance = 0.0;
    double diodeResistance = 0.0;
    double conductance = 0.0;

    if (mFetState) {
        switchResistance = mFetOnResistance;
    } else {
        switchResistance = mFetOffResistance;
    }

    if (mDiodeReverseBias) {
        diodeResistance = mDiodeReverseResistance;
    } else {
        diodeResistance = mDiodeForwardResistance;
    }

    if ( (diodeResistance > 0.0) && (diodeResistance > 0.0) ){
        conductance = 1/switchResistance + 1/ diodeResistance;
    } else {
        conductance = mConductanceLimit;
    }

    return conductance;

}


double FetSwitch::getPowerDissipation(){
    return mPowerDissipation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
/// @param[in] value (--) Malfunction value (1 fails the switch open).
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void FetSwitch::setMalfSwitch(const bool flag, const int value)
{
    mSwitchMalfFlag = flag;
    mSwitchMalf     = value;
}
