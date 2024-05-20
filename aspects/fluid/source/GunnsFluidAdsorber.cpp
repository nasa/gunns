/****************************** TRICK HEADER *******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o))
***************************************************************************************************/

#include "math/MsMath.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "aspects/fluid/fluid/GunnsFluidTraceCompounds.hh"

#include "GunnsFluidAdsorber.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      name                  (--)    Name of object.
/// @param[in,out]  nodes                 (--)    Pointer to nodes.
/// @param[in]      maxConductivity       (m2)    Maximum conductivity.
/// @param[in]      expansionScaleFactor  (--)    Scale factor for isentropic gas cooling.
/// @param[in]      gasType               (--)    Fluid type of constituent gas being sorbed.
/// @param[in]      efficiency            (--)    Efficiency of gas adsorbtion from atmosphere.
/// @param[in]      maxAdsorbtionRate     (kg/s)  Maximum mass rate at which gas can be adsorbed from atmosphere.
/// @param[in]      maxAdsorbedMass       (kg)    Maximum amount of adsorbed mass.
/// @param[in]      desorbtionRate        (kg/s)  Mass rate at which gas can be desorbed to atmosphere.
/// @param[in]      thermalLength         (m)     Tube length for thermal convection.
/// @param[in]      thermalDiameter       (m)     Tube inner diameter for thermal convection.
/// @param[in]      surfaceRoughness      (m)     Tube wall surface roughness for thermal convection.
///
/// @details  Default constructs this GUNNS Fluid Adsorber link model configuration data with
///           arguments and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAdsorberConfigData::GunnsFluidAdsorberConfigData(
                                                 const std::string&               name,
                                                 GunnsNodeList*                   nodes,
                                                 const double                     maxConductivity,
                                                 const double                     expansionScaleFactor,
                                                 const FluidProperties::FluidType gasType,
                                                 const double                     efficiency,
                                                 const double                     maxAdsorbtionRate,
                                                 const double                     maxAdsorbedMass,
                                                 const double                     desorbtionRate,
                                                 const double                     thermalLength,
                                                 const double                     thermalDiameter,
                                                 const double                     surfaceRoughness)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, expansionScaleFactor),
    mGasType(gasType),
    mEfficiency(efficiency),
    mMaxAdsorbtionRate(maxAdsorbtionRate),
    mMaxAdsorbedMass(maxAdsorbedMass),
    mDesorbtionRate(desorbtionRate),
    mThermalLength(thermalLength),
    mThermalDiameter(thermalDiameter),
    mSurfaceRoughness(surfaceRoughness)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Adsorber link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAdsorberConfigData::GunnsFluidAdsorberConfigData(const GunnsFluidAdsorberConfigData& that)
    :
    GunnsFluidConductorConfigData(that),
    mGasType(that.mGasType),
    mEfficiency(that.mEfficiency),
    mMaxAdsorbtionRate(that.mMaxAdsorbtionRate),
    mMaxAdsorbedMass(that.mMaxAdsorbedMass),
    mDesorbtionRate(that.mDesorbtionRate),
    mThermalLength(that.mThermalLength),
    mThermalDiameter(that.mThermalDiameter),
    mSurfaceRoughness(that.mSurfaceRoughness)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Adsorber link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAdsorberConfigData::~GunnsFluidAdsorberConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag     (--)  Blockage malfunction flag.
/// @param[in]  malfBlockageValue    (--)  Blockage malfunction fractional value (0-1).
/// @param[in]  desorbtionCycle      (--)  Desorbtion cycle flag.
/// @param[in]  adsorbedMass         (kg)  Adsorbed mass in the adsorber.
/// @param[in]  wallTemperature      (K)   Tube wall temperature for thermal convection.
/// @param[in]  tcEfficiency         (--)  Adsorption/desorption efficiency for trace compounds.
/// @param[in]  tcMaxAdsorbedMass    (kg)  Maximum amount of adsorbed mass of trace compounds.
/// @param[in]  tcAdsorbedMass       (kg)  Initial amount of adsorbed mass of trace compounds.
/// @param[in]  malfEfficiencyFlag   (--)  Initial efficiency malfunction flag.
/// @param[in]  malfEfficiencyValue  (--)  Initial efficiency malfunction value.
///
/// @details  Default constructs this GUNNS Fluid Adsorber link model input data with arguments
///           and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAdsorberInputData::GunnsFluidAdsorberInputData(
        const bool                         malfBlockageFlag,
        const double                       malfBlockageValue,
        const bool                         desorbtionCycle,
        const double                       adsorbedMass,
        const double                       wallTemperature,
        GunnsFluidTraceCompoundsInputData* tcEfficiency,
        GunnsFluidTraceCompoundsInputData* tcMaxAdsorbedMass,
        GunnsFluidTraceCompoundsInputData* tcAdsorbedMass,
        const bool                         malfEfficiencyFlag,
        const double                       malfEfficiencyValue)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mDesorbtionCycle(desorbtionCycle),
    mAdsorbedMass(adsorbedMass),
    mWallTemperature(wallTemperature),
    mTcEfficiency(tcEfficiency),
    mTcMaxAdsorbedMass(tcMaxAdsorbedMass),
    mTcAdsorbedMass(tcAdsorbedMass),
    mMalfEfficiencyFlag(malfEfficiencyFlag),
    mMalfEfficiencyValue(malfEfficiencyValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Adsorber link model input data.  Note this is a
///           shallow copy of pointers.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAdsorberInputData::GunnsFluidAdsorberInputData(const GunnsFluidAdsorberInputData& that)
    :
    GunnsFluidConductorInputData(that),
    mDesorbtionCycle(that.mDesorbtionCycle),
    mAdsorbedMass(that.mAdsorbedMass),
    mWallTemperature(that.mWallTemperature),
    mTcEfficiency(that.mTcEfficiency),
    mTcMaxAdsorbedMass(that.mTcMaxAdsorbedMass),
    mTcAdsorbedMass(that.mTcAdsorbedMass),
    mMalfEfficiencyFlag(that.mMalfEfficiencyFlag),
    mMalfEfficiencyValue(that.mMalfEfficiencyValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Adsorber link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAdsorberInputData::~GunnsFluidAdsorberInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Adsorber link model with obviously invalid data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAdsorber::GunnsFluidAdsorber()
    :
    GunnsFluidConductor(),
    mMalfEfficiencyFlag(false),
    mMalfEfficiencyValue(0.0),
    mGasType(FluidProperties::NO_FLUID),
    mEfficiency(0.0),
    mMaxAdsorbtionRate(0.0),
    mMaxAdsorbedMass(0.0),
    mDesorbtionRate(0.0),
    mThermalDiameter(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mDesorbtionCycle(false),
    mAdsorbedMass(0.0),
    mWallTemperature(0.0),
    mFluidTemperature(0.0),
    mWallHeatFlux(0.0),
    mMass(0.0),
    mSorbtionFlowRate(0.0),
    mSorbtionFluid(0),
    mGasIndex(0),
    mTcEfficiency(0),
    mTcMaxAdsorbedMass(0),
    mTcAdsorbedMass(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Adsorber link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAdsorber::~GunnsFluidAdsorber()
{
    /// - Delete the trace compounds arrays.
    TS_DELETE_ARRAY(mTcAdsorbedMass);
    TS_DELETE_ARRAY(mTcMaxAdsorbedMass);
    TS_DELETE_ARRAY(mTcEfficiency);

    /// - Delete the sorbtion fluid.
    TS_DELETE_OBJECT(mSorbtionFluid);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      inputData   (--)  Input data.
/// @param[in,out]  links       (--)  Link vector.
/// @param[in]      port0       (--)  Nominal inlet port map index.
/// @param[in]      port1       (--)  Nominal outlet port map index.
///
/// @return      void
///
/// @throws      TsInitializationException
///
/// @details     Initializes this GUNNS Fluid Adsorber link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAdsorber::initialize(const GunnsFluidAdsorberConfigData& configData,
                                    const GunnsFluidAdsorberInputData&  inputData,
                                    std::vector<GunnsBasicLink*>&       links,
                                    const int                           port0,
                                    const int                           port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset the initialization complete flag.
    mInitFlag           = false;

    /// - Create the internal fluid and sorbtion fluids.
    createInternalFluid();
    TS_DELETE_OBJECT(mSorbtionFluid);
    TS_NEW_PRIM_OBJECT_EXT(mSorbtionFluid, PolyFluid,
                           (*mInternalFluid, std::string(mName) + ".mSorbtionFluid"),
                           std::string(mName) + ".mSorbtionFluid");
    mSorbtionFluid->setFlowRate(0.0);
    mSorbtionFlowRate = 0.0;

    /// - Validate the configuration and input data.
    validate(configData, inputData);

    /// - Initialize attributes from the validated configuration data.
    mGasType            = configData.mGasType;
    mEfficiency         = configData.mEfficiency;
    mMaxAdsorbtionRate  = configData.mMaxAdsorbtionRate;
    mMaxAdsorbedMass    = configData.mMaxAdsorbedMass;
    mDesorbtionRate     = configData.mDesorbtionRate;
    mThermalDiameter    = configData.mThermalDiameter;
    mThermalSurfaceArea = UnitConversion::PI_UTIL * configData.mThermalLength * configData.mThermalDiameter;
    if (mThermalSurfaceArea > DBL_EPSILON) {
        mThermalROverD  = configData.mSurfaceRoughness / mThermalDiameter;
    } else {
        mThermalROverD  = 0.0;
    }

    /// - Initialize attributes from the validated input data.
    mDesorbtionCycle     = inputData.mDesorbtionCycle;
    mAdsorbedMass        = inputData.mAdsorbedMass;
    mWallTemperature     = inputData.mWallTemperature;
    mFluidTemperature    = 0.5 * (mInternalFluid->getTemperature() + inputData.mWallTemperature);
    mMalfEfficiencyFlag  = inputData.mMalfEfficiencyFlag;
    mMalfEfficiencyValue = inputData.mMalfEfficiencyValue;

    /// - Initialize the derived attributes.
    mMass     = 0.0;
    mGasIndex = mInternalFluid->find(configData.mGasType);

    /// - Initialize the trace compounds arrays.
    if (GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds()) {
        const int tcNtypes = tc->getConfig()->mNTypes;
        TS_NEW_PRIM_ARRAY_EXT(mTcEfficiency,      tcNtypes, double, mName + ".mTcEfficiency");
        TS_NEW_PRIM_ARRAY_EXT(mTcMaxAdsorbedMass, tcNtypes, double, mName + ".mTcMaxAdsorbedMass");
        TS_NEW_PRIM_ARRAY_EXT(mTcAdsorbedMass,    tcNtypes, double, mName + ".mTcAdsorbedMass");

        for (int i = 0; i < tcNtypes; ++i) {
            if (inputData.mTcEfficiency) {
                mTcEfficiency[i]      = inputData.mTcEfficiency->mState[i];
            } else {
                mTcEfficiency[i]      = 0.0;
            }
            if (inputData.mTcMaxAdsorbedMass) {
                mTcMaxAdsorbedMass[i] = inputData.mTcMaxAdsorbedMass->mState[i];
            } else {
                mTcMaxAdsorbedMass[i] = 0.0;
            }
            if (inputData.mTcAdsorbedMass) {
                mTcAdsorbedMass[i]    = inputData.mTcAdsorbedMass->mState[i];
            } else {
                mTcAdsorbedMass[i]    = 0.0;
            }
        }
    }

    /// - Set the initialization complete flag.
    mInitFlag           = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      inputData   (--)  Input data.
///
/// @return      void
///
/// @throws      TsInitializationException
///
/// @details     Validates the initialization of this GUNNS Fluid Adsorber link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAdsorber::validate(const GunnsFluidAdsorberConfigData& configData,
                                  const GunnsFluidAdsorberInputData&  inputData) const
{
    /// - Throw an exception on gas type not in atmosphere.
    try {
        mInternalFluid->find(configData.mGasType);
    } catch (const TsOutOfBoundsException&) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Adsorbed fluid is not in atmosphere.");
    }

    /// - Throw an exception on adsorption efficiency outside valid range (0.0 to 1.0)).
    if (!MsMath::isInRange(0.0, configData.mEfficiency, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Adsorption efficiency outside valid range (0.0 to 1.0).");
    }

    /// - Throw an exception on maximum adsorbtion rate < 0.0.
    if (configData.mMaxAdsorbtionRate < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Maximum adsorbtion rate < 0.0.");
    }

    /// - Throw an exception on maximum adsorbed mass < 0.0.
    if (configData.mMaxAdsorbedMass < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Maximum adsorbed mass < 0.0.");
    }

    /// - Throw an exception on desorbtion rate < 0.0.
    if (configData.mDesorbtionRate < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Desorbtion rate < 0.0.");
    }

    /// - Throw an exception on mass in adsorber < 0.0.
    if (inputData.mAdsorbedMass < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Mass in adsorber < 0.0.");
    }

    /// - Throw an exception if adsorber wall temperature < 0.0.
    if (inputData.mWallTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Adsorber wall temperature < 0.0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAdsorber::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    mFluidTemperature = 0.0;
    mMass             = 0.0;
    mSorbtionFlowRate = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt         (s)     Time step.
/// @param[in]   flowRate   (kg/s)  Mass flow rate (not used).
///
/// @return      void
///
/// @details     Update the internal fluids for constituent mass removed by adsorbtion or added by desorbtion.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAdsorber::updateFluid(const double dt, const double flowRate __attribute__((unused)))
{
    /// - Zero the sorbtion flow rate and reset the sorbtion fluid.
    mSorbtionFluid->resetState();
    mSorbtionFlowRate = 0.0;
    mMass             = 0.0;

    /// - Skip sorbtion when the time step is negligible.
    if (dt > DBL_EPSILON) {

        /// - Skip sorbtion with the atmosphere when the mass flow rate is negligible.
        updateTemperature(dt);
        const double efficiency = computeEfficiency();
        const double rate       = fabs(mFlowRate);
        if (mDesorbtionCycle) {
            desorb(dt, rate, efficiency);
        } else {
            adsorb(dt, rate, efficiency);
        }

        if (fabs(mSorbtionFlowRate) > m100EpsilonLimit) {
            /// - Update sorbtion fluid mass and temperature.
            mSorbtionFluid->setMass(mGasIndex, fabs(mMass));
            mSorbtionFluid->updateMass();
            mSorbtionFluid->setTemperature(mFluidTemperature);

            /// - Shove sorbtion fluid into outlet node.
            /// - Add sorbtion fluid to outlet node and update source vector for flow between
            ///   downstream node and ground.  The source vector will be used next cycle so there
            ///   will be a small error in pressure, but mass will be conserved and GUNNS washes out
            ///   pressure errors. The alternative is to compute reaction in update state using
            ///   previous cycle flowrate, which could result in mass errors, which GUNNS can't fix.
            if (mFlowRate >= 0.0) {
                mNodes[1]->collectInflux(mSorbtionFlowRate, mSorbtionFluid);
                mSourceVector[0] = 0.0;
                mSourceVector[1] = mSorbtionFlowRate / mSorbtionFluid->getMWeight();
            } else {
                mNodes[0]->collectInflux(mSorbtionFlowRate, mSorbtionFluid);
                mSourceVector[0] = mSorbtionFlowRate / mSorbtionFluid->getMWeight();
                mSourceVector[1] = 0.0;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt          (s)     Time step.
/// @param[in]   rate        (kg/s)  Mass flow rate magnitude.
/// @param[in]   efficiency  (--)    Adsorbtion efficiency (0 to 1).
///
/// @return      void
///
/// @details     Update for adsorbtion.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAdsorber::adsorb(const double dt, const double rate, const double availability_efficiency)
{
    /// - Compute the mass available for adsorbtion this cycle.
    const double availableMass  =  mInternalFluid->getMassFraction(mInternalFluid->getType(mGasIndex)) * rate * dt;
    /// - Compute the mass adsorbed this cycle subject to availability, capacity, availability efficiency and rate limit.
    mMass                       =  std::fmax(std::fmin(availableMass * availability_efficiency, std::fmin(mMaxAdsorbtionRate * dt, mMaxAdsorbedMass - mAdsorbedMass)), 0.0);
    /// - Update the mass in the adsorber.
    mAdsorbedMass              +=  mMass;
    /// - Compute the adsorbtion mass flow rate.
    mSorbtionFlowRate           = -mMass / dt;

    /// - Adsorb trace compounds from the internal fluid.  Trace compounds adsorption efficiency is
    ///   reduced as the accumulated adsorbed mass approaches the maximum the adsorber can hold.
    if (GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds()) {
        mInternalFluid->setMole(mFlux);
        tc->updateMasses();
        for (int i = 0; i < tc->getConfig()->mNTypes; ++i) {
            double adsorption_efficiency = mTcEfficiency[i];
            if (mTcMaxAdsorbedMass[i] > DBL_EPSILON) {
                adsorption_efficiency *= std::fmax(0.0, (mTcMaxAdsorbedMass[i] - mTcAdsorbedMass[i]) / mTcMaxAdsorbedMass[i]);
            }
            if (adsorption_efficiency > DBL_EPSILON) {
                /// - We don't bother limiting the adsorption rate to what would exceed the max
                ///   adsorbed mass because a reasonably tuned adsorber should never actually reach
                ///   the max since efficiency drops as adsorbed mass approaches the max and thus
                ///   the approach should be asymptotic.  The risk of not conserving mass is low.
                const double inletRate      = tc->getMasses()[i];
                const double adsorptionRate = inletRate * adsorption_efficiency;
                mTcAdsorbedMass[i]         += adsorptionRate * dt;
                tc->setMass(i, inletRate - adsorptionRate);
            }
        }
        tc->updateMoleFractions();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt          (s)     Time step.
/// @param[in]   rate        (kg/s)  Mass flow rate magnitude (not used).
/// @param[in]   efficiency  (--)    Desorbtion rate efficiency (-1 to 0).
///
/// @return      void
///
/// @details     Update for desorbtion.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAdsorber::desorb(const double dt, const double rate __attribute__((unused)), const double efficiency)
{
    /// - Compute the mass desorbed this cycle subject to rate, rate efficiency and availability.
    mMass              = -fmax(fmin(mAdsorbedMass, -efficiency * mDesorbtionRate * dt), 0.0);
    /// - Update the mass in the adsorber.
    mAdsorbedMass     +=  mMass;
    /// - Compute the desorbtion mass flow rate.
    mSorbtionFlowRate  = -mMass / dt;

    /// - Desorb trace compounds to the internal fluid.
    //TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt  (s)  Time step (not used).
///
/// @note     Derived classes may override, e.g., to model a heater.
///
/// @details  Updates the sorbtion fluid temperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAdsorber::updateTemperature(const double dt __attribute__((unused)))
{
    /// - Compute the heat transfer from the fluid to the adsorber, and update the internal and
    ///   reaction fluid temperatures.
    mWallHeatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(mInternalFluid,
                                                               mFlowRate,
                                                               mThermalROverD,
                                                               mThermalDiameter,
                                                               mThermalSurfaceArea,
                                                               mWallTemperature);
    mFluidTemperature = mInternalFluid->getTemperature();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (--)  Efficiency of the reaction (-1 to 1).
///
/// @note     Derived classes may override, e.g., to model temperature dependence.
///
/// @details  Computes and returns the efficiency of the sorbtion.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidAdsorber::computeEfficiency()
{
    /// - Default is to return the nominal constant efficiency for the adsorbtion or full efficiency
    ///   for desorbtion.
    double efficiency;
    if (mDesorbtionCycle) {
        efficiency = -1.0;
    } else {
        efficiency = mEfficiency;
    }

    /// - The efficiency malfunction overrides the normal value.
    if (mMalfEfficiencyFlag) {
        efficiency = mMalfEfficiencyValue;
    }
    return efficiency;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (m2)    New Thermal Surface Area.
///
/// @returns  void
///
/// @details  Sets the thermal surface area of this this GUNNS Fluid Adsorber link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAdsorber::setThermalSurfaceArea(const double value)
{
    mThermalSurfaceArea = std::max(0.0, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (K)    New Wall Temperature.
///
/// @returns  void
///
/// @details  Sets the wall temperature of this this GUNNS Fluid Adsorber link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAdsorber::setWallTemperature(const double value)
{
    mWallTemperature = std::max(0.0, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
/// @param[in] value (--) (0-1) Malfunction value to override the efficiency.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAdsorber::setMalfEfficiency(const bool flag, const double value)
{
    mMalfEfficiencyFlag  = flag;
    mMalfEfficiencyValue = value;
}
