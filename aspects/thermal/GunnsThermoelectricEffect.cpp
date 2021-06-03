/**
@file
@brief    GUNNS thermoelectric effect implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o))
*/

#include "GunnsThermoelectricEffect.hh"
#include "aspects/thermal/PtcsMacros.hh"
#include "math/MsMath.hh"
#include <cfloat>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] numThermocouples            (--)      Number of thermocouples in series.
/// @param[in] geometryFactor              (m)       Cross-sectional area over length of each thermocouple.
/// @param[in] electricalResistivityCoeff0 (ohm*m)   0th-order coefficient of electrical resistivity of material line function of temperature.
/// @param[in] electricalResistivityCoeff1 (ohm*m/K) 1st-order coefficient of electrical resistivity of material line function of temperature.
/// @param[in] seebeckCoeff0               (V/K)     0th-order coefficient of Seebeck coefficient of material polynomial of temperature.
/// @param[in] seebeckCoeff1               (V/K2)    1st-order coefficient of Seebeck coefficient of material polynomial of temperature.
/// @param[in] seebeckCoeff2               (V/K3)    2nd-order coefficient of Seebeck coefficient of material polynomial of temperature.
/// @param[in] thermalConductivityCoeff0   (W/m/K)   0th-order coefficient of thermal conductivity of material polynomial of temperature.
/// @param[in] thermalConductivityCoeff1   (W/m/K2)  1st-order coefficient of thermal conductivity of material polynomial of temperature.
/// @param[in] thermalConductivityCoeff2   (W/m/K3)  2nd-order coefficient of thermal conductivity of material polynomial of temperature.
/// @param[in] endPlateThermalConductance  (W/K)     Thermal conductance of one end plate.
/// @param[in] minTemperature              (K)       Minimum temperature for evaluating material properties functions.
/// @param[in] maxTemperature              (K)       Maximum temperature for evaluating material properties functions.
///
/// @details  Default constructs this Thermoelectric Effect Config Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermoelectricEffectConfigData::GunnsThermoelectricEffectConfigData(
        const double numThermocouples,
        const double geometryFactor,
        const double electricalResistivityCoeff0,
        const double electricalResistivityCoeff1,
        const double seebeckCoeff0,
        const double seebeckCoeff1,
        const double seebeckCoeff2,
        const double thermalConductivityCoeff0,
        const double thermalConductivityCoeff1,
        const double thermalConductivityCoeff2,
        const double endPlateThermalConductance,
        const double minTemperature,
        const double maxTemperature)
    :
    mNumThermocouples(numThermocouples),
    mGeometryFactor(geometryFactor),
    mElectricalResistivityCoeff(),
    mSeebeckCoeff(),
    mThermalConductivityCoeff(),
    mEndPlateThermalConductance(endPlateThermalConductance),
    mMinTemperature(minTemperature),
    mMaxTemperature(maxTemperature)
{
    mElectricalResistivityCoeff[0] = electricalResistivityCoeff0;
    mElectricalResistivityCoeff[1] = electricalResistivityCoeff1;
    mSeebeckCoeff[0]               = seebeckCoeff0;
    mSeebeckCoeff[1]               = seebeckCoeff1;
    mSeebeckCoeff[2]               = seebeckCoeff2;
    mThermalConductivityCoeff[0]   = thermalConductivityCoeff0;
    mThermalConductivityCoeff[1]   = thermalConductivityCoeff1;
    mThermalConductivityCoeff[2]   = thermalConductivityCoeff2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs this Thermoelectric Effect Config Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermoelectricEffectConfigData::~GunnsThermoelectricEffectConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfThermoelectricEffectsFlag  (--)  Initial thermoelectric effects scalar malfunction flag.
/// @param[in] malfThermoelectricEffectsValue (--)  Initial thermoelectric effects scalar malfunction value.
/// @param[in] current                        (amp) Initial electrical current thru the device.
/// @param[in] temperatureHot                 (K)   Initial "hot" side temperature.
/// @param[in] temperatureCold                (K)   Initial "cold" side temperature.
///
/// @details  Default constructs this Thermoelectric Effect Input Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermoelectricEffectInputData::GunnsThermoelectricEffectInputData(
        const bool   malfThermoelectricEffectsFlag,
        const double malfThermoelectricEffectsScalar,
        const double current,
        const double temperatureHot,
        const double temperatureCold)
    :
    mMalfThermoelectricEffectsFlag(malfThermoelectricEffectsFlag),
    mMalfThermoelectricEffectsScalar(malfThermoelectricEffectsScalar),
    mCurrent(current),
    mTemperatureHot(temperatureHot),
    mTemperatureCold(temperatureCold)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Thermoelectric Effect Input Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermoelectricEffectInputData::~GunnsThermoelectricEffectInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs the Thermoelectric Effect Object with default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermoelectricEffect::GunnsThermoelectricEffect()
    :
    mMalfThermoelectricEffectsFlag(false),
    mMalfThermoelectricEffectsScalar(0.0),
    mName(),
    mResistanceCoeffs(),
    mSeebeckCoeffs(),
    mThermalConductanceCoeffs(),
    mMinTemperature(0.0),
    mMaxTemperature(0.0),
    mCurrent(0.0),
    mTemperatureHot(0.0),
    mTemperatureCold(0.0),
    mThermalConductance(0.0),
    mHeatFluxHot(0.0),
    mHeatFluxCold(0.0),
    mHeatFluxThru(0.0),
    mElectricalConductance(0.0),
    mVoltage(0.0),
    mInitFlag(false)
{
    mResistanceCoeffs[0]         = 0.0;
    mResistanceCoeffs[1]         = 0.0;
    mSeebeckCoeffs[0]            = 0.0;
    mSeebeckCoeffs[1]            = 0.0;
    mSeebeckCoeffs[2]            = 0.0;
    mThermalConductanceCoeffs[0] = 0.0;
    mThermalConductanceCoeffs[1] = 0.0;
    mThermalConductanceCoeffs[2] = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Thermoelectric Effect Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermoelectricEffect::~GunnsThermoelectricEffect()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   configData   (--)  Model Config Data.
/// @param[in]   inputData    (--)  Model Input Data.
/// @param[in]   name         (--)  Instance name for output messages.
///
/// @throw    TsInitializationException
///
/// @details  This initializes the model and validates the initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermoelectricEffect::initialize(const GunnsThermoelectricEffectConfigData& configData,
                                           const GunnsThermoelectricEffectInputData&  inputData,
                                           const std::string&                         name)
{
    /// - Reset init flag.
    mInitFlag = false;

    /// - Validate and set the instance name.
    TS_PTCS_NAME_ERREX("GunnsThermoelectricEffect");

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize from config data.
    const double twoN = 2.0 * configData.mNumThermocouples;
    mResistanceCoeffs[0]         = twoN * configData.mElectricalResistivityCoeff[0] / configData.mGeometryFactor;
    mResistanceCoeffs[1]         = twoN * configData.mElectricalResistivityCoeff[1] / configData.mGeometryFactor;

    mSeebeckCoeffs[0]            = twoN * configData.mSeebeckCoeff[0];
    mSeebeckCoeffs[1]            = twoN * configData.mSeebeckCoeff[1];
    mSeebeckCoeffs[2]            = twoN * configData.mSeebeckCoeff[2];

    mThermalConductanceCoeffs[0] = twoN * configData.mGeometryFactor * configData.mThermalConductivityCoeff[0];
    mThermalConductanceCoeffs[1] = twoN * configData.mGeometryFactor * configData.mThermalConductivityCoeff[1];
    mThermalConductanceCoeffs[2] = twoN * configData.mGeometryFactor * configData.mThermalConductivityCoeff[2];

    if (configData.mEndPlateThermalConductance > DBL_EPSILON) {
        mThermalConductanceCoeffs[0] = 1.0 / (1.0/mThermalConductanceCoeffs[0]
                                            + 2.0/configData.mEndPlateThermalConductance);
    }

    mMinTemperature = configData.mMinTemperature;
    mMaxTemperature = configData.mMaxTemperature;

    /// - Initialize from input data.
    mMalfThermoelectricEffectsFlag   = inputData.mMalfThermoelectricEffectsFlag;
    mMalfThermoelectricEffectsScalar = inputData.mMalfThermoelectricEffectsScalar;
    mCurrent                         = inputData.mCurrent;
    mTemperatureHot                  = inputData.mTemperatureHot;
    mTemperatureCold                 = inputData.mTemperatureCold;

    /// - Initialize state data.
    update();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--) Configuration data.
/// @param[in]      inputData   (--) Input data.
///
/// @throw    TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Thermoelectric Effect model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermoelectricEffect::validate(const GunnsThermoelectricEffectConfigData& configData,
                                         const GunnsThermoelectricEffectInputData&  inputData) const
{
    /// - Throw an exception if the number of thermocouples is < 0.5.
    TS_PTCS_IF_ERREX(configData.mNumThermocouples < 0.5, TsInitializationException,
            "Invalid Configuration Data", "Number of thermocouples is < 0.5.");

    /// - Throw an exception if the geometry factor is < DBL_EPSILON.
    TS_PTCS_IF_ERREX(configData.mGeometryFactor < DBL_EPSILON, TsInitializationException,
            "Invalid Configuration Data", "Geometry factor <= 0.");

    /// - Throw an exception if end plate thermal conductance is < 0.
    TS_PTCS_IF_ERREX(configData.mEndPlateThermalConductance < 0.0, TsInitializationException,
            "Invalid Configuration Data", "End plate thermal conductance < 0.");

    /// - Throw an exception if the minimum temperature < DBL_EPSILON.
    TS_PTCS_IF_ERREX(configData.mMinTemperature < DBL_EPSILON, TsInitializationException,
            "Invalid Configuration Data", "Minimum temperature <= 0.");

    /// - Throw an exception if the maximum temperature <= minimum temperature.
    TS_PTCS_IF_ERREX(configData.mMaxTemperature <= configData.mMinTemperature, TsInitializationException,
            "Invalid Configuration Data", "Maximum temperature <= minimum temperature.");

    /// - Throw an exception if the "hot" side temperature < DBL_EPSILON.
    TS_PTCS_IF_ERREX(inputData.mTemperatureHot < DBL_EPSILON, TsInitializationException,
            "Invalid Input Data", "Hot-side temperature <= 0.");

    /// - Throw an exception if the "cold" side temperature < DBL_EPSILON.
    TS_PTCS_IF_ERREX(inputData.mTemperatureCold < DBL_EPSILON, TsInitializationException,
            "Invalid Input Data", "Cold-side temperature <= 0.");

    /// - Throw an exception if the effects malfunction scalar < 0.
    TS_PTCS_IF_ERREX(inputData.mMalfThermoelectricEffectsScalar < 0.0, TsInitializationException,
            "Invalid Input Data", "Thermoelectric effects malfunction scalar < 0.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This updates the model after a simulation restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermoelectricEffect::restart()
{
    update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the state of this GUNNS Thermoelectric Effect model.  Given input electrical
///           current and end-plate temperatures, outputs electrical & thermal conductivity, Seebeck
///           effect voltage, and Joule/Peltier/Thompson effects heat sources.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermoelectricEffect::update()
{
    /// - Constants used in evaluating polynomials, limited to the valid temperature range.
    const double hotT  = MsMath::limitRange(mMinTemperature, mTemperatureHot,  mMaxTemperature);
    const double coldT = MsMath::limitRange(mMinTemperature, mTemperatureCold, mMaxTemperature);
    const double dT    = hotT - coldT;
    const double dT2   = hotT * hotT - coldT * coldT;
    const double dT3   = hotT * hotT * hotT  - coldT * coldT * coldT;
    const double avgT  = 0.5 * (coldT + hotT);

    /// - Update thermal conductance and Seebeck constants.
    mThermalConductance = mThermalConductanceCoeffs[0];
    double seebeckAvg   = mSeebeckCoeffs[0];
    if (fabs(dT) > DBL_EPSILON) {
        mThermalConductance += dT2/dT * (mThermalConductanceCoeffs[1] / 2.0)
                             + dT3/dT * (mThermalConductanceCoeffs[2] / 3.0);
        seebeckAvg          += dT2/dT * (mSeebeckCoeffs[1] / 2.0)
                             + dT3/dT * (mSeebeckCoeffs[2] / 3.0);
    }
    double seebeckHot  = mSeebeckCoeffs[0]
                       + mSeebeckCoeffs[1] * hotT
                       + mSeebeckCoeffs[2] * hotT  * hotT;
    double seebeckCold = mSeebeckCoeffs[0]
                       + mSeebeckCoeffs[1] * coldT
                       + mSeebeckCoeffs[2] * coldT * coldT;

    /// - Scale thermoelectric effects by the malfunction.
    if (mMalfThermoelectricEffectsFlag) {
        const double scalar = std::max(0.0, mMalfThermoelectricEffectsScalar);
        seebeckAvg  *= scalar;
        seebeckHot  *= scalar;
        seebeckCold *= scalar;
    }

    /// - Update electrical conductance using the average of hot & cold terminal temperatures.
    mElectricalConductance = 1.0 / std::max((mResistanceCoeffs[0] + mResistanceCoeffs[1] * avgT),
                                            DBL_EPSILON);

    /// - Predict thermal conducted thru flux from hot to cold sides, for reference.  In a thermal
    ///   network, the parent link will generate the same flux.
    mHeatFluxThru = mThermalConductance * (mTemperatureHot - mTemperatureCold);

    /// - Calculate Joule heating due to electrical resistance and current.
    const double heatJoule = mCurrent * mCurrent / mElectricalConductance;

    /// - Calculate the Peltier effect flux at the hot & cold terminals.  The difference in these
    ///   represents the Thompson effect.
    const double heatPeltierHot  = mCurrent * hotT  * seebeckHot;
    const double heatPeltierCold = mCurrent * coldT * seebeckCold;

    /// - Calculate the total heat flux added to the hot & cold terminal nodes, including Joule,
    ///   Peltier & Thompson effects.  Positive Peltier effect removes heat from the hot side and
    ///   adds to the old side.
    mHeatFluxHot  = heatJoule / 2.0 - heatPeltierHot;
    mHeatFluxCold = heatJoule / 2.0 + heatPeltierCold;

    /// - Calculate source voltage from the Seebeck effect.
    mVoltage = seebeckAvg * dT;
}
