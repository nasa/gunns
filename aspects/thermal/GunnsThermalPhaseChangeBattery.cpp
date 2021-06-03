/**
@file
@brief    GUNNS Thermal Phase Change Battery Link implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
     (core/GunnsBasicLink.o)
     (math/UnitConversion.o)
    )
*/

#include "GunnsThermalPhaseChangeBattery.hh"
#include "aspects/thermal/PtcsMacros.hh"
#include "math/MsMath.hh"
#include <cfloat>

/// @details  This value is a compromise between being large enough to act like an ideal potential
///           source under large heat loads, and small enough to avoid truncating other conductances
///           incident on the node.
const double GunnsThermalPhaseChangeBattery::mIdealAdmittance = 1.0e6;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                   (--)    Instance name for output messages.
/// @param[in] nodes                  (--)    Network nodes array.
/// @param[in] phaseChangeTemperature (K)     Temperature at which the thermal battery medium changes phase.
/// @param[in] phaseChangeHeat        (J/g)   Heat of phase change of the thermal battery medium.
/// @param[in] hotPhaseSpecificHeat   (J/g/K) Specific heat of the thermal battery medium in the hotter phase.
/// @param[in] coldPhaseSpecificHeat  (J/g/K) Specific heat of the thermal battery medium in the colder phase.
/// @param[in] structureCapacitance   (J/K)   Thermal capacitance of non-phase changing structure.
///
/// @details  Default constructs this Thermal Phase Change Battery link Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPhaseChangeBatteryConfigData::GunnsThermalPhaseChangeBatteryConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const double       phaseChangeTemperature,
        const double       phaseChangeHeat,
        const double       hotPhaseSpecificHeat,
        const double       coldPhaseSpecificHeat,
        const double       structureCapacitance)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mPhaseChangeTemperature(phaseChangeTemperature),
    mPhaseChangeHeat(phaseChangeHeat),
    mHotPhaseSpecificHeat(hotPhaseSpecificHeat),
    mColdPhaseSpecificHeat(coldPhaseSpecificHeat),
    mStructureCapacitance(structureCapacitance)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs this Thermal Phase Change Battery link Config Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPhaseChangeBatteryConfigData::~GunnsThermalPhaseChangeBatteryConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] mass                 (kg)   Initial mass of the phase-changing thermal battery medium.
/// @param[in] temperature          (K)    Initial temperature of the thermal battery medium.
/// @param[in] hotPhaseFraction     (--)   Initial mass fraction (0-1) of the phase-changing thermal battery medium in the hotter phase.
/// @param[in] malfHotPhaseLeakFlag (--)   Initial hot phase mass leak malfunction activation flag.
/// @param[in] malfHotPhaseLeakRate (kg/s) Initial hot phase mass leak malfunction leak rate.
///
/// @details  Default constructs this Thermal Phase Change Battery link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPhaseChangeBatteryInputData::GunnsThermalPhaseChangeBatteryInputData(
        const double mass,
        const double temperature,
        const double hotPhaseFraction,
        const bool   malfHotPhaseLeakFlag,
        const double malfHotPhaseLeakRate)
    :
    GunnsBasicLinkInputData(false, 0.0),
    mMass(mass),
    mTemperature(temperature),
    mHotPhaseFraction(hotPhaseFraction),
    mMalfHotPhaseLeakFlag(malfHotPhaseLeakFlag),
    mMalfHotPhaseLeakRate(malfHotPhaseLeakRate)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Thermal Phase Change Battery link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPhaseChangeBatteryInputData::~GunnsThermalPhaseChangeBatteryInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Thermal Phase Change Battery link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPhaseChangeBattery::GunnsThermalPhaseChangeBattery()
    :
    GunnsBasicLink(NPORTS),
    mMalfHotPhaseLeakFlag(false),
    mMalfHotPhaseLeakRate(0.0),
    mPhaseChangeTemperature(0.0),
    mPhaseChangeHeat(0.0),
    mHotPhaseSpecificHeat(0.0),
    mColdPhaseSpecificHeat(0.0),
    mStructureCapacitance(0.0),
    mMass(0.0),
    mTemperature(0.0),
    mHotPhaseFraction(0.0),
    mActualLeakRate(0.0),
    mExternalHeatFlux(),
    mSumExternalHeatFluxes(0.0),
    mAdmittance(0.0)
{
    zeroExternalFluxes();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs this Thermal Phase Change Battery link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPhaseChangeBattery::~GunnsThermalPhaseChangeBattery()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data.
/// @param[in,out] inputData    (--) Reference to Link Input Data.
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector.
/// @param[in]     port0        (--) Port 0 Mapping.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Thermal Phase Change Battery link with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPhaseChangeBattery::initialize(
        const GunnsThermalPhaseChangeBatteryConfigData& configData,
        const GunnsThermalPhaseChangeBatteryInputData&  inputData,
        std::vector<GunnsBasicLink*>&                   networkLinks,
        const int                                       port0)
{
    /// - Initialize the parent class.
    int ports[1] = {port0};
    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Validate config & input data.
    validate(configData, inputData);

    /// - Initialize from config & input data.
    mPhaseChangeTemperature = configData.mPhaseChangeTemperature;
    mPhaseChangeHeat        = configData.mPhaseChangeHeat;
    mHotPhaseSpecificHeat   = configData.mHotPhaseSpecificHeat;
    mColdPhaseSpecificHeat  = configData.mColdPhaseSpecificHeat;
    mStructureCapacitance   = configData.mStructureCapacitance;
    mMass                   = inputData.mMass;
    mHotPhaseFraction       = inputData.mHotPhaseFraction;
    mMalfHotPhaseLeakFlag   = inputData.mMalfHotPhaseLeakFlag;
    mMalfHotPhaseLeakRate   = inputData.mMalfHotPhaseLeakRate;
    setTemperature(inputData.mTemperature);

    /// - Initialize state data.
    zeroExternalFluxes();
    mActualLeakRate = 0.0;
    mAdmittance     = 0.0;

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data.
/// @param[in,out] inputData    (--) Reference to Link Input Data.
///
/// @throws   TsInitializationException
///
/// @details  Checks config & input values for valid ranges and throws exceptions for out of range.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPhaseChangeBattery::validate(
        const GunnsThermalPhaseChangeBatteryConfigData& configData,
        const GunnsThermalPhaseChangeBatteryInputData&  inputData) const
{
    /// - Throw an exception if phase change temperature < DBL_EPSILON.
    TS_PTCS_IF_ERREX(configData.mPhaseChangeTemperature < DBL_EPSILON, TsInitializationException,
            "Invalid Configuration Data", "mPhaseChangeTemperature < DBL_EPSILON.");

    /// - Throw an exception if phase change heat < DBL_EPSILON.
    TS_PTCS_IF_ERREX(configData.mPhaseChangeHeat < DBL_EPSILON, TsInitializationException,
            "Invalid Configuration Data", "mPhaseChangeHeat < DBL_EPSILON.");

    /// - Throw an exception if hot phase specific heat < DBL_EPSILON.
    TS_PTCS_IF_ERREX(configData.mHotPhaseSpecificHeat < DBL_EPSILON, TsInitializationException,
            "Invalid Configuration Data", "mHotPhaseSpecificHeat < DBL_EPSILON.");

    /// - Throw an exception if cold phase specific heat < DBL_EPSILON.
    TS_PTCS_IF_ERREX(configData.mColdPhaseSpecificHeat < DBL_EPSILON, TsInitializationException,
            "Invalid Configuration Data", "mColdPhaseSpecificHeat < DBL_EPSILON.");

    /// - Throw an exception if structure capacitance < DBL_EPSILON.
    TS_PTCS_IF_ERREX(configData.mStructureCapacitance < DBL_EPSILON, TsInitializationException,
            "Invalid Configuration Data", "mStructureCapacitance < DBL_EPSILON.");

    /// - Throw an exception if initial mass < DBL_EPSILON.
    TS_PTCS_IF_ERREX(inputData.mMass < DBL_EPSILON, TsInitializationException,
            "Invalid Input Data", "mMass < DBL_EPSILON.");

    /// - Throw an exception if initial temperature < DBL_EPSILON.
    TS_PTCS_IF_ERREX(inputData.mTemperature < DBL_EPSILON, TsInitializationException,
            "Invalid Input Data", "mTemperature < DBL_EPSILON.");

    /// - Throw an exception if initial hot phase fraction not in (0-1).
    TS_PTCS_IF_ERREX(!MsMath::isInRange(0.0, inputData.mHotPhaseFraction, 1.0), TsInitializationException,
            "Invalid Input Data", "mHotPhaseFraction not in (0-1).");

    /// - Throw an exception if initial leak rate < 0.
    TS_PTCS_IF_ERREX(inputData.mMalfHotPhaseLeakRate < 0.0, TsInitializationException,
            "Invalid Input Data", "mMalfHotPhaseLeakRate < 0.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPhaseChangeBattery::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    zeroExternalFluxes();
    mAdmittance     = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  Updates this Thermal Phase Change Battery link's contributions to the network system
///           of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPhaseChangeBattery::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Call virtual updateState method so a derived model override admittance.
    updateState(dt);

    /// - Build the system admittance matrix and source vector contributions.
    if (fabs(mAdmittanceMatrix[0] - mAdmittance) > 0.0) {
        mAdmittanceMatrix[0] = mAdmittance;
        mAdmittanceUpdate    = true;
    }
    mSourceVector[0] = mPotentialVector[0] * mAdmittanceMatrix[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  Updates the link admittance depending on the battery material phase.  When in single-
///           phase, this link acts like a normal thermal capacitor on the Port 0 node.  When in
///           mixed-phase, this link acts like an ideal potential source to constrain the Port 0
///           node to the phase-change temperature.
///
///           This also integrates heat fluxes into the new battery temperature for when the network
///           is in DUMMY mode.  This is overwritten by the network solution in NORMAL network mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPhaseChangeBattery::updateState(const double dt)
{
    /// - Limit hot phase fraction to valid range in case of bad inputs during run-time.
    mHotPhaseFraction = MsMath::limitRange(0.0, mHotPhaseFraction, 1.0);

    /// - Update capacitance and external heat fluxes.
    const double capacitance = computeCapacitance();
    sumExternalFluxes();

    /// - Update the link admittance and temperatures.
    mAdmittance = DBL_EPSILON;
    if (not mOverrideVector[0]) {
        if (capacitance > mStructureCapacitance) {
            mAdmittance = capacitance / std::max(dt, DBL_EPSILON);
            setTemperature(mPotentialVector[0] + mSumExternalHeatFluxes * dt / capacitance);
        } else {
            mAdmittance = mIdealAdmittance;
            setTemperature(mPhaseChangeTemperature + mSumExternalHeatFluxes / mAdmittance);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (J/K) Link capacitance.
///
/// @details  If the battery is in mixed-phase, then this returns just the structural capacitance.
///           If in single-phase, then it adds the thermal capacitance of all of the phase-change
///           mass in that phase.  The caller can cmopare the result to the structural capacitance
///           to determine whether the battery is in single- or mixed-phase.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsThermalPhaseChangeBattery::computeCapacitance() const
{
    double result = mStructureCapacitance;
    if (0.0 == mHotPhaseFraction) {
        result += mMass * mColdPhaseSpecificHeat * UnitConversion::UNIT_PER_KILO;
    } else if (1.0 == mHotPhaseFraction) {
        result += mMass * mHotPhaseSpecificHeat * UnitConversion::UNIT_PER_KILO;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] temperature (K) Desired temperature to set.
///
/// @details  The link's temperature and Port 0 potential vector are set to the given value.  The
///           attached node's potential is also set if this is not the network Ground node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPhaseChangeBattery::setTemperature(const double temperature)
{
    mPotentialVector[0] = temperature;
    mTemperature        = temperature;
    if (mNodeMap[0] != getGroundNodeIndex()) {
        mNodes[0]->setPotential(temperature);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  Updates the link state in response to flows resulting from the network solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPhaseChangeBattery::computeFlows(const double dt)
{
    /// - Update temperatures from the network solution.
    mTemperature   = mPotentialVector[0];
    mPotentialDrop = mPotentialVector[0];

    /// - Compute & transport fluxes.
    computeFlux();
    computePower();
    transportFlux();

    /// - Update the battery mass & phase in response to flows.
    updateFlux(dt, 0.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details mPower is the total heat rise in the heat battery, including flux into the node from
///          other links in this network (mFlux), and the heat added by this link from external
///          model interfaces (mSumExternalHeatFluxes).
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPhaseChangeBattery::computePower()
{
    mPower = mFlux + mSumExternalHeatFluxes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fromPort (--) Not used.
/// @param[in] toPort   (--) Not used.
///
/// @details This updates the flux values in the attached node, so the node can check that KCL is
///          satisfied.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPhaseChangeBattery::transportFlux(const int fromPort __attribute__((unused)),
                                                   const int toPort   __attribute__((unused)))
{
    if (mFlux > 0.0) {
        mNodes[0]->collectOutflux(mFlux);
    } else if (mFlux < 0.0) {
        mNodes[0]->collectInflux (-mFlux);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt   (s) Integration time step.
/// @param[in] flux (W) Not used.
///
/// @details  Updates the hot phase mass fraction, total mass and actual leak rate in response to
///           heat fluxes and the hot phase leak malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPhaseChangeBattery::updateFlux(const double dt,
                                                const double flux __attribute__((unused)))
{
    /// - Protect for divide-by-zero below, in case of bad inputs during run-time.
    mPhaseChangeHeat = std::max(mPhaseChangeHeat, DBL_EPSILON);
    mMass            = std::max(mMass,            DBL_EPSILON * 3.0); //TODO explain the 3, leak stuff below

    updatePhaseFraction(dt);
    updateMassLeak(dt);

    mHotPhaseFraction = MsMath::limitRange(0.0, mHotPhaseFraction, 1.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  Updates the hot phase mass fraction during phase change.
///
/// @note     The caller must ensure that mPhaseChangeHeat and mMass are > 0 to avoid dividing by
///           zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPhaseChangeBattery::updatePhaseFraction(const double dt)
{
    if (0.0 == mHotPhaseFraction and mTemperature > mPhaseChangeTemperature) {
        /// - Entering mixed-phase by heating: convert the excess temperature from the cold-phase
        ///   specific heat to phase change.
        const double heat = mColdPhaseSpecificHeat * mMass * (mTemperature - mPhaseChangeTemperature);
        const double dm   = heat / mPhaseChangeHeat;
        mHotPhaseFraction += dm / mMass;

    } else if (1.0 == mHotPhaseFraction and mTemperature < mPhaseChangeTemperature) {
        /// - Entering mixed-phase by cooling.
        const double heat = mHotPhaseSpecificHeat * mMass * (mTemperature - mPhaseChangeTemperature);
        const double dm   = heat / mPhaseChangeHeat;
        mHotPhaseFraction += dm / mMass;

    } else if (0.0 < mHotPhaseFraction and 1.0 > mHotPhaseFraction) {
        /// - Continuing in mixed-phase: the total heat flux into the battery goes into phase change.
        const double dm = UnitConversion::KILO_PER_UNIT * mPower * dt / mPhaseChangeHeat;
        mHotPhaseFraction += dm / mMass;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  Models the hot phase leak malfunction and updates the phase-change mass in response.
///
/// @note     The caller must ensure that mMass is > 0 to avoid dividing by zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPhaseChangeBattery::updateMassLeak(const double dt)
{
    /// - Negative leak rate isn't allowed so we don't have to handle the resulting corner cases.
    mMalfHotPhaseLeakRate = std::max(0.0, mMalfHotPhaseLeakRate);

    /// - Update the phase-change mass, hot phase fraction and actual leak rate for the hot phase
    ///   leak malfunction.
    if (mMalfHotPhaseLeakFlag and mHotPhaseFraction > 0.0 and dt > 0.0) {
        double hotMass = mMass * mHotPhaseFraction;
        /// - The leak can't remove all total mass or reduce hot phase fraction to zero all by
        ///   itself.  This allows heat added to the battery to feed the leak when it's all cold-
        ///   phase at the phase-change T.
        const double minMass = std::max(0.0, std::min(hotMass - DBL_EPSILON,
                                                      mMass - 2.0 * DBL_EPSILON));
        mActualLeakRate = std::min(mMalfHotPhaseLeakRate, minMass / dt);
        hotMass -= mActualLeakRate * dt;
        mMass   -= mActualLeakRate * dt;
        mHotPhaseFraction = hotMass / mMass;
    } else {
        mActualLeakRate = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Zeroes the external heat fluxes array and the sum.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPhaseChangeBattery::zeroExternalFluxes()
{
    for (int i = 0; i < GunnsThermalCapacitor::NUM_EXT_HEATFLUXES; ++i) {
        mExternalHeatFlux[i] = 0.0;
    }
    mSumExternalHeatFluxes = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Sums the external heat fluxes array into the mSumExternalHeatFluxes term.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPhaseChangeBattery::sumExternalFluxes()
{
    mSumExternalHeatFluxes = 0.0;
    for (int i = 0; i < GunnsThermalCapacitor::NUM_EXT_HEATFLUXES; ++i) {
        mSumExternalHeatFluxes += mExternalHeatFlux[i];
    }
}
