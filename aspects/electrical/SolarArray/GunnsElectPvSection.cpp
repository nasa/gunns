/**
@file     GunnsElectPvSection.cpp
@brief    GUNNS Electrical Photovoltaic Section Model implementation

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  (
   (GunnsElectPvString.o)
   (GunnsElectPvString2.o)
   (software/exceptions/TsInitializationException.o)
  )
*/

#include "math/MsMath.hh"
#include "GunnsElectPvSection.hh"
#include "core/GunnsBasicNode.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Photovoltaic Section model config data.  This only exists to
///           avoid ambiguity with the overloaded custom constructors below.  This shouldn't
///           actually be used, as a section configured with this constructor will not be able to
///           initialize.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvSectionConfigData::GunnsElectPvSectionConfigData()
    :
    mSourceAngleExponent(0.0),
    mBacksideReduction(0.0),
    mSourceAngleEdgeOn(false),
    mRefSourceFluxMagnitude(0.0),
    mStringConfig()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] sectionSourceAngleExponent     (--)   Exponent on trig function of light source incident angle.
/// @param[in] sectionBacksideReduction       (--)   Reduction fraction (0-1) when lit from back side.
/// @param[in] sectionSourceAngleEdgeOn       (--)   Angle of light source to surface is edge-on instead of normal.
/// @param[in] sectionRefSourceFluxMagnitude  (W/m2) Reference ambient flux magnitude of light source at the surface.
/// @param[in] stringBlockingDiodeVoltageDrop (v)    Voltage drop across the diode at end of string.
/// @param[in] stringBypassDiodeVoltageDrop   (v)    Voltage drop across each bypass diode.
/// @param[in] stringBypassDiodeInterval      (--)   Number of cells per bypass diode.
/// @param[in] stringNumCells                 (--)   Number of cells in this string.
/// @param[in] cellSurfaceArea                (m2)   Cell surface area of one side.
/// @param[in] cellEfficiency                 (--)   Cell photovoltaic efficiency (0-1).
/// @param[in] cellSeriesResistance           (ohm)  Cell series resistance.
/// @param[in] cellShuntResistance            (ohm)  Cell shunt resistance.
/// @param[in] cellOpenCircuitVoltage         (v)    Cell open-circuit voltage.
/// @param[in] cellRefTemperature             (K)    Cell Reference temperature for temperature effects.
/// @param[in] cellTemperatureVoltageCoeff    (1/K)  Cell coefficient for temperature effect on open-circuit voltage.
/// @param[in] cellTemperatureVoltageCoeff    (1/K)  Cell coefficient for temperature effect on source current.
///
/// @details  This overloaded constructor constructs this Photovoltaic Section Mode config data for
///           the original version strings.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvSectionConfigData::GunnsElectPvSectionConfigData(
        const double       sectionSourceAngleExponent,
        const double       sectionBacksideReduction,
        const bool         sectionSourceAngleEdgeOn,
        const double       sectionRefSourceFluxMagnitude,
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
    mSourceAngleExponent(sectionSourceAngleExponent),
    mBacksideReduction(sectionBacksideReduction),
    mSourceAngleEdgeOn(sectionSourceAngleEdgeOn),
    mRefSourceFluxMagnitude(sectionRefSourceFluxMagnitude),
    mStringConfig(stringBlockingDiodeVoltageDrop,
                  stringBypassDiodeVoltageDrop,
                  stringBypassDiodeInterval,
                  stringNumCells,
                  cellSurfaceArea,
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
/// @param[in] cellRefVoc                     (V)     Reference cell open-circuit voltage.
/// @param[in] cellRefIsc                     (amp)   Reference cell short-circuit current.
/// @param[in] cellRefVmp                     (V)     Reference cell voltage at the maximum power point.
/// @param[in] cellRefImp                     (amp)   Reference cell current at the maximum power point.
/// @param[in] cellRefTemperature             (K)     Reference cell temperature.
/// @param[in] cellCoeffDVocDT                (V/K)   Coefficient for temperature effect on open-circuit voltage.
/// @param[in] cellCoeffDIscDT                (amp/K) Coefficient for temperature effect on source current.
/// @param[in] cellIdeality                   (1)     Cell equivalent diode ideality constant.
/// @param[in] cellArea                       (m2)    Optional cell area, used for efficiency estimation.
/// @param[in] sectionSourceAngleExponent     (--)    Exponent on trig function of light source incident angle.
/// @param[in] sectionBacksideReduction       (--)    Reduction fraction (0-1) when lit from back side.
/// @param[in] sectionSourceAngleEdgeOn       (--)    Angle of light source to surface is edge-on instead of normal.
/// @param[in] sectionRefSourceFluxMagnitude  (W/m2)  Reference ambient flux magnitude of light source absorbed by the surface.
/// @param[in] stringBlockingDiodeVoltageDrop (v)     Voltage drop across the diode at end of string.
/// @param[in] stringBypassDiodeVoltageDrop   (v)     Voltage drop across each bypass diode.
/// @param[in] stringBypassDiodeInterval      (--)    Number of cells per bypass diode.
/// @param[in] stringNumCells                 (--)    Number of cells in this string.
///
/// @details  This overloaded constructor constructs this Photovoltaic Section Mode config data for
///           the version 2 strings.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvSectionConfigData::GunnsElectPvSectionConfigData(
        const double       cellRefVoc,
        const double       cellRefIsc,
        const double       cellRefVmp,
        const double       cellRefImp,
        const double       cellRefTemperature,
        const double       cellCoeffDVocDT,
        const double       cellCoeffDIscDT,
        const double       cellIdeality,
        const double       cellArea,
        const double       sectionSourceAngleExponent,
        const double       sectionBacksideReduction,
        const bool         sectionSourceAngleEdgeOn,
        const double       sectionRefSourceFluxMagnitude,
        const double       stringBlockingDiodeVoltageDrop,
        const double       stringBypassDiodeVoltageDrop,
        const unsigned int stringBypassDiodeInterval,
        const unsigned int stringNumCells)
    :
    mSourceAngleExponent(sectionSourceAngleExponent),
    mBacksideReduction(sectionBacksideReduction),
    mSourceAngleEdgeOn(sectionSourceAngleEdgeOn),
    mRefSourceFluxMagnitude(sectionRefSourceFluxMagnitude),
    mStringConfig(stringBlockingDiodeVoltageDrop,
                  stringBypassDiodeVoltageDrop,
                  stringBypassDiodeInterval,
                  stringNumCells,
                  cellRefVoc,
                  cellRefIsc,
                  cellRefVmp,
                  cellRefImp,
                  sectionRefSourceFluxMagnitude,
                  cellRefTemperature,
                  cellCoeffDVocDT,
                  cellCoeffDIscDT,
                  cellIdeality,
                  cellArea)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic Section Model config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvSectionConfigData::~GunnsElectPvSectionConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Reference to the object to be assigned from.
///
/// @details  Assignment operator for the Photovoltaic Section Model config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvSectionConfigData& GunnsElectPvSectionConfigData::operator =(const GunnsElectPvSectionConfigData& that)
{
    if (&that != this) {
        mSourceAngleExponent    = that.mSourceAngleExponent;
        mBacksideReduction      = that.mBacksideReduction;
        mSourceAngleEdgeOn      = that.mSourceAngleEdgeOn;
        mRefSourceFluxMagnitude = that.mRefSourceFluxMagnitude;
        mStringConfig           = that.mStringConfig;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] sectionSourceFluxMagnitude   (W/m2) Ambient flux magnitude of light source at the surface.
/// @param[in] sectionSourceAngle           (rad)  Angle of light source to surface.
/// @param[in] sectionSourceExposedFraction (--)   Surface area fraction exposed to light source (0-1).
/// @param[in] sectionTemperature           (K)    Temperature of the section.
///
/// @details  Default constructs this Photovoltaic Section Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvSectionInputData::GunnsElectPvSectionInputData(
        const double sectionSourceFluxMagnitude,
        const double sectionSourceAngle,
        const double sectionSourceExposedFraction,
        const double sectionTemperature)
    :
    mSourceFluxMagnitude(sectionSourceFluxMagnitude),
    mSourceAngle(sectionSourceAngle),
    mSourceExposedFraction(sectionSourceExposedFraction),
    mTemperature(sectionTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic Section Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvSectionInputData::~GunnsElectPvSectionInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Reference to the object to be assigned from.
///
/// @details  Assignment operator for the Photovoltaic Section Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvSectionInputData& GunnsElectPvSectionInputData::operator =(const GunnsElectPvSectionInputData& that)
{
    if (&that != this) {
        mSourceFluxMagnitude   = that.mSourceFluxMagnitude;
        mSourceAngle           = that.mSourceAngle;
        mSourceExposedFraction = that.mSourceExposedFraction;
        mTemperature           = that.mTemperature;
    }
    return *this;
}

/// @details  This value is chosen as a upper sanity limit to avoid math fault.
const double GunnsElectPvSection::mMaxAngleExponent = 10.0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Photovoltaic Section Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvSection::GunnsElectPvSection()
    :
    mStrings(0),
    mStringsInput(),
    mInput(),
    mConfig(0),
    mNumStrings(0),
    mPercentInsolation(0.0),
    mTerminalPower(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Pointer to the section config data.
///
/// @details  Default constructs this Photovoltaic Section Model with pointer to its config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvSection::GunnsElectPvSection(const GunnsElectPvSectionConfigData* configData)
    :
    mStrings(0),
    mStringsInput(),
    mInput(),
    mConfig(configData),
    mNumStrings(0),
    mPercentInsolation(0.0),
    mTerminalPower(9.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic Section Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvSection::~GunnsElectPvSection()
{
    TS_DELETE_ARRAY(mStrings);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name               (--) The instance name for H&S messages.
/// @param[in] inputData          (--) Reference to this section's initial input data.
/// @param[in] numStringsOverride (--) Overrides the config data number of strings for this section.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Photovoltaic Section Model with its instance name and input data and
///           validates its configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvSection::initialize(const std::string&                  name,
                                     const GunnsElectPvSectionInputData& inputData,
                                     const unsigned int                  numStrings)
{
    /// - Initialize the instance name and input data and validate configuration and input data.
    mName       = name;
    mInput      = inputData;
    mNumStrings = numStrings;
    validate();

    /// - Construct the array of string objects of type determined from the config data.
    if (mConfig->mStringConfig.mCellConfig.isVersion2()) {
        TS_NEW_CLASS_ARRAY_EXT(mStrings, static_cast<int>(mNumStrings), GunnsElectPvString2,
                               (&mConfig->mStringConfig, &mStringsInput), std::string(mName) + ".mStrings");
    } else {
        TS_NEW_CLASS_ARRAY_EXT(mStrings, static_cast<int>(mNumStrings), GunnsElectPvString,
                               (&mConfig->mStringConfig, &mStringsInput), std::string(mName) + ".mStrings");
    }

    /// - Initialize the strings.
    for (unsigned int i=0; i<mNumStrings; ++i) {
        std::ostringstream stream;
        stream << ".mStrings_" << i;
        mStrings[i].initialize(mName + stream.str());
    }

    /// - Initialize state.
    mPercentInsolation = 0.0;
    mTerminalPower     = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this Photovoltaic Section configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvSection::validate() const
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

    /// - Throw an exception on source angle exponent not in range.
    if (not MsMath::isInRange(1.0/mMaxAngleExponent, mConfig->mSourceAngleExponent, mMaxAngleExponent)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "source angle exponent not in limits");
    }

    /// - Throw an exception on backside reduction not in 0-1.
    if (not MsMath::isInRange(0.0, mConfig->mBacksideReduction, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "backside reduction not in 0-1.");
    }

    /// - Throw an exception on # strings < 1.
    if (mNumStrings < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "number of strings < 1.");
    }

    /// - Throw an exception on source flux magnitude < 0.
    if (mInput.mSourceFluxMagnitude < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "initial source flux magnitude < 0.");
    }

    /// - Throw an exception on source exposed fraction not in 0-1.
    if (not MsMath::isInRange(0.0, mInput.mSourceExposedFraction, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "initial source exposed fraction not in 0-1.");
    }

    /// - Throw an exception on temperature < 0.
    if (mInput.mTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "initial temperature < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  Updates this Photovoltaic Section Model's state, including the contained string
///           states.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvSection::update(const double dt)
{
    updateEnvironment(dt);

    /// - Update the strings internal states, and accumulate total power for the section.  The
    ///   total power is negative, this can be given to a thermal aspect as a negative heat, as this
    ///   is the portion of total absorbed solar power that became electricity instead of heat.
    double totalPower = 0.0;
    for (unsigned int string=0; string<mNumStrings; ++string) {
        mStrings[string].update();
        totalPower -= mStrings[string].getTerminal().mPower;
    }
    mTerminalPower = totalPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  This drives the environment interface to the contained strings, including lighting and
///           temperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvSection::updateEnvironment(const double dt)
{
    /// - Fraction of ambient power absorbed by the section due to facing away from the light source.
    double trigAngle = 0.0;
    if (mConfig->mSourceAngleEdgeOn) {
        trigAngle = sin(mInput.mSourceAngle);
    } else {
        trigAngle = cos(mInput.mSourceAngle);
    }
    double facing = pow(fabs(trigAngle), mConfig->mSourceAngleExponent);
    if (trigAngle < 0.0) {
        facing *= MsMath::limitRange(0.0, (1.0 - mConfig->mBacksideReduction), 1.0);
    }

    /// - Update environment input data to the strings.
    mStringsInput.mPhotoFlux             = mInput.mSourceFluxMagnitude
                                         * mInput.mSourceExposedFraction * facing;
    mStringsInput.mSourceExposedFraction = mInput.mSourceExposedFraction;
    mStringsInput.mTemperature           = mInput.mTemperature;
    mStringsInput.applyOverrides(dt);

    /// - Update the percent insolation indicator.
    if (mConfig->mRefSourceFluxMagnitude > DBL_EPSILON) {
        mPercentInsolation = UnitConversion::PERCENTAGE * mStringsInput.mPhotoFlux
                           / mConfig->mRefSourceFluxMagnitude;
    }
}
