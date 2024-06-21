/**
@file
@brief    GUNNS Electrical Photovoltaic String Model implementation

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  (
   (software/exceptions/TsInitializationException.o)
   (math/UnitConversion.o)
  )
*/

#include "math/MsMath.hh"
#include "GunnsElectPvString.hh"
#include "core/GunnsBasicNode.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Photovoltaic Cell model config data.  This only exists to
///           avoid ambiguity with the overloaded custom constructors below.  This shouldn't
///           actually be used, as a cell configured with this constructor will not be able to
///           initialize.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvCellConfigData::GunnsElectPvCellConfigData()
    :
    mSurfaceArea(0.0),
    mEfficiency(0.0),
    mSeriesResistance(0.0),
    mShuntResistance(0.0),
    mOpenCircuitVoltage(0.0),
    mRefTemperature(0.0),
    mTemperatureVoltageCoeff(0.0),
    mTemperatureCurrentCoeff(0.0),
    mShortCircuitCurrent(0.0),
    mMppVoltage(0.0),
    mMppCurrent(0.0),
    mPhotoFlux(0.0),
    mIdeality(0.0),
    mIsVersion2(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] cellSurfaceArea             (m2)  Surface area of one side.
/// @param[in] cellEfficiency              (--)  Photovoltaic efficiency (0-1).
/// @param[in] cellSeriesResistance        (ohm) Series resistance.
/// @param[in] cellShuntResistance         (ohm) Shunt resistance.
/// @param[in] cellOpenCircuitVoltage      (V)   Open-circuit voltage.
/// @param[in] cellRefTemperature          (K)   Reference temperature for temperature effects.
/// @param[in] cellTemperatureVoltageCoeff (1/K) Coefficient for temperature effect on open-circuit voltage.
/// @param[in] cellTemperatureVoltageCoeff (1/K) Coefficient for temperature effect on source current.
///
/// @details  This overloaded constructor constructs this Photovoltaic Cell config data for an
///           original version cell model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvCellConfigData::GunnsElectPvCellConfigData(const double cellSurfaceArea,
                                                       const double cellEfficiency,
                                                       const double cellSeriesResistance,
                                                       const double cellShuntResistance,
                                                       const double cellOpenCircuitVoltage,
                                                       const double cellRefTemperature,
                                                       const double cellTemperatureVoltageCoeff,
                                                       const double cellTemperatureCurrentCoeff)
    :
    mSurfaceArea(cellSurfaceArea),
    mEfficiency(cellEfficiency),
    mSeriesResistance(cellSeriesResistance),
    mShuntResistance(cellShuntResistance),
    mOpenCircuitVoltage(cellOpenCircuitVoltage),
    mRefTemperature(cellRefTemperature),
    mTemperatureVoltageCoeff(cellTemperatureVoltageCoeff),
    mTemperatureCurrentCoeff(cellTemperatureCurrentCoeff),
    mShortCircuitCurrent(0.0),
    mMppVoltage(0.0),
    mMppCurrent(0.0),
    mPhotoFlux(0.0),
    mIdeality(0.0),
    mIsVersion2(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] voc         (V)     Open-circuit voltage.
/// @param[in] isc         (amp)   Short-circuit current.
/// @param[in] vmp         (V)     Voltage at the maximum power point.
/// @param[in] imp         (amp)   Current at the maximum power point.
/// @param[in] photoFlux   (W/m2)  Absorbed photo power flux incident on the string.
/// @param[in] temperature (K)     Temperature.
/// @param[in] coeffDVocDT (V/K)   Coefficient for temperature effect on open-circuit voltage.
/// @param[in] coeffDIscDT (amp/K) Coefficient for temperature effect on source current.
/// @param[in] ideality    (1)     Diode ideality constant.
/// @param[in] cellArea    (m2)    Optional cell area, used for efficiency estimation.
///
/// @details  This overloaded constructor constructs this Photovoltaic Cell config data for a
///           version 2 cell model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvCellConfigData::GunnsElectPvCellConfigData(const double voc,
                                                       const double isc,
                                                       const double vmp,
                                                       const double imp,
                                                       const double photoFlux,
                                                       const double temperature,
                                                       const double coeffDVocDT,
                                                       const double coeffDIscDT,
                                                       const double ideality,
                                                       const double cellArea)
    :
    mSurfaceArea(cellArea),
    mEfficiency(0.0),
    mSeriesResistance(0.0),
    mShuntResistance(0.0),
    mOpenCircuitVoltage(voc),
    mRefTemperature(temperature),
    mTemperatureVoltageCoeff(coeffDVocDT),
    mTemperatureCurrentCoeff(coeffDIscDT),
    mShortCircuitCurrent(isc),
    mMppVoltage(vmp),
    mMppCurrent(imp),
    mPhotoFlux(photoFlux),
    mIdeality(ideality),
    mIsVersion2(true)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic Cell config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvCellConfigData::~GunnsElectPvCellConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Reference to the object to be assigned from.
///
/// @details  Assignment operator for the Photovoltaic Cell config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvCellConfigData& GunnsElectPvCellConfigData::operator =(const GunnsElectPvCellConfigData& that)
{
    if (&that != this) {
        mSurfaceArea             = that.mSurfaceArea;
        mEfficiency              = that.mEfficiency;
        mSeriesResistance        = that.mSeriesResistance;
        mShuntResistance         = that.mShuntResistance;
        mOpenCircuitVoltage      = that.mOpenCircuitVoltage;
        mRefTemperature          = that.mRefTemperature;
        mTemperatureVoltageCoeff = that.mTemperatureVoltageCoeff;
        mTemperatureCurrentCoeff = that.mTemperatureCurrentCoeff;
        mShortCircuitCurrent     = that.mShortCircuitCurrent;
        mMppVoltage              = that.mMppVoltage;
        mMppCurrent              = that.mMppCurrent;
        mPhotoFlux               = that.mPhotoFlux;
        mIdeality                = that.mIdeality;
        mIsVersion2              = that.mIsVersion2;
    }
    return *this;
}

/// @details  The ratio of the Boltzmann constant over the electron charge constant.
const double GunnsElectPvCellEquivCircuit::mBoltzmannOverCharge = UnitConversion::BOLTZMANN_CONST / UnitConversion::ELECTRON_CHARGE;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Photovoltaic Cell Equivalent Circuit Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvCellEquivCircuit::GunnsElectPvCellEquivCircuit()
    :
    mIL(0.0),
    mRsh(0.0),
    mRs(0.0),
    mNVt(0.0),
    mI0(0.0),
    mFillFactor(0.0),
    mEfficiency(0.0),
    mVoc(0.0),
    mIsc(0.0),
    mVmp(0.0),
    mImp(0.0),
    mPhotoFlux(0.0),
    mIdeality(0.0),
    mTemperature(0.0),
    mCoeffDVocDT(0.0),
    mCoeffDIscDT(0.0),
    mSurfaceArea(0.0),
    mName("")
{
    clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic Cell Equivalent Circuit Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvCellEquivCircuit::~GunnsElectPvCellEquivCircuit()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Reference to the instance to be assigned values from.
///
/// @returns  GunnsElectPvCellEquivCircuit& (--) Reference to this instance.
///
/// @details  Assigns this Photovoltaic Cell Equivalent Circuit Model to values from the given
///           instance, except for the instance name
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvCellEquivCircuit& GunnsElectPvCellEquivCircuit::operator =(const GunnsElectPvCellEquivCircuit& that)
{
    if (&that != this) {
        mIL          = that.mIL;
        mRsh         = that.mRsh;
        mRs          = that.mRs;
        mNVt         = that.mNVt;
        mI0          = that.mI0;
        mFillFactor  = that.mFillFactor;
        mEfficiency  = that.mEfficiency;
        mVoc         = that.mVoc;
        mIsc         = that.mIsc;
        mVmp         = that.mVmp;
        mImp         = that.mImp;
        mPhotoFlux   = that.mPhotoFlux;
        mIdeality    = that.mIdeality;
        mTemperature = that.mTemperature;
        mCoeffDVocDT = that.mCoeffDVocDT;
        mCoeffDIscDT = that.mCoeffDIscDT;
        mSurfaceArea = that.mSurfaceArea;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Zeroes all attributes of this Photovoltaic Cell Equivalent Circuit Model, except for
///           the instance name.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvCellEquivCircuit::clear()
{
    mIL          = 0.0;
    mRsh         = 0.0;
    mRs          = 0.0;
    mNVt         = 0.0;
    mI0          = 0.0;
    mFillFactor  = 0.0;
    mEfficiency  = 0.0;
    mVoc         = 0.0;
    mIsc         = 0.0;
    mVmp         = 0.0;
    mImp         = 0.0;
    mPhotoFlux   = 0.0;
    mIdeality    = 0.0;
    mTemperature = 0.0;
    mCoeffDVocDT = 0.0;
    mCoeffDIscDT = 0.0;
    mSurfaceArea = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Photovoltaic String Model Load State.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvLoadState::GunnsElectPvLoadState()
{
    clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic String Model Load State.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvLoadState::~GunnsElectPvLoadState()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object to be assigned from.
///
/// @returns  GunnsElectPvLoadState&  (--)  A reference to this object.
///
/// @details  Copies the state of the given Photovoltaic String Model Load State.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvLoadState& GunnsElectPvLoadState::operator =(const GunnsElectPvLoadState& that)
{
    if (&that != this) {
        mConductance = that.mConductance;
        mCurrent     = that.mCurrent;
        mPower       = that.mPower;
        mVoltage     = that.mVoltage;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Zeroes all attributes of this Photovoltaic String Model Load State.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvLoadState::clear()
{
    mConductance = 0.0;
    mCurrent     = 0.0;
    mPower       = 0.0;
    mVoltage     = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Photovoltaic String model config data.  This only exists to
///           avoid ambiguity with the overloaded custom constructors below.  This shouldn't
///           actually be used, as a string configured with this constructor will not be able to
///           initialize.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvStringConfigData::GunnsElectPvStringConfigData()
    :
    mBlockingDiodeVoltageDrop(0.0),
    mBypassDiodeVoltageDrop(0.0),
    mBypassDiodeInterval(0),
    mNumCells(0),
    mCellConfig()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] stringBlockingDiodeVoltageDrop (V)   Voltage drop across the diode at end of string.
/// @param[in] stringBypassDiodeVoltageDrop   (V)   Voltage drop across each bypass diode.
/// @param[in] stringBypassDiodeInterval      (--)  Number of cells per bypass diode.
/// @param[in] stringNumCells                 (--)  Number of cells in this string.
/// @param[in] cellSurfaceArea                (m2)  Cell surface area of one side.
/// @param[in] cellEfficiency                 (--)  Cell photovoltaic efficiency (0-1).
/// @param[in] cellSeriesResistance           (ohm) Cell series resistance.
/// @param[in] cellShuntResistance            (ohm) Cell shunt resistance.
/// @param[in] cellOpenCircuitVoltage         (V)   Cell open-circuit voltage.
/// @param[in] cellRefTemperature             (K)   Cell Reference temperature for temperature effects.
/// @param[in] cellTemperatureVoltageCoeff    (1/K) Cell coefficient for temperature effect on open-circuit voltage.
/// @param[in] cellTemperatureVoltageCoeff    (1/K) Cell coefficient for temperature effect on source current.
///
/// @details  This overloaded constructor constructs this Photovoltaic String Model config data for
///           an original version string model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvStringConfigData::GunnsElectPvStringConfigData(
        const double       stringBlockingDiodeVoltageDrop,
        const double       stringBypassDiodeVoltageDrop,
        const unsigned int stringBypassDiodeInterval,
        const unsigned int stringNumCells,
        const double       cellSurfaceArea,
        const double       cellEfficiency,
        const double       cellSeriesResistance,
        const double       cellShuntResistance,
        const double       cellOpenCircuitVoltage,
        const double       cellRefTemperature,
        const double       cellTemperatureVoltageCoeff,
        const double       cellTemperatureCurrentCoeff)
    :
    mBlockingDiodeVoltageDrop(stringBlockingDiodeVoltageDrop),
    mBypassDiodeVoltageDrop(stringBypassDiodeVoltageDrop),
    mBypassDiodeInterval(stringBypassDiodeInterval),
    mNumCells(stringNumCells),
    mCellConfig(cellSurfaceArea,
                cellEfficiency,
                cellSeriesResistance,
                cellShuntResistance,
                cellOpenCircuitVoltage,
                cellRefTemperature,
                cellTemperatureVoltageCoeff,
                cellTemperatureCurrentCoeff)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] stringBlockingDiodeVoltageDrop (V)     Voltage drop across the diode at end of string.
/// @param[in] stringBypassDiodeVoltageDrop   (V)     Voltage drop across each bypass diode.
/// @param[in] stringBypassDiodeInterval      (--)    Number of cells per bypass diode.
/// @param[in] stringNumCells                 (--)    Number of cells in this string.
/// @param[in] cellRefVoc                     (V)     Reference cell open-circuit voltage.
/// @param[in] cellRefIsc                     (amp)   Reference cell short-circuit current.
/// @param[in] cellRefVmp                     (V)     Reference cell voltage at the maximum power point.
/// @param[in] cellRefImp                     (amp)   Reference cell current at the maximum power point.
/// @param[in] cellRefPhotoFlux               (W/m2)  Reference cell absorbed photo power flux.
/// @param[in] cellRefTemperature             (K)     Reference cell temperature.
/// @param[in] cellCoeffDVocDT                (V/K)   Coefficient for temperature effect on open-circuit voltage.
/// @param[in] cellCoeffDIscDT                (amp/K) Coefficient for temperature effect on source current.
/// @param[in] cellIdeality                   (1)     Cell equivalent diode ideality constant.
/// @param[in] cellArea                       (m2)    Optional cell area, used for efficiency estimation.
///
/// @details  This overloaded constructor constructs this Photovoltaic String Model config data for
///           a Version 2 string model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvStringConfigData::GunnsElectPvStringConfigData(
        const double       stringBlockingDiodeVoltageDrop,
        const double       stringBypassDiodeVoltageDrop,
        const unsigned int stringBypassDiodeInterval,
        const unsigned int stringNumCells,
        const double       cellRefVoc,
        const double       cellRefIsc,
        const double       cellRefVmp,
        const double       cellRefImp,
        const double       cellRefPhotoFlux,
        const double       cellRefTemperature,
        const double       cellCoeffDVocDT,
        const double       cellCoeffDIscDT,
        const double       cellIdeality,
        const double       cellArea)
    :
    mBlockingDiodeVoltageDrop(stringBlockingDiodeVoltageDrop),
    mBypassDiodeVoltageDrop(stringBypassDiodeVoltageDrop),
    mBypassDiodeInterval(stringBypassDiodeInterval),
    mNumCells(stringNumCells),
    mCellConfig(cellRefVoc,
                cellRefIsc,
                cellRefVmp,
                cellRefImp,
                cellRefPhotoFlux,
                cellRefTemperature,
                cellCoeffDVocDT,
                cellCoeffDIscDT,
                cellIdeality,
                cellArea)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic String Model config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvStringConfigData::~GunnsElectPvStringConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Reference to the object to be assigned from.
///
/// @details  Assignment operator for the Photovoltaic String Model config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvStringConfigData& GunnsElectPvStringConfigData::operator =(const GunnsElectPvStringConfigData& that)
{
    if (&that != this) {
        mBlockingDiodeVoltageDrop = that.mBlockingDiodeVoltageDrop;
        mBypassDiodeVoltageDrop   = that.mBypassDiodeVoltageDrop;
        mBypassDiodeInterval      = that.mBypassDiodeInterval;
        mNumCells                 = that.mNumCells;
        mCellConfig               = that.mCellConfig;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] stringPhotoFlux             (W/m2) Photo power flux incident on the string.
/// @param[in] stringSourceExposedFraction (--)   Surface area fraction exposed to light source (0-1).
/// @param[in] stringTemperature           (K)    Temperature of the string.
///
/// @details  Default constructs this Photovoltaic String Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvStringInputData::GunnsElectPvStringInputData(const double stringPhotoFlux,
                                                         const double stringSourceExposedFraction,
                                                         const double stringTemperature)
    :
    mPhotoFlux(stringPhotoFlux),
    mSourceExposedFraction(stringSourceExposedFraction),
    mTemperature(stringTemperature),
    mMalfPhotoFluxFlag(false),
    mMalfPhotoFluxMagnitude(0.0),
    mMalfPhotoFluxDuration(0.0),
    mMalfPhotoFluxRampTime(0.0),
    mMalfExposedFractionFlag(false),
    mMalfExposedFractionValue(0.0),
    mMalfTemperatureFlag(false),
    mMalfTemperatureValue(0.0),
    mPhotoFluxElapsedTime(0.0),
    mPhotoFluxStartMagnitude(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic String Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvStringInputData::~GunnsElectPvStringInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  Applies malfunctions to override the string input data values.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvStringInputData::applyOverrides(const double dt)
{
    /// - The photo power flux malfunction overrides the flux from its starting value to a hold
    ///   value, and back to the environment value over the given duration.  Ramping between the
    ///   hold and start/end values is with a sinusoid function.  This can be used to mimic eclipse
    ///   and subsequent insolation events by a planetary body.
    if (mMalfPhotoFluxFlag and mMalfPhotoFluxDuration > 0.0) {
        /// - Limit ramp time to 1/2 duration.
        mMalfPhotoFluxRampTime = MsMath::limitRange(0.0, mMalfPhotoFluxRampTime, 0.5 * mMalfPhotoFluxDuration);
        if (mPhotoFluxElapsedTime > mMalfPhotoFluxDuration) {
            /// - Automatically switch off when full duration has elapsed.
            mPhotoFluxElapsedTime = 0.0;
            mMalfPhotoFluxFlag    = false;
        } else if (0.0 == mPhotoFluxElapsedTime) {
            /// - At the start, store the starting photo flux magnitude.
            mPhotoFluxStartMagnitude = mPhotoFlux;
        } else if (mPhotoFluxElapsedTime > (mMalfPhotoFluxDuration - mMalfPhotoFluxRampTime)) {
            /// - Ramp out from the hold value to the input magnitude value from environment.
            mPhotoFlux = rampPhotoFlux(mMalfPhotoFluxDuration - mPhotoFluxElapsedTime, mPhotoFlux);
        } else if (mPhotoFluxElapsedTime > mMalfPhotoFluxRampTime) {
            /// - Hold the value between ramp in/ramp out.
            mPhotoFlux = mMalfPhotoFluxMagnitude;
        } else {
            /// - Ramp in from the starting magnitude to the hold value.
            mPhotoFlux = rampPhotoFlux(mPhotoFluxElapsedTime, mPhotoFluxStartMagnitude);
        }
        mPhotoFluxElapsedTime += dt;
    } else {
        mPhotoFluxElapsedTime = 0.0;
    }

    /// - Apply the source exposed fraction malfunction.
    if (mMalfExposedFractionFlag) {
        mSourceExposedFraction = mMalfExposedFractionValue;
    }

    /// - Apply the temperature malfunction.
    if (mMalfTemperatureFlag) {
        mTemperature = mMalfTemperatureValue;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] time     (s)    Elapsed ramp time.
/// @param[in] outValue (W/m2) Ramp-out value to ramp from/to.
///
/// @returns  double (W/m2) Photo power flux.
///
/// @details  Computes and returns the photo power flux as a sinusoid from the start value to the
///           hold value over the given elapsed time.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsElectPvStringInputData::rampPhotoFlux(const double time, const double outValue)
{
    double rampFunction = 1.0;
    if (mMalfPhotoFluxRampTime > 0.0) {
        const double timeFraction = MsMath::limitRange(0.0, time / mMalfPhotoFluxRampTime, 1.0);
        rampFunction = 0.5 * (1.0 + sin(UnitConversion::PI_UTIL * (timeFraction - 0.5)));
    }
    return outValue + rampFunction * (mMalfPhotoFluxMagnitude - outValue);
}

/// @details  This value is used as the ratio between forward and reverse bias conductance of the
///           modeled diodes.  This value is a trade between creating an ideal enough conductance
///           (high ratio) and avoiding truncation between conductances of differing magnitudes in
///           the network solution (low ratio).
const double GunnsElectPvString::mIdealDiodeFactor = 1.0E6;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Photovoltaic String Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvString::GunnsElectPvString()
    :
    mMalfCellGroupFlag(false),
    mMalfCellGroupValue(0),
    mMalfDegradeFlag(false),
    mMalfDegradeValue(0.0),
    mName(""),
    mConfig(0),
    mInput(0),
    mShuntVoltageDrop(0.0),
    mSeriesVoltageDrop(0.0),
    mShortCircuitCurrent(0.0),
    mOpenCircuitVoltage(0.0),
    mEqProps(0),
    mMpp(),
    mTerminal(),
    mNumBypassedGroups(0),
    mNumActiveCells(0),
    mShunted(false),
    mRefCell(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Pointer to the string config data.
/// @param[in] inputData  (--) Pointer to this string's input data.
///
/// @details  Default constructs this Photovoltaic String Model with pointers to its configuration
///           & input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvString::GunnsElectPvString(const GunnsElectPvStringConfigData* configData,
                                       const GunnsElectPvStringInputData*  inputData)
    :
    mMalfCellGroupFlag(false),
    mMalfCellGroupValue(0),
    mMalfDegradeFlag(false),
    mMalfDegradeValue(0.0),
    mName(""),
    mConfig(configData),
    mInput(inputData),
    mShuntVoltageDrop(0.0),
    mSeriesVoltageDrop(0.0),
    mShortCircuitCurrent(0.0),
    mOpenCircuitVoltage(0.0),
    mEqProps(0),
    mMpp(),
    mTerminal(),
    mNumBypassedGroups(0),
    mNumActiveCells(0),
    mShunted(false),
    mRefCell(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic String Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvString::~GunnsElectPvString()
{
    TS_DELETE_OBJECT(mEqProps);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name (--) The instance name for H&S messages.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Photovoltaic String Model with its instance name and validates its
///           configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvString::initialize(const std::string& name)
{
    /// - Initialize the instance name and validate configuration data.
    mName = name;
    validate();

    /// - Create the equivalent cell model for this string version.
    TS_NEW_CLASS_OBJECT_EXT(mEqProps, GunnsElectPvCellEquivCircuit, (), mName + ".mEqProps");

    /// - Initialize state.
    mShortCircuitCurrent = 0.0;
    mOpenCircuitVoltage  = 0.0;
    mEqProps->clear();
    mMpp.clear();
    mTerminal.clear();
    mNumBypassedGroups   = 0;
    mNumActiveCells      = 0;
    mShunted             = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this Photovoltaic String Model configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvString::validate() const
{
    /// - Throw an exception on missing instance name.
    if (mName.empty()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "empty instance name.");
    }

    /// - Throw an exception on null pointer to config data.
    if (!mConfig) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "mConfig is null pointer.");
    }

    /// - Throw an exception on cell surface area < DBL_EPSILON.
    if (mConfig->mCellConfig.mSurfaceArea < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "cell surface area < DBL_EPSILON.");
    }

    /// - Throw an exception on cell efficiency not in range.
    if (not MsMath::isInRange(0.0, mConfig->mCellConfig.mEfficiency, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "cell efficiency not in 0-1.");
    }

    /// - Throw an exception on cell series resistance < DBL_EPSILON.
    if (mConfig->mCellConfig.mSeriesResistance < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "cell series resistance < DBL_EPSILON.");
    }

    /// - Throw an exception on cell shunt resistance < DBL_EPSILON.
    if (mConfig->mCellConfig.mShuntResistance < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "cell shunt resistance < DBL_EPSILON.");
    }

    /// - Throw an exception on cell open-circuit voltage < DBL_EPSILON.
    if (mConfig->mCellConfig.mOpenCircuitVoltage < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "cell open-circuit voltage < DBL_EPSILON.");
    }

    /// - Throw an exception on # cells < 1.
    if (mConfig->mNumCells < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "number of cells < 1.");
    }

    /// - Throw an exception on bypass diode interval < 1.
    if (mConfig->mBypassDiodeInterval < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "bypass diode interval < 1.");
    }

    /// - Throw an exception if bypass diode interval not evenly divisible in total # cells.
    if (0 != mConfig->mNumCells % mConfig->mBypassDiodeInterval) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "bypass diode interval not evenly divisible in number of cells.");
    }

    /// - Throw an exception on null pointer to input data.
    if (!mInput) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "mInput is null pointer.");
    }

    /// - Throw an exception on photo flux magnitude < 0.
    if (mInput->mPhotoFlux < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "initial photo flux magnitude < 0.");
    }

    /// - Throw an exception on source exposed fraction not in 0-1.
    if (not MsMath::isInRange(0.0, mInput->mSourceExposedFraction, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "initial source exposed fraction not in 0-1.");
    }

    /// - Throw an exception on temperature < 0.
    if (mInput->mTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "initial temperature < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates this Photovoltaic String Model's state, including the equivalent circuit
///           properties, based on the current input conditions such as lighting and temperature.
///           These states do not depend on the current loading of the string.  Effects of loading
///           are calculated in other functions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvString::update()
{
    /// - Number of active cells reduced by the bypassed groups.
    updateBypassedGroups();
    mNumActiveCells = mConfig->mNumCells - mNumBypassedGroups * mConfig->mBypassDiodeInterval;

    /// - Shunt resistance.
    mEqProps->mRsh = std::max(1.0 / mIdealDiodeFactor,
                                         mConfig->mCellConfig.mShuntResistance);

    /// - Shunt voltage drop is the effective open-circuit voltage of the string based on number of
    ///   active cells & temperature.
    const double dT = mInput->mTemperature - mConfig->mCellConfig.mRefTemperature;
    mShuntVoltageDrop = mNumActiveCells * mConfig->mCellConfig.mOpenCircuitVoltage
            * std::max(1.0 + dT * mConfig->mCellConfig.mTemperatureVoltageCoeff, 0.0);

    /// - Series resistance.
    mEqProps->mRs = std::max(1.0 / mIdealDiodeFactor,
                                          mNumActiveCells * mConfig->mCellConfig.mSeriesResistance);

    /// - Series diode voltage drop.
    mSeriesVoltageDrop = mConfig->mBlockingDiodeVoltageDrop
                       + mNumBypassedGroups * mConfig->mBypassDiodeVoltageDrop;

    /// - Compute short-circuit current and Maximum Power Point.
    if (mShuntVoltageDrop > DBL_EPSILON) {
        /// - Fraction of photo power converted to electricity based on cell efficiency and
        ///   temperature.
        const double conv = mConfig->mCellConfig.mEfficiency
                          * std::max(1.0 + dT * mConfig->mCellConfig.mTemperatureCurrentCoeff, 0.0);

        /// - Electrical power (W) produced by photovoltaic conversion and reduced by the degrade
        ///   malf.
        double power = mInput->mPhotoFlux * mNumActiveCells * mConfig->mCellConfig.mSurfaceArea * conv;
        if (mMalfDegradeFlag) {
            power *= MsMath::limitRange(0.0, (1.0 - mMalfDegradeValue), 1.0);
        }
        mEqProps->mIL = power / mShuntVoltageDrop;
    } else {
        mEqProps->mIL = 0.0;
    }

    if (mNumActiveCells > 0) {
        mOpenCircuitVoltage  = mShuntVoltageDrop - mSeriesVoltageDrop
                + mEqProps->mIL * mEqProps->mRsh / mIdealDiodeFactor;
        mShortCircuitCurrent = std::max(0.0,
                (mEqProps->mIL * mEqProps->mRsh - mSeriesVoltageDrop)
                / (mEqProps->mRsh + mEqProps->mRs));
        updateMpp();
    } else {
        mOpenCircuitVoltage  = 0.0;
        mShortCircuitCurrent = 0.0;
        mMpp.clear();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Determines the number of cell groups that are bypassed.  Since all cells in a group
///           are in series, if any one cell is either shaded or failed, it blocks current through
///           the entire cell group, and current from upstream is bypassed around the group through
///           its bypass diode.
///
///           For simplicity, we don't specify which groups are failed vs. shaded.  Rather the # of
///           active groups is reduced by the # of failed or shaded groups, whichever is greater.
///
///           In reality, bad facing and low ambient light flux would cause bypass too.  But these
///           affect all groups equally and their bulk affects on the string are already modeled, so
///           we don't bother to count them here.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvString::updateBypassedGroups()
{
    const int numGroups = mConfig->mNumCells / mConfig->mBypassDiodeInterval;
    int numBypassedGroups = ceil(numGroups * (1.0 - MsMath::limitRange(0.0, mInput->mSourceExposedFraction, 1.0)));
    if (mMalfCellGroupFlag) {
        mMalfCellGroupValue = MsMath::limitRange(0, mMalfCellGroupValue, numGroups);
        numBypassedGroups   = std::max(numBypassedGroups, mMalfCellGroupValue);
    }
    mNumBypassedGroups = numBypassedGroups;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the Maximum Power Point assuming the junction node is at the open-circuit
///           voltage, shunt diode is reverse biased and series diode is forward biased.
///
/// @note     Caller must ensure mEqProps->mSeriesConductance > 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvString::updateMpp()
{
    /// - Total electrical power, shunt current and series current.
    const double power = mShuntVoltageDrop * mEqProps->mIL;
    const double ish   = mShuntVoltageDrop / mEqProps->mRsh;
    const double is    = mEqProps->mIL - ish;

    /// - Maximum Power Point.
    mMpp.mPower = std::max(0.0, power - mShuntVoltageDrop*ish - is*mSeriesVoltageDrop
                - is*is*mNumActiveCells*mConfig->mCellConfig.mSeriesResistance);

    if (mMpp.mPower > 0.0) {
        mMpp.mVoltage     = std::max(DBL_EPSILON, mShuntVoltageDrop
                          - is*mEqProps->mRs - mSeriesVoltageDrop);
        mMpp.mCurrent     = mMpp.mPower / mMpp.mVoltage;
        mMpp.mConductance = mMpp.mCurrent / mMpp.mVoltage;
    } else {
        mMpp.mVoltage     = 0.0;
        mMpp.mCurrent     = 0.0;
        mMpp.mConductance = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  power      (W)   The output power load to apply.
/// @param[in]  shortSide  (--)  True uses the solution on the short-circuit side of maximum power.
///
/// @details  This loads the string at the given power output and on the given side of the string's
///           Maximum Power Point on its I-V performance curve.  If the given power exceeds the
///           string's maximum power output then the terminal outputs are zeroed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvString::loadAtPower(const double power, const bool shortSide)
{
    if ((power > 0.0) and (power < mMpp.mPower) and (mMpp.mCurrent < mShortCircuitCurrent)) {
        double qa, qb, sign;
        double qc = -power;
        if (shortSide) {
            qa = mMpp.mVoltage / (mMpp.mCurrent - mShortCircuitCurrent);
            qb = -qa * mShortCircuitCurrent;
            sign = -1.0;
        } else {
            qb = mOpenCircuitVoltage;
            qa = (mMpp.mVoltage - mOpenCircuitVoltage) / mMpp.mCurrent;
            sign = 1.0;
        }
        const double bb4ac = qb*qb - 4.0*qa*qc;
        if (bb4ac > 0.0 and qa < 0.0) {
            mTerminal.mCurrent     = 0.5 * (sign * sqrt(bb4ac) - qb) / qa;
            mTerminal.mPower       = power;
            mTerminal.mVoltage     = mTerminal.mPower / mTerminal.mCurrent;
            mTerminal.mConductance = mTerminal.mCurrent / mTerminal.mVoltage;
        } else {
            GUNNS_WARNING("cannot compute load.");
        }
    } else {
        mTerminal.clear();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  v1  (V)  The terminal voltage to calculate outputs for.
///
/// @details  This loads the string at the given terminal voltage on its I-V curve and computes the
///           resulting terminal output state.  If the given terminal voltage exceeds the limits of
///           the I-V curve, then the terminal outputs are zeroed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvString::loadAtVoltage(const double v1)
{
    mTerminal.mVoltage = v1;
    if (not MsMath::isInRange(DBL_EPSILON, v1, mOpenCircuitVoltage)) {
        mTerminal.mPower       = 0.0;
        mTerminal.mConductance = 0.0;
        mTerminal.mCurrent     = 0.0;

    } else if (v1 <= mMpp.mVoltage) {
        mTerminal.mCurrent     = mShortCircuitCurrent - (mShortCircuitCurrent - mMpp.mCurrent)
                               * v1 / mMpp.mVoltage;
        mTerminal.mPower       = v1 * mTerminal.mCurrent;
        mTerminal.mConductance = mTerminal.mCurrent / v1;

    } else {
        mTerminal.mCurrent     = mMpp.mCurrent * (mOpenCircuitVoltage - v1)
                               / (mOpenCircuitVoltage - mMpp.mVoltage);
        mTerminal.mPower       = v1 * mTerminal.mCurrent;
        mTerminal.mConductance = mTerminal.mCurrent / v1;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  g  (1/ohm)  The load conductance to compute the results for.
///
/// @details  This loads the string with the given conductive load and computes the resulting
///           terminal output state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvString::loadAtConductance(const double g)
{
    mTerminal.mConductance = g;
    if (g < DBL_EPSILON or mShortCircuitCurrent < DBL_EPSILON) {
        mTerminal.mVoltage = mOpenCircuitVoltage;
        mTerminal.mCurrent = 0.0;
        mTerminal.mPower   = 0.0;

    } else if (g <= mMpp.mConductance) {
        mTerminal.mCurrent = g * mOpenCircuitVoltage
                           / (1.0 - g * (mMpp.mVoltage - mOpenCircuitVoltage) / mMpp.mCurrent);
        mTerminal.mVoltage = mTerminal.mCurrent / g;
        mTerminal.mPower   = mTerminal.mCurrent * mTerminal.mVoltage;

    } else {
        const double slope = mMpp.mVoltage / (mMpp.mCurrent - mShortCircuitCurrent);
        mTerminal.mCurrent = -g * slope * mShortCircuitCurrent / (1.0 - g * slope);
        mTerminal.mVoltage = mTerminal.mCurrent / g;
        mTerminal.mPower   = mTerminal.mCurrent * mTerminal.mVoltage;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  v1  (V)  The terminal voltage to calculate outputs for.
///
/// @returns  double (amp) String current at the given voltage.
///
/// @details  This is similar to loadAtVoltage method, but only returns the current, and doesn't
///           store the result or actually load the string.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsElectPvString::predictCurrentAtVoltage(const double v1) const
{
    double current;
    if (not MsMath::isInRange(DBL_EPSILON, v1, mOpenCircuitVoltage)) {
        current = 0.0;

    } else if (v1 <= mMpp.mVoltage) {
        current = mShortCircuitCurrent - (mShortCircuitCurrent - mMpp.mCurrent) * v1 / mMpp.mVoltage;

    } else {
        current = mMpp.mCurrent * (mOpenCircuitVoltage - v1) / (mOpenCircuitVoltage - mMpp.mVoltage);
    }
    return current;
}
