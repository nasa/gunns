/**
@file
@brief    GUNNS Fluid Balloon Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  (
   (aspects/fluid/capacitor/GunnsFluidTank.o)
   (core/GunnsFluidUtils.o)
  )
*/

#include "GunnsFluidBalloon.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "core/GunnsFluidUtils.hh"

/// @details A minimum node volume is needed for stability when gas is first flowed into an empty
///          balloon and inflation begins.
const double GunnsFluidBalloon::mMinVolume = 1.0e-12;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                 (--)       Link name.
/// @param[in] nodes                (--)       Network nodes array.
/// @param[in] expansionScaleFactor (--)       Scale factor for isentropic gas cooling (0-1).
/// @param[in] dpdtFilterGain       (--)       Gain for pressure rate filter.
/// @param[in] thermalDampingMass   (kg)       Thermal damping mass.
/// @param[in] editFluxTarget       (kg*mol/s) Target threshold for edit completion.
/// @param[in] surfaceArea          (m2)       Inner surface area of balloon wall.
/// @param[in] shellRadius          (m)        Distance from balloon wall to center.
/// @param[in] inflatability        (m3/kPa)   Inflatability of balloon.
/// @param[in] maxVolume            (m3)       Maximum balloon volume, at which wall becomes rigid.
///
/// @details  Default GUNNS Fluid Balloon Config Data Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalloonConfigData::GunnsFluidBalloonConfigData(const std::string& name,
                                                         GunnsNodeList*     nodes,
                                                         const double       expansionScaleFactor,
                                                         const double       dpdtFilterGain,
                                                         const double       thermalDampingMass,
                                                         const double       editFluxTarget,
                                                         const float        surfaceArea,
                                                         const float        shellRadius,
                                                         const double       inflatability,
                                                         const double       maxVolume)
    :
    GunnsFluidTankConfigData(name,
                             nodes,
                             expansionScaleFactor,
                             dpdtFilterGain,
                             thermalDampingMass,
                             editFluxTarget,
                             surfaceArea,
                             shellRadius),
    mInflatability(inflatability),
    mMaxVolume(maxVolume)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy.
///
/// @details  GUNNS Fluid Balloon Config Data Copy Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalloonConfigData::GunnsFluidBalloonConfigData(
        const GunnsFluidBalloonConfigData& that)
    :
    GunnsFluidTankConfigData(that),
    mInflatability(that.mInflatability),
    mMaxVolume(that.mMaxVolume)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Balloon Config Data Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalloonConfigData::~GunnsFluidBalloonConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfStuckFlag               (--)  Initial activation state of the stuck wall malfunction.
/// @param[in] malfInflatabilityScaleFlag  (--)  Initial activation state of the inflatability scale malfunction.
/// @param[in] malfInflatabilityScaleValue (--)  Initial value of the inflatability scale malfunction.
/// @param[in] initialFluidState           (--)  Initial state of the internal ballon (port 0 node) contents.
/// @param[in] initialShellTemperature     (K)   Initial temperature of the balloon wall.
/// @param[in] biasHeatFlux                (J/s) User bias heat flux added to fluid.
///
/// @details  Default GUNNS Fluid Balloon Input Data Constructor.  The base class blockage malf and
///           initial volume aren't used so we send zero values to its constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalloonInputData::GunnsFluidBalloonInputData(const bool          malfStuckFlag,
                                                       const bool          malfInflatabilityScaleFlag,
                                                       const double        malfInflatabilityScaleValue,
                                                       PolyFluidInputData* initialFluidState,
                                                       const float         initialShellTemperature,
                                                       const float         biasHeatFlux)
    :
    GunnsFluidTankInputData(false,
                            0.0,
                            0.0,
                            initialFluidState,
                            initialShellTemperature,
                            biasHeatFlux),
    mMalfStuckFlag(malfStuckFlag),
    mMalfInflatabilityScaleFlag(malfInflatabilityScaleFlag),
    mMalfInflatabilityScaleValue(malfInflatabilityScaleValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy.
///
/// @details  GUNNS Fluid Balloon Input Data Copy Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalloonInputData::GunnsFluidBalloonInputData(const GunnsFluidBalloonInputData& that)
    :
    GunnsFluidTankInputData(that),
    mMalfStuckFlag(that.mMalfStuckFlag),
    mMalfInflatabilityScaleFlag(that.mMalfInflatabilityScaleFlag),
    mMalfInflatabilityScaleValue(that.mMalfInflatabilityScaleValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Balloon Input Data Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalloonInputData::~GunnsFluidBalloonInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Balloon Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalloon::GunnsFluidBalloon():
    GunnsFluidTank(),
    mMalfStuckFlag(false),
    mMalfInflatabilityScaleFlag(false),
    mMalfInflatabilityScaleValue(0.0),
    mInflatability(0.0),
    mMaxVolume(0.0),
    mInflation(0.0),
    mInflationState(DEFLATED),
    mActualInflatability(0.0),
    mPressureCorrection(0.0),
    mPressureCorrectionGain(0.0),
    mInflatableCapacitance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Balloon Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidBalloon::~GunnsFluidBalloon()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) GUNNS Fluid Balloon Config Data.
/// @param[in]     inputData    (--) GUNNS Fluid Balloon Input Data.
/// @param[in,out] networkLinks (--) Network links.
/// @param[in]     port0        (--) Network port 0.
/// @param[in]     port1        (--) Network port 1.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Balloon with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalloon::initialize(const GunnsFluidBalloonConfigData& configData,
                                   const GunnsFluidBalloonInputData&  inputData,
                                   std::vector<GunnsBasicLink*>&      networkLinks,
                                   const int                          port0,
                                   const int                          port1)
{
    /// - Initialize & validate parent.
    GunnsFluidTank::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag   = false;

    /// - Validate initialization data.
    validate(configData);

    /// - Initialize from config and input data.
    mInflatability               = configData.mInflatability;
    mMaxVolume                   = configData.mMaxVolume;
    mMalfStuckFlag               = inputData.mMalfStuckFlag;
    mMalfInflatabilityScaleFlag  = inputData.mMalfInflatabilityScaleFlag;
    mMalfInflatabilityScaleValue = inputData.mMalfInflatabilityScaleValue;

    /// - Initialize state attributes.  If port 1 is Vacuum then we can't use its pressure because
    ///   it hasn't been zeroed yet.  So we just set port 1 ourselves.
    /// - Note that if this link initializes before links that override the initial Port 1 node
    ///   volume pressure, then this is initializing to the wrong volume & pressure, and the balloon
    ///   volume can jump when you go to run as it adjusts to the new actual external pressure. This
    ///   can be avoided by ensuring this link initializes after links on the external node.
    if (port1 == getGroundNodeIndex()) {
        mPotentialVector[1] = 0.0;
    } else {
        mPotentialVector[1] = mNodes[1]->getContent()->getPressure();
    }
    updateInflatability();
    mNodes[0]->initVolume(updateInflation());
    mPressureCorrection     = 0.0;
    mPressureCorrectionGain = 0.0;
    mInflatableCapacitance  = 0.0;

    /// - Set init flag on successful initialization.
    mInitFlag   = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) GUNNS Fluid Balloon Config Data.
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Balloon initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalloon::validate(const GunnsFluidBalloonConfigData& configData) const
{
    /// - Issue an error on inflatability < FLT_EPISLON.
    if (configData.mInflatability < FLT_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Link has inflatability < FLT_EPSILON.");
    }

    /// - Issue an error on volume < minimum.
    if (configData.mMaxVolume < (mMinVolume + FLT_EPSILON)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Link has maximum volume < minimum allowed.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalloon::restartModel()
{
    /// - Reset the base class.
    GunnsFluidTank::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.  Since the fluid node doesn't
    ///   checkpoint its volume, we have to re-initialize it from our checkpointed inflation value.
    updateInflatability();
    mNodes[0]->initVolume(updateInflation());
    mPressureCorrection     = 0.0;
    mPressureCorrectionGain = 0.0;
    mInflatableCapacitance  = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details Method to update the link during a time step. For a fluid capacitor, like a Tank or
///          any volume, we define the actual capacitive node to be Nodes[0], and Nodes[1] is set
///          as a zero potential node.  Make sure to always map Nodes[0] as the volume object node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalloon::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.  Note that you should zero out the
    ///   volume before removing a capacitive link from a node.
    processUserPortCommand();

    updateInflatability();
    processVolumeEdit();

    /// - Call the virtual updateState method for the tank edit stuff.
    mNodes[0]->setExpansionScaleFactor(mExpansionScaleFactor);
    updateState(dt);
    mCapacitance = GunnsFluidUtils::computeCapacitance(mNodes[0]->getContent(), getVolume());

    /// - Set the new capacitance based on the volume.
    if (mNodes[0]->getVolume() == mMaxVolume or mMalfStuckFlag) {
        mInflatableCapacitance = 0.0;
    } else {
        mInflatableCapacitance = mActualInflatability * mNodes[0]->getContent()->getDensity()
                                                      / mNodes[0]->getContent()->getMWeight();
        mCapacitance += mInflatableCapacitance;
        correctPpRateEdits(dt);
    }

    /// - Apply pressure correction for fluid state error.
    if (!mDisablePressureCorrection) {
        mPressureCorrection = computePressureCorrection();
    } else {
        mPressureCorrection = 0.0;
    }
    mPotentialVector[0] += mPressureCorrection;

    /// - Build this link's contributions to the system of equations.
    buildAdmittanceMatrix(dt);
    buildSourceVector(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details A lower bound on actual inflatability is applied in run-time since it is used as a
///          divisor elsewhere.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalloon::updateInflatability()
{
    if (mMalfInflatabilityScaleFlag) {
        mActualInflatability = std::max(mInflatability * mMalfInflatabilityScaleValue, DBL_EPSILON);
    } else {
        mActualInflatability = std::max(mInflatability, DBL_EPSILON);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt       (s)    Integration time step.
/// @param[in] flowrate (kg/s) Link mass flow rate.
///
/// @details Updates the node volume & balloon inflation terms based on new pressure solution, and
///          calls the tank update for fluid edits.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalloon::updateFluid(const double dt, const double flowrate)
{
    mNodes[0]->setVolume(updateInflation());
    GunnsFluidTank::updateFluid(dt, flowrate);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double  (m3)  Internal balloon volume with limits applied.
///
/// @details Computes & returns the new balloon internal volume based on the balloon inflatability
///          and current pressures, and applies min & max volume limits.  Updates the inflation
///          states.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidBalloon::updateInflation()
{
    double volume = 0.0;
    if (mMalfStuckFlag) {
        volume     = mInflation * (mMaxVolume - mMinVolume) + mMinVolume;
    } else {
        volume     = computeVolumeFromPressure(mPotentialVector[0]);
        mInflation = (volume - mMinVolume) / (mMaxVolume - mMinVolume);
    }
    if (mMinVolume == volume) {
        mInflationState = DEFLATED;
    } else if (mMaxVolume == volume) {
        mInflationState = INFLATED;
    } else if (volume > mNodes[0]->getVolume() + FLT_EPSILON) {
        mInflationState = INFLATING;
    } else if (volume < mNodes[0]->getVolume() - FLT_EPSILON) {
        mInflationState = DEFLATING;
    } else {
        mInflationState = PARTIALLY_INFLATED;
    }
    return volume;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Process a volume edit and synchronizes with pressure edits, since pressure and volume
///          are coupled.  A volume edit activates a simultaneous coordinated pressure edit, and
///          vice-versa.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalloon::processVolumeEdit()
{
    if (mEditTemperaturePressureFlag or mEditTemperaturePartialPressureFlag) {
        mEditVolumeFlag = true;
        mEditVolume     = computeVolumeFromPressure(computeEditTargetPressure());

    } else if (mEditVolumeFlag) {
        mEditTemperaturePressureFlag = true;
        mEditTemperatureValue        = mNodes[0]->getContent()->getTemperature();
        mEditPressureValue           = MsMath::limitRange(mMinVolume, mEditVolume, mMaxVolume)
                                     / mActualInflatability + mPotentialVector[1];
    }

    if (mEditVolumeFlag) {
        mEditVolumeFlag = false;
        double volume = mEditVolume;
        if (volume < mMinVolume) {
            volume = mMinVolume;
            GUNNS_WARNING("actual edit volume was limited to minimum allowed.");
        } else if (volume > mMaxVolume) {
            volume = mMaxVolume;
            GUNNS_WARNING("actual edit volume was limited to maximum allowed.");
        }
        mNodes[0]->initVolume(volume);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details At this point, mInternalFluid and mFlowRate contain the gas flows for partial pressure
///          rate edits from GunnsFluidTank, assuming constant volume.  Once we know the inflatable
///          capacitance, we must adjust mInternalFluid and mFlowRate to account for the changing
///          volume.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalloon::correctPpRateEdits(const double dt)
{
    /// - If a partial pressure rate edit is in progress...
    if (fabs(mFlowRate) > DBL_EPSILON) {

        /// - Predict the change in total pressure due to PP rate edits this pass.  Then
        ///   inflatability combined with change in pressure gives expected change in volume.
        const double delMoles    = -dt * mFlowRate / mInternalFluid->getMWeight();
        const double delPressure = delMoles / mInflatableCapacitance;
        const double delVolume   = delPressure * mActualInflatability;
        const double temperature = mNodes[0]->getContent()->getTemperature();

        /// - For each gas constituent, compute change in mass to keep constant density in the
        ///   changing volume, then add this mass to the edit flow.
        for (int i = 0; i < mNConstituents; ++i) {
            MonoFluid* constituent = mInternalFluid->getConstituent(mInternalFluid->getType(i));
            if (FluidProperties::GAS == constituent->getPhase()) {
                const double delMass = constituent->computeDensity(temperature, mPartialPressure[i])
                                     * delVolume;
                mFlowRate           -= delMass / dt;
                constituent->setMass(delMass + constituent->getMass());
            }
        }
        mInternalFluid->updateMass();
        mInternalFluid->setTemperature(temperature);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double (kPa) Adjusted node pressure correction for state error.
///
/// @details Computes and returns an adjusted node pressure correction for state error to account
///          for the volume change this correction will cause due to the balloon inflatability.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidBalloon::computePressureCorrection()
{
    mPressureCorrectionGain = 1.0;
    const double pc = mNodes[0]->computePressureCorrection();

    /// - At the volume limits or when stuck, the balloon goes rigid and we can use the node's
    ///   normal correction.
    if (mInflation > 0.0 and mInflation < 1.0 and not mMalfStuckFlag) {

        /// - Pressure correction affects the inflatable volume which then feeds back into the next
        ///   pressure correction -- this feedback loop can be unstable.  The higher the
        ///   inflatability, the more a given pressure correction will change the volume, thus more
        ///   instability.
        ///
        ///   First we assume ideal gas.  The node's pressure correction pc has been calculated as:
        ///       pc = mRT/v0 - p0,
        ///   where v0 & p0 are last node 0 volume & pressure.  We assume mass m and temperature T
        ///   are constant.  If we adjust the pressure correction with a factor g as g * pc, then
        ///   the new node volume and pressure after the correction is applied will be:
        ///       v1 = v0 + g * pc * k
        ///       p1 = p0 + g * pc
        ///   where k is the balloon inflatability (m3/kPa) and g is our adjustment factor.  Then
        ///   the next node pressure correction pc1, which we want to be 0, will be:
        ///       pc1 = mRT/v1 - p1 = 0
        ///       p1  = mRT/v1
        ///   Substitute v1 & p1, results in a quadratic for g:
        ///       p0 + g*pc = mRT / (v0 + g*pc*k)
        ///       (p0 + g*pc) * (v0 + g*pc*k) = mRT
        ///       p0*v0 + p0*g*pc*k + g*pc*v0 + g*pc*g*pc*k = mRT
        ///       (k*pc*pc)*g*g + (k*pc*p0 + pc*v0)*g + (p0*v0 - mRT) = 0
        ///   We solve for g using the quadratic equation, with coefficients:
        ///       a = k*pc^2
        ///       b = pc*(k*p0 + v0)
        ///       c = v0*p0 - mRT
        const double a = mActualInflatability * pc * pc;
        const double b = pc * (mActualInflatability * mPotentialVector[0] + mNodes[0]->getVolume());
        const double c = mNodes[0]->getVolume() * mPotentialVector[0]
                       - mNodes[0]->getContent()->getTemperature()
                       * mNodes[0]->getContent()->getMass() * UnitConversion::UNIV_GAS_CONST_SI
                       / mNodes[0]->getContent()->getMWeight();
        const double bb4ac = b*b - 4.0*a*c;
        if (fabs(a) > 0.0 and bb4ac > 0.0) {
            const double solnp = (-b + sqrt(bb4ac)) * 0.5 / a;
            const double solnm = (-b - sqrt(bb4ac)) * 0.5 / a;
            if (fabs(solnp) < fabs(solnm)) {
                mPressureCorrectionGain = solnp;
            } else {
                mPressureCorrectionGain = solnm;
            }
        } else {
            mPressureCorrectionGain = 0.0;
        }
    }
    return mPressureCorrectionGain * pc;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  This method builds the link's molar capacitance, thermal capacitance, and fluid edit
///           flow source effects into the link source vector.  The compression effect from
///           GunnsFluidCapacitor is not included because we've already accounted for it.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidBalloon::buildSourceVector(const double dt)
{
    /// - Add the molar capacitance and the tank edit flow contribution.
    mSourceVector[0] = mPotentialVector[0] * mAdmittanceMatrix[0]
                     - mFlowRate / mInternalFluid->getMWeight();

    /// - Add the thermal capacitance contribution.
    if (dt > DBL_EPSILON) {
        mSourceVector[0] += mNodes[0]->computeThermalCapacitance() / dt;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned.
/// @param[in] node (--) The desired node to assign the port to.
///
/// @returns  bool  (--) Returns true if all rules checks pass.
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - A GunnsFluidBalloon must map port 0 to a non-Ground gas-phase node.
///
///           This overrides GunnsFluidCapacitor to allow connecting port 1 to a non-Ground node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidBalloon::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 0 is Ground or is not in gas phase.
    GunnsFluidNode* nodePtr = dynamic_cast<GunnsFluidNode*> (mNodeList->mNodes);
    if ( (0 == port) && ((FluidProperties::GAS != nodePtr[node].getContent()->getPhase()) or
                         (node == getGroundNodeIndex())) ) {
        GUNNS_WARNING("aborted setting a port: must assign port 0 to a non-Ground gas node.");
        result = false;
    }

    return result;
}
