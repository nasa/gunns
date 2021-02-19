/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
    (GunnsFluidAccum.o)
 )

 PROGRAMMERS:
 ((Tim Caldwell) (GHG) (Initial) (2012-03))

***************************************************************************************************/
#include "GunnsFluidAccumGas.hh"
#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"

    const int GunnsFluidAccumGas::GAS_PORT = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name                             (--)     Link name
/// @param[in]  nodeList                         (--)     Network nodes array
/// @param[in]  maxConductivity                  (m2)     Maximum possible eff cond of the link
/// @param[in]  minConductivityScale             (--)     Min scale to crack open liquid side at bellows limit
/// @param[in]  accumVolume                      (m3)     Documented accumulator volume
/// @param[in]  minChamberVolPercent             (--)     Percent accum volume added for min chamber vol
/// @param[in]  minChamberVolDeadBandPercent     (--)     Percent accum volume for dead band
/// @param[in]  forceBellowsMaxRate              (--)     Fraction/sec. Max rate to force bellows
/// @param[in]  editHoldTime                     (s)      Hold time after edit for system stability
/// @param[in]  minTemperature                   (K)      Min temperature - all applicable chambers
/// @param[in]  maxTemperature                   (K)      Max temperature - all applicable chambers
/// @param[in]  maxPressure                      (kPa)    Max Pressure computed by accumulator.
/// @param[in]  springCoeff0;                    (kPa)    Spring pressure at bellows zero position, spring coefficient 0
/// @param[in]  springCoeff1;                    (--)     Spring coefficient 1, or spring constant, k (kPa/bellows position), in a linear spring system
/// @param[in]  springCoeff2;                    (--)     Spring coefficient 2 (kPa/bellows position squared)
/// @param[in]  gasMaxConductivity               (--)     Maximum possible effective conductivity of the gas link
/// @param[in]  gasMinConductivity               (--)     Minimum possible effective conductivity of the gas link
/// @param[in]  maxForcePressureRate             (kPa/s)  Max rate for overriding pressure.
/// @param[in]  fillModePressureThreshold;       (kPa)    Threshold for computing Fill Mode. Non-zero activates "one way" mEffConductivityScale ramping
/// @param[in]  effCondScaleRate;                (--)     Rate for "one way" mEffConductivityScale ramping (0-1)/sec
///
/// @details Default constructs this gas accumulator model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccumGasConfigData::GunnsFluidAccumGasConfigData(const std::string& name,
                                  GunnsNodeList* nodeList,
                                  const double maxConductivity,               // suggest 1.0
                                  const double minConductivityScale,          // suggest 0.1
                                  const double accumVolume,                   // documented vol
                                  const double minChamberVolPercent,          // suggest 5 - 10%
                                  const double minChamberVolDeadBandPercent,  // suggest 10 - 15%
                                  const double forceBellowsMaxRate,           // suggest 0.016
                                  const double editHoldTime,                  // suggest 5.0
                                  const double minTemperature,                // suggest > liq freezing point
                                  const double maxTemperature,                // suggest < liq boiling point
                                  const double maxPressure,                   // suggest a little > max operating pressure
                                  const double springCoeff0,                  // 0.0 and
                                  const double springCoeff1,                  // 0.0 and
                                  const double springCoeff2,                  // 0.0 to turn off spring.
                                  const double gasMaxConductivity,            // suggest 1.0
                                  const double gasMinConductivity,            // suggest 1.0E-09
                                  const double maxForcePressureRate,          // suggest 30.0 or less
                                  const double fillModePressureThreshold,     // 0.0 use original mEffConductivityScale ramp. > 0.0 activates "one way" ramp.
                                  const double effCondScaleOneWayRate)        // 0.0 Rate for "one way" ramp. (0-1)/sec
        :
        GunnsFluidAccumConfigData(name,
                                  nodeList,
                                  maxConductivity,
                                  minConductivityScale,
                                  accumVolume,
                                  minChamberVolPercent,
                                  minChamberVolDeadBandPercent,
                                  forceBellowsMaxRate,
                                  editHoldTime,
                                  minTemperature,
                                  maxTemperature,
                                  maxPressure,
                                  springCoeff0,
                                  springCoeff1,
                                  springCoeff2,
                                  fillModePressureThreshold,
                                  effCondScaleOneWayRate),
        mGasMaxConductivity(gasMaxConductivity),
        mGasMinConductivity(gasMinConductivity),
        mMaxForcePressureRate(maxForcePressureRate)
{
    // nothing to do.
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details  Copy constructs this gas accumulator model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccumGasConfigData::GunnsFluidAccumGasConfigData(const GunnsFluidAccumGasConfigData& that)
    :
    GunnsFluidAccumConfigData(that),
    mGasMaxConductivity(that.mGasMaxConductivity),
    mGasMinConductivity(that.mGasMinConductivity),
    mMaxForcePressureRate(that.mMaxForcePressureRate)
{
    // Nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this gas accumulator model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccumGasConfigData::~GunnsFluidAccumGasConfigData()
{
    // Nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag          (--)     Blockage malfunction flag
/// @param[in]  malfBlockageValue         (--)     Blockage malfunction fractional value (0-1)
/// @param[in]  initialBellowsPosition    (--)     Fraction of liquid chamber that is filled (0-1)
/// @param[in]  liquidFluidInputData      (--)     Input data for liquid fluid
/// @param[in]  gasFluidInputData         (--)     Input data for gas fluid
///
/// @details Default constructs this gas accumulator model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccumGasInputData::GunnsFluidAccumGasInputData(const bool   malfBlockageFlag,
                                                   const double malfBlockageValue,
                                                   const double initialBellowsPosition,
                                                   PolyFluidInputData* liquidFluidInputData,
                                                   PolyFluidInputData* gasFluidInputData)
    :
    GunnsFluidAccumInputData(malfBlockageFlag, malfBlockageValue, initialBellowsPosition, liquidFluidInputData),
    mGasFluidInputData(gasFluidInputData)
{
    // Nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details  Copy constructs this gas accumulator model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccumGasInputData::GunnsFluidAccumGasInputData(const GunnsFluidAccumGasInputData& that)
    :
    GunnsFluidAccumInputData(that),
    mGasFluidInputData(that.mGasFluidInputData)
{
    // Nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this gas accumulator model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccumGasInputData::~GunnsFluidAccumGasInputData()
{
    // Nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling the step
///           and computeFlows methods.
///
/// @details  Default constructs this gas accumulator model with purposely invalid data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccumGas::GunnsFluidAccumGas()
    :
    GunnsFluidAccum(),
    mMalfBellowsRuptureFlag(0.0),
    mMalfBellowsRuptureBellowsPos(0.0),
    mMalfBellowsRuptureBellowsRate(0.0),
    mMalfBellowsRupturePress(0.0),
    mMalfBellowsRupturePressRate(0.0),
    mGasMaxConductivity(0.0),
    mGasMinConductivity(0.0),
    mGasEffectiveConductivity(0.0),
    mGasCapacitance(0.0),
    mGasPressureReading(0.0),
    mGasFlowRate(0.0),
    mGasFlux(0.0),
    mGasHousingQ(0.0),
    mEditPressureFlag(false),
    mEditPressureValue(0.0),
    mEditPressureRate(0.0),
    mEditPressureTimer(0.0),
    mGasInternalFluid(0),
    mMaxForcePressureRate(0.0),
    mPressureMalfSet(false),
    mEditHoldPressure(0.0)
{
    // Nothing to do
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this gas accumulator model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccumGas::~GunnsFluidAccumGas()
{
    TS_DELETE_OBJECT(mGasInternalFluid);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     void
///
/// @details    Updates the admittance matrix for this Gunns object.  For the node at port 0
///             (gas port), mAdmittanceMatrix[0] represents the path to ground. The  accumulator
///             model acts as a GunnsFluidPotential, controlling the flow through this path. Refer
///             to buildConductance inside the GunnsFluidAccum class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccumGas::buildGasConductance()
{
    /// - Compute system conductance using effective conductivity.
    double gasSystemConductance = MsMath::limitRange(0.0, mGasEffectiveConductivity, mConductanceLimit);
    if (fabs(mAdmittanceMatrix[0] - gasSystemConductance) > 0.0) {
        /// - Set mAdmittanceMatrix[0] to flow between port 0 node and accumulator gas chamber.
        mAdmittanceMatrix[0]   =  gasSystemConductance;
        mAdmittanceUpdate = true;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Time step.
///
/// @details    Compute the molar flow rate to/from the nodes as a result of the network solution,
///             schedules outflows from the nodes, and sets port direction flags for the flow
///             orchestrator.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccumGas::computeFlows(const double dt)
{
    GunnsFluidAccum::computeFlows(dt);
    mGasFlux = computeFlux(mAdmittanceMatrix[0], mNodes[GAS_PORT], mGasInternalFluid);
    if (mGasFlux > DBL_EPSILON) {
        mNodes[GAS_PORT]->scheduleOutflux(mGasFlux);
        mPortDirections[GAS_PORT] = SOURCE;
    } else if (mGasFlux < -DBL_EPSILON) {
        mPortDirections[GAS_PORT] = SINK;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return         Void
///
/// @details        Forces gas chamber temperature, based on base class temperature edit terms.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccumGas::editPressurizerTemperature()
{
    editTemperature(mPressurizerVolume, mGasInternalFluid);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt                    (s)     Delta time.
/// @param[in,out]  forcedPressure        (kPa/s) Desired pressure.
/// @param[in,out]  forcedPressureRate    (kPa/s) Desired pressure change rate.
/// @param[in]      updateTemperature     (--)    Update temperature flag.
///
/// @return         (--)  Force gas chamber complete bool.
///
/// @details        Forces pressure based on either edit or malfunction.
///                 Desired value and rate are limited within "rampValue".
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidAccumGas::forceGasChamber(const double dt,
                                          double &forcedPressure,
                                          double &forcedPressureRate,
                                          bool   updateTemperature)
{
    // Use liquid chamber pressure, which combines gas and spring pressures.
    double resultPressure = mInternalFluid->getPressure();
    /// -  Ramp pressure to desired value.
    bool forceGasChamberComplete = rampValue(dt,
                                              resultPressure,
                                              forcedPressure,
                                              forcedPressureRate,
                                              DBL_EPSILON,
                                              mMaxPressure,
                                              0.0,
                                              mMaxForcePressureRate);
    /// - Set new pressure within gas fluid, subtracting out the spring pressure.
    ///   The resultant pressure of the liquid chamber will be the desired value.
    mGasInternalFluid->setPressure(resultPressure - mSpringPressure);
    /// - Update temperature if allowed. Otherwise, set to existing value.
    if (updateTemperature) {
        /// - Back calculate mass to match new pressure with old volume. Do this prior to updating temperature normally
        mGasInternalFluid->setMass(computeMass(mPressurizerVolume, mGasInternalFluid->getDensity()));
        mGasInternalFluid->setTemperature(computeTemperature(dt,
                                                             mGasFlowRate,
                                                             mNodes[GAS_PORT]->getOutflow()->getSpecificEnthalpy(),
                                                             mGasInternalFluid,
                                                             mGasHousingQ));

    } else {
        /// - Force temperature to remain as is.
        mGasInternalFluid->setTemperature(mGasInternalFluid->getTemperature());
        /// - Back calculate mass to match new pressure with old volume as well as forced temperature.
        /// - Do this after forcing temperature.
        mGasInternalFluid->setMass(computeMass(mPressurizerVolume, mGasInternalFluid->getDensity()));
    }

    return (forceGasChamberComplete);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return         double
///
/// @details        Return gas chamber pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidAccumGas::getPressurizerPressure()
{
    return(mGasInternalFluid->getPressure());
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Config Data
/// @param[in]     inputData    (--) Input Data
/// @param[in,out] networkLinks (--) Gunns network links
/// @param[in]     port0        (--) Gunns network port 0
/// @param[in]     port1        (--) Gunns network port 1
///
/// @return     void
///
/// @throws     TsInitializationException
///
/// @details    Initializes this gas accumulator model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccumGas::initialize(const GunnsFluidAccumGasConfigData &configData,
                             const GunnsFluidAccumGasInputData &inputData,
                             std::vector<GunnsBasicLink*>& networkLinks,
                             const int port0,      // Gas Node
                             const int port1)      // Liquid Node
{
    /// - Initialize the parent class.
    GunnsFluidAccum::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset initialization flag.
    mInitFlag = false;

    /// - Validate config and input data.
    validate(configData, inputData);

    /// - Initialize Gunns related attributes.
    mGasMaxConductivity = configData.mGasMaxConductivity;
    mGasMinConductivity = configData.mGasMinConductivity;
    mGasEffectiveConductivity = mGasMaxConductivity;
    mGasCapacitance = 0.0;

    /// - Initialize accumulator chamber attributes.
    mGasPressureReading = inputData.mGasFluidInputData->mPressure;
    mGasFlowRate = 0.0;
    mGasFlux     = 0.0;
    mGasHousingQ = 0.0;

    /// - Initialize Edit attributes
    mEditPressureFlag = false;
    mEditPressureValue = 0.0;
    mEditPressureRate = configData.mMaxForcePressureRate;
    mEditPressureTimer = 0.0;
    mPressureMalfSet = false;

    /// - Initialize fluid attributes.
    TS_DELETE_OBJECT(mGasInternalFluid);
    TS_NEW_PRIM_OBJECT_EXT(mGasInternalFluid, PolyFluid,
                           (*(mNodes[GAS_PORT]->getContent()), std::string(mName) + ".mGasInternalFluid"),
                           std::string(mName) + ".mGasInternalFluid");
    mGasInternalFluid->setMassAndMassFractions(FLT_EPSILON,
                                            inputData.mGasFluidInputData->mMassFraction);
    mGasInternalFluid->setPressure(inputData.mGasFluidInputData->mPressure);
    mGasInternalFluid->setTemperature(inputData.mGasFluidInputData->mTemperature);
    mGasInternalFluid->setMass(computeMass(mPressurizerVolume, mGasInternalFluid->getDensity()));
    mGasInternalFluid->setFlowRate(0.0);

    /// - Initialize malfunction attributes.
    mMalfBellowsRuptureFlag = false;
    mMalfBellowsRuptureBellowsPos = 0.0;
    mMalfBellowsRuptureBellowsRate = configData.mForceBellowsMaxRate;
    mMalfBellowsRupturePress = 0.0;
    mMalfBellowsRupturePressRate = configData.mMaxForcePressureRate;

    /// - Initialize other maximums.
    mMaxForcePressureRate = configData.mMaxForcePressureRate;

    mPressureMalfSet = false;
    mEditHoldPressure = inputData.mGasFluidInputData->mPressure;

    /// - Set Initialization flag.
    mInitFlag = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt (s)  Delta time.
///
/// @return     Bool
///
/// @details    Override bellows position of the base class. Return boolean to indicate whether
///             override is taking place.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccumGas::pressurizerOrideBellows(const double dt)
{
        if (mMalfBellowsRuptureFlag) {
            /// - For bellows rupture, move bellows to rupture position.
            ///   Update temperature normally
            resetEditFlagsAndTimers();
            forceLiqChamber(dt, mMalfBellowsRuptureBellowsPos, mMalfBellowsRuptureBellowsRate, true);
        } else {
            /// - For pressure edit, hold bellows in position.
            ///   Force temperature to be static (edits not reactive or simultaneous edit).
            double rate = 0.0;
            forceLiqChamber(dt, mBellowsPosition, rate, false);
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return         Void
///
/// @details        Sets all edit flags to false and all timers to 0.0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccumGas::resetPressurizerEditFlagsAndTimers()
{
    mEditPressureFlag = false;
    mSimultaneousEditsInProgress = false;
    mEditPressureTimer = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccumGas::restartModel()
{
    /// - Reset the base class.
    GunnsFluidAccum::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag      (--)    Malfunction activation flag, true activates
/// @param[in] pos       (--)    Fraction, Forced bellows position value
/// @param[in] posRate   (--)    Fraction/sec, Rate for bellows movement
/// @param[in] press     (kPa)   Forced pressure for base class bellows rupture malfunction
/// @param[in] pressRate (kPa/s) Forced pressure rate for base class bellows rupture malfunction
///
/// @details  Sets the bellows rupture malfunction parameters to the given values.
///           Calling this method with default arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccumGas::setMalfBellowsRupture(const bool   flag,
                                               const double pos,
                                               const double posRate,
                                               const double press,
                                               const double pressRate)
{
    mMalfBellowsRuptureFlag        = flag;
    mMalfBellowsRuptureBellowsPos  = pos;
    mMalfBellowsRuptureBellowsRate = posRate;
    mMalfBellowsRupturePress       = press;
    mMalfBellowsRupturePressRate   = pressRate;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     Void
///
/// @details    Update capacitance of the liquid and gas chambers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccumGas::updateCapacitance()
{
    /// - Base class computes spring capacitance.
    GunnsFluidAccum::updateCapacitance();

    /// - Compute gas chamber fluid capacitance.
    mGasCapacitance = GunnsFluidUtils::computeCapacitance(mGasInternalFluid,
                                                          mPressurizerVolume);

    const double gasDensity = mGasInternalFluid->getDensity();
    const double liqMWeight = mInternalFluid->getMWeight();

    /// - Compute liquid chamber fluid capacitance, which is a function of gas capacitance.
    if (gasDensity > 0.0 and liqMWeight > 0.0) {
        const double liqCapacitance = mGasCapacitance *
                                      mInternalFluid->getDensity() *
                                      mGasInternalFluid->getMWeight() /
                                      gasDensity /
                                      liqMWeight;

        /// - use the capacitance that is smallest between the spring capacitance and
        ///   computed liquid capacitance. If mKSpring <=  DBL_EPSILON (no spring), use
        ///   computed capacitance.
        if (mSpringCoeff0 > DBL_EPSILON || mSpringCoeff1 > DBL_EPSILON ||mSpringCoeff2 > DBL_EPSILON) {
            mLiqCapacitance = fmin(liqCapacitance, mSpringCapacitance);
        } else {
            mLiqCapacitance = liqCapacitance;
        }
    } else {
        GUNNS_WARNING("density or molecular weight <= 0.0. Liquid capacitance update aborted.");
        mLiqCapacitance = 0.0;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt    (s)  Delta time.
///
/// @return     Void
///
/// @details    Update gas chamber during nominal conditions. Update mass, pressure and
///             temperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccumGas::updateGasChamber(const double dt)
{
    /// - If hold accum flag set, do not update.
    if (mHoldAccumFlag) {
        return;
    }

    /// - Update temperature.
    mGasInternalFluid->setTemperature(computeTemperature(dt,
                                                         mGasFlowRate,
                                                         mNodes[GAS_PORT]->getOutflow()->getSpecificEnthalpy(),
                                                         mGasInternalFluid,
                                                         mGasHousingQ));
    /// - Update gas mass.
    const double oldMass = mGasInternalFluid->getMass();
    const double newMass = computeMass(dt, mGasFlowRate, oldMass);
    if (mGasFlowRate > DBL_EPSILON) {
        GunnsFluidUtils::mixFluidMasses(mGasInternalFluid,
                                        oldMass,
                                        mNodes[GAS_PORT]->getInflow(),
                                        dt * mGasFlowRate,
                                        mNodes[GAS_PORT]->getFluidConfig()->mNTypes);
    } else {
        mGasInternalFluid->setMass(newMass);
    }

    if (mPressurizerVolume > 0.0) {        // May be impossible to fail, but always check for divide by 0.0
        /// - Update gas pressure.
        const double density = newMass / mPressurizerVolume;
        double gasPressure   = mGasInternalFluid->computePressure(mGasInternalFluid->getTemperature(), density);
        gasPressure          = MsMath::limitRange(DBL_EPSILON, gasPressure, mMaxPressure);
        mGasInternalFluid->setPressure(gasPressure);
    } else {
        GUNNS_WARNING("pressurizer volume <= 0.0. Invalid volume config. Pressure update aborted.");
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt               (s)           Delta time.
///
/// @return     Void
///
/// @details    Update effective conductivity of gas side. By dynamically adjusting the
///             conductivity, stability is provided given changing volumes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccumGas::updatePressurizerEffCond(const double dt)
{
    /// - Compute gas side conductivity using gas capacitance.
    ///   Although the liquid side closes entirely when the bellows position hits a hard stop,
    ///   The gas side conductivity never goes to 0.0.
    mGasEffectiveConductivity = computeConductivity(mGasCapacitance,
                                                    mGasMaxConductivity,
                                                    mGasEffectiveConductivity,
                                                    mGasMinConductivity,
                                                    dt);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt           (s) Delta time.
///
/// @return     Void
///
/// @details    Update gas fluid state. Solve mass, temperature, pressure.
///             Note: Temperature edit is called separately from the accumulator base class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccumGas::updatePressurizerFluid(const double dt)
{
    /// - reset override bellows flag for fall-through logic.
    mPressurizerOrideBellowsFlag = false;
    /// - Compute flow rate
    mGasFlowRate = computeFlowRate(mGasFlux, mNodes[GAS_PORT], mGasInternalFluid);

    /// - Process Bellows rupture malfunction if active.
    if (mMalfBellowsRuptureFlag) {
        /// - Override bellows position. The pressurizerOrideBellows method will move bellows to rupture position.
        mPressurizerOrideBellowsFlag = true;
        /// - When malfunction pressure and bellows position have been reached, start updating again.
        if (mPressureMalfSet and mBellowsMalfSet) {
            updateGasChamber(dt);
        } else {
            /// - Ramp up pressure until requested malfunction value is reached.  Also signal the
            ///   base class to call pressurizerOrideBellows to set bellows position to rupture malf
            ///   desired position.  Force pressure based on malf desired value.
            bool malfforceComplete = forceGasChamber(dt, mMalfBellowsRupturePress, mMalfBellowsRupturePressRate, true);
            if (malfforceComplete) {
                mPressureMalfSet = true;
            }
        }
    } else {
        mPressureMalfSet = false;
        mBellowsMalfSet = false;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// - Edits can be done separately or simultaneously.
        ///   If mEditsAreReactive is true, then editing one attribute (bellows for example) will cause pressure
        ///   and temperature to react as if the bellows position is moving due to normal forces.
        ///   If mEditsAreReactive is false, then editing one attribute will not change the other two
        ///   attributes.
        ///
        /// - mEditsAreReactive applies to separate edits only.
        ///
        /// - If a bellows edit and a pressure edit take place at the same time, then upon completion, each
        ///   attribute will remain static until both edits are complete. This behavior occurs regardless
        ///   of the mEditsAreReactive flag because editing two attributes at the same time indicates from
        ///   the user that reactivity is not desired for this particular edit. Temperature will also follow this rule.
        ///
        /// - An edit can complete naturally or be forced off by the user by setting the edit flag to false.
        ///
        ///   The relationship between pressure and temperature is not modeled in the accumulator.
        ///   Forcing the temperature to remain constant serves to keep inflows from affecting the temperature.
        ///
        /// - Note about mEditHoldPressure...
        ///   This variable contains the value for liquid pressure that should be forced in several instances (noted below),
        ///   all of which involve bellows edits. In these situations, the gas pressure cannot be forced to be what
        ///   it was last iteration because the spring pressure will change due to the bellows position changing.
        ///   Last iteration liquid pressure also cannot be used because it will be 1 iteration off based on when the
        ///   gas/liquid pressures are updated v.s. bellows position and spring pressure.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// - Determine if a simultaneous edit is taking place
        ///   Release only when both edits are complete.
        if (mEditPressureFlag && mEditBellowsFlag) {
            mSimultaneousEditsInProgress = true;
        } else if (!mEditPressureFlag && !mEditBellowsFlag) {
            mSimultaneousEditsInProgress = false;
        }

        if (mEditPressureFlag) {
            /// - Record latest liquid pressure, which includes spring pressure.
            ///   If the edit is stopped early and simultaneous edits flag is true, then
            ///   the pressure will be forced to this value until bellows edit is finished.
            mEditHoldPressure = mInternalFluid->getPressure();
            if (!mEditBellowsFlag && (!mEditsAreReactive || mSimultaneousEditsInProgress)) {
                /// - If bellows edit is not active, then...
                ///   Bellows will not move during pressure edit if edits are not reactive.
                ///   Bellows will not move during a simultaneous edit, after the bellows portion is finished,
                ///   until the pressure portion is also finished.
                mPressurizerOrideBellowsFlag = true;
            }
            /// - Determine whether to update temperature normally.
            bool updateTemperatureNormally = (mEditsAreReactive && !mSimultaneousEditsInProgress);
            // - Edit pressure
            bool forceComplete = forceGasChamber(dt, mEditPressureValue, mEditPressureRate, updateTemperatureNormally);
            /// - Reset flag when complete.
            if (forceComplete and processTimer(dt, mEditPressureTimer, mEditHoldTime)) {
                mEditPressureFlag = false;
                /// - Record edit pressure value, which represents liquid pressure and includes spring pressure.
                ///   If the edit stops naturally and simultaneous edits flag is true, then
                ///   the pressure will be forced to this value until bellows edit is finished.
                mEditHoldPressure = mEditPressureValue;
            }

        } else {
            mEditPressureTimer = 0.0;
            if (mSimultaneousEditsInProgress || (mEditBellowsFlag && !mEditsAreReactive)) {
                /// - The pressure will be forced to remain static if...
                ///   - A simultaneous edit is in progress, the pressure edit is finished, and the bellows
                ///     edit is still active or
                ///   - if edits are not reactive and there is a bellows edit taking place.
                forceGasChamber(dt, mEditHoldPressure, mMaxForcePressureRate, false);

            } else {
                /// - Record latest liquid pressure, which includes spring pressure.
                ///   If a bellows edit starts (not simultaneous), and edits are not reactive,
                ///   then the pressure will be forced to this value until the bellows edit is finished.
                mEditHoldPressure = mInternalFluid->getPressure();
                /// - Update mass, pressure, temperature.
                updateGasChamber(dt);
            }
        }
    }
    /// - Update gas pressure reading, which is different than the chamber pressure.
    ///   Using node pressure eliminates an issue where the pressure rises slightly
    ///   when the bellows approachesn the hard stop, because the conductivity is being
    ///   decreased.
    mGasPressureReading = mNodes[GAS_PORT]->getPotential();
    /// - Transport fluid to and from associated Gunns nodes.
    transportAccumFluid(mGasFlowRate, mNodes[GAS_PORT], mGasInternalFluid);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt (s)  Delta time.
///
/// @return     Void
///
/// @details    Update gas chamber state. Solve link admittance, conductance, and potential.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccumGas::updatePressurizerState(const double dt __attribute__((unused)))
{
    /// - Build conductance and potential.
    buildGasConductance();
    buildGasPotential();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Configuration data.
/// @param[in]  inputData   (--)  Input data.
///
/// @return     void
///
/// @throws     TsInitializationException
///
/// @details    Validates gas accumulator configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccumGas::validate(const GunnsFluidAccumGasConfigData& configData,
                                  const GunnsFluidAccumGasInputData& inputData) const
{
    /// - Throw an exception on gas side max conductivity < 0.0.
    ///   Max conductivity is allowed to be lower than minimum for debug.
    if (configData.mGasMaxConductivity < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Gas link has max conductivity < 0.0.");
    }
    /// - Throw an exception on gas side min conductivity < DBL_EPSILON.
    if (configData.mGasMinConductivity < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Gas link has min conductivity < DBL_EPSILON.");
    }
    /// - Throw an exception on Force pressure max rate <= 0.0.
    if (configData.mMaxForcePressureRate <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Force pressure Max Rate must be greater than 0.0.");
    }
    /// - Throw an exception on fluid input data pointer being NULL.
    if (0 == inputData.mGasFluidInputData) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Gas fluid input data is NULL.");
    }
}
