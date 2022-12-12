/**
@file
@brief    GUNNS Fluid High-Fidelity Orifice Model implementation

@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsFluidLink.o))
**************************************************************************************************/

#include "GunnsFluidHiFiOrifice.hh"

#include "core/GunnsFluidUtils.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

/// @details  See derivation in Redmine issue #345.
const double GunnsFluidHiFiOrificeConfigData::mConvertKvToCd = 50911.7;
/// @details  Reference: "Control Valve Handbook",
///           http://www.chemicalprocessing.com/assets/Media/MediaManager/control_valves.pdf
///           Cv = Kv / 0.865
///           Cd = Kv / (A * KvToCd)
///           Cd = Cv / (A * KvToCd / 0.865)
///           Cd = Cv / (A * CvToCd), CvToCd = KvToCd / 0.865
const double GunnsFluidHiFiOrificeConfigData::mConvertCvToCd =
             GunnsFluidHiFiOrificeConfigData::mConvertKvToCd / 0.865;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                   (--) Name of object.
/// @param[in] nodes                  (--) Pointer to nodes.
/// @param[in] coefficientType        (--) Type of coefficient to be specified.
/// @param[in] coefficientValue       (--) Coefficient value of the specified type.
/// @param[in] throatDiameter         (m)  Throat diameter.
/// @param[in] criticalReynolds       (--) Optional Reynolds number at laminar/turbulent transition.
/// @param[in] expansionScaleFactor   (--) (0-1) Optional scaling for expansion gas cooling.
/// @param[in] flowTuningFactor       (--) Optional factor for flow tuning.
///
/// @details  Default constructs this GUNNS Fluid Hi-Fi Orifice link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiOrificeConfigData::GunnsFluidHiFiOrificeConfigData(const std::string& name,
                                                                 GunnsNodeList*     nodes,
                                                                 const CoeffTypes   coefficientType,
                                                                 const double       coefficientValue,
                                                                 const double       throatDiameter,
                                                                 const double       criticalReynolds,
                                                                 const double       expansionScaleFactor,
                                                                 const double       flowTuningFactor)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mCoefficientType     (coefficientType),
    mCoefficientValue    (coefficientValue),
    mThroatDiameter      (throatDiameter),
    mCriticalReynolds    (criticalReynolds),
    mExpansionScaleFactor(expansionScaleFactor),
    mFlowTuningFactor    (flowTuningFactor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Hi-Fi Orifice link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiOrificeConfigData::GunnsFluidHiFiOrificeConfigData(const GunnsFluidHiFiOrificeConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mCoefficientType     (that.mCoefficientType),
    mCoefficientValue    (that.mCoefficientValue),
    mThroatDiameter      (that.mThroatDiameter),
    mCriticalReynolds    (that.mCriticalReynolds),
    mExpansionScaleFactor(that.mExpansionScaleFactor),
    mFlowTuningFactor    (that.mFlowTuningFactor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hi-Fi Orifice link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiOrificeConfigData::~GunnsFluidHiFiOrificeConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag   (--)  Blockage malfunction flag.
/// @param[in]  malfBlockageValue  (--)  Blockage malfunction fractional value (0-1).
///
/// @details  Default constructs this GUNNS Fluid Hi-Fi Orifice link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiOrificeInputData::GunnsFluidHiFiOrificeInputData(const bool   malfBlockageFlag,
                                                               const double malfBlockageValue)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   --  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Hi-Fi Orifice link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiOrificeInputData::GunnsFluidHiFiOrificeInputData(const GunnsFluidHiFiOrificeInputData& that)
    :
    GunnsFluidLinkInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hi-Fi Orifice link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiOrificeInputData::~GunnsFluidHiFiOrificeInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Hi-Fi Orifice link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiOrifice::GunnsFluidHiFiOrifice()
    :
    GunnsFluidLink(NPORTS),
    mCdDefault(0.0),
    mThroatDiameter(0.0),
    mReCritical(0.0),
    mExpansionScaleFactor(0.0),
    mFlowTuningFactor(0.0),
    mThroatArea(0.0),
    mReActual(0.0),
    mCdActual(0.0),
    mSystemConductance(0.0),
    mPressureRatio(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hi-Fi Orifice link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiOrifice::~GunnsFluidHiFiOrifice()
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
/// @details  Initializes this GUNNS Fluid Hi-Fi Orifice link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFiOrifice::initialize(const GunnsFluidHiFiOrificeConfigData& configData,
                                       const GunnsFluidHiFiOrificeInputData&  inputData,
                                       std::vector<GunnsBasicLink*>&          links,
                                       const int                              port0,
                                       const int                              port1)
{
    /// - Initialize the parent class.
    int ports[2] = {port0, port1};
    GunnsFluidLink::initialize(configData, inputData, links, ports);

    /// - Reset initialization status flag.
    mInitFlag              = false;

    /// - Initialize with configuration data:

    /// - Throw an exception if throat diameter <= 0.
    if (configData.mThroatDiameter < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Throat diameter <= 0.");
    }
    mThroatDiameter        = configData.mThroatDiameter;
    mThroatArea            = UnitConversion::PI_OVER_4 * mThroatDiameter * mThroatDiameter;
    mReCritical            = configData.mCriticalReynolds;

    switch (configData.mCoefficientType) {

        case GunnsFluidHiFiOrificeConfigData::DISCHARGE_COEFF:
            /// - Given Coefficient of Discharge, Cd, is used as-is:
            mCdDefault = configData.mCoefficientValue;
            break;

        case GunnsFluidHiFiOrificeConfigData::FLOW_FACTOR:
            /// - Convert given Flow Factor, Kv, to Coefficient of Discharge, Cd:
            mCdDefault = configData.mCoefficientValue
                                  / (mThroatArea * GunnsFluidHiFiOrificeConfigData::mConvertKvToCd);
            break;

        case GunnsFluidHiFiOrificeConfigData::FLOW_COEFF:
            /// - Convert given Flow Coefficient, Cv, to Coefficient of Discharge, Cd:
            mCdDefault = configData.mCoefficientValue
                                  / (mThroatArea * GunnsFluidHiFiOrificeConfigData::mConvertCvToCd);
            break;

        default:
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "Invalid coefficient type.");
            break;
    }

    mExpansionScaleFactor  = configData.mExpansionScaleFactor;
    mFlowTuningFactor      = configData.mFlowTuningFactor;

    /// - Initialize remaining state data.
    mReActual              = 0.0;
    mCdActual              = 0.0;
    mSystemConductance     = 0.0;
    mPressureRatio         = 0.0;
    createInternalFluid();

    /// - Validate the initial state.
    validate();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag              = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Hi-Fi Orifice link model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFiOrifice::validate() const
{
    /// - Throw an exception if Cd <= 0 or > 1.
    if (!MsMath::isInRange(DBL_EPSILON, mCdDefault, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Coefficient of discharge outside valid range [0-1).");
    }

    /// - Throw an exception if flow critical Reynolds < 0.
    if (mReCritical < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Critical Reynolds Number < 0.");
    }

    /// - Throw an exception if expansion scale factor < 0 or > 1.
    if (!MsMath::isInRange(0.0, mExpansionScaleFactor, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Expansion scale factor outside valid range (0-1).");
    }

    /// - Throw an exception if flow tuning factor < 0.
    if (mFlowTuningFactor < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Flow tuning factor < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFiOrifice::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mReActual              = 0.0;
    mCdActual              = 0.0;
    mSystemConductance     = 0.0;
    mPressureRatio         = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step
///
/// @details  Computes the conductance effect contribution to the network system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFiOrifice::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Determine source node from last pass pressures.
    int inletPort = 0;
    int exitPort  = 1;
    if (mNodeMap[0] == getGroundNodeIndex() or mPotentialVector[1] > mPotentialVector[0]) {
        inletPort = 1;
        exitPort  = 0;
    }

    /// - This skips processing when both ports are on the network ground node.
    if (mNodeMap[inletPort] == getGroundNodeIndex()) {
        return;
    }

    /// - Determine Reynolds number and actual coefficient of discharge considering laminar flow.
    const double vm = fabs(mFlowRate) / mNodes[inletPort]->getOutflow()->getDensity() / mThroatArea;
    mReActual = GunnsFluidUtils::computeReynoldsNumber(mNodes[inletPort]->getOutflow(),
                                                       vm, mThroatDiameter);

    mCdActual = mCdDefault * sqrt(std::min(mReCritical, mReActual)
                                / std::max(mReCritical, DBL_EPSILON));
    if (mCdActual < DBL_EPSILON) mCdActual = mCdDefault;

    /// - Update derived model state.
    updateState(dt);

    computeConductance(mNodes[inletPort]->getOutflow(), mNodes[exitPort]->getOutflow());

    buildAdmittanceMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  fluid0  (--)  Pointer to the inlet node fluid.
/// @param[in]  fluid1  (--)  Pointer to the outlet node fluid.
///
/// @details  Computes the final linearized molar conductance to go into the network system of
///           equations admittance matrix.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFiOrifice::computeConductance(const PolyFluid* fluid0, const PolyFluid* fluid1)
{
    double conductivity = 0.0;

    /// - Determine conductivity based on phase of the fluid.
    if (FluidProperties::GAS == fluid0->getPhase()) {
        conductivity = computeGasConductivity(fluid0, fluid1);
    } else if (FluidProperties::LIQUID == fluid0->getPhase()) {
        conductivity = computeLiquidConductivity(fluid0, fluid1);
    }

    /// - Conductance from conductivity and actual throat area: fixed area (at full open) is reduced
    ///   by optional derived class open/close position scalar and blockage malfunction.
    const double conductance = conductivity * getEffectiveArea();

    /// - Convert mass to molar conductance by average molecular weight of the inlet & exit fluids.
    ///   This avoids using the Ground node's MW since it is zero.  If both ports are on Ground
    ///   then conductance is zero.
    const double MW0 = fluid0->getMWeight();
    const double MW1 = fluid1->getMWeight();
    double avgMW = 0.0;
    if (MW0 < DBL_EPSILON) {
        avgMW = MW1;
    } else if (MW1 < DBL_EPSILON) {
        avgMW = MW0;
    } else {
        avgMW = 0.5 * (MW0 + MW1);
    }
    if (avgMW > DBL_EPSILON) {
        mSystemConductance = MsMath::limitRange(0.0, (conductance / avgMW), mConductanceLimit);
    } else {
        mSystemConductance = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  fluid0  (--)  Pointer to the inlet node fluid.
/// @param[in]  fluid1  (--)  Pointer to the outlet node fluid.
///
/// @returns  double (kg/s/kPa/m2) Conductivity of gas flow.
///
/// @details  Determines the gas flow regime and returns the linearized mass flow conductivity of
///           the gas flow in the flow regime.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidHiFiOrifice::computeGasConductivity(const PolyFluid* fluid0, const PolyFluid* fluid1)
{
    /// - Gamma, adiabatic index, ratio of specific heats.
    const double g = fluid0->getAdiabaticIndex();
    if (g <= 1.0) {
        GUNNS_ERROR(TsOutOfBoundsException, "Value out of bounds",
                    "Adiabatic index for gas is not > 1.");
    }

    /// - Determine predicted mass flux (flow rate per unit area) based on flow regime.
    const double rho0  = fluid0->getDensity();
    double p1          = std::max(fluid1->getPressure(), 0.0);
    double p0          = std::max(fluid0->getPressure(), p1 + mMinLinearizationPotential);
    p1                *= UnitConversion::PA_PER_KPA;
    p0                *= UnitConversion::PA_PER_KPA;
    const double pstar = p0 * powf( (2.0 / (g+1)), (g/(g-1)));
    mPressureRatio     = p1 / pstar;
    double massFlux    = 0.0;
    if (mPressureRatio < 1.0) {                                             // choked gas flow
        massFlux       = mCdActual * computeCriticalGasFlux(g, p0, rho0);
    } else {                                                                // non-choked gas flow
        massFlux       = mCdActual * computeSubCriticalGasFlux(g, p0, rho0, p1);
    }

    /// - Conductivity is the flux per unit pressure, linearized through the pressure/flow origin.
    return (massFlux * UnitConversion::PA_PER_KPA / (p0 - p1));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] g     (--)      Inlet gamma, adiabatic index, ratio of specific heats of the gas.
/// @param[in] p0    (Pa)      Inlet pressure.
/// @param[in] rho0  (kg/m3)   Inlet density.
///
/// @returns  double (kg/s/m2) Ideal mass flow rate per unit area.
///
/// @details  Returns the ideal mass flux for critical (choked) gas flow through an orifice.  It
///           doesn't apply orifice area or Coefficient of Discharge here.  We use the standard
///           orifice flow equation derived from the continuity equation and isentropic relations
///           for an ideal gas.  For an ideal gas, the flux is linear with inlet pressure.
///
/// @note   Argument g must be > 1 to avoid divide-by-zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidHiFiOrifice::computeCriticalGasFlux(const double g,
                                                     const double p0,
                                                     const double rho0) const
{
    return sqrt(g * p0 * rho0 * powf((2/(g+1)), (g+1)/(g-1)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] g     (--)      Inlet gamma, adiabatic index, ratio of specific heats of the gas.
/// @param[in] p0    (Pa)      Inlet pressure.
/// @param[in] rho0  (kg/m3)   Inlet density.
/// @param[in] p1    (Pa)      Exit pressure.
///
/// @returns  double (kg/s/m2) Ideal mass flow rate per unit area.
///
/// @details  Returns the ideal mass flux for sub-critical (non-choked) gas flow through an orifice.
///           It doesn't apply orifice area or Coefficient of Discharge here.  We use the standard
///           orifice flow equation derived from the continuity equation and isentropic relations
///           for an ideal gas.
///
/// @note   Argument g must be > 1 and p0 must be > 0 to avoid divide-by-zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidHiFiOrifice::computeSubCriticalGasFlux(const double g,
                                                        const double p0,
                                                        const double rho0,
                                                        const double p1) const
{
    return sqrt(2 * p0 * rho0 * g/(g-1) * (powf(p1/p0, 2/g) - powf(p1/p0, (g+1)/g)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  fluid0  (--)  Pointer to the inlet node fluid.
/// @param[in]  fluid1  (--)  Pointer to the outlet node fluid.
///
/// @returns  double (kg/s/kPa/m2) Conductivity of liquid flow.
///
/// @details  Returns the linearized mass flow conductivity of the liquid flow.  Sets the flow
///           regime; for now we assume liquid flow to always be non-choked.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidHiFiOrifice::computeLiquidConductivity(const PolyFluid* fluid0,
                                                        const PolyFluid* fluid1) const
{
    const double rho = 0.5 * (fluid0->getDensity() + fluid1->getDensity());
    const double dp  = UnitConversion::PA_PER_KPA * std::max((fluid0->getPressure() - fluid1->getPressure()),
                                                             mMinLinearizationPotential);

    /// - Conductivity is the flux per unit pressure, linearized through the pressure/flow origin.
    /// - Divide by zero is protected by Gunns does not allow minimum linearization potential to be
    ///   equal to or less than zero, so dp can never result in zero.
    return (mCdActual * computeBernoulliFlux(rho, dp) * UnitConversion::PA_PER_KPA / dp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] rho  (kg/m3)  Average density.
/// @param[in] dp   (Pa)     Delta-pressure.
///
/// @returns  double (kg/s/m2) Ideal mass flow rate per unit area.
///
/// @details  Returns the ideal mass flux for incompressible flow through an orifice.  It doesn't
///           apply orifice area or Coefficient of Discharge here.  We use the Bernoulli equation,
///           which assumes laminar, inviscid flow with no body forces and no friction losses.  This
///           also assumes incompressible flow; however using an average of the inlet and outlet
///           densities is a simple correction for compressible flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidHiFiOrifice::computeBernoulliFlux(const double rho,
                                                   const double dp) const
{
    return sqrt(2 * rho * dp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the Admittance Matrix for the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFiOrifice::buildAdmittanceMatrix() 
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
/// @param[in] dt (s) Integration time step (unused).
///
/// @details  Computes the flux from the nodes and the updated the Nodal port directions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFiOrifice::computeFlows(const double dt __attribute__((unused)))
{
    mPotentialDrop = getDeltaPotential();

    /// - Compute molar flow rate.
    computeFlux();

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
/// @param[in] dt (s) Integration time step.
///
/// @details  Computes the flow rate across this link and then transports the flow across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFiOrifice::transportFlows(const double dt)
{
    /// - Convert the molar flow rate to mass rate using the molecular weight of the source node.
    int    sourcePort    = determineSourcePort(mFlux, 0, 1);
    double sourceDensity =         mNodes[sourcePort]->getOutflow()->getDensity();
    mFlowRate            = mFlux * mNodes[sourcePort]->getOutflow()->getMWeight();

    /// - Calculate true volumetric flow rate from the mass flow rate, using the density of the
    ///   source node.
    if (sourceDensity > DBL_EPSILON) {
        mVolFlowRate = mFlowRate / sourceDensity;
    } else {
        mVolFlowRate = 0.0;
    }

    /// - Calculate hydraulic power.
    computePower();

    /// - Isentropic gas expansion cooling across a link.  We only do this for links that define an
    ///   internal fluid.  We store the expanded gas temperature in the internal fluid before giving
    ///   it to the derived model to add its contributions.
    if (mInternalFluid) {

        if (mFlowRate > m100EpsilonLimit) {

            /// - Copy the source node contents into the internal fluid, but only if it isn't the
            ///   network's vacuum node.  If it is, then leave the internal fluid alone and let the
            ///   derived class own it.
            if (0 == sourcePort) {
                mInternalFluid->setState(mNodes[0]->getOutflow());
            }
            mInternalFluid->setTemperature(
                    GunnsFluidUtils::computeIsentropicTemperature(mExpansionScaleFactor,
                                                                  mPotentialVector[0],
                                                                  mPotentialVector[1],
                                                                  mInternalFluid));

        } else if (mFlowRate < -m100EpsilonLimit) {

            if (1 == sourcePort) {
                mInternalFluid->setState(mNodes[1]->getOutflow());
            }
            mInternalFluid->setTemperature(
                    GunnsFluidUtils::computeIsentropicTemperature(mExpansionScaleFactor,
                                                                  mPotentialVector[1],
                                                                  mPotentialVector[0],
                                                                  mInternalFluid));
        }

        /// - If the derived model has declared that it modifies the fluid passing through it (by
        ///   instantiating mInternalFluid), then call the derived model to do the update.
        updateFluid(dt, mFlowRate);
    }

    /// - Call the base class to transport the fluid between the nodes.  The false bool argument
    ///   indicates that this class cannot force a mixture change in the supplying node.
    transportFluid(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the molar flux rate through the link.  Units:
///           Flux (kgmol/s) = Admittance (kgmol/kPa/s) * dP (kPa)
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFiOrifice::computeFlux()
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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (m2)  The effective throat area of the device.
///
/// @details  Getter method that returns the effective throat area including variable-position
///           devices (valves) and blockage and leak malfunctions.  For this fixed-area Orifice, it
///           only includes the fixed throat area and the blockage malfunction.  This is intended to
///           be overridden by variable-position devices (valves).
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidHiFiOrifice::getEffectiveArea() const
{
    if (mMalfBlockageFlag) {
        return mThroatArea * (1.0 - mMalfBlockageValue);
    } else {
        return mThroatArea;
    }
}
