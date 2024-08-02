/***************************************** TRICK HEADER ********************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Provides the classes for modeling the GUNNS Resistive Load.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     ()

LIBRARY DEPENDENCY:
    (
      (aspects/electrical/resistive/GunnsResistiveLoad.o)
      (aspects/electrical/Switch/GunnsElectSwitchUtil.o)
    )

 PROGRAMMERS:
    (
     (Mike Moore) (L3) (2013-05)
    )
 **************************************************************************************************/
#include "aspects/electrical/resistive/GunnsResistiveLoad.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                (--) Link name
/// @param[in] nodes               (--) Network nodes array
/// @param[in] defaultResistance   (ohm) Default resistance of the link
///
/// @details  Constructs the Resistive Load Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistiveLoadConfigData::GunnsResistiveLoadConfigData(const std::string& name,
                                                           GunnsNodeList*     nodes,
                                                           const double       loadResistance,
                                                           const double       electricalEfficiency,
                                                           const double       underVoltageLimit,
                                                           const double       switchResistance,
                                                           const double       posTripLimit,
                                                           const double       negTripLimit,
                                                           const int          tripPriority)
    :
    GunnsElectricalResistorConfigData(name, nodes, loadResistance, electricalEfficiency),
    mUnderVoltageLimit(underVoltageLimit),
    mLoadSwitchResistance(switchResistance),
    mPosTripLimit(posTripLimit),
    mNegTripLimit(negTripLimit),
    mTripPriority(tripPriority)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from
///
/// @details  Copy Constructs the Resistive Load Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistiveLoadConfigData::GunnsResistiveLoadConfigData(
        const GunnsResistiveLoadConfigData& that)
    :
    GunnsElectricalResistorConfigData(that),
    mUnderVoltageLimit(that.mUnderVoltageLimit),
    mLoadSwitchResistance(that.mLoadSwitchResistance),
    mPosTripLimit(that.mPosTripLimit),
    mNegTripLimit(that.mNegTripLimit),
    mTripPriority(that.mTripPriority)
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Resistive Load Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistiveLoadConfigData::~GunnsResistiveLoadConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
///
/// @details  Default constructs this Resistive Load input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistiveLoadInputData::GunnsResistiveLoadInputData(const bool   malfBlockageFlag,
                                                         const double malfBlockageValue,
                                                         const bool   initialSwitchPosition)
    :
    GunnsElectricalResistorInputData(malfBlockageFlag, malfBlockageValue),
    mInitialSwitchPosition(initialSwitchPosition)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Resistive Load input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistiveLoadInputData::GunnsResistiveLoadInputData(const GunnsResistiveLoadInputData& that)
    :
    GunnsElectricalResistorInputData(that),
    mInitialSwitchPosition(that.mInitialSwitchPosition)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Resistive Load input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistiveLoadInputData::~GunnsResistiveLoadInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Resistive Load Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistiveLoad::GunnsResistiveLoad():
    mLoadSwitch(),
    mLoadSwitchConfig(),
    mLoadSwitchInput(),
    mVoltage(0.0),
    mUnderVoltageLimit(0.0),
    mPowerValid(false),
    mSwitchPrevClosed(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Resistive Load Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistiveLoad::~GunnsResistiveLoad()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Resistive Load Config Data
/// @param[in]     inputData    (--) Reference to Resistive Load Input Data
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector
/// @param[in]     port0        (--) Port 0 Mapping
/// @param[in]     port1        (--) Port 1 Mapping
///
/// @throws   TsInitializationException
///
/// @details  Initializes the resistive load with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsResistiveLoad::initialize(const GunnsResistiveLoadConfigData& configData,
                                     const GunnsResistiveLoadInputData&  inputData,
                                     std::vector<GunnsBasicLink*>&        networkLinks,
                                     const int                            port0,
                                     const int                            port1)
{
    /// - Initialize the parent class.
    GunnsElectricalResistor::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag = false;
    /// - Initialize Switch Utility for use in this Resistive Load model.
    mLoadSwitchConfig.init(configData.mLoadSwitchResistance, true, true, configData.mUnderVoltageLimit, false, 0, configData.mTripPriority);
    mLoadSwitchInput.init(inputData.mMalfBlockageFlag, static_cast<int>(inputData.mMalfBlockageValue), inputData.mInitialSwitchPosition, configData.mPosTripLimit, configData.mNegTripLimit);
    mLoadSwitch.initialize(mLoadSwitchConfig, mLoadSwitchInput, mName, 0);

    /// - Initialize class attributes.
    mUnderVoltageLimit          = configData.mUnderVoltageLimit;
    mLoadConfigResistance             = configData.mResistance;

    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates the link
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsResistiveLoad::validate() const
{
    /// - Issue an error on conductivity being less than zero.
    if (mUnderVoltageLimit < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    " Resistive Load has an Under Voltage Limit  < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsResistiveLoad::restartModel()
{
    /// - Reset the base class.
    GunnsElectricalResistor::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Override updateState so that resistance can be converted to conductivity and updated
///           in the solver.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsResistiveLoad::updateState(const double dt)
{
    mVoltage = mVoltageIn;
    /// - Update the load switch's state.
    mLoadSwitch.updateSwitchState(true);
    if (!mLoadSwitch.isClosed())
    {
        /// - Set resistance to very large value to effectively prevent any flow if switch is open
        mResistance = 1.0E12;
        mVoltage = 0.0;
    }else if (mSwitchPrevClosed == false && mLoadSwitch.isClosed())
    {
        // When the switch goes from closed to open, set the switch resistance to the config
        // resistance. If the load receives its resistance from the sim-bus, then this will
        // very quickly be over-written.
        mResistance = mLoadConfigResistance;
        //mResistance = 100.0;
    }

    mSwitchPrevClosed = mLoadSwitch.isClosed();
    /// - Call parent class state update routine
    GunnsElectricalResistor::updateState(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Override processOutputs method. This is called by solver after solution converges.
///           It is used here to set PowerValid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsResistiveLoad::processOutputs()
{
    // set power valid to true if voltage is > than underVoltage limit and the load switch is closed.
    mPowerValid = (mVoltageIn > mUnderVoltageLimit) && mLoadSwitch.isClosed();
}
