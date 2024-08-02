/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Classes for the Editable Thermal Fluid Volume Capacitor/Source Model.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
    ((core/GunnsFluidCapacitor.o))

PROGRAMMERS:
   ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-09))
**************************************************************************************************/

#include <cmath>

#include "software/exceptions/TsInitializationException.hh"

#include "GunnsFluidTank.hh"
#include "core/GunnsFluidUtils.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    name                 (--)   Name of object
/// @param[in]    nodes                (--)   Pointer to nodes
/// @param[in]    expansionScaleFactor (--)   Scale factor for isentropic gas cooling
/// @param[in]    dpdtFilterGain       (--)   Gain for pressure rate filter
/// @param[in]    thermalDampingMass   (kg)   Thermal damping mass
/// @param[in]    editFluxTarget       (kg*mol/s)    Target threshold for edit completion
/// @param[in]    surfaceArea          (m2)   Inner surface area of tank shell
/// @param[in]    shellRadius          (m)    Distance from tank shell to center
///
/// @details  Default constructs this GUNNS Fluid Tank link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTankConfigData::GunnsFluidTankConfigData(const std::string& name,
                                                   GunnsNodeList*     nodes,
                                                   const double       expansionScaleFactor,
                                                   const double       dpdtFilterGain,
                                                   const double       thermalDampingMass,
                                                   const double       editFluxTarget,
                                                   const float        surfaceArea,
                                                   const float        shellRadius)
    :
    GunnsFluidCapacitorConfigData(name,  nodes, expansionScaleFactor),
    mDpdtFilterGain              (dpdtFilterGain),
    mThermalDampingMass          (thermalDampingMass),
    mEditFluxTarget              (editFluxTarget),
    mSurfaceArea                 (surfaceArea),
    mShellRadius                 (shellRadius)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that   --  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Tank link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTankConfigData::GunnsFluidTankConfigData(const GunnsFluidTankConfigData& that)
    :
    GunnsFluidCapacitorConfigData(that),
    mDpdtFilterGain              (that.mDpdtFilterGain),
    mThermalDampingMass          (that.mThermalDampingMass),
    mEditFluxTarget              (that.mEditFluxTarget),
    mSurfaceArea                 (that.mSurfaceArea),
    mShellRadius                 (that.mShellRadius)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Tank link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTankConfigData::~GunnsFluidTankConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    malfBlockageFlag        (--)  Blockage malfunction flag
/// @param[in]    malfBlockageValue       (--)  Blockage malfunction fractional value (0-1)
/// @param[in]    volume                  (m3)  Volume of the link
/// @param[in]    fluidState              (--)  State of the port 0 node contents
/// @param[in]    initialShellTemperature (K)   Initial temperature of the tank shell
/// @param[in]    biasHeatFlux;           (J/s) User bias heat flux added to fluid
///
/// @details  Default constructs this GUNNS Fluid Tank link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTankInputData::GunnsFluidTankInputData(const bool          malfBlockageFlag,
                                                 const double        malfBlockageValue,
                                                 const double        volume,
                                                 PolyFluidInputData* fluidState,
                                                 const float         initialShellTemperature,
                                                 const float         biasHeatFlux)
    :
    GunnsFluidCapacitorInputData(malfBlockageFlag, malfBlockageValue, volume, fluidState),
    mShellTemperature(initialShellTemperature),
    mBiasHeatFlux(biasHeatFlux)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that   --  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Tank link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTankInputData::GunnsFluidTankInputData(const GunnsFluidTankInputData& that)
    :
    GunnsFluidCapacitorInputData(that),
    mShellTemperature           (that.mShellTemperature),
    mBiasHeatFlux               (that.mBiasHeatFlux)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Tank link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTankInputData::~GunnsFluidTankInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Tank link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTank::GunnsFluidTank()
    :
    GunnsFluidCapacitor(),
    mNConstituents(0),
    mEditTemperatureFlag(false),
    mEditTemperaturePressureFlag(false),
    mEditTemperaturePartialPressureFlag(false),
    mEditPartialPressureRateFlag(0),
    mEditTemperatureValue(0.0),
    mEditPressureValue(0.0),
    mEditPartialPressureValue(0),
    mEditPartialPressureRateValue(0),
    mEditFluxTarget(0.0),
    mBiasHeatFlux(0.0),
    mHeatFluxFromShell(0.0),
    mHeatFluxToShell(0.0),
    mTemperature(0.0),
    mShellTemperature(0.0),
    mSurfaceArea(0.0),
    mShellRadius(0.0),
    mPreviousPressure(0.0),
    mDpdt(0.0),
    mDpdtFilterGain(0.0),
    mPartialPressure(0),
    mMassFraction(0),
    mMoleFraction(0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Tank link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTank::~GunnsFluidTank()
{
    TS_DELETE_ARRAY(mMoleFraction);
    TS_DELETE_ARRAY(mMassFraction);
    TS_DELETE_ARRAY(mPartialPressure);
    TS_DELETE_ARRAY(mEditPartialPressureRateValue);
    TS_DELETE_ARRAY(mEditPartialPressureValue);
    TS_DELETE_ARRAY(mEditPartialPressureRateFlag);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data
/// @param[in]      inputData    (--)  Input data
/// @param[in,out]  links        (--)  Link vector
/// @param[in]      port0        (--)  Nominal inlet port map index
/// @param[in]      port1        (--)  Nominal outlet port map index
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Tank link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTank::initialize(const GunnsFluidTankConfigData& configData,
                                const GunnsFluidTankInputData&  inputData,
                                std::vector<GunnsBasicLink*>&   links,
                                const int                       port0,
                                const int                       port1)
{
    /// - First initialize & validate parent.
    GunnsFluidCapacitor::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Validate the config & input data.
    validate(configData, inputData);

    /// - Allocate arrays for the number of constituents in the contained fluid.
    mNConstituents = mNodes[0]->getFluidConfig()->mNTypes;
    TS_NEW_PRIM_ARRAY_EXT(mEditPartialPressureRateFlag,  mNConstituents, bool,   std::string(mName) + ".mEditPartialPressureRateFlag");
    TS_NEW_PRIM_ARRAY_EXT(mEditPartialPressureValue,     mNConstituents, double, std::string(mName) + ".mEditPartialPressureValue");
    TS_NEW_PRIM_ARRAY_EXT(mEditPartialPressureRateValue, mNConstituents, double, std::string(mName) + ".mEditPartialPressureRateValue");
    TS_NEW_PRIM_ARRAY_EXT(mPartialPressure,              mNConstituents, double, std::string(mName) + ".mPartialPressure");
    TS_NEW_PRIM_ARRAY_EXT(mMassFraction,                 mNConstituents, double, std::string(mName) + ".mMassFraction");
    TS_NEW_PRIM_ARRAY_EXT(mMoleFraction,                 mNConstituents, double, std::string(mName) + ".mMoleFraction");

    for (int i = 0; i < mNConstituents; ++i) {
        mEditPartialPressureRateFlag[i]  = false;
        mEditPartialPressureValue[i]     = 0.0;
        mEditPartialPressureRateValue[i] = 0.0;
        mPartialPressure[i]              = 0.0;
        mMassFraction[i]                 = 0.0;
        mMoleFraction[i]                 = 0.0;
    }

    /// - Create an internal fluid for partial pressure rate edits.
    GunnsFluidLink::createInternalFluid();

    /// - Node initialization.
    mNodes[0]->setThermalDampingMass(configData.mThermalDampingMass);

    /// - Initialize class attributes.
    mHeatFluxFromShell = 0.0;
    mHeatFluxToShell   = 0.0;
    mTemperature       = mNodes[0]->getContent()->getTemperature();
    mShellTemperature  = inputData.mShellTemperature;
    mBiasHeatFlux      = inputData.mBiasHeatFlux;
    mPreviousPressure  = mNodes[0]->getPotential();
    mDpdt              = 0.0;
    mDpdtFilterGain    = configData.mDpdtFilterGain;
    mEditFluxTarget    = configData.mEditFluxTarget;
    mSurfaceArea       = configData.mSurfaceArea;
    mShellRadius       = configData.mShellRadius;
    processOutputs();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Checks config and input data-derived  parameters for valid ranges and throws
///           exceptions and issues Health & Status errors on failures.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTank::validate(const GunnsFluidTankConfigData& configData,
                              const GunnsFluidTankInputData&  inputData)
{
    /// - Throw an exception on dp/dt filter gain not (0-1).
    if (!MsMath::isInRange(0.0, configData.mDpdtFilterGain, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "dp/dt gain not (0-1).");
    }

    /// - Throw an exception on thermal damping mass < 0.
    if (0.0 > configData.mThermalDampingMass) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "thermal damping mass < 0.");
    }

    /// - Throw an exception on edit flux target < 0.
    if (0.0 > configData.mEditFluxTarget) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "edit flux target < 0.");
    }

    /// - Throw an exception on surface area < 0.
    if (0.0F > configData.mSurfaceArea) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "surface area < 0.");
    }

    /// - Throw an exception on shell radius < 0.
    if (0.0F > configData.mShellRadius) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "shell radius < 0.");
    }

    /// - Throw an exception on shell temperature < 0.
    if (0.0F > inputData.mShellTemperature) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "shell temperature < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTank::restartModel()
{
    /// - Reset the base class.
    GunnsFluidCapacitor::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.  Do not allow active edits through a
    ///   checkpoint.  We allow the parameter values themselves to be checkpointed in case the user
    ///   has a favorite set of values, etc.
    mEditTemperatureFlag                = false;
    mEditTemperaturePressureFlag        = false;
    mEditTemperaturePartialPressureFlag = false;
    for (int i = 0; i < mNConstituents; ++i) {
        mEditPartialPressureRateFlag[i] = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    dt  (s)  Integration time step
///
/// @return   void
///
/// @details  Processes any partial pressure rate edits and applies them to this GUNNS Fluid Tank
///           link model internal fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTank::updateState(const double dt)
{
    /// Zero out source vector and flow rate.
    mSourceVector[0]            = 0.0;
    mSourceVector[1]            = 0.0;
    mFlowRate                   = 0.0;

    /// Skip if time step too small.
    if (dt < static_cast<double>(FLT_EPSILON)) {
        GUNNS_WARNING("time step too small to perform partial pressure rate edit.");
        return;
    }

    /// Reset completion flags.
    bool           complete     = true;
    bool*          editComplete = new bool[mNConstituents];

    /// Save fluid temperature.
    const double temperature    = mNodes[0]->getContent()->getTemperature();

    /// If there is a partial pressure rate edit in progress,
    for (int i = 0; i < mNConstituents; ++i) {
        editComplete[i]         = true;
        if (mEditPartialPressureRateFlag[i]) {

            const double rate = fabs(mEditPartialPressureRateValue[i]);
            if (rate < DBL_EPSILON) {
                mEditPartialPressureRateFlag[i] = false;
                GUNNS_WARNING("partial pressure rate edit canceled, zero rate not allowed.");

            } else {
                double partialPressure;
                const double deltaPartial = rate * dt;
                /// - and if a partial pressure target is not yet achieved,
                if (mPartialPressure[i] < mEditPartialPressureValue[i] - deltaPartial) {
                    /// - Either reset the completion flag, and increment to the new partial
                    ///   pressure, without overshooting the target,
                    editComplete[i] = false;
                    partialPressure = mPartialPressure[i] + deltaPartial;
                } else if (mPartialPressure[i] > mEditPartialPressureValue[i] + deltaPartial) {
                    /// - Or reset the completion flag, decrement to the new partial pressure,
                    ///   without overshooting the target.
                    editComplete[i] = false;
                    partialPressure = mPartialPressure[i] - deltaPartial;
                }

                if (!editComplete[i]) {
                    complete        = false;
                    /// Update the internal fluid constituent mass as the difference in constituent
                    /// density times the constant volume.
                    const MonoFluid* constituent =
                            mInternalFluid->getConstituent(mInternalFluid->getType(i));
                    const double constituentMass = (constituent->computeDensity(temperature,
                                                                                partialPressure) -
                                                    constituent->computeDensity(temperature,
                                                                                mPartialPressure[i])) *
                                                                                mNodes[0]->getVolume();
                    mInternalFluid->setMass(i, constituentMass);
                } else {
                    mEditPartialPressureRateFlag[i] = false;
                }
            }

        } else {
            /// Constituents that are not undergoing edits should have no portion of the internal
            /// fluid.
            mInternalFluid->setMass(i, 0.0);
        }
    }

    //TODO also skip if all constituent masses zero, and set complete = true
    if (!complete) {
        /// During a partial pressure edit, update the internal fluid state and mass flow rate.
        /// Since the capacitive node is on port 0, flow going into the node is a negative link flow
        /// rate.
        mInternalFluid->updateMass();
        mInternalFluid->setTemperature(temperature);
        mFlowRate = -mInternalFluid->getMass() / dt;

        /// During a partial pressure edit, disable the isentropic expansion effect in the node.
        /// This prevents our partial pressure changes from changing the node temperature, which
        /// would feed back into further total pressure changes that would cause the perceived
        /// dP/dt to differ from the desired amount.  Note that this disables the expansion due to
        /// all other flows into the node as well.  When a partial pressure edit is not active,
        /// the node's isentropic expansion effect will be restored to the desired level by the
        /// base class.
        mNodes[0]->setExpansionScaleFactor(0.0);
    }

    delete[] editComplete;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    dt  (s)  Integration time step
///
/// @return   void
///
/// @details  This method builds the normal GunnsFluidCapacitor source vector, and then adds the
///           tank's partial pressure edit flows contribution.  This adds or subtracts mass to/from
///           the network at a rate to produce the desired dP/dt from the edit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTank::buildSourceVector(const double dt)
{
    /// - The mass flow rate is divided by the molecular weight of the internal fluid, which the
    ///   Polyfluid class prevents from being zero so divide by zero is protected against.  The
    ///   Gunns convention is to define flow positive from port 0 to port 1, and since the
    ///   capacitive node being edited is always port 0, we reverse the sign here.
    GunnsFluidCapacitor::buildSourceVector(dt);
    mSourceVector[0] -= mFlowRate / mInternalFluid->getMWeight();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  This link only flows to or from the node for edits.  Edits can flow to or from the
///           node, and can also do cross-flow.  The edits are not meant to conserve mass & energy,
///           as they create & destroy mass & energy directly in the node. The edits do not need the
///           node's mOutflow, as they are meant to interface directly with the node's mContents.
///
///           Therefore the node is never a SOURCE to this link, even when the edit's net flow is
///           out of the node.  However we do want this link to transport its edit flows to the
///           node's mInflow before the node updates.  The result of all this is that the node
///           should be a SINK to this link whenever the edit flow rate is non-zero, otherwise NONE.
///           The node is never a SOURCE, and we never use its getOutflow method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTank::computeFlows(const double dt)
{
    GunnsFluidCapacitor::computeFlows(dt);
    if (mFlowRate != 0.0) {
        mPortDirections[0] = SINK;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    dt  (s)  Integration time step
///
/// @return   void
///
/// @details  Processes any temperature and pressure/partial pressure edits and applies them to
///           this GUNNS Fluid Tank link model port 0 node fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTank::updateFluid(const double dt, const double)
{
    /// - Calculate heat flux from shell if this tank is configured to own the flux.  It is so
    ///   configured when both the shell surface area and radius parameters are > zero.  If so, the
    ///   shell temperature will come from the thermal aspect via simbus, or be defined in input
    ///   data and left alone, making a thermal aspect optional.  If not, the heat flux is received
    ///   from the thermal aspect via simbus (or is set by hand).
    ///
    /// - Assume zero flow at the tank walls, so flux is all conductive, and no need to consider
    ///   convection.  Use the lesser of the conductivity between the wall and the fluid - assume
    ///   fluid is always the lesser (safe assumption as tank walls are usually metal and therefore
    ///   much higher conductivity).  We ignore radiative flux.
    if (mSurfaceArea > FLT_EPSILON && mShellRadius > FLT_EPSILON) {
        mHeatFluxFromShell = mNodes[0]->getContent()->getThermalConductivity()
                           * (mShellTemperature - mNodes[0]->getContent()->getTemperature())
                           * static_cast<double>(mSurfaceArea / mShellRadius);
        mHeatFluxToShell   = -mHeatFluxFromShell;
    }

    /// If there is a total temperature and total or partial pressure edit,
    if (mEditTemperaturePressureFlag || mEditTemperaturePartialPressureFlag) {

        checkEditTemperatureValue();

        mEditPressureValue = computeEditTargetPressure();
        if (mEditPressureValue < static_cast<double>(FLT_EPSILON)) {
            mEditPressureValue = 0.0;
            for (int i = 0; i < mNConstituents; ++i) {
                mEditPartialPressureValue[i] =
                        mNodes[0]->getContent()->getPartialPressure(mInternalFluid->getType(i));
                mEditPressureValue += mEditPartialPressureValue[i];
            }
            GUNNS_WARNING("user attempted pressure edit to zero, using current pressure instead.");
        }

        /// - Switch off the flag once total pressure & temperature have converged to desired
        ///   values.  This will normally take two passes, unless the node is not completely
        ///   isolated, or the edit is an extreme change in temperature or pressure.  The edit can
        ///   cause momentary mass flux in or out of the node as attached nodes come up to the edit
        ///   P - even non-cap nodes do this - so also check for net flux of the node approaching
        ///   zero.
        if (std::fabs(mEditTemperatureValue - mNodes[0]->getContent()->getTemperature()) < static_cast<double>(FLT_EPSILON) &&
            std::fabs(mEditPressureValue    - mNodes[0]->getPotential()) < static_cast<double>(FLT_EPSILON) &&
            std::fabs(mNodes[0]->getNetFlux()) < mEditFluxTarget ) {
            mEditTemperaturePartialPressureFlag = false;
            mEditTemperaturePressureFlag        = false;
            mOverrideVector[0]                  = false;
            mDisablePressureCorrection          = false;
        } else {

            performFluidEdit();

            /// - Override the solver's potential solution with the new total pressure.
            mPotentialVector[0] = mNodes[0]->getContent()->getPressure();
            mOverrideVector[0]  = true;

            /// - Disable pressure corrections in the node while the edit is active.
            mDisablePressureCorrection = true;

            /// - Disable heat flux from the shell while the edit is active - this helps the edit
            ///   automatically turn off when the target temperature is reached.
            mHeatFluxFromShell = 0.0;
            mHeatFluxToShell   = 0.0;
        }

    } else {
        /// - Due to interactions with other attached nodes, the total pressure & temperature edit
        ///   may never automatically reset, so we need to detect instructor reset of the edit flag
        ///   and de-activate the rest of the edit accordingly.
        if (mDisablePressureCorrection) {
            mOverrideVector[0]           = false;
            mDisablePressureCorrection   = false;
        }

        /// - The temperature-only edit is a one-pass shot so the flag is reset immediately.
        if (mEditTemperatureFlag) {
            checkEditTemperatureValue();
            performFluidEdit();
            mEditTemperatureFlag = false;
        }
    }

    /// - TODO temporary fix, omit shell heat flux to avoid instability in temperature at low
    ///   pressure.  A proper solution is to scale shell heat flux as a function of fluid density.
    if (1.0 > mPotentialVector[0]) {
        mHeatFluxFromShell = 0.0;
        mHeatFluxToShell   = 0.0;
    }

    /// - Update the delta-pressure/delta-time parameter.
    computeDpdt(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Check if the user forgot to set target temperature - a zero value will be interpreted
///           as they forgot because absolute zero is not really allowed in GUNNS.  In this case,
///           just use the current node temperature as the target.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTank::checkEditTemperatureValue()
{
    if (mEditTemperatureValue < static_cast<double>(FLT_EPSILON)) {
        mEditTemperatureValue = mNodes[0]->getContent()->getTemperature();
        GUNNS_WARNING("user attempted temperature edit to zero, using current temperature instead.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (kPa)  Target total pressure for the edit
///
/// @details  This method computes and returns the target total pressure for an edit.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidTank::computeEditTargetPressure()
{
    double editTotalPressure = 0.0;

    /// - For a temperature & total pressure edit, the target is the pressure edit value.
    if (mEditTemperaturePressureFlag) {
        editTotalPressure = mEditPressureValue;

    /// - For a temperature & partial pressure edit, the target is the sum of the partial pressure
    ///   edit values.
    } else if (mEditTemperaturePartialPressureFlag) {
        for (int i = 0; i < mNConstituents; ++i) {
            editTotalPressure += mEditPartialPressureValue[i];
        }
    }

    return editTotalPressure;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @details  This method calls the appropriate edit method in the fluid class for either a total
///           or partial pressure edit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTank::performFluidEdit()
{
    if (mEditTemperaturePressureFlag) {
        mNodes[0]->getContent()->edit(mEditTemperatureValue, mEditPressureValue);
        static_cast<GunnsFluidNode*>(mNodes[0])->updatePreviousPressure();
    } else if (mEditTemperaturePartialPressureFlag) {
        mNodes[0]->getContent()->edit(mEditTemperatureValue, mEditPartialPressureValue);
        static_cast<GunnsFluidNode*>(mNodes[0])->updatePreviousPressure();
    } else if (mEditTemperatureFlag) {
        mNodes[0]->getContent()->edit(mEditTemperatureValue, mNodes[0]->getPotential());
    }
    static_cast<GunnsFluidNode*>(mNodes[0])->updatePreviousTemperature();
    /// - Update the node mass to match the new temperature & pressure.
    mNodes[0]->updateMass();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    dt  (s)  Integration time step
///
/// @return   void
///
/// @details  This method computes the true delta-pressure/delta-time (dP/dt) in the node.  This is
///           useful in examining network stability and for validation of sensed dP/dt.  A low-pass
///           filter is used to smooth out noise in the network, if desired.  The filter gain can be
///           adjusted.  A zero value (the default) de-activates this calculation.  A value of 1.0
///           gives the true node dP/dt.  A value of 0.1 is a good typical value for seeing network
///           trends.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTank::computeDpdt(const double dt)
{
    /// - Skip the calculation if network timestep is zero or filter gain is zero.
    if (dt > DBL_EPSILON && mDpdtFilterGain != 0.0) {
        mDpdt = mDpdt + mDpdtFilterGain *
                ((mNodes[0]->getPotential() - mPreviousPressure) / dt - mDpdt);
        // Avoid arithmetic underflow when approaching zero.
        if (fabs(mDpdt) < DBL_EPSILON) {
            mDpdt = 0.0;
        }
    } else {
        mDpdt = 0.0;
    }

    /// - Save node pressure for the next filter pass.
    mPreviousPressure = mNodes[0]->getPotential();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @details  This method stores the node's fluid composition information (partial pressures, mass
///           fractions, mole fractions and temperature) for output to the sim bus or display. This
///           happens after the node has updated so the fluid composition is fresh.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTank::processOutputs()
{
    for (int i = 0; i < mNConstituents; ++i) {
        mPartialPressure[i] =
                mNodes[0]->getContent()->getPartialPressure(mInternalFluid->getType(i));
        mMassFraction[i] = mNodes[0]->getContent()->getMassFraction(mInternalFluid->getType(i));
        mMoleFraction[i] = mNodes[0]->getContent()->getMoleFraction(mInternalFluid->getType(i));
        mTemperature     = mNodes[0]->getContent()->getTemperature();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flag         (--)   Edit activation flag
/// @param[in]  temperature  (K)    Edit temperature value
///
/// @details  This method sets the parameters associated with a temperature-only edit to the desired
///           values.  Calling the method with no arguments supplied deactivates the edit, leaving
///           the other control values alone.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTank::editTemperature(const bool   flag,
                                     const double temperature)
{
    if (flag) {
        mEditTemperatureValue = temperature;
    }
    mEditTemperatureFlag      = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flag         (--)   Edit activation flag
/// @param[in]  temperature  (K)    Edit temperature value
/// @param[in]  pressure     (kPa)  Edit pressure value
///
/// @details  This method sets the parameters associated with a temperature and total pressure edit
///           to the desired values.  Calling the method with no arguments supplied deactivates the
///           edit, leaving the other control values alone.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTank::editTemperaturePressure(const bool   flag,
                                             const double temperature,
                                             const double pressure)
{
    if (flag) {
        mEditTemperatureValue    = temperature;
        mEditPressureValue       = pressure;
    }
    mEditTemperaturePressureFlag = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flag              (--)   Edit activation flag
/// @param[in]  temperature       (K)    Edit temperature value
/// @param[in]  partialPressures  (kPa)  Array of edit partial pressure values
///
/// @details  This method sets the parameters associated with a temperature and partial pressures
///           edit to the desired values.  Calling the method with no arguments supplied deactivates
///           the edit, leaving the other control values alone.
///
/// @note     The supplied partial pressures array size must be as large as the network number of
///           fluid constituents or very bad things can happen to the sim.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTank::editTemperaturePartialPressure(const bool    flag,
                                                    const double  temperature,
                                                    const double* partialPressures)
{
    mEditTemperaturePartialPressureFlag         = flag;
    if (flag) {
        /// - If the temperature is not provided > 0 or the partial pressure array is not provided,
        ///   then leave those edit terms alone and assume the users wants to keep using the
        ///   previously set values.
        if (DBL_EPSILON < temperature) {
            mEditTemperatureValue               = temperature;
        }
        if (partialPressures) {
            for (int i = 0; i < mNConstituents; ++i) {
                mEditPartialPressureValue[i]    = partialPressures[i];
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type             (--)     The fluid constituent to be edited
/// @param[in]  flag             (--)     Edit activation flag
/// @param[in]  partialPressure  (kPa)    Edit partial pressure target value
/// @param[in]  rate             (kPa/s)  Edit partial pressure rate value
///
/// @throws   TsOutOfBoundsException
///
/// @details  This method sets the parameters associated with an individual constituent's partial
///           pressure rate edit.  Calling the method with no arguments supplied does nothing.
///           Supplying a fluid type but leaving the remaining arguments defaulted deactivates the
///           edit, leaving the other control values alone.  An exception is thrown by the fluid
///           class if the supplied constituent type is not present in the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTank::editPartialPressureRate(const FluidProperties::FluidType type,
                                             const bool                       flag,
                                             const double                     partialPressure,
                                             const double                     rate)
{
    if (FluidProperties::NO_FLUID != type) {
        const int index = mNodes[0]->getContent()->find(type);
        if (flag) {
            mEditPartialPressureValue[index]     = partialPressure;
            mEditPartialPressureRateValue[index] = rate;
        }
        mEditPartialPressureRateFlag[index]      = flag;
    }
}
