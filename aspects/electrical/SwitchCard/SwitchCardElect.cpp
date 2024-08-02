/*********************** TRICK HEADER *************************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (The SwitchCardElect class is the electrical aspect of a generic 'box of switches' to distribute
  power.  It can have a configurable number of switches, with varying trip set-points.)

 REQUIREMENTS:
 (none)

 REFERENCE:
 (none)

 ASSUMPTIONS AND LIMITATIONS:
 ()

 LIBRARY DEPENDENCY:
  (
   (aspects/electrical/Switch/Switch.o)
   (aspects/electrical/UserLoad/UserLoadBase.o)
   (common/sensors/SensorAnalog.o)
   (SwitchCardElect.o)
  )


 PROGRAMMERS:
 (
 (Minh Tran) (L-3) (Initial Prototype)
 (Philip Trottman) (USA) (Updates for ISS RPCM) (Feb 2012)
 )
 **************************************************************************************************/
#include <cfloat>
#include <string>
#include <vector>
#include <string.h>
#include "SwitchCardElect.hh"
#include "math/MsMath.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "simulation/hs/TS_hs_msg_types.h"
#include "software/exceptions/TsInitializationException.hh"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    SwitchCardElect Config Data
/// @param[in]    name                           (--) identifier for this instance of the switchcard
/// @param[in]    nodes                          (--) GUNNS network nodes this switchcard is connected to
/// @param[in]    numSwitches                    (--) total number of switches in the box
/// @param[in]    numTwoPortSwitches             (--) number of switches which power other switchcards
/// @param[in]    powerSupplyLoad                (W) power used by this device's power supply to run the firmware
/// @param[in]    inputVoltSensorConfigData      (--)  Config data for the input voltage sensor
/// @param[in]    inputCurrentSensorConfigData   (--)  Config data for the input current sensor
/// @param[in]    switchVoltSensorConfigData     (--)  Config data for the individual switch output voltage sensors
/// @param[in]    switchCurrentSensorConfigData  (--)  Config data for the individual switch output current sensors
/// @details  Constructs the SwitchCardElect Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
SwitchCardElectConfigData::SwitchCardElectConfigData(const std::string name,
        GunnsNodeList *nodes,
        const int numSwitches,
        const int numTwoPortSwitches,
        const double powerSupplyLoad,
        const SensorAnalogConfigData* inputVoltSensorConfigData,
        const SensorAnalogConfigData* inputCurrentSensorConfigData,
        const SensorAnalogConfigData* switchVoltSensorConfigData,
        const SensorAnalogConfigData* switchCurrentSensorConfigData,
        const double uvTripVoltage,
        const double ovTripVoltage,
        const GunnsElectIps* powerSupply)
:
        GunnsBasicLinkConfigData(name, nodes),
        mNumSwitches(numSwitches),
        mNumTwoPortSwitches(numTwoPortSwitches),
        mDefaultPowerSupplyLoad(powerSupplyLoad),
        mSwitchConfigData(0),
        mPorts(0),
        mInputVoltSensorConfigData(inputVoltSensorConfigData),
        mInputCurrentSensorConfigData(inputCurrentSensorConfigData),
        mSwitchVoltSensorConfigData(switchVoltSensorConfigData),
        mSwitchCurrentSensorConfigData(switchCurrentSensorConfigData),
        mUvTripVoltage(uvTripVoltage),
        mOvTripVoltage(ovTripVoltage),
        mPowerSupply(powerSupply) {
    int lNumPorts = mNumTwoPortSwitches +2;

    TS_NEW_CLASS_ARRAY_EXT(mSwitchConfigData, mNumSwitches, SwitchConfigData, (), name + ".mSwitchConfigData");
    TS_NEW_PRIM_ARRAY_EXT(mPorts, lNumPorts, int, name + ".mPorts");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the SwitchCardElect Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
SwitchCardElectConfigData::~SwitchCardElectConfigData() {
    cleanup();
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name                 Name for object instance
/// @param[in]  numSwitches          total number of switches in this switchCard
/// @param[in]  numTwoPortSwitches   how many of the total number of switches power other GUNNS objects
/// @param[in]  powerSupplyLoad      power consumed by the SwitchCards own power supply
/// @details  Initialize the Switch Input data
///////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElectConfigData::initData(const std::string name,
        const int numSwitches,
        const int numTwoPortSwitches,
        const double powerSupplyLoad) {
    mName = name;
    mNumSwitches = numSwitches;
    mNumTwoPortSwitches = numTwoPortSwitches;
    mDefaultPowerSupplyLoad = powerSupplyLoad;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes dynamically allocated arrays.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElectConfigData::cleanup()
{
    TS_DELETE_ARRAY(mPorts);
    TS_DELETE_ARRAY(mSwitchConfigData);
}

/// @details  Instance count for unique naming.
std::size_t SwitchCardElectInputData::mCounter = 0;

//////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   malfBlockageFlag                (--)  flag to enable/disable the malfunction
/// @param[in]   malfBlockageValue               (--)  amount of blockage to add to the network link
/// @param[in]   numSwitches                     (--)  number of switches in this switch card
/// @param[in]   minInputVoltage                 (V)   minimum input voltage where the device will turn on
/// @param[in]   inputVoltSensorInputData        (--)  input data for the input voltage sensor
/// @param[in]   inputCurrentSensorConfigData    (--)  input data for the input current sensor
/// @param[in]   switchVoltSensorConfigData      (--)  input data for the individual switch output voltage sensors
/// @param[in]   switchCurrentSensorConfigData   (--)  input data for the individual switch output current sensors
/// @details  Constructs the SwitchCardElect Input data
//////////////////////////////////////////////////////////////////////////////////////////////
SwitchCardElectInputData::SwitchCardElectInputData(const bool   malfBlockageFlag,
        const double malfBlockageValue,
        const int numSwitches,
        const SensorAnalogInputData* inputVoltSensorInputData,
        const SensorAnalogInputData* inputCurrentSensorInputData,
        const SensorAnalogInputData* switchVoltSensorInputData,
        const SensorAnalogInputData* switchCurrentSensorInputData,
        const double minInputVoltage,
        const double maxConductance)

:
  GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue),
  mNumSwitches(numSwitches),
  mSwitchInputData(0),
  mInputVoltSensorInputData(inputVoltSensorInputData),
  mInputCurrentSensorInputData(inputCurrentSensorInputData),
  mSwitchVoltSensorInputData(switchVoltSensorInputData),
  mSwitchCurrentSensorInputData(switchCurrentSensorInputData),
  mMinInputVoltage(minInputVoltage),
  mMaxConductance(maxConductance)
{
    // Increment the count of instances created and convert it to string for attaching to the memory
    // allocation name.
    mCounter++;
    std::ostringstream count;
    count << mCounter;
    std::string String = count.str();
    TS_DELETE_ARRAY(mSwitchInputData);
    TS_NEW_CLASS_ARRAY_EXT(mSwitchInputData, mNumSwitches, SwitchInputData, (), "SwitchCardElectInputData.mSwitchInputData" + String);
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the SwitchCardElect Input Data Object
///////////////////////////////////////////////////////////////////////////////////////////////
SwitchCardElectInputData::~SwitchCardElectInputData() {
    cleanup();
    mCounter--;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  numSwitches    total number of switches in the SwitchCard
/// @param[in]  minInputVoltage   lowest input voltage before the SwitchCard will turn on
/// @param[in]  maxConductance    upper limit on the flow of current through a SwitchCard
/// @details  Initialize the Switch Input data
///////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElectInputData::init(const int numSwitches,
        const double minInputVoltage,
        const double maxConductance) {
    mNumSwitches = numSwitches;
    mMinInputVoltage = minInputVoltage;
    mMaxConductance = maxConductance;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @param  that    (--)  source to copy
/// @details  Copy Construct the SwitchCardElect Input Data Object
///////////////////////////////////////////////////////////////////////////////////////////////
SwitchCardElectInputData::SwitchCardElectInputData(const SwitchCardElectInputData& that)
:
                            GunnsBasicLinkInputData(that.mMalfBlockageFlag, that.mMalfBlockageValue),
                            mNumSwitches(that.mNumSwitches),
                            mSwitchInputData(0),
                            mInputVoltSensorInputData(that.mInputVoltSensorInputData),
                            mInputCurrentSensorInputData(that.mInputCurrentSensorInputData),
                            mSwitchVoltSensorInputData(that.mInputVoltSensorInputData),
                            mSwitchCurrentSensorInputData(that.mInputCurrentSensorInputData),
                            mMinInputVoltage(that.mMinInputVoltage),
                            mMaxConductance(that.mMaxConductance) {
    // Increment the count of instances created and convert it to string for attaching to the memory
    // allocation name.
    mCounter++;
    std::ostringstream count;
    count << mCounter;
    std::string String = count.str();
    TS_NEW_CLASS_ARRAY_EXT(mSwitchInputData, mNumSwitches, SwitchInputData, (), "SwitchCardElectInputDataCopy.mSwitchInputData" + String);

    for (int i = 0; i < that.mNumSwitches; i++) {
        mSwitchInputData[i] = that.mSwitchInputData[i];
    }

    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes dynamically allocated arrays.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElectInputData::cleanup()
{
    TS_DELETE_ARRAY(mSwitchInputData);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    SwitchCardElect
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the SwitchCardElect
///////////////////////////////////////////////////////////////////////////////////////////////////
SwitchCardElect::SwitchCardElect()
:
                            GunnsBasicLink(1),
                            mSwitch(0),
                            mLoad(0),
                            mInputVoltSensor(),
                            mInputCurrentSensor(),
                            mSwitchVoltSensors(),
                            mSwitchCurrentSensors(),
                            mPowerSupply(NULL),
                            mMagicPowerFlag(false),
                            mMagicPowerValue(123.456),
                            mMagicPowerAdmittance(1E-8),
                            mMagicPowerFlagResetFromOverride(false),
                            mMalfPowerSupplyFailFlag(false),
                            mMalfBusFault(false),
                            mMalfBusFaultValue(0.0),
                            mLastBusFault(false),
                            mMalfADCFailHigh(false),
                            mMalfADCFailLow(false),
                            mVerbose(false),
                            mDeltaTime(0.1),
                            mMinVoltage(70.0),
                            mLoadChangeTolerance(0.0001),    // by how many mhos a switch/load conductance has to change before it affects the GUNNS network
                            mNumSwitches(1),                 // SwitchCard should always have at least one switch
                            mNumTwoPortSwitches(0),
                            mNumLoadSwitches(1),             // make that 1 minimum switch a simpler load switch
                            mBusVoltage(0.0),
                            mTwoPortSwitchActiveConductance(0),
                            mTotalCPowerLoadsPower(0.0),
                            mInputCurrent(0.0),
                            mInputVoltage(0.0),
                            mInputPowerUsed(0.0),
                            mDefaultPowerSupplyLoad(0.0),
                            mPowerSupplyLoad(0.0),
                            mPowerSupplyConductance(1.0),      // value to avoid divide by zero
                            mResLoadsConductance(0.0),
                            mCPowerLoadsConductance(0.0),
                            mActiveResLoadsConductance(0.0),
                            mActiveCPowerLoadsConductance(0.0),
                            mMaxConductance(),
                            mMinConductance(1.0E-8),          // equivalent of 100 megaOhm load, valid for open circuit
                            mMaxResistance(),
                            mIsMinorStep(false),
                            mAnySwitchTripped(false),
                            mAnySwitchWaitingToTrip(false),
                            mCurrentMinorStep(0),
                            mNonLinear(true),
                            mUndervoltFdirActive(false),
                            mUvTripVoltage(110.0),
                            mUndervoltTrip(false),
                            mFdirTrip(false),
                            mClearUndervoltTrip(false),
                            mOvervoltFdirActive(),
                            mOvTripVoltage(190.0),
                            mOvervoltTrip(),
                            mClearOvervoltTrip(false),
                            mTlmPowerSupplyValid(true),
                            mTlmActualSwitchPosition(),
                            mTlmSensedSwitchPosition(),
                            mTlmPosTrip(),
                            mTlmNegTrip(),
                            mTlmUndervoltTrip(),
                            mTlmSensedBusVolt(0.0),
                            mTlmSensedInputCurrent(0.0),
                            mTlmSensedSwitchVolts(),
                            mTlmSensedSwitchCurrent(),
                            mTlmSwitchVolts(0),
                            mTlmSwitchCurrent(0),
                            mTlmSwitchPower(0),
                            mUserLoadPowerValid(0),
                            mThermWasteHeat(0.0),
                            mThermalTuning(0.05) {
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the SwitchCardElect
///////////////////////////////////////////////////////////////////////////////////////////////////
SwitchCardElect::~SwitchCardElect() {
    cleanup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initialize the SwitchCardElect
///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   configData   (--) Reference to Link Config Data
/// @param[in]   inputData    (--) Reference to Link Input Data
/// @param[in]   networkLinks (--) Reference to the Network Link Vector
/// @param[in]   loads        (--) Reference to UserLoads
/// @param[in]   ports        (--) GUNNS network I/O Port Mapping
/// @throw TsInitializationException
/// @details  Initialize the SwitchCardElect with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElect::initialize(SwitchCardElectConfigData &configData,
        SwitchCardElectInputData &inputData,
        std::vector<GunnsBasicLink*> &networkLinks,
        UserLoadBase **loads,
        const int *ports) {

    mMaxResistance = 1.0 / mMinConductance;

    // set up the internal power supply, if this switch card has one
    if (configData.mPowerSupply != 0) {
        mPowerSupply = const_cast<GunnsElectIps*> (configData.mPowerSupply);
        //mPowerSupply->setOutputPower(configData.mDefaultPowerSupplyLoad);
        //mPowerSupply->setUnderVoltageLimit(inputData.mMinInputVoltage);
    }

    // simple power supply load for RPCMs
    if (configData.mDefaultPowerSupplyLoad >= 0.0) {
        mDefaultPowerSupplyLoad = configData.mDefaultPowerSupplyLoad;
    } else {
        mDefaultPowerSupplyLoad = 0.0;
        GUNNS_ERROR(TsInitializationException,  "Invalid Configuration Data",
                " Initialize: Can't have a negative power supply load!");
    }

    // safety checks
    // check for sane number of switches
    if (configData.mNumSwitches > 0) {
        mNumSwitches = configData.mNumSwitches;
    } else {
        mNumSwitches = 1;
        GUNNS_ERROR(TsInitializationException,  "Invalid Configuration Data",
                "Initialize: Can't have less than one switch, set to one. ");
    }

    // check for sane number of two-port switches
    if (configData.mNumTwoPortSwitches > mNumSwitches) {
        mNumTwoPortSwitches = mNumSwitches;
        GUNNS_ERROR(TsInitializationException,  "Invalid Configuration Data",
                " Initialize: Can't have more two-port switches than total number of switches, set equal");
    } else {
        mNumTwoPortSwitches = configData.mNumTwoPortSwitches;
    }

    // check for config data number of switches vs. input data number of switches.
    if (configData.mNumSwitches != inputData.mNumSwitches) {
        GUNNS_ERROR(TsInitializationException,  "Invalid Configuration Data",
                " Initialize: ConfigData number of switches doesn't match InputData number of switches!");
    }

    // extra port to accommodate ground?
    mNumPorts = mNumTwoPortSwitches + 2;
    mNumLoadSwitches = mNumSwitches - mNumTwoPortSwitches;

    mMinVoltage = inputData.mMinInputVoltage;
    mMaxConductance = inputData.mMaxConductance;

    mUvTripVoltage = configData.mUvTripVoltage;
    mOvTripVoltage = configData.mOvTripVoltage;

    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    // be sure and erase anything from a possible previous initialize
    cleanup();

    // allocate memory for structures in Trick
    allocateArrays();

    // pass config and input data down to the individual switches
    for (int i = 0; i < mNumSwitches; i++) {
        mSwitch[i].initialize(configData.mSwitchConfigData[i], inputData.mSwitchInputData[i], mName, (i+1));
    }

    // set up the default conductances and assign loads to switches that have them
    for (int i=0; i < mNumSwitches; i++) {
        // regardless of switch type initialize conductance to 0.0
        mTwoPortSwitchActiveConductance[i] = 0.0;

        // EDIT 9/11/2014: assign the load object regardless of switch type, so we have a name to put on the display page.
        // but only do this for RPCMs... test for a power supply object, and if we don't have one, assign a load object
        // to every switch
        if(0 == mPowerSupply) {
            // no power supply = RPCM, so every switch gets a load
            mLoad[i] = loads[i];
        } else {
            // do the original thing
            if (!configData.mSwitchConfigData[i].mIsTwoPortSwitch) {
                // this is a load switch, so assign it the appropriate load
                mLoad[i] = loads[i];
            } else {
                mLoad[i] = 0;
            }
        }
        mUserLoadPowerValid[i] = false ;

        // while we're looping through switches reset some FDIR status
        mOvervoltFdirActive[i] = false;    // function inhibit active = do not test for over-volt condition

    }

    // initialize all sensor objects
    mInputVoltSensor.initialize(*(configData.mInputVoltSensorConfigData),
            *(inputData.mInputVoltSensorInputData),
            mName + "_input_volt");

    mInputCurrentSensor.initialize(*(configData.mInputCurrentSensorConfigData),
            *(inputData.mInputCurrentSensorInputData),
            mName + "_input_current");

    std::string lName(mName);

    for (int i = 0; i < mNumSwitches; i++) {

        mSwitchVoltSensors[i].initialize(*(configData.mSwitchVoltSensorConfigData),
                *(inputData.mSwitchVoltSensorInputData),
                lName);

        mSwitchCurrentSensors[i].initialize(*(configData.mSwitchCurrentSensorConfigData),
                *(inputData.mSwitchCurrentSensorInputData),
                lName);

        mTlmActualSwitchPosition[i] = mSwitch[i].isClosed();  // load up initial switch positions
    }

    mBusVoltage = mNodes[0]->getPotential();

    // - Once we're done with the config & input data objects, call their cleanup methods so they
    //   can delete their dynamic arrays.  This prevents Trick from checkpointing them.
    configData.cleanup();
    inputData.cleanup();

    /// - Warn of deprecation due to obscelescence, includes non-generic features and needs a redesign.
    GUNNS_WARNING("this link is deprecated!  It is obsoleted, includes non-generic features and needs a redesign.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Allocate arrays based on the number of switches
////////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElect::allocateArrays() {
    TS_NEW_CLASS_ARRAY_EXT       (mSwitch,                         mNumSwitches, Switch,       (), std::string(mName) + ".mSwitch");
    TS_NEW_PRIM_POINTER_ARRAY_EXT(mLoad,                           mNumSwitches, UserLoadBase,     std::string(mName) + ".mLoad");
    TS_NEW_PRIM_ARRAY_EXT        (mTwoPortSwitchActiveConductance, mNumSwitches, double,           std::string(mName) + ".mTwoPortSwitchActiveConductance");
    TS_NEW_CLASS_ARRAY_EXT       (mSwitchVoltSensors,              mNumSwitches, SensorAnalog, (), std::string(mName) + ".mSwitchVoltSensors");
    TS_NEW_CLASS_ARRAY_EXT       (mSwitchCurrentSensors,           mNumSwitches, SensorAnalog, (), std::string(mName) + ".mSwitchCurrentSensors");
    TS_NEW_PRIM_ARRAY_EXT        (mTlmSwitchVolts,                 mNumSwitches, double,           std::string(mName) + ".mTlmSwitchVolts");
    TS_NEW_PRIM_ARRAY_EXT        (mTlmSwitchCurrent,               mNumSwitches, double,           std::string(mName) + ".mTlmSwitchCurrent");
    TS_NEW_PRIM_ARRAY_EXT        (mTlmSwitchPower,                 mNumSwitches, double,           std::string(mName) + ".mTlmSwitchPower");
    TS_NEW_PRIM_ARRAY_EXT        (mUserLoadPowerValid,             mNumSwitches, bool,             std::string(mName) + ".mUserLoadPowerValid");

    // clear garbage values out of primitive arrays.
    for (int i = 0; i < mNumSwitches; ++i) {
        mTwoPortSwitchActiveConductance[i] = 0.0;
        mTlmSwitchVolts[i]                 = 0.0;
        mTlmSwitchCurrent[i]               = 0.0;
        mTlmSwitchPower[i]                 = 0.0;
        mUserLoadPowerValid[i]             = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes allocated memory objects
////////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElect::cleanup() {
    TS_DELETE_ARRAY(mUserLoadPowerValid);
    TS_DELETE_ARRAY(mTlmSwitchCurrent);
    TS_DELETE_ARRAY(mTlmSwitchVolts);
    TS_DELETE_ARRAY(mTlmSwitchPower);
    TS_DELETE_ARRAY(mSwitchCurrentSensors);
    TS_DELETE_ARRAY(mSwitchVoltSensors);
    TS_DELETE_ARRAY(mTwoPortSwitchActiveConductance);
    TS_DELETE_ARRAY(mLoad);
    TS_DELETE_ARRAY(mSwitch);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElect::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method to clear an input under-volt trip flag
////////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElect::clearUndervoltTrip() {
    mUndervoltTrip = false;
    for (int i = 0; i < mNumSwitches; i++) {
        mSwitch[i].setExternalTrip(false);
        mTlmUndervoltTrip[i] = false;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt  (s)  model iteration period
/// @details  Method for updating the link for the network
/////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElect::step(const double dt) {
    int lPortAssigned = 0;
    int lTwoPortCounter = 0;   // keep track of how many two port switches have been processed
    // so that the userLoads line up with the remaining switches
    double lSwitchResistance = 0.0;
    double lSwitchConductance = 0.0;
    double lLoadVoltage = 0.0;

    catchDebug();

    mAdmittanceUpdate = false;

    mTlmPowerSupplyValid = false;
    // mBusVoltage = 0.0;

    // save off delta time for use in confirmSolutionAcceptable
    mDeltaTime = dt;


    mBusVoltage= mPotentialVector[0];

    // factor in magic power override.  This won't produce current,
    // but will keep the switchcard alive long enough to get to that code later
    if (mMagicPowerFlag) {
        mBusVoltage = std::max(mMagicPowerValue,mBusVoltage);
    }

    // process bus fault failed malfunction
    if (mMalfBusFault && (mPowerSupply == 0)){
        mPowerSupplyLoad = mDefaultPowerSupplyLoad + mMalfBusFaultValue * mBusVoltage;   // for RPCMs
    } else {
        mPowerSupplyLoad = mDefaultPowerSupplyLoad ;                                     // for DCSUs/MBSUs do bus fault a different way later
    }

    // test if this switchcard has an IPS associated with it
    if (mPowerSupply == 0) {
        // If it doesn't, power supply is on as long as the input voltage is good
        if (mBusVoltage > mMinVoltage) {
            mTlmPowerSupplyValid = true;
        }
    } else {
        // this switchcard *does* have an IPS, so ask it if power is OK
        mTlmPowerSupplyValid = mPowerSupply->isPowerSupplyOn();

        //Bus Fault  -- PRB0044564 Bus fault in the wrong place.  Can't put it on the power supply for DCSUs/MBSUs
        // because it doesn't reflect on the input switch then.
//        if (mMalfBusFault != mLastBusFault ) {
//            mPowerSupply->mMalfBiasPowerConsumedOn = mMalfBusFault;
//            mPowerSupply->mMalfBiasPowerConsumedOnValue = mMalfBusFaultValue * mBusVoltage;
//        }
    }

    mLastBusFault = mMalfBusFault;

    // process power supply failed malfunction
    if (mMalfPowerSupplyFailFlag) {
        mTlmPowerSupplyValid = false;
    }


    // finally if input volts are valid and power supply is not failed,
    // pass the input voltage into the bus (if this device does not have a power supply,
    // implying it does not have multiple inputs)
    if (!mTlmPowerSupplyValid ) {
        clearUndervoltTrip();
    }

    // run the final input voltage through the voltage sensor to send to signal aspect
    if(mMalfADCFailHigh) {
        mTlmSensedBusVolt = mInputVoltSensor.sense(dt, mTlmPowerSupplyValid, 1E5);      // let the sensor limit at whatever it's upper value is
    } else if (mMalfADCFailLow) {
        mTlmSensedBusVolt = mInputVoltSensor.sense(dt, mTlmPowerSupplyValid, -1E5);     // let the sensor limit at whatever it's lower value is
    } else {
        mTlmSensedBusVolt = mInputVoltSensor.sense(dt, mTlmPowerSupplyValid, mBusVoltage);
    }


    // start by zeroing out the admittance matrix for this link object
    for (int i = 0; i < mNumPorts * mNumPorts; i++) {
        mAdmittanceMatrix[i] = 0.0;
    }

    // switch cards don't generate any current, so no source vector
    for (int i = 0; i < mNumPorts; i++) {
        mSourceVector[i] = 0.0;
    }

    mTotalCPowerLoadsPower = 0.0;    // sum total wattage of constant power loads
    mResLoadsConductance = 0.0;      // sum total conductance (1 / resistance) of resistive loads

    // loop through all the switches to build up the new admittance matrix for this link object
    for (int i = 0; i < mNumSwitches; i++) {
        mSwitch[i].updateSwitchState(mTlmPowerSupplyValid);
        mTlmSwitchCurrent[i] = mSwitch[i].getCurrent();         // pull the latest switch current

        if (mSwitch[i].isTwoPortSwitch()) {
            lTwoPortCounter++;  // keep track of how many 2-port switches so userLoads line up with remaining non 2-port switches
            lPortAssigned = mSwitch[i].getPortAssigned();
            lSwitchConductance = mSwitch[i].getConductance();

            // if there's a big enough change in the two-port switch conductance, flag to recalculate admittance matrix
            if (fabs(mTwoPortSwitchActiveConductance[lPortAssigned-1]-lSwitchConductance) > mLoadChangeTolerance) {
                mTwoPortSwitchActiveConductance[lPortAssigned-1] = lSwitchConductance;
                mAdmittanceUpdate = true;
            }

            /////////////////////////////////////////////////////////////////////////////////////////////////////
            // HISTORY:  general case for calculating the admittance matrix for a GUNNS link with multiple ports
            // -
            // mAdmittanceMatrix[Row * mNumPorts + Row]  += mTwoPortSwitchActiveConductance[PORT];
            // mAdmittanceMatrix[Row * mNumPorts + Col]  += -mTwoPortSwitchActiveConductance[PORT];
            // mAdmittanceMatrix[Col * mNumPorts + Row]  += -mTwoPortSwitchActiveConductance[PORT];
            // mAdmittanceMatrix[Col * mNumPorts + Col]  += mTwoPortSwitchActiveConductance[PORT];
            // -
            // for the switch cards since all switches are connected to port 0 at one end, there only needs
            // to be one row in the admittance matrix, so Row = 0.  This simplifies the math to the following:
            /////////////////////////////////////////////////////////////////////////////////////////////////////
            mAdmittanceMatrix[0]                                         += mTwoPortSwitchActiveConductance[lPortAssigned-1];
            mAdmittanceMatrix[lPortAssigned]                             += -mTwoPortSwitchActiveConductance[lPortAssigned-1];
            mAdmittanceMatrix[lPortAssigned * mNumPorts]                 += -mTwoPortSwitchActiveConductance[lPortAssigned-1];
            mAdmittanceMatrix[lPortAssigned * mNumPorts + lPortAssigned] += mTwoPortSwitchActiveConductance[lPortAssigned-1];


        } else {   // this is a loadSwitch

            if (mSwitch[i].isClosed()) {
                lLoadVoltage = mBusVoltage;
                lSwitchResistance = mLoad[i]->getResistance();
                if (CONSTANT_POWER_LOAD == mLoad[i]->getLoadType()) {  // this is a constant power load
                    mTotalCPowerLoadsPower += mLoad[i]->getPower();    // sum up all Constant Power load values
                } else if (MsMath::isInRange(0.0, lSwitchResistance, mMaxResistance)) {  // this is a resistive load, ignore value if resistance is invalid
                    mResLoadsConductance += (1.0 / lSwitchResistance);             // sum up the load conductance
                }
            } else {
                lLoadVoltage = 0.0;         // set voltage to pass to the user load to zero in case the switch is open(ed)
            }

            // Step the UserLoad attached to loadSwitch
            mLoad[i]->step(lLoadVoltage);
        }  // if two port or load switch

        // sync switch position status
        mTlmActualSwitchPosition[i] = mSwitch[i].isClosed();
    }

    // if there's a big enough change in the resistive loads, flag to recalculate the admittance matrix
    if (fabs(mActiveResLoadsConductance - mResLoadsConductance) > mLoadChangeTolerance) {
        mActiveResLoadsConductance = mResLoadsConductance;
        mAdmittanceUpdate = true;
    }

    // add in the totaled conductance for all resistive loads to the admittance matrix at the input port (0)
    mAdmittanceMatrix[0] += mActiveResLoadsConductance;

    // total up conductance (1 / resistance) of any and all constant-power loads
    if (mBusVoltage > 0.1) {
        mCPowerLoadsConductance = (mTotalCPowerLoadsPower / (mBusVoltage * mBusVoltage));
    } else {
        //mCPowerLoadsConductance = mMinConductance;
        mCPowerLoadsConductance = 0.0;
    }

    if (!MsMath::isInRange(mMinConductance, mCPowerLoadsConductance, mMaxConductance)) {
        // mCPowerLoadsConductance = mMinConductance;
        mCPowerLoadsConductance = 0.0;
    }

    // if there's a big enough change in the constant power loads, flag to recalculate the admittance matrix
    if (fabs(mActiveCPowerLoadsConductance - mCPowerLoadsConductance) > mLoadChangeTolerance) {
        mActiveCPowerLoadsConductance = mCPowerLoadsConductance;
        mAdmittanceUpdate = true;
    }

    // add in the totaled conductance for all constant power loads to the admittance matrix at the input port (0)
    mAdmittanceMatrix[0] += mActiveCPowerLoadsConductance;

    // add in a little bit to the admittance matrix for the load from the power supply itself, if applicable
    if (mBusVoltage > 0.1) {
        double powerSupplyConductance = (mPowerSupplyLoad / (mBusVoltage * mBusVoltage));
        if (fabs(mPowerSupplyConductance - powerSupplyConductance) > mLoadChangeTolerance) {
            mPowerSupplyConductance = powerSupplyConductance ;
            mAdmittanceUpdate = true;
        }
        mAdmittanceMatrix[0] += mPowerSupplyConductance;
    }

    // check for input undervolt clear command from the firmware
    if (mClearUndervoltTrip) {
        SwitchCardElect::clearUndervoltTrip();
        mClearUndervoltTrip = false;
    }

    // bus fault malf for DCSUs and MBSUs (switchcards with power supplies)
    if(mMalfBusFault && (0 != mPowerSupply)) {
        // find the conductance value we need to get the current desired
        double lFaultConductance = 0.0;
        if(mBusVoltage > 0.0) {
            lFaultConductance = mMalfBusFaultValue / mBusVoltage;    // G = i / v
        } else {
            // device has no voltage, so no need to enable any extra current flow
            lFaultConductance = 0.0;
        }

        // now add this fault conductance to the four corners of the admittance matrix
        // (assumes the connection to the main bus node is port zero, and the connection
        //  to the ground node is the last port) to generate the short from the main bus
        // to the ground node.
        mAdmittanceMatrix[0]                                   += lFaultConductance;
        mAdmittanceMatrix[mNumPorts-1]                         += -lFaultConductance;
        mAdmittanceMatrix[(mNumPorts * mNumPorts) - mNumPorts] += -lFaultConductance;
        mAdmittanceMatrix[(mNumPorts * mNumPorts) - 1]         += lFaultConductance;
    }

    //-----------------------------------------------------------------------------
    // temp. fix for the potential floating node.  will remove when the gunns has this fix
    //-----------------------------------------------------------------------------
    mAdmittanceMatrix[0] += DBL_EPSILON / .0000001;


    if ((mMagicPowerFlag) && (mMagicPowerValue > 0.0) && (mMagicPowerValue >= mBusVoltage)){
        // loop through the admittance matrix and zero out any negative values,
        // this should clean up the off-diagonals so they don't interfere with
        // the current we're trying to send out the switches
        for (int i = 0; i < mNumPorts * mNumPorts; i++) {
            if(mAdmittanceMatrix[i] < 0.0) {
                // a negative value implies an off-diagonal, so clear it
                mAdmittanceMatrix[i] = 0.0;
            } else {
                // leave the diagonals alone!
            }
        }

        mAdmittanceMatrix[0] = mMagicPowerAdmittance;               // keep current from feeding back upstream when magic power is active

        mSourceVector[0] = 0.0;                                     // don't send any current out of the RPCM input
        double lSumSourceVector = 0.0;
        int lSwitchPort = 0;

        for(int i=0; i < mNumSwitches; i++) {                       // loop through all the switches
            if(mSwitch[i].isTwoPortSwitch()) {                      // if this one is a two port switch
                lSwitchPort = mSwitch[i].getPortAssigned();         // find out which link port it's connected to and...

                if(mSwitch[i].isClosed()) {
                    mSourceVector[lSwitchPort] = mMagicPowerValue *
                            mAdmittanceMatrix[lSwitchPort * mNumPorts + lSwitchPort];   // push current through it
                    lSumSourceVector += mSourceVector[lSwitchPort];                     // keep track of current sent out so we can pull that much from ground
                } else {
                    // switch is open, don't push any current out for it
                }
            } else {
                // not a two port switch, so don't mess with admittance or source vectors
            }
        }
        mSourceVector[(mNumPorts - 1)] = -1.0 * lSumSourceVector;   // assumes last port is the new 'ground port'
    }

    // this should only set after startup and then only for around 5 iterations
    if (mMagicPowerFlagResetFromOverride) {
        mMagicPowerFlag = false;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt         (s)  model iteration period
/// @param[in]   minorStep  (--) current minor step inside a major step
/// @details  Method for updating the link for the network - non-linear link
/////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElect::minorStep(const double dt, const int minorStep __attribute__((unused))) {
    catchDebug();
    mIsMinorStep = true;
    computeFlows(dt);
    step(dt);
    mIsMinorStep = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   dt  (s)  model iteration period
/// @details  Method for computing link flow
/////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElect::computeFlows(const double dt) {
    catchDebug();
    mInputCurrent = 0.0;
    mAnySwitchTripped = false;
    mAnySwitchWaitingToTrip = false;
    mThermWasteHeat = 0.0;

    double lCurrent = 0.0;
    int lPortAssigned = 0;
    int lTwoPortCounter = 0;
    double lSwitchResistance = 0.0;
    double lBusVoltage = 0.0;

    // make sure to use the magic power value as the bus voltage (when appropriate)
    // so that switch currents are calculated correctly.  Because no current is sent
    // upstream when magic power is active, potentialVector[0] will be 0.0 which
    // breaks the math.
    if(mMagicPowerFlag) {
        lBusVoltage = mMagicPowerValue;
    } else {
        lBusVoltage = mPotentialVector[0];
    }


    for (int i = 0; i < mNumSwitches; i++) {
        lCurrent = 0.0;
        // first find the current flowing through the switch
        // if it's a two-port switch just use the admittance matrix to calculate current
        // if it's a load switch calculate the current based on the UserLoad data
        if (mSwitch[i].isTwoPortSwitch()) {
            lTwoPortCounter++;
            lPortAssigned = mSwitch[i].getPortAssigned();

            if(mSwitch[i].isPortsReversed()){
                //lCurrent = SwitchCardElect::getDeltaPotential(lPortAssigned, 0) * (-mAdmittanceMatrix[lPortAssigned]);
                lCurrent = (mPotentialVector[lPortAssigned] - lBusVoltage) * (mSwitch[i].getConductance());
            } else {
                //lCurrent = SwitchCardElect::getDeltaPotential(0, lPortAssigned) * (-mAdmittanceMatrix[lPortAssigned]);
                lCurrent = (lBusVoltage - mPotentialVector[lPortAssigned]) * (mSwitch[i].getConductance());
            }
        } else {
            lSwitchResistance = mLoad[i]->getResistance();
            if (mSwitch[i].isClosed()) {
                if ((CONSTANT_POWER_LOAD == mLoad[i]->getLoadType()) && mBusVoltage > 0.0) {
                    lCurrent = mLoad[i]->getPower() / mBusVoltage;
                } else if ((RESISTIVE_LOAD == mLoad[i]->getLoadType()) && MsMath::isInRange(0.0, lSwitchResistance, mMaxResistance)) {
                    lCurrent = mBusVoltage / lSwitchResistance;
                } else {
                    lCurrent = 0.0;
                }
            }
            mUserLoadPowerValid[i] = mLoad[i]->getPowerValid();
        }

        // pass the calculated switch current to the current sensor to pass it back to the switch itself
        if(mMalfADCFailHigh) {
            mTlmSensedSwitchCurrent[i] = mSwitchCurrentSensors[i].sense(dt, mTlmPowerSupplyValid, 1E5);
        } else if (mMalfADCFailLow) {
            mTlmSensedSwitchCurrent[i] = mSwitchCurrentSensors[i].sense(dt, mTlmPowerSupplyValid, -1E5);
        } else {
            mTlmSensedSwitchCurrent[i] = mSwitchCurrentSensors[i].sense(dt, mTlmPowerSupplyValid, lCurrent);
        }

        // update the switch itself
        mSwitch[i].updateSwitchFlow(lCurrent, mTlmSensedSwitchCurrent[i], mBusVoltage, mCurrentMinorStep, mIsMinorStep);

        // keep a running total of all the switch currents
        // this number is irrelevant for ISS MBSU/DCSUs because the sum
        // of all the currents will always be close to zero
        mInputCurrent += mSwitch[i].getCurrent();

        // keep track of whether any switch in the switchCard has officially tripped
        if (mSwitch[i].isTripped()) {
            mAnySwitchTripped = true;
        }

        // keep track of whether any switch in the switchCard has a high current but can't open yet due to trip priority calculations
        if (mSwitch[i].isWaitingToTrip()) {
            mAnySwitchWaitingToTrip = true;
        }

        // add up waste heat for closed switches
        mThermWasteHeat += mSwitch[i].getPowerDissipation();

        // sense the switch output voltages
        // test to see if this is a two-port switch though, and if so pull the output
        // volts from the node-map instead of the switch utility.  This is to cover things
        // like DCSU and MBSU input switches which can show a voltage at the device input
        // (which is still the switch output) even when the switch is open
        if (mSwitch[i].isTwoPortSwitch()) {
            mTlmSwitchVolts[i] = mPotentialVector[lTwoPortCounter];
        } else {
            mTlmSwitchVolts[i] = mSwitch[i].getOutputVolts();
        }

        // update switch voltage sensors
        if(mMalfADCFailHigh) {
            mTlmSensedSwitchVolts[i] = mSwitchVoltSensors[i].sense(dt, mTlmPowerSupplyValid, 1E5);
        } else if (mMalfADCFailLow) {
            mTlmSensedSwitchVolts[i] = mSwitchVoltSensors[i].sense(dt, mTlmPowerSupplyValid, -1E5);
        } else {
            mTlmSensedSwitchVolts[i] = mSwitchVoltSensors[i].sense(dt, mTlmPowerSupplyValid, mTlmSwitchVolts[i]);
        }

        mTlmSwitchPower[i] = mTlmSwitchVolts[i] * lCurrent;
    }  // loop through mNumSwitches


    if(mTlmPowerSupplyValid) {
        // check for undervolt trip situation (for RPCMs only)
        // volts are high enough for PS to be on, but low enough to open RPC switches,
        // if the firmware is allowed to test for it at all.
        if ((mTlmSensedBusVolt < mUvTripVoltage) && !mUndervoltFdirActive) {
            // set the trip flag
            mUndervoltTrip = true;

            // trip open all the (closed) switches
            for (int i = 0; i < mNumSwitches; i++) {
                if (mSwitch[i].isClosed()) {
                    mSwitch[i].setExternalTrip(true);
                    mTlmUndervoltTrip[i] = true;
                }
            }
            // keep current trip status
            //        } else {
            //            for (int i = 0; i < mNumSwitches; i++) {
            //                mTlmVoltTrip[i] = mSwitch[i].isTripped();
            //            }
        }

        bool atLeastOneRpcStillTripped = false;
        // check all the switches for over-volt trip
        for (int i = 0; i < mNumSwitches; i++) {
            if ((mTlmSensedSwitchVolts[i] > mOvTripVoltage) && mOvervoltFdirActive[i]) {
                // set the trip flag
                mOvervoltTrip[i] = true;

                // open the switch if it's closed
                if (mSwitch[i].isClosed()) {
                    mSwitch[i].setExternalTrip(true);
                    mTlmOvervoltTrip[i] = true;
                }
            }
            // switch volts are fine
            else {
                // if the volts are currently fine, but the switch had an overvolt trip, is now open, and has been commanded open
                // clear the trip
                if(mOvervoltTrip[i] && !mSwitch[i].isClosed() && !mSwitch[i].isCommandedClosed()) {
                    mSwitch[i].setExternalTrip(false);
                    mTlmOvervoltTrip[i] = false;
                    mOvervoltTrip[i] = false;
                }

//                mTlmUndervoltTrip[i] = mSwitch[i].isTripped();
                if (mTlmUndervoltTrip[i] ) {
                   atLeastOneRpcStillTripped = true;
                }
//                mOvervoltTrip[i] = mSwitch[i].isTripped();
//                mTlmOvervoltTrip[i] = mSwitch[i].isTripped();
            }
        }

        // Reset the undervolt trip flag, only when all the RPC trip flags have been cleared and voltage above trip level
        if ((mTlmSensedBusVolt > mUvTripVoltage) && !mUndervoltFdirActive) {
           if (mUndervoltTrip && !atLeastOneRpcStillTripped) {
                mUndervoltTrip = false;
           }
        }

        // add in the power supply load if applicable
        mInputCurrent += (mPowerSupplyConductance * mBusVoltage);
        // add in power supply load to total waste heat if power supply is on
        // if this is an RPCM and a bus fault is active, only add in a portion of the fault power as heat
        // so that the RPCM doesn't heat up too fast (PRB0044468).  If this is not an RPCM the power supply load
        // will be zero so it doesn't factor in anyway.
        if(mMalfBusFault) {
            mThermalTuning = std::max(mThermalTuning, 0.0);   // safety for negative thermal multipliers
            mThermWasteHeat += mPowerSupplyLoad * mThermalTuning;
        } else {
            mThermWasteHeat += mPowerSupplyLoad;
        }
    } // if mTlmPowerSupplyValid


    // update input current sensor after it's all added up
    if(mMalfADCFailHigh) {
        mTlmSensedInputCurrent = mInputCurrentSensor.sense(dt, mTlmPowerSupplyValid, 1E5);
    } else if (mMalfADCFailLow) {
        mTlmSensedInputCurrent = mInputCurrentSensor.sense(dt, mTlmPowerSupplyValid, -1E5);
    } else {
        mTlmSensedInputCurrent = mInputCurrentSensor.sense(dt, mTlmPowerSupplyValid, mInputCurrent);
    }

    // use the total current to figure out the power used for all the device
    mInputPowerUsed = (mInputCurrent * mBusVoltage);

}


/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Returns if the link is non-linear
/// @return bool
/////////////////////////////////////////////////////////////////////////////////////////////////
bool SwitchCardElect::isNonLinear() {
    /*HISTORY:
     *  Used to be only switchCards with constant power loads were non-linear,
     *  but the trip timing logic requires that all switchCards be flagged as non-linear
     *
     * mNonLinear = false;
     * for(int i = mNumTwoPortSwitches; i < mNumSwitches; i++) {
     *   if (CONSTANT_POWER_LOAD == mLoad[i-mNumTwoPortSwitches]->getLoadType()) {
     *       mNonLinear = true;
     *   }
     * }
     * return mNonLinear;
     *}
     */
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Returns if the link admittance matrix has been updated
/// @return bool
/////////////////////////////////////////////////////////////////////////////////////////////////
bool SwitchCardElect::needAdmittanceUpdate() {
    return mAdmittanceUpdate;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  convergedStep  (--)  number of times the GUNNS network has converged in the current major step
/// @param[in]  absoluteStep   (--)  current minor step inside the major step
/// @details  Returns TRUE if the network needs re-solving for a trip condition
/////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult SwitchCardElect::confirmSolutionAcceptable(const int convergedStep, const int absoluteStep) {
    GunnsBasicLink::SolutionResult lThingToReturn = DELAY;
    bool lAnySwitchDelays = false;
    bool lAnySwitchRejects = false;
    double lCurrent = 0.0;
    int lPortAssigned = 0;
    int lTwoPortCounter = 0;
    double lSwitchResistance = 0.0;
    double lBusVoltage = 0.0;

    catchDebug();

    // make sure to use the magic power value as the bus voltage (when appropriate)
    // so that switch currents are calculated correctly.  Because no current is sent
    // upstream when magic power is active, potentialVector[0] will be 0.0 which
    // breaks the math.
    if(mMagicPowerFlag) {
        lBusVoltage = mMagicPowerValue;
    } else {
        lBusVoltage = mPotentialVector[0];
    }

    mCurrentMinorStep = convergedStep;

    if (convergedStep > 0) {
        for (int i = 0; i < mNumSwitches; i++) {
            lCurrent = 0.0;
            if (mSwitch[i].isTwoPortSwitch()) {
                lTwoPortCounter++;
                lPortAssigned = mSwitch[i].getPortAssigned();
                if(mSwitch[i].isPortsReversed()){
                    //lCurrent = SwitchCardElect::getDeltaPotential(lPortAssigned, 0) * (-mAdmittanceMatrix[lPortAssigned]);
                    lCurrent = (mPotentialVector[lPortAssigned] - lBusVoltage) * (mSwitch[i].getConductance());
                } else {
                    //lCurrent = SwitchCardElect::getDeltaPotential(0, lPortAssigned) * (-mAdmittanceMatrix[lPortAssigned]);
                    lCurrent = (lBusVoltage - mPotentialVector[lPortAssigned]) * (mSwitch[i].getConductance());
                }
            } else {
                lSwitchResistance = mLoad[i]->getResistance();

                if (mSwitch[i].isClosed()) {
                    if ((CONSTANT_POWER_LOAD == mLoad[i]->getLoadType()) && mBusVoltage > 0.0) {
                        lCurrent = mLoad[i]->getPower() / mBusVoltage;
                    } else if ((RESISTIVE_LOAD == mLoad[i]->getLoadType()) && MsMath::isInRange(0.0, lSwitchResistance, mMaxResistance)) {
                        lCurrent = mBusVoltage / lSwitchResistance;
                    } else {
                        lCurrent = 0.0;
                    }
                } else {
                    lCurrent = 0.0;
                }
            }

            // pass the calculated switch current to the current sensor to pass it back to the switch itself
            mTlmSwitchCurrent[i] = lCurrent;
            mSwitchCurrent[i]    = lCurrent;

            if(mMalfADCFailHigh) {
                mTlmSensedSwitchCurrent[i] = mSwitchCurrentSensors[i].sense(mDeltaTime, mTlmPowerSupplyValid, 1E5);
            } else if (mMalfADCFailLow) {
                mTlmSensedSwitchCurrent[i] = mSwitchCurrentSensors[i].sense(mDeltaTime, mTlmPowerSupplyValid, -1E5);
            } else {
                mTlmSensedSwitchCurrent[i] = mSwitchCurrentSensors[i].sense(mDeltaTime, mTlmPowerSupplyValid, lCurrent);
            }

            // update the switch itself
            mSwitch[i].updateSwitchFlow(lCurrent, mTlmSensedSwitchCurrent[i], mBusVoltage, mCurrentMinorStep, false);

            if (mSwitch[i].isWaitingToTrip()) {
                lAnySwitchDelays = true;
                if(mVerbose) {
                    TsHsMsg msg(TS_HS_WARNING, TS_HS_EPS);
                    msg <<  mName << " switch " << (i+1) <<
                            " is waiting to trip (DELAY) on converged step:" << convergedStep << " absolute step:" << absoluteStep <<
                            " with potentials [" << mPotentialVector[0] << ", " << mPotentialVector[i+1] << "] and current " << mSwitch[i].getCurrent();
                    hsSendMsg(msg);
                }
            } else if (mSwitch[i].isJustTripped()) {
                lAnySwitchRejects = true;
                if(mVerbose) {
                    TsHsMsg msg(TS_HS_WARNING, TS_HS_EPS);
                    msg << mName << " switch " << (i+1) <<
                            " *just* tripped (REJECT) on converged step:" << convergedStep << " absolute step:" << absoluteStep <<
                            " with potentials [" << mPotentialVector[0] << ", " << mPotentialVector[i+1] << "] and current " << mSwitch[i].getCurrent();
                    hsSendMsg(msg);
                }

            } else {
                lThingToReturn = CONFIRM;
            }  // is tripped or waiting to trip

            // update telemetry to send to the signal aspect
            mTlmActualSwitchPosition[i] = mSwitch[i].isClosed();
            mTlmSensedSwitchPosition[i] = mSwitch[i].isClosed();   // will be updated to use sensors if need be (requirements are vague)
            mTlmPosTrip[i] = mSwitch[i].isPosTrip();
            mTlmNegTrip[i] = mSwitch[i].isNegTrip();
            if (!mTlmPosTrip[i] && !mTlmNegTrip[i] ) {
                mTlmSwitchPower[i] = mTlmSwitchVolts[i] * mTlmSwitchCurrent[i];
            } else {
                mTlmSwitchPower[i] = 0.0;
                mTlmSwitchCurrent[i] = 0.0;
            }
        }  // loop through all switches

        if(lAnySwitchDelays || lAnySwitchRejects) {
            // a trip has occurred, so re-do the admittance matrix
            step(mDeltaTime);
        }

        if (lAnySwitchDelays) {
            lThingToReturn = DELAY;
        }

        if (lAnySwitchRejects) {
            lThingToReturn = REJECT;
        }
    } else {
        return GunnsBasicLink::DELAY;
    }

    return lThingToReturn;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: setSwitchCommandedClosed
///////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElect::setSwitchCommandedClosed(const int switchNumber, const bool switchCommandedClosed) {
    mSwitch[switchNumber].setSwitchCommandedClosed(switchCommandedClosed);
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details Method: setAllSwitchCommandedClosed
///////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElect::setAllSwitchCommandedClosed(const bool *switchCommandedClosed) {
    for (int i = 0; i < mNumSwitches; i++) {
        mSwitch[i].setSwitchCommandedClosed(switchCommandedClosed[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElect::setMalfPowerSupplyFail(const bool flag)
{
    mMalfPowerSupplyFailFlag = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details utility function to catch specific instances in the debugger
////////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchCardElect::catchDebug()
{
    // do nothing, this is a placeholder for adding debugging breakpoints
}
