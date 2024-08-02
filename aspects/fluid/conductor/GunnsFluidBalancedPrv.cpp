/**
@file
@brief    GUNNS Fluid Balanced Pressure Reducing Valve Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
     (core/GunnsFluidLink.o)
    )
*/

#include "GunnsFluidBalancedPrv.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "math/MsMath.hh"
#include <cfloat>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name              (--)       Link name.
/// @param[in] nodes             (--)       Network nodes array.
/// @param[in] exitPressureDroop (kPa*s/kg) Exit pressure droop per unit exit mass flow.
///
/// @details  Default constructs this Balanced Pressure Reducing Valve configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalancedPrvConfigData::GunnsFluidBalancedPrvConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const double       exitPressureDroop)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mExitPressureDroop(exitPressureDroop)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy.
///
/// @details  Copy constructs this Balanced Pressure Reducing Valve configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalancedPrvConfigData::GunnsFluidBalancedPrvConfigData(
        const GunnsFluidBalancedPrvConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mExitPressureDroop(that.mExitPressureDroop)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Balanced Pressure Reducing Valve configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalancedPrvConfigData::~GunnsFluidBalancedPrvConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag      (--)  Initial activation flag for blockage malfunction.
/// @param[in] malfBlockageValue     (--)  Initial fraction value (0-1) for Blockage malfunction.
/// @param[in] malfPressureBiasFlag  (--)  Initial activation flag for pressure setpoint bias malfunction.
/// @param[in] malfPressureBiasValue (kPa) Initial value for pressure setpoint bias malfunction.
/// @param[in] pressureSetpoint      (kPa) Initial regulated pressure setpoint.
///
/// @details  Default constructs this Balanced Pressure Reducing Valve input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalancedPrvInputData::GunnsFluidBalancedPrvInputData(const bool   malfBlockageFlag,
                                                               const double malfBlockageValue,
                                                               const bool   malfPressureBiasFlag,
                                                               const double malfPressureBiasValue,
                                                               const double pressureSetpoint)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue),
    mMalfPressureBiasFlag(malfPressureBiasFlag),
    mMalfPressureBiasValue(malfPressureBiasValue),
    mPressureSetpoint(pressureSetpoint)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy.
///
/// @details  Copy constructs this Balanced Pressure Reducing Valve input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalancedPrvInputData::GunnsFluidBalancedPrvInputData(const GunnsFluidBalancedPrvInputData& that)
    :
    GunnsFluidLinkInputData(that),
    mMalfPressureBiasFlag(that.mMalfPressureBiasFlag),
    mMalfPressureBiasValue(that.mMalfPressureBiasValue),
    mPressureSetpoint(that.mPressureSetpoint)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Balanced Pressure Reducing Valve input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalancedPrvInputData::~GunnsFluidBalancedPrvInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Balanced Pressure Reducing Valve.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalancedPrv::GunnsFluidBalancedPrv()
    :
    GunnsFluidLink(NPORTS),
    mMalfPressureBiasFlag(false),
    mMalfPressureBiasValue(0.0),
    mExitPressureDroop(0.0),
    mPressureSetpoint(0.0),
    mInletConductance(0.0),
    mExitConductance(0.0),
    mInletMassBuffer(0.0),
    mRegulatedPressure(0.0),
    mInletFlux(0.0),
    mInletFlowRate(0.0),
    mBackPressureCutoff(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Balanced Pressure Reducing Valve.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalancedPrv::~GunnsFluidBalancedPrv()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Fluid Balanced Pressure Reducing Valve Config Data.
/// @param[in]     inputData    (--) Fluid Balanced Pressure Reducing Valve Input Data.
/// @param[in,out] networkLinks (--) Network links.
/// @param[in]     port0        (--) Inlet node.
/// @param[in]     port1        (--) Outlet node.
/// @param[in]     port2        (--) Reference pressure node.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Balanced Pressure Reducing Valve with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalancedPrv::initialize(const GunnsFluidBalancedPrvConfigData& configData,
                                       const GunnsFluidBalancedPrvInputData&  inputData,
                                       std::vector<GunnsBasicLink*>&          networkLinks,
                                       const int                              port0,
                                       const int                              port1,
                                       const int                              port2)
{
    /// - Initialize the parent class.
    int ports[NPORTS] = {port0, port1, port2};
    GunnsFluidLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize class attributes.
    mExitPressureDroop     = configData.mExitPressureDroop;
    mMalfPressureBiasFlag  = inputData.mMalfPressureBiasFlag;
    mMalfPressureBiasValue = inputData.mMalfPressureBiasValue;
    mPressureSetpoint      = inputData.mPressureSetpoint;
    mInletConductance      = 0.0;
    mExitConductance       = 0.0;
    mInletMassBuffer       = 0.0;
    mRegulatedPressure     = 0.0;
    mInletFlux             = 0.0;
    mInletFlowRate         = 0.0;
    mBackPressureCutoff    = false;

    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this Balanced Pressure Reducing Valve initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalancedPrv::validate() const
{
    /// - Issue an error on exit droop < 0.
    if (mExitPressureDroop < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Link has exit pressure droop < 0.");
    }

    /// - Issue an error on pressure setpoint < 0.
    if (mPressureSetpoint < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Link has pressure setpoint < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalancedPrv::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mInletConductance      = 0.0;
    mExitConductance       = 0.0;
    mRegulatedPressure     = 0.0;
    mInletFlux             = 0.0;
    mInletFlowRate         = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  Updates this link's contributions to the network system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalancedPrv::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Cut-off to prevent backflow.
    if (mPotentialVector[1] > mPotentialVector[0]) {
        mBackPressureCutoff = true;
    } else if (mPotentialVector[1] < mPressureSetpoint) {
        mBackPressureCutoff = false;
    }

    /// - Find the actual pressure being regulated to.
    mRegulatedPressure = mPressureSetpoint + mPotentialVector[2];
    if (mMalfPressureBiasFlag) {
        mRegulatedPressure += mMalfPressureBiasValue;
    }
    mRegulatedPressure = fmin(mRegulatedPressure, mPotentialVector[0]);

    double flt_epsilon = static_cast<double>(FLT_EPSILON);
    /// - Update exit-side molar conductance.
    if (mBackPressureCutoff or mNodes[0]->getOutflow()->getMWeight() < flt_epsilon) {
        mExitConductance = 0.0;
    } else {
        mExitConductance = 1.0 / mNodes[0]->getOutflow()->getMWeight()
                         / MsMath::limitRange(flt_epsilon, mExitPressureDroop, 1.0 / flt_epsilon);
        if (mMalfBlockageFlag) {
            mExitConductance *= (1.0 - mMalfBlockageValue);
        }
    }

    /// - Inlet-side molar conductance is set to drain the mass buffer in approximately 5 frames
    ///   regardless of time-step.
    if (dt < DBL_EPSILON or mNodes[0]->getOutflow()->getMWeight() < flt_epsilon or
            mPotentialVector[0] < DBL_EPSILON) {
        mInletConductance = 0.0;
    } else {
        const double inletMdot = 0.2 * mInletMassBuffer / dt;
        const double inletFlux = inletMdot / mNodes[0]->getOutflow()->getMWeight();
        mInletConductance = inletFlux / mPotentialVector[0];
    }
    mInletConductance = MsMath::limitRange(0.0, mInletConductance, mConductanceLimit);

    /// - Build link's contributions to the network.
    buildAdmittanceMatrix();
    buildSourceVector();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the link's admittance matrix contributions to the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalancedPrv::buildAdmittanceMatrix()
{
    if ( (mAdmittanceMatrix[0] != mInletConductance) or
         (mAdmittanceMatrix[4] != mExitConductance) ) {
        mAdmittanceMatrix[0]  = mInletConductance;
        mAdmittanceMatrix[1]  = 0.0;
        mAdmittanceMatrix[2]  = 0.0;
        mAdmittanceMatrix[3]  = 0.0;
        mAdmittanceMatrix[4]  = mExitConductance;
        mAdmittanceMatrix[5]  = 0.0;
        mAdmittanceMatrix[6]  = 0.0;
        mAdmittanceMatrix[7]  = 0.0;
        mAdmittanceMatrix[8]  = 0.0;
        mAdmittanceUpdate     = true;
    } else {
        mAdmittanceUpdate     = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the link's source vector contributions to the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalancedPrv::buildSourceVector()
{
    mSourceVector[0] = 0.0;
    mSourceVector[1] = mRegulatedPressure * mExitConductance;
    mSourceVector[2] = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step (unused).
///
/// @details  Computes the flow rates across this link and transports fluid flows between nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalancedPrv::computeFlows(const double dt __attribute__((unused)))
{
    mPotentialDrop = getDeltaPotential();

    /// - Compute molar flow rates.  Only forward (positive) flow is allowed.  In the event of the
    ///   network solution requiring backflow, this link will not transport it and this will create
    ///   state error in the affected node.  This should only happen for one frame at a backflow
    ///   cut-off event.
    mInletFlux = std::max(0.0,  mPotentialVector[0] * mAdmittanceMatrix[0]);
    mFlux      = std::max(0.0, -mPotentialVector[1] * mAdmittanceMatrix[4] + mSourceVector[1]);

    if (mInletFlux > DBL_EPSILON) {
        mPortDirections[0] = SOURCE;
        mPortDirections[1] = SINK;
        mNodes[0]->scheduleOutflux(mInletFlux);
    } else {
        mPortDirections[0] = NONE;
        mPortDirections[1] = NONE;
    }
    mPortDirections[2] = NONE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details   Updates final flow properties and transports fluid from source to sink nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalancedPrv::transportFlows(const double dt)
{
    /// - Convert the molar flow rates to mass rates using the molecular weight of the upstream
    ///   node.
    mInletFlowRate = mNodes[0]->getOutflow()->getMWeight() * mInletFlux;
    mFlowRate      = mNodes[0]->getOutflow()->getMWeight() * mFlux;

    /// - Calculate true volumetric flow rate from the mass flow rate, using the density of the
    ///   upstream node.
    const double sourceDensity = mNodes[0]->getOutflow()->getDensity();
    if (sourceDensity > DBL_EPSILON) {
        mVolFlowRate = mFlowRate / sourceDensity;
    } else {
        mVolFlowRate = 0.0;
    }

    /// - Calculate hydraulic power rise from inlet to exit port (should always be negative).
    mPower = -UnitConversion::PA_PER_KPA * mVolFlowRate * getDeltaPotential();

    /// - Transport flow between the nodes.
    if (mInletFlowRate > m100EpsilonLimit) {
        mNodes[0]->collectOutflux(mInletFlowRate);
    }
    if (mFlowRate > m100EpsilonLimit) {
        mNodes[1]->collectInflux(mFlowRate, mNodes[0]->getOutflow());
    }

    /// - Update inlet mass buffer with flows.
    mInletMassBuffer += (mFlowRate - mInletFlowRate) * dt;
    if (mInletMassBuffer < DBL_EPSILON) {
        mInletMassBuffer = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned.
/// @param[in] node (--) The desired node to assign the port to.
///
/// @return   bool  (--) Returns true if all rules checks pass.
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - A GunnsFluidBalancedPrv cannot map port 0 to the Ground node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidBalancedPrv::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 0 is the Ground node.
    if ( (0 == port) and (node == getGroundNodeIndex()) ) {
        GUNNS_WARNING("aborted setting a port: cannot assign port 0 to Ground.");
        result = false;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--)  Malfunction activation flag, true activates.
/// @param[in] value (kPa) Bias value to add to the pressure setpoint.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalancedPrv::setMalfPressureBias(const bool flag, const double value)
{
    mMalfPressureBiasFlag  = flag;
    mMalfPressureBiasValue = value;
}
