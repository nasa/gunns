/**
@file
@brief    GUNNS Fluid Simple Rocket Model implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsFluidLink.o)
    (properties/CombustFactory.o)
    (properties/Combust.o))
**************************************************************************************************/

#include <cmath>

#include "GunnsFluidSimpleRocket.hh"
#include "math/UnitConversion.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

/// @details  Nozzle exit Mach number is computed by an iterative method and this is the tolerance
///           for the solution.  With this value it usually converges in about 20 iterations.
const double GunnsFluidSimpleRocket::mMachTolerance = 1.0e-4;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                   (--)  Name of object.
/// @param[in] nodes                  (--)  Pointer to nodes.
/// @param[in] throatArea             (m2)  Area of nozzle throat. */
/// @param[in] areaRatio              (--)  Ratio of nozzle exit to throat areas. */
/// @param[in] characteristicVelocity (m/s) Combustion characteristic velocity at design point. */
/// @param[in] combustionTemperature  (K)   Combustion adiabatic flame temperature at design point. */
/// @param[in] combustionGamma        (--)  Combustion ratio of specific heats at design point. */
/// @param[in] thrustTuning           (--)  Thrust tuning scale factor. */
/// @param[in] recombinationTuning    (--)  Tuning scale factor (0-1) for effect of chemical recombination in nozzle on thrust. */
/// @param[in] needsIgnition          (--)  True if ignitor is required to start combustion. */
/// @param[in] combustType            (--)  Optional combustion model type.
///
/// @details  Default constructs this GUNNS Fluid Simple Rocket link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleRocketConfigData::GunnsFluidSimpleRocketConfigData(
        const std::string&                  name,
        GunnsNodeList*                      nodes,
        const double                        throatArea,
        const double                        areaRatio,
        const double                        characteristicVelocity,
        const double                        combustionTemperature,
        const double                        combustionGamma,
        const double                        thrustTuning,
        const double                        recombinationTuning,
        const bool                          needsIgnition,
        const CombustFactory::CombustorType combustType)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mThroatArea(throatArea),
    mAreaRatio(areaRatio),
    mCharacteristicVelocity(characteristicVelocity),
    mCombustionTemperature(combustionTemperature),
    mCombustionGamma(combustionGamma),
    mThrustTuning(thrustTuning),
    mRecombinationTuning(recombinationTuning),
    mNeedsIgnition(needsIgnition),
    mCombustType(combustType)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Simple Rocket link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleRocketConfigData::GunnsFluidSimpleRocketConfigData(
        const GunnsFluidSimpleRocketConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mThroatArea(that.mThroatArea),
    mAreaRatio(that.mAreaRatio),
    mCharacteristicVelocity(that.mCharacteristicVelocity),
    mCombustionTemperature(that.mCombustionTemperature),
    mCombustionGamma(that.mCombustionGamma),
    mThrustTuning(that.mThrustTuning),
    mRecombinationTuning(that.mRecombinationTuning),
    mNeedsIgnition(that.mNeedsIgnition),
    mCombustType(that.mCombustType)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Simple Rocket link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleRocketConfigData::~GunnsFluidSimpleRocketConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag   (--)  Blockage malfunction flag.
/// @param[in]  malfBlockageValue  (--)  Blockage malfunction fractional value (0-1).
/// @param[in]  ignition           (--)  Ignition is occurring.
/// @param[in]  combustion         (--)  Combustion is occurring.
///
/// @details  Default constructs this GUNNS Fluid Simple Rocket link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleRocketInputData::GunnsFluidSimpleRocketInputData(const bool   malfBlockageFlag,
                                                                 const double malfBlockageValue,
                                                                 const bool   ignition,
                                                                 const bool   combustion)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue),
    mIgnition(ignition),
    mCombustion(combustion)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Simple Rocket link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleRocketInputData::GunnsFluidSimpleRocketInputData(
        const GunnsFluidSimpleRocketInputData& that)
    :
    GunnsFluidLinkInputData(that),
    mIgnition(that.mIgnition),
    mCombustion(that.mCombustion)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Simple Rocket link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleRocketInputData::~GunnsFluidSimpleRocketInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Simple Rocket link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleRocket::GunnsFluidSimpleRocket()
    :
    GunnsFluidLink(NPORTS),
    mThroatArea(0.0),
    mAreaRatio(0.0),
    mCombustionTemperature(0.0),
    mCombustionGamma(0.0),
    mCombustionMWeight(0.0),
    mThrustTuning(0.0),
    mRecombinationTuning(0.0),
    mNeedsIgnition(false),
    mCombustModel(0),
    mIgnition(false),
    mCombustion(false),
    mDefaultConductance(0.0),
    mChamberPressure(0.0),
    mChamberTemperature(0.0),
    mChamberGamma(0.0),
    mChamberMWeight(0.0),
    mExitMach(0.0),
    mExitPressure(0.0),
    mExitTemperature(0.0),
    mExitGamma(0.0),
    mExitMWeight(0.0),
    mExitVelocity(0.0),
    mSpecificImpulse(0.0),
    mThrust(0.0),
    mCharacteristicVelocity(0.0),
    mSystemConductance(0.0),
    mCombustionCount(0),
    mCombustFactory()
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Simple Rocket link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleRocket::~GunnsFluidSimpleRocket()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      inputData   (--)  Input data.
/// @param[in,out]  links       (--)  Link vector.
/// @param[in]      port0       (--)  Nominal inlet port map index.
/// @param[in]      port1       (--)  Nominal outlet port map index.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Simple Rocket link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleRocket::initialize(const GunnsFluidSimpleRocketConfigData& configData,
                                        const GunnsFluidSimpleRocketInputData&  inputData,
                                        std::vector<GunnsBasicLink*>&           links,
                                        const int                               port0,
                                        const int                               port1)
{
    /// - Initialize the parent class.
    int ports[2] = {port0, port1};
    GunnsFluidLink::initialize(configData, inputData, links, ports);

    /// - Reset initialization status flag.
    mInitFlag               = false;

    /// - Validate the configuration data.
    validate(configData);

    /// - Initialize with configuration data:
    mThroatArea             = configData.mThroatArea;
    mAreaRatio              = configData.mAreaRatio;
    mCombustionTemperature  = configData.mCombustionTemperature;
    mCombustionGamma        = configData.mCombustionGamma;
    mThrustTuning           = configData.mThrustTuning;
    mRecombinationTuning    = configData.mRecombinationTuning;
    mNeedsIgnition          = configData.mNeedsIgnition;
    mCombustModel           = mCombustFactory.createCombustor(configData.mCombustType) ;

    /// - From Hill & Peterson, Eqn. 11.8:
    // Divide by zero is protected against by validate method above.
    mDefaultConductance     = UnitConversion::PA_PER_KPA * mThroatArea
                            / configData.mCharacteristicVelocity;

    /// - From Hill & Peterson, Eqn. 11.9:
    const double g   = mCombustionGamma;
    const double gp1 = g + 1.0;
    const double gm1 = g - 1.0;
    // Divide by zero is protected against by validate method above.
    mCombustionMWeight      = UnitConversion::UNIT_PER_KILO * UnitConversion::UNIV_GAS_CONST_SI
                            * mCombustionTemperature * (1.0 / g) * pow((gp1)/2.0, (gp1)/(gm1))
                            / configData.mCharacteristicVelocity / configData.mCharacteristicVelocity;

    /// - Initialize with input data:
    mIgnition               = inputData.mIgnition;
    mCombustion             = inputData.mCombustion;

    /// - Initialize remaining state data.
    mChamberPressure        = 0.0;
    mChamberTemperature     = 0.0;
    mChamberGamma           = 0.0;
    mChamberMWeight         = 0.0;
    mExitMach               = 0.0;
    mExitPressure           = 0.0;
    mExitTemperature        = 0.0;
    mExitGamma              = 0.0;
    mExitMWeight            = 0.0;
    mExitVelocity           = 0.0;
    mSpecificImpulse        = 0.0;
    mThrust                 = 0.0;
    mCharacteristicVelocity = 0.0;
    mSystemConductance      = 0.0;
    mCombustionCount        = 0;

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag               = true;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Link configuration data to be validated.
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Simple Rocket link model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleRocket::validate(const GunnsFluidSimpleRocketConfigData& configData) const
{
    /// - Throw an exception if throat area < DBL_EPSILON.
    if (configData.mThroatArea < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Throat area < DBL_EPSILON.");
    }

    /// - Throw an exception if area ratio < 1.
    if (configData.mAreaRatio < 1.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Area ratio < 1.");
    }

    /// - Throw an exception if characteristic velocity < DBL_EPSILON.
    if (configData.mCharacteristicVelocity < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Characteristic velocity < DBL_EPSILON.");
    }

    /// - Throw an exception if combustion temperature < DBL_EPSILON.
    if (configData.mCombustionTemperature < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Combustion temperature < DBL_EPSILON.");
    }

    /// - Throw an exception if combustion gamma <= 1.
    if (configData.mCombustionGamma < 1.0 + DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Combustion gamma <= 1.");
    }

    /// - Throw an exception if thrust tuning factor < 0.
    if (configData.mThrustTuning < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Thrust tuning factor < 0.");
    }

    /// - Throw an exception if recombination tuning factor not in (0-1).
    if (!MsMath::isInRange(0.0, configData.mRecombinationTuning, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Recombination tuning factor < 0 or > 1.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleRocket::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.  We must be careful not to reset
    ///   mDefaultConductance & mCombustionMWeight.  These are not strictly config data, but are
    ///   derived from config data only during init, not checkpointed, so we need to keep them.
    mChamberGamma           = 0.0;
    mChamberMWeight         = 0.0;
    mExitMach               = 0.0;
    mExitPressure           = 0.0;
    mExitTemperature        = 0.0;
    mExitGamma              = 0.0;
    mExitMWeight            = 0.0;
    mExitVelocity           = 0.0;
    mSpecificImpulse        = 0.0;
    mCharacteristicVelocity = 0.0;
    mSystemConductance      = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step (not used).
///
/// @details  Computes the conductance effect contribution to the network system of equations.
///           The rocket combustion & performance model is updated, and its modeled flow rate is
///           the target flow rate for this link to create in the network via the conductance
///           effect.  Final thrust output of the rocket is not computed here, but in computeFlows
///           based on the actual link flow rate from the network solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleRocket::step(const double dt __attribute__((unused)))
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Update system conductance and admittance matrix.
    updateConductance(updateRocketPerformance());
    buildAdmittanceMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double (kg/s) Desired mass flow rate through the link.
///
/// @details Models the rocket combustion chamber, combustion & nozzle performance and returns the
///          modeled total propellant mass flow rate through the engine.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidSimpleRocket::updateRocketPerformance()
{
    mChamberPressure = mPotentialVector[0];
    updateCombustion();
    updateChamber();
    return updateNozzle();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Determines whether combustion is occurring and updates the combusted gas properties.
///           If the optional combustion model is used, it is called to calculate the combustion
///           properties, otherwise the combustion properties are constant.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleRocket::updateCombustion()
{
    /// - Determine our combustion state based on ignition and forward flow.  This can be reset by
    ///   the combustion model below.
    if (mNeedsIgnition) {
        if (mFlowRate > static_cast<double>(FLT_EPSILON)) {
            mCombustion = mCombustion or mIgnition;
        } else {
            mCombustion = false;
        }
    } else {
        mCombustion = (mFlowRate > static_cast<double>(FLT_EPSILON));
    }

    if (mCombustModel and mCombustion) {
        /// - Default combustion temperature result to inlet temperature.  The combustion model will
        ///   change this to the actual combustion flame temperature if it finishes successfully.
        PolyFluid* inletFluid = mNodes[0]->getOutflow();
        double temperature = inletFluid->getTemperature();

        /// - Assemble mole fractions of the inlet mixture.
        int NTypes = mNodes[0]->getFluidConfig()->mNTypes;
        double newMoleFractions[NTypes];
        for (int i = 0; i < NTypes; ++i) {
            newMoleFractions[i] = inletFluid->getMoleFraction(inletFluid->getType(i));
        }

        /// - Get results of combustion from the combustion model.
        try {
            mCombustModel->updateCombustionMixture(mCombustionTemperature, mChamberPressure,
                                                   temperature, newMoleFractions,
                                                   mNodes[0]->getFluidConfig()->mTypes, NTypes);

            /// - The combustion model will return the inlet temperature if it deems that
            ///   flammability conditions are not met, extinguishing or preventing combustion, in
            ///   which case we turn off our combustion flag.  mCombustionTemperature is only
            ///   updated if combustion is still occurring.
            const double Tc = mCombustModel->getTemp();
            if (Tc <= inletFluid->getTemperature()) {
                mCombustion = false;
            } else {
                mCombustionTemperature = Tc;
                mCombustionGamma       = mCombustModel->getGamma();
                mCombustionMWeight     = mCombustModel->getMolecWeight();
            }
        } catch (TsInitializationException& e) {
            GUNNS_WARNING("caught exception from combustion model update combustion.");
        }
    }

    /// - Increment or reset the current combustion run frame count.
    if (mCombustion) {
        mCombustionCount++;
    } else {
        mCombustionCount = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the chamber fluid properties to either the results of combustion (if combusting)
///           or the inlet properties (if not combusting).
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleRocket::updateChamber()
{
    if (mCombustion) {
        mChamberTemperature = mCombustionTemperature;
        mChamberMWeight     = mCombustionMWeight;
        mChamberGamma       = mCombustionGamma;
    } else {
        mChamberTemperature = mNodes[0]->getOutflow()->getTemperature();
        mChamberMWeight     = mNodes[0]->getOutflow()->getMWeight();
        mChamberGamma       = mNodes[0]->getOutflow()->getAdiabaticIndex();
    }

    /// - Apply sane minimums to chamber properties to avoid divide-by-zero in later functions.
    mChamberTemperature = std::max(mChamberTemperature, 1.0);
    mChamberMWeight     = std::max(mChamberMWeight,     2.0);
    mChamberGamma       = std::max(mChamberGamma,       1.0 + static_cast<double>(FLT_EPSILON));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (kg/s) Nozzle exit mass flow rate.
///
/// @details  Models the nozzle performance, based on input products of combustion from the chamber,
///           back-pressure from the ambient atmosphere, and nozzle goemetry.  This outputs specific
///           impulse and mass flow rate.
///
///           Shifting chemical equilibrium in the nozzle can be modeled, where the dissociated
///           compounds from combustion tend to recombine as temperature & pressure drop across the
///           nozzle, and the effects of this recombination on impulse and flow rate.  This effect
///           can be scaled by the mRecombinationTuning term, where 1.0 is full recombination
///           (fluid stays in perfect chemical equilibrium through the nozzle), and 0.0 is no
///           recombination (fluid is "frozen" i.e. undergoes no further reaction after combustion
///           and maintains constant mixture through the nozzle).
///
///           Regardless of actual inlet conditions or chamber & nozzle geometry, we assume that
///           flow is always ideal gas and choked at the throat.  For non-hypergolic propellants
///           that aren't combusting, this might not always be valid, so then we zero Isp output.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidSimpleRocket::updateNozzle()
{
    /// - Exit Mach & pressure, Hill & Peterson, Eqn. 3.11.  When using the recombination model,
    ///   gamma changes through the nozzle, but not by much.  We assume it's constant.
    const double g   = mChamberGamma;
    updateExitMach(g);
    const double gm1 = g - 1.0;
    const double gMM = 1.0 + 0.5 * gm1 * mExitMach * mExitMach;
    mExitPressure = mChamberPressure * std::pow(gMM, -g/gm1);

    /// - Exit velocity assuming 'frozen' flow for now, Hill & Peterson, Eqn. 11.3.
    const double Tc  = mChamberTemperature;
    const double MWc = mChamberMWeight;
    double Ue = std::sqrt( (1.0 - 1.0/gMM) * 2.0 * g * Tc * UnitConversion::UNIT_PER_KILO
                    * UnitConversion::UNIV_GAS_CONST_SI / gm1 / MWc );

    /// - Initial calculation of exit temperature by isentropic relations asssuming 'frozen' flow.
    ///   This is used as the lower bound starting guess for the combustion recombination model,
    ///   since recombination always adds more heat.
    double Te  = Tc / gMM;
    double ge  = g;
    double MWe = MWc;

    /// - Get results of recombination in nozzle from the combustion model.  This is set up so that
    ///   if the combustion model fails or gives an invalid output, we fall back to the 'frozen',
    ///   no-recombination result.  This would cause a jump in thrust.
    const double RTF = MsMath::limitRange(0.0, mRecombinationTuning, 1.0);
    if (mCombustion) {
        if (mCombustModel and RTF > static_cast<double>(FLT_EPSILON)) {
            try {
                /// - Enthalpy from the combustion model is in (kJ/g), so we convert to (J/kg).
                double dH = mCombustModel->getEnth() * UnitConversion::UNIT_PER_MEGA;
                //TODO Don't give Te < 1500 K or so, does bad things to combust model.
                mCombustModel->updateRecombinationMixture(Te, mExitPressure);
                dH -= mCombustModel->getEnth() * UnitConversion::UNIT_PER_MEGA;

                /// - Exit velocity based on change from stagnation (chamber) and exit enthalpies,
                ///   Hill & Peterson, Eqn. 3.5.  Interpolate between the equilibrium and frozen
                ///   results by tuning factor.
                if (dH > 0.0) {
                    Ue  = (1.0 - RTF) * Ue  + RTF * sqrt(2.0 * dH);
                    Te  = (1.0 - RTF) * Te  + RTF * mCombustModel->getTemp();
                    ge  = (1.0 - RTF) * ge  + RTF * mCombustModel->getGamma();
                    MWe = (1.0 - RTF) * MWe + RTF * mCombustModel->getMolecWeight();
                }
            } catch (TsInitializationException& e) {
                GUNNS_WARNING("caught exception from combustion model update recombination.");
            }
        }
        mSpecificImpulse = Ue / UnitConversion::STD_GRAVITY_SI;
    } else {
        /// - We zero specific impulse & thrust if there isn't combustion, as a simplification.
        ///   For cold gas jets, set the engine's mNeedsIgnition flag false so "combustion" will
        ///   always be true for all flows and it will still create thrust.
        mSpecificImpulse = 0.0;
    }
    mExitTemperature = Te;
    mExitGamma       = ge;
    mExitMWeight     = MWe;
    mExitVelocity    = Ue;

    /// - Mass flow rate from conservation of mass across the exit plane & density from ideal gas.
    return mThroatArea * mAreaRatio * mExitVelocity * mExitPressure * mExitMWeight
         / UnitConversion::UNIV_GAS_CONST_SI / mExitTemperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// param[in] gamma (--) Ratio of specific heats of the fluid inlet to the nozzle.
///
/// @details  From Hill & Peterson, Eqn. 3.15, the exit Mach number of the nozzle is only a function
///           of the area ratio and gamma of the fluid.  This can't be solved algebraically so we
///           iterate on it using a bisection method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleRocket::updateExitMach(const double gamma)
{
    const double gp1         = gamma + 1.0;
    const double gm1         = gamma - 1.0;
    const double gp1OverGm1  = gp1 / gm1;
    double       machPrev    = 20.0;
    double       mach        = 10.0;
    double       ratio       = 0.0;

    for (int i=0; i<30; ++i) {
        ratio = (1.0 / mach) * pow( 2.0 / gp1 + mach * mach / gp1OverGm1, gp1OverGm1 / 2.0);
        const double delta = 0.5 * fabs(mach - machPrev);
        if (delta < mMachTolerance) {
            break;
        }
        machPrev = mach;
        if (ratio > mAreaRatio) {
            mach -= delta;
        } else {
            mach += delta;
        }
    }
    mExitMach = mach;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] mdot (kg/s) Desired mass flow rate through the link.
///
/// @details  Computes the final linearized molar conductance to go into the network system of
///           equations admittance matrix.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleRocket::updateConductance(const double mdot)
{
    double p1 = mNodes[1]->getOutflow()->getPressure();
    double p0 = std::max(mNodes[0]->getOutflow()->getPressure(), p1 + mMinLinearizationPotential);

    /// - Note that normally GUNNS conductors use the average of the 2 node's molecular weights for
    ///   the admittance calculation to reduce state error when flowing between different mixtures.
    ///   But in our case that will throw off our flow rates too much and we don't care about state
    ///   error in the downstream node since it is assumed to be an ambient node in which accurate
    ///   mixed state isn't needed.  So we just use the inlet node's molecular weight.
    const double MW = mNodes[0]->getOutflow()->getMWeight();

    /// - Protect against divide by zero molecular weight, and shut off the link when no forward
    ///   pressure gradient.
    if (MW > DBL_EPSILON and (mPotentialVector[0] > mPotentialVector[1] - static_cast<double>(FLT_EPSILON))) {

        double conductance = 0.0;
        if (1 == mCombustionCount) {
            /// - On the first frame of combustion, kick-start the link conductance to the steady-
            ///   state value corresponding to the default combustion products.  This is to get a
            ///   faster convergence to steady-state in the network solution.
            conductance = mDefaultConductance;
        } else {
            conductance = mdot / (p0 - p1);
        }

        /// - Convert mass to molar conductance by molecular weight the inlet fluid.
        mSystemConductance = MsMath::limitRange(0.0, (conductance / MW), mConductanceLimit);
    } else {
        mSystemConductance = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the Admittance Matrix for the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleRocket::buildAdmittanceMatrix()
{
    if (fabs(mAdmittanceMatrix[0] - mSystemConductance) > 0.0) {
        mAdmittanceMatrix[0]   =  mSystemConductance;
        mAdmittanceMatrix[1]   = -mAdmittanceMatrix[0];
        mAdmittanceMatrix[2]   = -mAdmittanceMatrix[0];
        mAdmittanceMatrix[3]   =  mAdmittanceMatrix[0];
        mAdmittanceUpdate      = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Not used.
///
/// @details  Computes the Flux from the node and updates the nodal port directions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleRocket::computeFlows(const double dt __attribute__((unused)))
{
    mPotentialDrop = getDeltaPotential();

    /// - Compute molar flow rate.
    updateFlux(0.0, 0.0);

    /// - Set Port Directions
    if (mFlux > DBL_EPSILON) {
        mPortDirections[0] = SOURCE;
        mPortDirections[1] = SINK;
        mNodes[0]->scheduleOutflux(mFlux);
    } else if (mFlux < -DBL_EPSILON) {
        mPortDirections[0] = SINK;
        mPortDirections[1] = SOURCE;
        mNodes[1]->scheduleOutflux(-mFlux);
    } else {
        mPortDirections[0] = NONE;
        mPortDirections[1] = NONE;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Not used.
///
/// @details  Computes the flow rate across this link, transports fluid through the link and updates
///           thrust output of the rocket.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleRocket::transportFlows(const double dt __attribute__((unused)))
{
    /// - Convert the molar flow rate to mass rate using the molecular weight of the source node.
    mFlowRate = mFlux * mNodes[0]->getOutflow()->getMWeight();

    /// - This characteristic velocity can be used to tune the config data term for faster
    ///   convergence to steady-state during startup.  Hill & Peterson, Eqn. 11.8:
    if (mFlowRate > static_cast<double>(FLT_EPSILON)) {
        mCharacteristicVelocity = mPotentialVector[0] * UnitConversion::PA_PER_KPA * mThroatArea
                                / mFlowRate;
    } else {
        mCharacteristicVelocity = 0.0;
    }

    /// - Call the base class to transport the fluid between the nodes.  The false bool argument
    ///   indicates that this class cannot force a mixture change in the supplying node.
    transportFluid(false);

    /// - Total thrust including a tuning scale factor.
    mThrust = mThrustTuning * mFlowRate * mSpecificImpulse * UnitConversion::STD_GRAVITY_SI;

    /// - Kinetic power of the exhaust gas.
    mPower  = 0.5 * mThrust * mExitVelocity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt   (s)  Integration time step (not used).
/// @param[in] flux (--) Generic flux through the link (not used).
///
/// @details  Computes the molar flux rate through the link.  Units:
///           Flux (kgmol/s) = Admittance (kgmol/kPa/s) * dP (kPa)
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleRocket::updateFlux(const double dt   __attribute__((unused)),
                                        const double flux __attribute__((unused)))
{
    const double hiP = std::max(mPotentialVector[0], mPotentialVector[1]);
    if (fabs(mPotentialDrop) < (hiP * m100EpsilonLimit)) {
        /// - Zero flux if dP is too low.  This eliminates most mass loss/creation due to rounding
        ///   error in the solver.
        mFlux = 0.0;
    } else {
        mFlux = mPotentialDrop * mAdmittanceMatrix[0];
    }
}
