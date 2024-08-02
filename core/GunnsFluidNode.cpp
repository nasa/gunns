/**
@file
@brief    GUNNS Fluid Node implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Source File for GUNNS Fluid Node)

REQUIREMENTS:
   (TBD)

REFERENCE:
   (TBD)

ASSUMPTIONS AND LIMITATIONS:
   (TBD)

LIBRARY DEPENDENCY:
   (
    (GunnsBasicNode.o)
    (GunnsFluidUtils.o)
    (aspects/fluid/fluid/PolyFluid.o)
   )

PROGRAMMERS:
    (
     (Jason Harvey) (L3) (2011-02) (Initial Prototype))
     (Kevin Supak)  (L3) (2011-02) (Updated to Coding Standards))
     (Eric Ferguson)(L3) (2012-02) (Updates and Maintenance))
    )
*/
#include "GunnsFluidNode.hh"
#include "GunnsFluidUtils.hh"
#include "properties/FluidProperties.hh"
#include <cmath>
#include <cfloat>
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

/// @details  In the computePressureCorrection method, we only apply the pressure correction if the
///           error is above this threshold, to avoid creating extra noise in the system.
const double GunnsFluidNode::mErrorThreshold = 1.0E-11;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the fluid node
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidNode::GunnsFluidNode()
    :
    mFluidConfig         (0),
    mNumFluidTypes       (0),
    mContent             (),
    mInflow              (),
    mOutflow             (),
    mTcInflow            (),
    mVolume              (0.0),
    mPreviousVolume      (0.0),
    mThermalCapacitance  (0.0),
    mCompression         (0.0),
    mInflowHeatFlux      (0.0),
    mNetHeatFlux         (0.0),
    mUndampedHeatFlux    (0.0),
    mThermalDampingMass  (0.0),
    mThermalDampingHeat  (0.0),
    mExpansionDeltaT     (0.0),
    mExpansionScaleFactor(0.0),
    mPreviousPressure    (0.0),
    mPreviousTemperature (0.0),
    mMassError           (0.0),
    mPressureCorrection  (0.0),
    mCorrectGain         (1.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the fluid node
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidNode::~GunnsFluidNode()
{
    TS_DELETE_ARRAY(mTcInflow.mState);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name               (--) Node name
/// @param[in] fluidConfig        (--) Input fluid configuration data
/// @param[in] initialFluidState  (--) Initial state of the port 0 node contents
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Gunns Fluid Node with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidNode::initialize(const std::string&         name,
                                const PolyFluidConfigData* fluidConfig,
                                const PolyFluidInputData*  initialFluidState)
{
    /// - First initialize parent.  The basic node potential isn't initialized until later in the
    ///   prepareForStart method, after the fluid contents have been finalized.
    GunnsBasicNode::initialize(name);

    /// - Throw exception on bad fluid config data.
    if (!fluidConfig) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                "Node is missing fluid configuration data.");
    }

    mFluidConfig   = fluidConfig;
    mNumFluidTypes = fluidConfig->mNTypes;

    /// - Check to see if an initial fluid state has been specified. If not, initialize the node
    ///   to standard temperature & pressure and a mixture of 100% of the first component type.
    ///   Network link objects can subsequently re-initialize the fluids with their own data.
    if (!initialFluidState) {
        double fractions[FluidProperties::NO_FLUID] = {1.0};

        PolyFluidInputData fluidInit( 270.0,          // temperature
                                      101.32501,      // pressure
                                      0.0,            // flowrate
                                      0.0,            // mass
                                      fractions);     // massFraction

        /// - Catch and rethrow exceptions from fluid object initialization to identify the node
        ///   instance with the problem.
        try {
            mContent.initializeName(name + ".mContent");
            mContent.initialize(*fluidConfig, fluidInit);
        } catch (...) {
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Caught exception from mContent initialization.");
        }
        try {
            mInflow .initializeName(name + ".mInflow");
            mInflow .initialize(*fluidConfig, fluidInit);
        } catch (...) {
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Caught exception from mInflow initialization.");
        }
        try {
            mOutflow .initializeName(name + ".mOutflow");
            mOutflow .initialize(*fluidConfig, fluidInit);
        } catch (...) {
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Caught exception from mOutflow initialization.");
        }

        mPreviousTemperature = fluidInit.mTemperature;
        mPreviousPressure    = fluidInit.mPressure;
    }

    else {
        try {
            mContent.initializeName(name + ".mContent");
            mContent.initialize(*fluidConfig, *initialFluidState);
        } catch (...) {
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Caught exception from mContent initialization.");
        }
        try {
            mInflow .initializeName(name + ".mInflow");
            mInflow .initialize(*fluidConfig, *initialFluidState);
        } catch (...) {
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Caught exception from mInflow initialization.");
        }
        try {
            mOutflow .initializeName(name + ".mOutflow");
            mOutflow .initialize(*fluidConfig, *initialFluidState);
        } catch (...) {
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Caught exception from mOutflow initialization.");
        }

        mPreviousTemperature = initialFluidState->mTemperature;
        mPreviousPressure    = initialFluidState->mPressure;
    }

    /// - Allocate memory for the direct trace compound flows into the node.
    const GunnsFluidTraceCompounds* traceCompounds = mContent.getTraceCompounds();
    if (traceCompounds) {
        const int numTcTypes = traceCompounds->getConfig()->mNTypes;
        TS_DELETE_ARRAY(mTcInflow.mState);
        TS_NEW_PRIM_ARRAY_EXT(mTcInflow.mState, numTcTypes, double, mName+".mTcInflow.mState");
        for (int i=0; i<numTcTypes; ++i) {
            mTcInflow.mState[i] = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Synchronizes the last pass values and generic node potential to be consistent with the
///           initial fluid properties that may have been set by any incident links.  This should
///           only be used at load start-up, not checkpoint/restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidNode::prepareForStart()
{
    mPotential           = mContent.getPressure();
    mPreviousTemperature = mContent.getTemperature();
    mPreviousPressure    = mPotential;
    mPreviousVolume      = mVolume;
    updateMass();
    mOutflow.setState(&mContent);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this Gunns Fluid Node initial state.  Exceptions are thrown for any errors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidNode::validate()
{
    //validate parent
    GunnsBasicNode::validate();

    // Reset init flag.
    mInitFlag = false;

    /// - Throw exception on bad fluid config data.
    if (!mFluidConfig) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                "Node is missing fluid configuration data.");
    }

    /// - Throw exception on pressure < FLT_EPSILON.
    if (mContent.getPressure() < static_cast<double>(FLT_EPSILON)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "Node has insufficient initial pressure.");
    }

    /// - Throw exception on temperature < FLT_EPSILON.
    if (mContent.getTemperature() < static_cast<double>(FLT_EPSILON)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "Node has insufficient initial temperature.");
    }

    /// - Throw exception on %mass error > FLT_EPSILON.
    computeMassError();
    if (mMassError > static_cast<double>(FLT_EPSILON)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "Node has excessive initial mass error.");
    }

    // Set init flag on successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidNode::restart()
{
    /// - Reset the base class.
    GunnsBasicNode::restart();

    /// - Reset non-config & non-checkpointed class attributes.  mInflow state is reset first thing
    ///   upon going to run.
    mPreviousVolume     = mVolume;
    mThermalCapacitance = 0.0;
    mCompression        = 0.0;
    mInflowHeatFlux     = 0.0;
    mNetHeatFlux        = 0.0;
    mUndampedHeatFlux   = 0.0;
    mThermalDampingHeat = 0.0;
    mExpansionDeltaT    = 0.0;
    mOutflow.setState(&mContent);

    /// - Determine if mass needs to be reset.  Compute a new mass error from current content
    ///   properties and compare it to the mMassError term, which has been loaded from checkpoint.
    ///   Any discrepancy between these two mass errors is assumed due to a difference in the node
    ///   volume (not checkpointed) and the volume the node had when the checkpoint was cut.  Only
    ///   in this case do we reset the mass.
    const double checkpointError = mMassError;
    computeMassError();
    if (std::fabs(mMassError - checkpointError) > static_cast<double>(FLT_EPSILON)) {
        GUNNS_WARNING("restarting mass.");
        updateMass();
        mMassError = 0.0;
    } else {
        mMassError = checkpointError;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] volume (m3) Desired node volume
///
/// @throws   TsInitializationException
///
/// @details  This method is used to initialize the node volume. Mass is updated to reflect the new
///           volume, keeping the other state parameters (Density, P, T) the same.  LastVolume is
///           reset to avoid compression from the volume change.  An exception is thrown if negative
///           volume is requested.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidNode::initVolume(const double volume)
{
    if (volume < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "Node has initial volume < 0.");
    }

    mVolume         = volume;
    mPreviousVolume = volume;
    updateMass();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] volume (m3) Desired node volume
///
/// @throws   TsOutOfBoundsException, TsInitializationException
///
/// @details  Sets the volume of the node in run-time, to allow for flexible volume objects such as
///           accumulators, etc.  An exception is thrown if negative volume is attempted.  If either
///           the new or last volume are zero, then initVolume method is called to ensure that there
///           will be no compression effects working on zero mass.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidNode::setVolume(const double volume)
{
    if (volume < 0.0) {
        GUNNS_ERROR(TsOutOfBoundsException, "Input Argument Out Of Range",
                "Node set volume < 0.");
    }

    else if ((0.0 == volume) || (0.0 == mPreviousVolume)) {
        initVolume(volume);
    }

    else {
        mVolume = volume;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  potential (kPa) New node potential (pressure)
///
/// @throws   TsInitializationException
///
/// @details  This updates the generic node object potential and fluid content pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidNode::setPotential(const double potential)
{
    mContent.setPressure(potential);
    GunnsBasicNode::setPotential(potential);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] expansionScaleFactor (--) Isentropic expansion effect scale factor (0-1)
///
/// @throws   TsOutOfBoundsException
///
/// @details  Sets the expansion scale factor.  An exception is thrown if the input argument is
///           outside of 0-1.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidNode::setExpansionScaleFactor(const double expansionScaleFactor)
{
    if (!MsMath::isInRange(0.0, expansionScaleFactor, 1.0)) {
        GUNNS_ERROR(TsOutOfBoundsException, "Input Argument Out Of Range",
                "Node set illegal expansion scale factor.");
    }

    mExpansionScaleFactor = expansionScaleFactor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] mass (kg) Non-fluid equivalent thermal mass
///
/// @throws   TsOutOfBoundsException
///
/// @details  Sets the thermal damping mass.  An exception is thrown if the input argument is
///           negative.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidNode::setThermalDampingMass(const double mass)
{
    if (mass < 0.0) {
        GUNNS_ERROR(TsOutOfBoundsException, "Input Argument Out Of Range",
                "Node set thermal damping mass < 0.");
    }

    mThermalDampingMass = mass;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This function resets the flows incident on the node.  Current pressure is also saved
///           for next pass.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidNode::resetFlows()
{
    GunnsBasicNode::resetFlows();
    mInflow.resetState();
    mInflowHeatFlux   = 0.0;
    updatePreviousPressure();
    mOutflow.setState(&mContent);
    const GunnsFluidTraceCompounds* traceCompounds = mContent.getTraceCompounds();
    if (traceCompounds) {
        for (int i=0; i<traceCompounds->getConfig()->mNTypes; ++i) {
            mTcInflow.mState[i] = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (kg*mol) Delta-temperature source vector contribution
///
/// @details  Computes the thermal capacitance of the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidNode::computeThermalCapacitance()
{
    /// \verbatim
    /// Thermal capacitance is the equivalent change in moles of fluid to cause the same pressure
    /// change as a unit change in temperature.  This involves calculating densities at small
    /// increments of temperature, holding pressure constant:
    ///
    ///                               ct = (V/MW) * (del-rho/del-T)
    ///
    /// ct  = thermal capacitance (kgmol/K)
    /// V   = volume              (m3)
    /// MW  = molecular weight    (kg/kgmol)
    /// rho = mass density        (kg/m3)
    /// T   = temperature         (K)
    ///
    /// Units:  c = (m3*kgmol/kg) * (kg/m3/K) = kgmol/K
    ///
    /// Protect for divide by zero by limiting temperature.  Molecular weight is limited in the
    /// PolyFluid class.
    /// \endverbatim
    double t = mContent.getTemperature();
    if (t < static_cast<double>(FLT_EPSILON)) {
        t = static_cast<double>(FLT_EPSILON);
    }
    const double t1 = t * 0.999;
    const double t2 = t * 1.001;

    mThermalCapacitance = (mContent.computeDensity(t1, mContent.getPressure()) -
                           mContent.computeDensity(t2, mContent.getPressure()))
                        * mVolume / (mContent.getMWeight() * (t2 - t1));

    /// - The actual source vector contribution to the system of equations is the product of the
    ///   thermal capacitance and the change in node temperature from last pass.
    ///   mThermalCapacitance isn't actually used by Gunns, but is calculated for model insight.
    ///   Save the current temperature for next pass.
    const double thermalSource = mThermalCapacitance
                               * (mContent.getTemperature() - mPreviousTemperature);
    updatePreviousTemperature();

    return thermalSource;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (kg*mol) Compression source vector contribution.
///
/// @details  Computes the fluid compression of the node.  This is the effect of changing volume on
///           pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidNode::computeCompression()
{
    /// \verbatim
    /// Compression is the equivalent change in moles of fluid to cause the same pressure change as
    /// a unit change in node volume:
    ///
    ///                                 cv = (rho/MW) * (del-V)
    ///
    /// cv  = compression      (kgmol)
    /// V   = volume           (m3)
    /// MW  = molecular weight (kg/kgmol)
    /// rho = mass density     (kg/m3)
    ///
    /// Units:  cv = ( kg-kgmol/kg/m3 ) * m3 = kgmol
    ///
    /// Divide by zero in molecular weight is protected for in the PolyFluid class.  Save the
    /// current volume for next pass.
    /// \endverbatim
    mCompression    = mContent.getDensity() * (mPreviousVolume - mVolume) / mContent.getMWeight();
    mPreviousVolume = mVolume;

    return mCompression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fluxRate (kg/s) Node influx Rate
/// @param[in] fluid    (--)   Node inflow fluid
///
/// @details  This function adds an incoming fluid to the collection terms for the Node, mixing this
///           fluid's properties with what has been collected so far.  When the Node updates, it
///           will zero out the collection terms for next pass.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidNode::collectInflux(const double fluxRate, const PolyFluid* fluid)
{
    try {
        mInflow.addState(fluid, fluxRate);
    } catch (TsOutOfBoundsException &e) {
        GUNNS_WARNING("caught exception from PolyFluid class.");
    }

    /// - We use the Gunns calculated flow rate rather than PolyFluid's mFlowRate because it doesn't
    ///   handle the very small flow rates that we get to/from non-cap nodes.
    mInflowHeatFlux += fluxRate * fluid->getSpecificEnthalpy();
    mInfluxRate     += fluxRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] tcIndex (--)   Index of the trace compound in the network's trace compounds.
/// @param[in] rate    (kg/s) Mass flow rate of the trace compound into the node.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Sets the flow rate state of the given trace compound index to the given rate.  This is
///           for trace compound flows into or out of the node that are not associated with the
///           mInflow or mOutflow bulk fluid flows.  Throws exception if the given index is out of
///           range of the network's trace compounds or if there are no trace compounds in this
///           network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidNode::collectTc(const int tcIndex, const double rate)
{
    const GunnsFluidTraceCompounds* traceCompounds = mContent.getTraceCompounds();
    if (traceCompounds) {
        if (tcIndex >= 0 and tcIndex < traceCompounds->getConfig()->mNTypes) {
            mTcInflow.mState[tcIndex] += rate;
        } else {
            GUNNS_ERROR(TsOutOfBoundsException, "Invalid Argument Range",
                    "trace compound index is out of range of the trace compounds in this network.");
        }
    } else {
        GUNNS_ERROR(TsOutOfBoundsException, "Invalid Argument Range",
                "there are no trace compounds in this network.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt          (s)  Integration time step.
///
/// @throws   TsInitializationException, TsOutOfBoundsException
///
/// @details  Calculates new fluid properties in a node as a result of inflows, outflows,
///           temperature, pressure & mixture changes.  Pressure is the only fluid property that is
///           not updated here, as that comes from the network solver.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidNode::integrateFlows(const double dt)
{
    mExpansionDeltaT    = 0.0;
    mThermalDampingHeat = 0.0;
    GunnsFluidTraceCompounds* traceCompounds = mContent.getTraceCompounds();

    /// - Remove the outflows from the node until the node has zero mass.  Keep track of any
    ///   overflow left over.
    ///
    /// - Moles (kg*mol) and mass (kg) of current node contents flowing out this step, limited to
    ///   the current contents amount.
    const double scheduledOutMoles = mScheduledOutflux * dt;
    const double contentOutMoles   = std::min(scheduledOutMoles, mContent.getMole());
    const double contentOutMass    = contentOutMoles * mContent.getMWeight();

    /// - Mass flow rate (kg/s) of current contents out of the node.
    double outFlowContent = 0.0;
    if (dt > 0.0) {
        outFlowContent = contentOutMass / dt;
    }

    /// - Save original mContent state for mixing with new mContent into mOutflow at the end.
    mOutflow.resetState();
    if (outFlowContent > 0.0) {
        mOutflow.addState(&mContent, outFlowContent);
        mOutflow.setFlowRate(outFlowContent);
    }

    /// - Moles (kg*mol) of incoming fluid.
    const double inflowMW = mInflow.getMWeight();
    double inflowMoles = 0.0;
    if (inflowMW > DBL_EPSILON) {
        /// - Since this is being used to find inflowOutMass below, we ignore negative inflow here.
        inflowMoles = std::max(mInfluxRate, 0.0) * dt / inflowMW;
    }

    /// - Moles (kg*mol) and mass (kg) of incoming fluid that is flowing straight through and out
    ///   this step, limited to the amount of incoming fluid.  Note that when this limit is applied,
    ///   mass is not conserved in the network.
    const double inflowOutMoles = std::min(inflowMoles, scheduledOutMoles - contentOutMoles);
    const double inflowOutMass  = inflowOutMoles * inflowMW;

    /// - Mass flow rate (kg/s) of incoming fluid that is flowing straight through and out this step.
    double outFlowThru = 0.0;
    if (dt > 0.0) {
        outFlowThru = inflowOutMass / dt;
    }

    /// - Total mass flow rate (kg/s) of fluid flow out of the node.
    const double outFlow = outFlowContent + outFlowThru;

    /// - Calculate net heat flux into the node from all input & output sources, and initial (last)
    ///   mass and heat content of the fluid.
    const double outHeatFlux = outFlowContent * mContent.getSpecificEnthalpy()
                             + outFlowThru    * mInflow.getSpecificEnthalpy();
    //    const double lastHeat = lastH * mContent.getMass();
    mNetHeatFlux = mInflowHeatFlux - outHeatFlux;

    /// - Capacitive nodes have volume and contain mass, and they are updated differently from
    ///   non-capacitive nodes.
    if(mVolume > 0.0) {

        /// - Total inflowing mass (kg) this step.
        /// - Note that contrary to our usual naming convention of 'flow' is mass and 'flux' is
        ///   moles, mInfluxRate is mass rate (kg/s), not mole rate.
        const double inMass = mInfluxRate * dt;

        /// - Current contents mass (kg) before flows are applied.
        const double lastMass = mContent.getMass();

        /// - Current contents mass (kg) remaining in the node after outflows.
        const double oldMass = std::max(0.0, lastMass - contentOutMass);

        /// - Final contents mass (kg) in the node after outflows and inflows.
        const double newMass = std::max(DBL_EPSILON, oldMass + inMass - inflowOutMass);

        /// - Calculate the change in temperature of the original mass due to thermal expansion.
        mExpansionDeltaT = GunnsFluidUtils::computeIsentropicTemperature(mExpansionScaleFactor,
                                                                         mPreviousPressure,
                                                                         mContent.getPressure(),
                                                                         &mContent)
                         - mContent.getTemperature();

        /// - If there is incoming flow, calculate the new mixture in the node, removing
        ///   negative or dirty zero mass fractions and re-normalizing if necessary.
        if (fabs(inMass) > DBL_EPSILON) {

            /// - First do outflow of trace compounds before mixing in their inflow.
            if (traceCompounds and mContent.getMWeight() > DBL_EPSILON) {
                mContent.getTraceCompounds()->flowOut(contentOutMass / mContent.getMWeight());
            }

            if (oldMass + inMass - inflowOutMass >= DBL_EPSILON) {
                GunnsFluidUtils::mixFluidMasses(&mContent, oldMass,
                                                &mInflow,  (inMass - inflowOutMass),
                                                mNumFluidTypes);
            } else {
                /// - This can happen if a link flows less into the node than what it made the
                ///   solver solve for, i.e. an adsorber that is adsorbing from its thru-flow into
                ///   this node.  If the difference between the solved thru-flux and the actual
                ///   inflow mass is greater than the node's mass, then this occurs and masses &
                ///   energy are not conserved.
                GunnsFluidUtils::mixFluidMasses(&mContent, 0.0,
                                                &mInflow,  DBL_EPSILON,
                                                mNumFluidTypes);
                GUNNS_WARNING("invalid mixture due to zero node mass after inflow, conservation " <<
                              "errors may result.");
            }
        } else {
            /// - If there is no incoming flow, the mass fractions will not change, therefore set
            ///   the mass without setting mass fractions.
            mContent.setMass(newMass);
        }

        /// - Add standalone trace compound flows, separate from the bulk fluid flows, into or out
        ///   of the node contents.
        if (traceCompounds and mContent.getMWeight() > DBL_EPSILON) {
            traceCompounds->flowIn(mTcInflow.mState, dt);
        }

        /// - Calculate the new node specific enthalpy, and update the fluid's enthalpy,
        ///   temperature and specific heat.
        ///
        /// - In the overflow case, if there is any incoming mass, the node contents take the
        ///   incoming mass fluid properties, otherwise hold the temperature constant.
        double newT = mContent.getTemperature();

        /// - The new enthalpy is a mix of the old and inflow enthalpy.  Because we haven't
        ///   called setTemperature yet, getSpecificiEnthalpy still represents the last mass.
        double newEnthalpy = lastMass * mContent.getSpecificEnthalpy() + mNetHeatFlux * dt;
        if (newEnthalpy < DBL_EPSILON) {
            newEnthalpy = mContent.getSpecificEnthalpy();
        } else {
            newEnthalpy /= newMass;
        }

        /// - Thermal damping mass represents the mass of a container shell or solid contents
        ///   that remain in thermal equilibrium with the fluid, and thus act to dampen changes
        ///   in fluid temperature due to hotter or colder flows coming in.  But it must not
        ///   damp the change in specific enthalpy caused by a mixture change at the same
        ///   temperature.  We can also specify an additional portion of heat to omit from the
        ///   damping.  Start with the specific enthalpy of the new mixture at the old
        ///   temperature, and ramp that towards the above-calculated new specific enthalpy as
        ///   thermal damping mass goes down from infinity to zero.
        if (mThermalDampingMass > 0.0) {

            const double mixtureEnthalpy =
                    mContent.computeSpecificEnthalpy(mContent.getTemperature())
                  + mUndampedHeatFlux * dt / newMass;
            const double dampedEnthalpy = (newEnthalpy - mixtureEnthalpy)
                    * newMass / (newMass + mThermalDampingMass);
            if (dt > DBL_EPSILON) {
                mThermalDampingHeat = (newEnthalpy - mixtureEnthalpy - dampedEnthalpy)
                                    * newMass / dt;
            }
            newEnthalpy = mixtureEnthalpy + dampedEnthalpy;
        }

        /// - Update the new fluid temperature due to damping and isentropic
        ///   expansion/compression.  Because we haven't called setTemperature yet,
        ///   getSpecificEnthalpy still represents the old mass.
        newEnthalpy += oldMass * mExpansionDeltaT * mContent.getSpecificHeat() / newMass;
        newT = mContent.computeTemperature(newEnthalpy);

        /// - Update the final thermal parameters and density.
        mContent.setTemperature(newT);

    /// - Non-capacitive nodes have no volume, and their fluid properties represent an infinitesimal
    ///   amount of mass.  As such, the node's fluid properties are completely replaced by those of
    ///   the inflow if there is any inflow rate.  Otherwise the properties remain static.  We use
    ///   the PolyFluid::setState method as a convenient way to copy the inflow state - however, we
    ///   must keep the node pressure that has been solved for by the Gunns network solver.  Since
    ///   the inflow fluid isn't used for anything after this, we just set its pressure equal to our
    ///   node pressure prior to the setState.
    ///
    /// - Before moving inflow into the content, update its temperature & enthalpy with the total
    ///   inflow heat flux, which may include additional injected heat from non-mass flows.
    } else if (mInfluxRate > 0.0) {
        mInflow.setPressure(mContent.getPressure());
        mInflow.setTemperature(mInflow.computeTemperature(mInflowHeatFlux / mInfluxRate));

        /// - Remove negative mass fractions from mInflow and re-normalize if necessary.
        double newMassFractions[mNumFluidTypes];
        for (int i = 0; i < mNumFluidTypes; ++i) {
            newMassFractions[i] = mInflow.getMassFraction(i);
        }
        if (GunnsFluidUtils::normalizeArray(newMassFractions, mNumFluidTypes)) {
            mInflow.setMassAndMassFractions(0.0, newMassFractions);
        }

        mContent.setState(&mInflow);

        /// - A non-capacitive node must have zero compound masses, and the compound mole fractions
        ///   are relative to zero moles.
        if (traceCompounds) {
            mContent.getTraceCompounds()->setMasses();
        }
    }

    /// - Prevent negative trace compound masses & mole fractions.
    if (traceCompounds) {
        traceCompounds->limitPositive();
    }

    /// - Calculate mass discrepancy between the solution density and the actual mass / volume.
    computeMassError();

    /// - Mix through flow of final contents to the outflow, and balance outflow properties.  If
    ///   there weren't any outflows, then set the outflow state to the new contents.
    if (outFlowThru > 0.0) {
        mOutflow.addState(&mContent, outFlowThru);
    } else if (outFlowContent <= 0.0) {
        mOutflow.setState(&mContent);
    }
    mOutflow.setPressure(mContent.getPressure());

    /// - Copy the in, out & throughput flow rates to display terms.  Note all these 'flux' class
    ///   attributes are mass flows (kg/s), not moles.
    mNetFlux = mInfluxRate - outFlow;
    mFluxThrough = std::min(mInfluxRate, outFlow);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (kPa) Pressure correction
///
/// @details  This method computes a correction to node pressure to flow out the accumulated mass
///           error.  There may be a small discrepancy between the actual mass and that predicted
///           by the equation of state from the solver's pressure.  This is due to the solver not
///           being able to perfectly solve for heat transfer, etc.  We correct this by resetting
///           node Pressure to that calculated by the new mass & temperature.  This is a very small
///           adjustment that is transparent to the users.  It is made to preserve perfect mass
///           balance.  Keep track of how big the adjustment is for debug purposes.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidNode::computePressureCorrection()
{
    /// - Only needed for capacitive nodes.
    if (mVolume > 0.0) {

        /// - The node density and pressure will disagree with the mass.  Calculate the ideal
        ///   (correct) density and pressure from mass, volume, and temperature.  The pressure error
        ///   is the difference between this pressure and the current node pressure.
        const double idealDensity  = mContent.getMass() / mVolume;
        const double idealPressure = mContent.computePressure(mContent.getTemperature(),
                                                              idealDensity);
        const double pressureError = idealPressure - mContent.getPressure();

        /// - We use a filter to damp out the correction when two nodes are interacting with each
        ///   other.  If sign of the error has reversed, reduce the correction gain.  If the sign of
        ///   the error is consistent, ramp the gain back to the maximum.
        ///
        /// - The correction filter gain, gain limit, and threshold are displayed for tuning and
        ///   insight into system performance.
        if ((pressureError * mPressureCorrection) < 0.0) {
            mCorrectGain = mCorrectGain * 0.5;
        }

        else {
            mCorrectGain = mCorrectGain * 1.01;
        }

        /// - Protect for arithmetic underflow in the gain.
        mCorrectGain = MsMath::limitRange( DBL_EPSILON, mCorrectGain, 1.0);

        /// - We only apply the pressure correction if the error is above a certain threshold, to
        ///   avoid creating extra noise in the system.
        if( fabs(pressureError) > mErrorThreshold ) {
            mPressureCorrection = pressureError * mCorrectGain;
        }

        else {
            mPressureCorrection  = 0.0;
        }
    }

    /// - Force pressure correction zero for non-capacitive nodes.
    else {
        mPressureCorrection  = 0.0;
    }

    return mPressureCorrection;
}
