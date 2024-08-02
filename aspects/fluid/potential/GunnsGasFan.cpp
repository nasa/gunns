/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Classes for the GUNNS Gas Fan Model.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((software/exceptions/TsNumericalException.o)
    (aspects/fluid/potential/GunnsGasFanCurve.o)
    (aspects/fluid/potential/GunnsGasFan.o)
    (core/GunnsFluidPotential.o))

PROGRAMMERS:
   ((Jason Harvey) (L-3 Communications) (Install) (2012-07))
***************************************************************************************************/

#include <complex>
#include <string>
#include "GunnsGasFan.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsNumericalException.hh"
#include "math/MsMath.hh"

/// @details These coefficients define a generalized shaft power curve for impellers with specific
///          speed between 0.2 and 5 radians, which covers most radial, mixed and axial flow pumps.
const double GunnsGasFan::mSpecificSpeedRadial = 0.2;
const double GunnsGasFan::mSpecificSpeedAxial  = 5.0;
const double GunnsGasFan::mPowerCoeffsRadial[] = {0.42, 0.69, -0.11,  0.0};
const double GunnsGasFan::mPowerCoeffsAxial[]  = {2.0, -5.98,  8.78, -3.8};
const double GunnsGasFan::mRefCoeffsRadial[]   = {1.09, 0.33, -0.59, -0.39, 1.32, -0.76};
const double GunnsGasFan::mRefCoeffsAxial[]    = {1.69, -5.45, 9.62, -4.88, 0.022, -0.013};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name                 (--)             Name of object.
/// @param[in,out] nodes                (--)             Pointer to nodes.
/// @param[in]     maxConductivity      (m2)             Max conductivity.
/// @param[in]     expansionScaleFactor (--)             Scale factor for isentropic gas cooling.
/// @param[in]     referenceDensity     (kg/m3)          Reference fluid density for power curve.
/// @param[in]     referenceSpeed       (revolution/min) Reference impeller speed for power curve.
/// @param[in]     referenceCoeff0      (kPa)            Reference performance curve 0th-order coefficient.
/// @param[in]     referenceCoeff1      (kPa)            Reference performance curve 1th-order coefficient.
/// @param[in]     referenceCoeff2      (kPa)            Reference performance curve 2th-order coefficient.
/// @param[in]     referenceCoeff3      (kPa)            Reference performance curve 3th-order coefficient.
/// @param[in]     referenceCoeff4      (kPa)            Reference performance curve 4th-order coefficient.
/// @param[in]     referenceCoeff5      (kPa)            Reference performance curve 5th-order coefficient.
/// @param[in]     bestEfficiency       (--)             (0-1) Efficiency at best efficiency point at reference.
/// @param[in]     referenceQBep        (m3/s)           Volume flow rate at best efficiency point at reference.
/// @param[in]     referencePBep        (kPa)            Pressure rise at best efficiency point at reference.
/// @param[in]     filterGain           (--)             (0-1) Flow filter gain for system curve estimate.
/// @param[in]     driveRatio           (--)             Gear ratio of motor to impeller speed.
/// @param[in]     thermalLength        (m)              Impeller length for thermal convection.
/// @param[in]     thermalDiameter      (m)              Impeller inner diameter for thermal convection.
/// @param[in]     surfaceRoughness     (m)              Impeller wall surface roughness for convection.
/// @param[in]     checkValveActive     (--)             Flag indicating that the built in check valve function is active.
///
/// @details  Default constructs this GUNNS Gas Fan link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasFanConfigData::GunnsGasFanConfigData(const std::string& name,
                                             GunnsNodeList*     nodes,
                                             const double       maxConductivity,
                                             const double       expansionScaleFactor,
                                             const double       referenceDensity,
                                             const double       referenceSpeed,
                                             const double       referenceCoeff0,
                                             const double       referenceCoeff1,
                                             const double       referenceCoeff2,
                                             const double       referenceCoeff3,
                                             const double       referenceCoeff4,
                                             const double       referenceCoeff5,
                                             const double       bestEfficiency,
                                             const double       referenceQBep,
                                             const double       filterGain,
                                             const double       driveRatio,
                                             const double       thermalLength,
                                             const double       thermalDiameter,
                                             const double       surfaceRoughness,
                                             const bool         checkValveActive)
    :
    GunnsFluidPotentialConfigData(name, nodes, maxConductivity, expansionScaleFactor),
    mReferenceDensity(referenceDensity),
    mReferenceSpeed  (referenceSpeed),
    mReferenceCoeff0 (referenceCoeff0),
    mReferenceCoeff1 (referenceCoeff1),
    mReferenceCoeff2 (referenceCoeff2),
    mReferenceCoeff3 (referenceCoeff3),
    mReferenceCoeff4 (referenceCoeff4),
    mReferenceCoeff5 (referenceCoeff5),
    mBestEfficiency  (bestEfficiency),
    mReferenceQBep   (referenceQBep),
    mFilterGain      (filterGain),
    mDriveRatio      (driveRatio),
    mThermalLength   (thermalLength),
    mThermalDiameter (thermalDiameter),
    mSurfaceRoughness(surfaceRoughness),
    mCheckValveActive(checkValveActive)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this GUNNS Gas Fan link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasFanConfigData::GunnsGasFanConfigData(const GunnsGasFanConfigData& that)
    :
    GunnsFluidPotentialConfigData(that),
    mReferenceDensity(that.mReferenceDensity),
    mReferenceSpeed  (that.mReferenceSpeed),
    mReferenceCoeff0 (that.mReferenceCoeff0),
    mReferenceCoeff1 (that.mReferenceCoeff1),
    mReferenceCoeff2 (that.mReferenceCoeff2),
    mReferenceCoeff3 (that.mReferenceCoeff3),
    mReferenceCoeff4 (that.mReferenceCoeff4),
    mReferenceCoeff5 (that.mReferenceCoeff5),
    mBestEfficiency  (that.mBestEfficiency),
    mReferenceQBep   (that.mReferenceQBep),
    mFilterGain      (that.mFilterGain),
    mDriveRatio      (that.mDriveRatio),
    mThermalLength   (that.mThermalLength),
    mThermalDiameter (that.mThermalDiameter),
    mSurfaceRoughness(that.mSurfaceRoughness),
    mCheckValveActive(that.mCheckValveActive)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Gas Fan link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasFanConfigData::~GunnsGasFanConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--)             Blockage malfunction flag.
/// @param[in] malfBlockageValue (--)             Blockage malfunction fractional value (0-1).
/// @param[in] sourcePressure    (kPa)            Initial pressure rise of the link.
/// @param[in] motorSpeed        (revolution/min) Initial speed of the motor.
/// @param[in] wallTemperature   (K)              Initial impeller wall temperature.
///
/// @details  Default constructs this GUNNS Gas Fan link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasFanInputData::GunnsGasFanInputData(const bool   malfBlockageFlag,
                                           const double malfBlockageValue,
                                           const double sourcePressure,
                                           const double motorSpeed,
                                           const double wallTemperature)
    :
    GunnsFluidPotentialInputData(malfBlockageFlag, malfBlockageValue, sourcePressure),
    mMotorSpeed     (motorSpeed),
    mWallTemperature(wallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this GUNNS Gas Fan link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasFanInputData::GunnsGasFanInputData(const GunnsGasFanInputData& that)
    :
    GunnsFluidPotentialInputData(that),
    mMotorSpeed     (that.mMotorSpeed),
    mWallTemperature(that.mWallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Gas Fan link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasFanInputData::~GunnsGasFanInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Gas Fan link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasFan::GunnsGasFan()
    :
    GunnsFluidPotential(),
    mReferenceDensity(0.0),
    mReferenceSpeed(0.0),
    mReferenceQBep(0.0),
    mFilterGain(0.0),
    mDriveRatio(0.0),
    mThermalDiameter(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mReferenceQ(0.0),
    mReferencePowerBep(0.0),
    mSpecificSpeed(0.0),
    mMotorSpeed(0.0),
    mWallTemperature(0.0),
    mWallHeatFlux(0.0),
    mImpellerTorque(0.0),
    mImpellerSpeed(0.0),
    mImpellerPower(0.0),
    mSystemConstant(0.0),
    mSourceQ(0.0),
    mCheckValveActive(false),
    mCheckValvePosition(0.0),
    mCurve()
{
    for (int i=0; i<6; ++i) {
        mReferenceCoeffs[i] = 0.0;
        mAffinityCoeffs[i]  = 0.0;
    }
    mPowerCoeffs[0] = 0.0;
    mPowerCoeffs[1] = 0.0;
    mPowerCoeffs[2] = 0.0;
    mPowerCoeffs[3] = 0.0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Gas Fan link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasFan::~GunnsGasFan()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData (--) Configuration data.
/// @param[in]     inputData  (--) Input data.
/// @param[in,out] links      (--) Link vector.
/// @param[in]     port0      (--) Nominal inlet port map index.
/// @param[in]     port1      (--) Nominal outlet port map index.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Gas Fan link model with configuration and input data.  Some
///           validation of config data is needed before state data can be derived from it.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasFan::initialize(const GunnsGasFanConfigData&  configData,
                             const GunnsGasFanInputData&   inputData,
                             std::vector<GunnsBasicLink*>& links,
                             const int                     port0,
                             const int                     port1)
{
    /// - First initialize & validate parent.
    GunnsFluidPotential::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag           = false;

    /// - Initialize from configuration data.
    mReferenceSpeed     = configData.mReferenceSpeed;
    mReferenceDensity   = configData.mReferenceDensity;
    mReferenceCoeffs[0] = configData.mReferenceCoeff0;
    mReferenceCoeffs[1] = configData.mReferenceCoeff1;
    mReferenceCoeffs[2] = configData.mReferenceCoeff2;
    mReferenceCoeffs[3] = configData.mReferenceCoeff3;
    mReferenceCoeffs[4] = configData.mReferenceCoeff4;
    mReferenceCoeffs[5] = configData.mReferenceCoeff5;
    mReferenceQBep      = configData.mReferenceQBep;
    mFilterGain         = configData.mFilterGain;
    mDriveRatio         = configData.mDriveRatio;
    mCheckValveActive   = configData.mCheckValveActive;

    mThermalDiameter    = configData.mThermalDiameter;
    mThermalSurfaceArea = configData.mThermalLength * UnitConversion::PI_UTIL * mThermalDiameter;
    if (mThermalSurfaceArea > DBL_EPSILON) {
        mThermalROverD     = configData.mSurfaceRoughness / configData.mThermalDiameter;
    } else {
        mThermalROverD     = 0.0;
    }

    /// - Find reference power curve from reference pressure curve and best efficiency point.
    ///   Specific speed is limited to between 0.2 and 5.0 radians, which covers the majority of
    ///   radial, mixed & axial flow centrifugal pumps & fans.
    if (mReferenceQBep > 0.0 and configData.mBestEfficiency > 0.0) {

        /// - Throw an exception if best efficiency is outside (0-1).
        if (1.0 < configData.mBestEfficiency) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "Best efficiency outside (0-1).");
        }

        /// - If reference coefficients 1-5 are zero, the model  needs to calculate its own
        /// - reference curve.
        bool defineCurve = true;
        for(int i = 1; i < 6; i++){
            if(mReferenceCoeffs[i] != 0.0){
                defineCurve = false;
            }
        }

        /// - Calculate pressure at best efficiency point (or use the user inputed value)
        double pressureBep = 0.0;
        if(defineCurve){
            pressureBep = mReferenceCoeffs[0];
        } else{
            mCurve.setCoeffs(mReferenceCoeffs);
            pressureBep = mCurve.evaluate(mReferenceQBep);
        }

        /// - Calculate Specific Speed
        mSpecificSpeed = mReferenceSpeed / UnitConversion::SEC_PER_MIN_PER_2PI * sqrt(mReferenceQBep)
                       * pow(UnitConversion::KPA_PER_PA * mReferenceDensity / pressureBep, 0.75);
        mSpecificSpeed = MsMath::limitRange(0.2, mSpecificSpeed, 5.0);
        const double frac = (mSpecificSpeed      - mSpecificSpeedRadial)
                          / (mSpecificSpeedAxial - mSpecificSpeedRadial);

        /// - If a Pressure at BEP is defined, use it to calculate Reference performance curve. The
        ///   Config curve coefficients are ignored.
        if(defineCurve){
            for(int i = 0; i < 6; i++){
                mReferenceCoeffs[i] = mRefCoeffsRadial[i] + frac * (mRefCoeffsAxial[i] - mRefCoeffsRadial[i]);
                mReferenceCoeffs[i] *= pressureBep / pow(mReferenceQBep, double(i));
            }
        }

        /// - Calculate power at BEP and power curve
        mReferencePowerBep = UnitConversion::PA_PER_KPA * pressureBep * mReferenceQBep
                           / configData.mBestEfficiency;

        mPowerCoeffs[0] = mPowerCoeffsRadial[0] + frac * (mPowerCoeffsAxial[0] - mPowerCoeffsRadial[0]);
        mPowerCoeffs[1] = mPowerCoeffsRadial[1] + frac * (mPowerCoeffsAxial[1] - mPowerCoeffsRadial[1]);
        mPowerCoeffs[2] = mPowerCoeffsRadial[2] + frac * (mPowerCoeffsAxial[2] - mPowerCoeffsRadial[2]);
        mPowerCoeffs[3] = mPowerCoeffsRadial[3] + frac * (mPowerCoeffsAxial[3] - mPowerCoeffsRadial[3]);
    } else {
        mSpecificSpeed = 0.0;
    }

    /// - Find the root of the reference curve, which represents the maximum volumetric flow rate
    ///   the fan can produce at reference conditions.  This may fail if the curve has an even
    ///   number of positive real roots.  It should ideally have exactly 1 such root.  Note this
    ///   imposes a maximum limit of 1000.0 m3/s on the fan curve root.
    try {
        mReferenceQ     = 0.0;
        mCurve.improveRoot(mReferenceQ, mReferenceCoeffs, 1000.0);
    } catch (TsNumericalException &e) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Can't find a suitable real root in reference curve.");
    }
    /// - Throw an exception if the reference flow rate at best efficiency is greater than the
    ///   reference curve max flow.
    if (mReferenceQ <= mReferenceQBep) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Reference flow at best efficiency point >= reference curve maximum flow.");
    }

    /// - Initialize from input data.
    mMotorSpeed         = inputData.mMotorSpeed;
    mWallTemperature    = inputData.mWallTemperature;

    /// - Initialize the system constant somewhere in the ballpark of fan performance boundaries
    ///   to kick-start the flow on first pass.
    mSystemConstant     = mReferenceQ / sqrt(std::max(DBL_EPSILON, mReferenceCoeffs[0]));

    /// - Initialize remaining state data.
    mWallHeatFlux       = 0.0;
    mImpellerTorque     = 0.0;
    mImpellerSpeed      = 0.0;
    mImpellerPower      = 0.0;
    mAffinityCoeffs[0]  = 0.0;
    mAffinityCoeffs[1]  = 0.0;
    mAffinityCoeffs[2]  = 0.0;
    mAffinityCoeffs[3]  = 0.0;
    mAffinityCoeffs[4]  = 0.0;
    mAffinityCoeffs[5]  = 0.0;
    mSourceQ            = 0.0;
    mCheckValvePosition = 0.0;

    /// - Create the internal fluid.
    createInternalFluid();

    /// - Validates the link initialization.
    //
    validate();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag           = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Gas Fan Model link model initial state.  Some validation of
///           config data has already occurred in the initialize method, so this method just checks
///           the final state of the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasFan::validate() const
{
    /// - Throw an exception if reference density is non-positive.
    if (mReferenceDensity < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Reference density < DBL_EPSILON.");
    }

    /// - Throw an exception if reference speed is non-positive.
    if (mReferenceSpeed < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Reference speed < DBL_EPSILON.");
    }

    /// - Throw an exception if reference curve at Q=0 is non-positive.
    if (mReferenceCoeffs[0] < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Reference curve dead-head pressure < DBL_EPSILON.");
    }

    /// - Throw an exception if drive ratio is non-positive.
    if (mDriveRatio < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Drive ratio < DBL_EPSILON.");
    }

    /// - Throw an exception if initial motor speed is negative,
    if (mMotorSpeed < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Motor speed < 0.");
    }

    /// - Throw an exception if initial wall temperature is negative,
    if (mWallTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Wall temperature < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasFan::restartModel()
{
    /// - Reset the base class.
    GunnsFluidPotential::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    mImpellerSpeed      = 0.0;
    mImpellerPower      = 0.0;
    mAffinityCoeffs[0]  = 0.0;
    mAffinityCoeffs[1]  = 0.0;
    mAffinityCoeffs[2]  = 0.0;
    mAffinityCoeffs[3]  = 0.0;
    mAffinityCoeffs[4]  = 0.0;
    mAffinityCoeffs[5]  = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s)  Time step (not used).
///
/// @returns  void
///
/// @details  Updates this GUNNS Gas Fan link model source pressure. Also calculates check valve
///           position is active, and updates the effective conductivity.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasFan::updateState(const double dt __attribute__((unused)))
{
    /// - Protect for divide by zero on drive ratio; also setting drive ratio to zero disconnects
    ///   the impeller from the motor, allowing manual control of the impeller speed for tuning.
    if (mDriveRatio > DBL_EPSILON) {
        mImpellerSpeed = mMotorSpeed / mDriveRatio;
    } else {
        GUNNS_WARNING("impeller is disconnected from motor.");
    }
    computeSourcePressure();

    /// - Check if check valve active/closed
    double deltaPress = mSourcePressure + mNodes[0]->getOutflow()->getPressure() - mNodes[1]->getOutflow()->getPressure();
    if(deltaPress > 0.0 or !mCheckValveActive){
        mCheckValvePosition = 1.0;
    } else{
        mCheckValvePosition = 0.0;
    }
    mEffectiveConductivity = mMaxConductivity * mCheckValvePosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt       (s)    Time step.
/// @param[in] flowRate (kg/s) Mass flow rate.
///
/// @return   void
///
/// @details  Updates this GUNNS Gas Fan link model internal fluid thermal state and fluid outputs
///           to the motor.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasFan::updateFluid(const double dt __attribute__((unused)), const double flowRate)
{
    /// - Impeller power.  Power is kept as a class attribute for reference & display.  We have two
    ///   different ways to calculate power depending on the user's need.  This first method
    ///   represents total shaft power load including useful power imparted to the flow downstream
    ///   plus wasted power due to aerodynamic inefficiencies, bearing & seal friction, etc.  This
    ///   equation is an empirical observation of typical pump/fan performance as a function of
    ///   best efficiency, flow rate and pump specific speed.
    if (mSpecificSpeed > 0.0 and mImpellerSpeed > static_cast<double>(FLT_EPSILON)) {
        const double speedRatio    = mImpellerSpeed / mReferenceSpeed;
        const double affinityQ     = mReferenceQBep * speedRatio;
        const double densityFactor = mNodes[0]->getOutflow()->getDensity() / mReferenceDensity;
        const double affinityP     = speedRatio * speedRatio * speedRatio
                                   * mReferencePowerBep * densityFactor;
        const double QQbep         = MsMath::limitRange(0.0, mVolFlowRate, mReferenceQ * speedRatio)
                                   / std::max(affinityQ, DBL_EPSILON);

        mImpellerPower             = affinityP * (mPowerCoeffs[0]
                                                + mPowerCoeffs[1] * QQbep
                                                + mPowerCoeffs[2] * QQbep * QQbep
                                                + mPowerCoeffs[3] * QQbep * QQbep * QQbep);

    /// - This version of power is only the useful power imparted to the flow downstream, and does
    ///   not include power wasted to aero inefficiencies or friction.
    } else {
        mImpellerPower = UnitConversion::PA_PER_KPA * fabs(mVolFlowRate) * mSourcePressure;
    }

    /// - Shaft torque opposes motor spin so has opposite sign.  Motor speed units are converted to
    ///   r/s to relate to torque in N*m and power in Watts.  Torque on the shaft is zero if the
    ///   drive ratio is zero, i.e. impeller is disconnected from the motor.
    if (mMotorSpeed > static_cast<double>(FLT_EPSILON) and mDriveRatio > DBL_EPSILON) {
        mImpellerTorque = -mImpellerPower * UnitConversion::SEC_PER_MIN_PER_2PI / mMotorSpeed;
    } else {
        mImpellerTorque = 0.0;
    }

    /// - Perform heat convection between the internal fluid and pipe wall.
    mWallHeatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(mInternalFluid,
                                                               flowRate,
                                                               mThermalROverD,
                                                               mThermalDiameter,
                                                               mThermalSurfaceArea,
                                                               mWallTemperature);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @details  Calculates the pressure produced by the impeller as a function of impeller speed,
///           inlet density & flow rate.  Implements the fan performance (P-Q) curve as a 5th-order
///           polynomial.  Affects of inlet density & fan speed follow the fan Affinity Laws.
///
///           There is naturally an unstable feedback loop between fan pressure and flow rate, which
///           causes these parameters to oscillate wildly if undamped.  To prevent this, we estimate
///           the system's pressure curve and solve for the flow rate (Q) at which the system & fan
///           curves intersect, and fan pressure approaches that point.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasFan::computeSourcePressure()
{
    /// - Impeller inlet fluid density uses the source node based on last-pass flow direction.
    const int    sourcePort    = determineSourcePort(mFlux, 0, 1);
    const double sourceDensity = mNodes[sourcePort]->getOutflow()->getDensity();

    /// - The impeller generates no pressure if it is stopped or there is no fluid.
    if (mImpellerSpeed > static_cast<double>(FLT_EPSILON) and sourceDensity > static_cast<double>(FLT_EPSILON)) {

        /// - Scale fan curve coefficients based on speed and density.  This implements the Affinity
        ///   Laws in the polynomial as:
        ///            Coeff_order = Coeff_ref_order * (rho/rho_ref) * (N/N_ref)^(2-order).
        const double densityFactor = sourceDensity  / mReferenceDensity;
        const double speedFactor   = mImpellerSpeed / mReferenceSpeed;
        for (int order=0; order<6; ++order) {
            mAffinityCoeffs[order] = mReferenceCoeffs[order] * densityFactor
                                                             * pow(speedFactor, 2.0 - order);
        }

        /// - Estimate system conductivity based on last-pass flow rate & pressure.  We assume the
        ///   system that the fan is flowing through follows the GUNNS fluid pressure-flow
        ///   relationship:  Q = Gsys * sqrt(dp).  Thus, the network's minimum linearization
        ///   potential should be configured to be no more than about 5% of maximum fan dead-head
        ///   delta-pressure, for best results.
        ///
        /// - Min/max limits are set to avoid locking up the pressure, and the result is
        ///   filtered for further stability as needed.
        const double gSys = std::max(mReferenceQ * speedFactor * 0.0001, mVolFlowRate)
                          / sqrt(MsMath::limitRange(DBL_EPSILON, mSourcePressure, mAffinityCoeffs[0]));

        mSystemConstant = mFilterGain * gSys + (1.0 - mFilterGain) * mSystemConstant;

        /// - Generate the coefficients for the (fan - system) polynomial, the root of which is the
        ///   predicted flow rate.
        double coeffs[6];
        for (int order=0; order<6; ++order) {
            coeffs[order] = mAffinityCoeffs[order];
        }
        if (mSystemConstant > DBL_EPSILON) {
            coeffs[2] -= 1.0 / mSystemConstant / mSystemConstant;
        }

        /// - Use a root-finding algorithm to solve for the predicted source flow.  The maximum flow
        ///   the fan can create is scaled by impeller speed by the Affinity Law, and is used as the
        ///   upper bound for the root-finders.
        try {
            mCurve.improveRoot(mSourceQ, coeffs, mReferenceQ * speedFactor);
        } catch (TsNumericalException &e) {
            GUNNS_WARNING(" failed to find the impeller-system intersection.");
        }

        /// - Finally, evaluate the fan curve for produced delta-pressure.
        mCurve.setCoeffs(mAffinityCoeffs);
        mSourcePressure = std::max(0.0, mCurve.evaluate(mSourceQ));

    } else {
        mSourcePressure = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (m2)    New Thermal Surface Area.
///
/// @returns  void
///
/// @details  Sets the thermal surface area of this this GUNNS Gas Fan link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasFan::setThermalSurfaceArea(const double value)
{
    mThermalSurfaceArea = std::max(0.0, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (K)    New Wall Temperature.
///
/// @returns  void
///
/// @details  Sets the wall temperature of this this GUNNS Gas Fan link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasFan::setWallTemperature(const double value)
{
    mWallTemperature = std::max(0.0, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (revolution/min)    New Motor Speed Temperature.
///
/// @returns  void
///
/// @details  Sets the motor speed of this this GUNNS Gas Fan link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasFan::setMotorSpeed(const double value)
{
    mMotorSpeed = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (--)    Check valve on/off flag
///
/// @returns  void
///
/// @details  Turns on or off the check valve functionality
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasFan::setCheckValveFlag(const bool value)
{
    mCheckValveActive = value;
}
