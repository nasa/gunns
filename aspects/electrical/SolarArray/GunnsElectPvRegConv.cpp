/**
@file
@brief    GUNNS Electrical Photovoltaic Array Converting Regulator Model implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  (
   (core/GunnsBasicLink.o)
   (GunnsElectPvArray.o)
   (GunnsElectPvRegSensors.o)
   (GunnsElectPvRegTrips.o)
   (software/exceptions/TsInitializationException.o)
  )
*/

#include "GunnsElectPvRegConv.hh"
#include "GunnsElectPvArray.hh"
#include "core/GunnsSensorAnalogWrapper.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                   (--)  Link name.
/// @param[in] nodes                  (--)  Network nodes array.
/// @param[in] voltageConvLimit       (V)   Maximum ratio this regulator can increase the input voltage from the array to the output.
/// @param[in] voltageConvEfficiency  (--)  Power efficiency (0-1) of input to output voltage conversion.
/// @param[in] array                  (--)  Pointer to the PV array link.
/// @param[in] inCurrentSensor        (--)  Optional pointer to the input current sensor wrapper.
/// @param[in] inVoltageSensor        (--)  Optional pointer to the input voltage sensor wrapper.
/// @param[in] outCurrentSensor       (--)  Optional pointer to the output current sensor wrapper.
/// @param[in] outVoltageSensor       (--)  Optional pointer to the output voltage sensor wrapper.
/// @param[in] inputOverCurrentTrip   (amp) Optional input over-current trip limit.
/// @param[in] inputOverVoltageTrip   (V)   Optional input over-volt trip limit.
/// @param[in] outputOverCurrentTrip  (amp) Optional output over-current trip limit.
/// @param[in] outputOverVoltageTrip  (V)   Optional output over-volt trip limit.
/// @param[in] outputUnderVoltageTrip (V)   Optional output under-volt trip limit.
/// @param[in] tripPriority           (--)  Trip network step priority.
///
/// @details  Default constructs this Photovoltaic Array Converting Regulator configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegConvConfigData::GunnsElectPvRegConvConfigData(
        const std::string&        name,
        GunnsNodeList*            nodes,
        const double              voltageConvLimit,
        const double              voltageConvEfficiency,
        GunnsElectPvArray*        array,
        GunnsSensorAnalogWrapper* inCurrentSensor,
        GunnsSensorAnalogWrapper* inVoltageSensor,
        GunnsSensorAnalogWrapper* outCurrentSensor,
        GunnsSensorAnalogWrapper* outVoltageSensor,
        const double              inOverCurrentTrip,
        const double              inOverVoltageTrip,
        const double              outOverCurrentTrip,
        const double              outOverVoltageTrip,
        const double              outUnderVoltageTrip,
        const unsigned int        tripPriority)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mVoltageConvLimit(voltageConvLimit),
    mVoltageConvEfficiency(voltageConvEfficiency),
    mArray(array),
    mInCurrentSensor(inCurrentSensor),
    mInVoltageSensor(inVoltageSensor),
    mOutCurrentSensor(outCurrentSensor),
    mOutVoltageSensor(outVoltageSensor),
    mInOverCurrentTrip(inOverCurrentTrip),
    mInOverVoltageTrip(inOverVoltageTrip),
    mOutOverCurrentTrip(outOverCurrentTrip),
    mOutOverVoltageTrip(outOverVoltageTrip),
    mOutUnderVoltageTrip(outUnderVoltageTrip),
    mTripPriority(tripPriority)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic Array Converting Regulator configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegConvConfigData::~GunnsElectPvRegConvConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] voltageSetpoint      (V)  Initial setpoint for the regulated output voltage.
/// @param[in] voltageSetpointDelta (V)  Initial nominal delta to voltage setpoint.
/// @param[in] powered              (--) Initial state of power on flag.
/// @param[in] enabled              (--) Initial state of enable command.
///
/// @details  Default constructs this Photovoltaic Array Converting Regulator input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegConvInputData::GunnsElectPvRegConvInputData(
        const double           voltageSetpoint,
        const double           voltageSetpointDelta,
        const bool             powered,
        const bool             enabled)
    :
    GunnsBasicLinkInputData(false, 0.0),
    mVoltageSetpoint(voltageSetpoint),
    mVoltageSetpointDelta(voltageSetpointDelta),
    mPowered(powered),
    mEnabled(enabled)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic Array Converting Regulator input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegConvInputData::~GunnsElectPvRegConvInputData()
{
    // nothing to do
}

/// @details  Default maximum admittance value is chosen as a good trade between ideal voltage
///           source without causing excessive rounding error in the network solution.
const double GunnsElectPvRegConv::mMaxAdmittance = 1.0E8;

/// @details  Default minimum admittance value is used to avoid divide by zero.
const double GunnsElectPvRegConv::mMinAdmittance = 1.0E-8;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Gunns Photovoltaic Array Converting Regulator Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegConv::GunnsElectPvRegConv()
    :
    GunnsBasicLink(NPORTS),
    mMalfVoltageBiasFlag(false),
    mMalfVoltageBiasValue(0.0),
    mVoltageConvLimit(0.0),
    mVoltageConvEfficiency(0.0),
    mArray(0),
    mVoltageSetpoint(0.0),
    mVoltageSetpointDelta(0.0),
    mPowered(false),
    mEnabled(false),
    mResetTrips(false),
    mSensors(),
    mTrips(),
    mState(OFF),
    mRegulatedVoltage(0.0),
    mInputPower(0.0),
    mOutputPower(0.0),
    mWasteHeat(0.0),
    mMaxRegCurrent(0.0),
    mStateAdmittance(),
    mStateSource(),
    mStateUpmodeLatch(false)
{
    for (unsigned int i=0; i<N_STATES; ++i) {
        mStateAdmittance[i] = 0.0;
        mStateSource[i]     = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Photovoltaic Array Converting Regulator.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegConv::~GunnsElectPvRegConv()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Photovoltaic Array Converting Regulator Config Data.
/// @param[in]     inputData    (--) Photovoltaic Array Converting Regulator Input Data.
/// @param[in,out] networkLinks (--) Network links.
/// @param[in]     port0        (--) Network port 0.
///
/// @throws   TsInitializationException
///
/// @details  This method initializes this Photovoltaic Array Converting Regulator with config and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegConv::initialize(const GunnsElectPvRegConvConfigData& configData,
                                     const GunnsElectPvRegConvInputData&  inputData,
                                     std::vector<GunnsBasicLink*>&        networkLinks,
                                     const int                            port0)
{
    /// - Initialize the parent class.
    int ports[1] = {port0};
    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag              = false;

    validate(configData, inputData);

    /// - Initialize config and input data.
    mArray                 = configData.mArray;
    mVoltageConvLimit      = configData.mVoltageConvLimit;
    mVoltageConvEfficiency = configData.mVoltageConvEfficiency;
    mVoltageSetpoint       = inputData.mVoltageSetpoint;
    mVoltageSetpointDelta  = inputData.mVoltageSetpointDelta;
    mPowered               = inputData.mPowered;
    mEnabled               = inputData.mEnabled;

    /// - Connect to sensors.
    mSensors.connectInCurrent(configData.mInCurrentSensor);
    mSensors.connectInVoltage(configData.mInVoltageSensor);
    mSensors.connectOutCurrent(configData.mOutCurrentSensor);
    mSensors.connectOutVoltage(configData.mOutVoltageSensor);

    /// - Initialize trip functions.
    mTrips.mInOverCurrent.initialize(static_cast<float>(configData.mInOverCurrentTrip),
                                     configData.mTripPriority, false);
    mTrips.mInOverVoltage.initialize(static_cast<float>(configData.mInOverVoltageTrip),
                                     configData.mTripPriority, false);
    mTrips.mOutOverCurrent.initialize(static_cast<float>(configData.mOutOverCurrentTrip),
                                      configData.mTripPriority, false);
    mTrips.mOutOverVoltage.initialize(static_cast<float>(configData.mOutOverVoltageTrip),
                                      configData.mTripPriority, false);
    mTrips.mOutUnderVoltage.initialize(static_cast<float>(configData.mOutUnderVoltageTrip),
                                       configData.mTripPriority, false);

    /// - Initialize class attributes.
    mState                 = OFF;
    mRegulatedVoltage      = 0.0;
    mInputPower            = 0.0;
    mOutputPower           = 0.0;
    mWasteHeat             = 0.0;
    mMaxRegCurrent         = 0.0;
    for (unsigned int i=0; i<N_STATES; ++i) {
        mStateAdmittance[i] = 0.0;
        mStateSource[i]     = 0.0;
    }
    mStateUpmodeLatch      = false;

    /// - Tie the array strings to a common output voltage.
    mArray->setCommonStringsOutput(true);

    mInitFlag              = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--) Photovoltaic Array Converting Regulator Config Data.
/// @param[in]  inputData   (--) Photovoltaic Array Converting Regulator Input Data.
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Photovoltaic Array Converting Regulator config & input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegConv::validate(const GunnsElectPvRegConvConfigData& configData,
                                   const GunnsElectPvRegConvInputData&  inputData) const
{
    /// - Throw an exception on voltage conversion ratio limit < 1.
    if (configData.mVoltageConvLimit < 1.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "voltage conversion ratio limit < 1.");
    }

    /// - Throw an exception on voltage conversion efficiency not in DBL_EPSILON, 1.
    if (!MsMath::isInRange(DBL_EPSILON, configData.mVoltageConvEfficiency, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "voltage conversion efficiency not in (DBL_EPSILON, 1).");
    }

    /// - Throw an exception on null array pointer.
    if (!configData.mArray) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "null pointer to array link.");
    }

    /// - Throw an exception if the array link has not been initialized already.  This ensures the
    ///   array link is stepped before this link, which is an order dependency that we require.
    if (!configData.mArray->isInitialized()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "array link not initialized before this link.");
    }

    /// - Throw an exception on voltage setpoint < DBL_EPSILON.
    if (inputData.mVoltageSetpoint < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "voltage setpoint < DBL_EPSILON.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegConv::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-config and non-checkpointed data.
    mState                 = OFF;
    mRegulatedVoltage      = 0.0;
    mInputPower            = 0.0;
    mOutputPower           = 0.0;
    mWasteHeat             = 0.0;
    mMaxRegCurrent         = 0.0;
    for (unsigned int i=0; i<N_STATES; ++i) {
        mStateAdmittance[i] = 0.0;
        mStateSource[i]     = 0.0;
    }
    mStateUpmodeLatch      = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Not used.
///
/// @details  Computes this link's contributions to the network system of equations prior to the
///           network major step solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegConv::step(const double dt __attribute__((unused)))
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Step the model.
    if (mResetTrips or not mPowered) {
        mTrips.resetTrips();
        mResetTrips = false;
    }
    updateRegulatedVoltage();

    /// - Ensure inputs are in valid range.
    if (mState < 0 or mState >= N_STATES) {
        mState = OFF;
    }
    mVoltageConvLimit = std::max(mVoltageConvLimit, 1.0);

    /// - Compute link admittance and source vector values for a potential source for each regulator
    ///   state, based on regulator and array inputs for this pass.  This creates a piece-wise
    ///   linear I-V performance curve for this regulator's outputs.
    const double cornerV = mArray->getIvCornerVoltage() * mVoltageConvLimit;
    const double cornerI = mArray->getIvCornerCurrent() * mVoltageConvEfficiency / mVoltageConvLimit;
    const double potentialSource = mArray->getOpenCircuitVoltage() * mVoltageConvLimit;
    const double currentLimit    = mArray->getShortCircuitCurrent() / mVoltageConvLimit;

    mStateAdmittance[OFF]   = mMinAdmittance;
    mStateAdmittance[REG]   = mMaxAdmittance;
    mStateAdmittance[SAG]   = std::min(mMaxAdmittance, std::max(mMinAdmittance,
                              cornerI / std::max(potentialSource - cornerV, DBL_EPSILON)));
    mStateAdmittance[SHORT] = std::min(mMaxAdmittance, std::max(mMinAdmittance,
                              (currentLimit - cornerI) / std::max(cornerV, DBL_EPSILON)));

    mStateSource[OFF]   = 0.0;
    mStateSource[REG]   = mRegulatedVoltage * mStateAdmittance[REG];
    mStateSource[SAG]   = potentialSource * mStateAdmittance[SAG];
    mStateSource[SHORT] = currentLimit;

    updateMaxOutputs();
    mStateUpmodeLatch = false;
    updateRegulatorState();

    /// - Build the contributions to the network solution.
    minorStep(0, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt        (s)  Not used.
/// @param[in] minorStep (--) Not used.
///
/// @details  Computes this link's contributions to the network system of equations prior to each
///           network minor step solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegConv::minorStep(const double dt __attribute__((unused)),
                                    const int    minorStep __attribute__((unused)))
{
    /// - Build the contributions to the network solution.
    buildAdmittanceMatrix();
    buildSourceVector();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes maximum current this regulator can output at the regulated voltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegConv::updateMaxOutputs()
{
    double p, g;
    double v = mRegulatedVoltage / mVoltageConvLimit;
    mArray->predictLoadAtVoltage(p, g, v);
    mMaxRegCurrent = p * mVoltageConvEfficiency / (v * mVoltageConvLimit);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  True if the mState value changed in this update.
///
/// @details  Determines the mState based on demanded power from the circuit, and array model
///           conditions.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsElectPvRegConv::updateRegulatorState()
{
    const PvRegStates lastState   = mState;
    const bool        backVoltage = (mPotentialVector[0] > mRegulatedVoltage);

    /// - Deactivate if disabled by command, power or trips, or if there is back-voltage when in the
    ///   REG state (which simulates a diode on the regulator output) or if the array can't supply
    ///   any power.
    if (not (mPowered and mEnabled) or mTrips.isTripped() or (REG == mState and backVoltage) or
            (mArray->getMpp().mPower < DBL_EPSILON)) {
        mState       = OFF;
        mFlux        = 0.0;
        mOutputPower = 0.0;
        mInputPower  = 0.0;
        mArray->loadAtPower(mInputPower, false);

    } else {
        /// - Calculate outputs to the downstream circuit resulting from this step's network
        ///   solution and load the array with the corresponding power load.
        mFlux        = -mPotentialVector[0] * mAdmittanceMatrix[0] + mSourceVector[0];
        mOutputPower =  mPotentialVector[0] * mFlux;
        mInputPower  =  mOutputPower / mVoltageConvEfficiency;
        mArray->loadAtPower(mInputPower, (SHORT == mState));
        const double inputVoltage = mArray->getTerminal().mVoltage;

        /// - Change states based on the array's voltage under load load.  To prevent oscillations
        ///   between states causing network convergence failure, we only allow one up-mode (a
        ///   transition from OFF/SHORT->SAG->REG) per major step.
        if (inputVoltage * mVoltageConvLimit > mRegulatedVoltage or (backVoltage and SAG == mState)) {
            if (REG != mState and not mStateUpmodeLatch) {
                mStateUpmodeLatch = true;
                mState = REG;
            }

        } else if (inputVoltage > mArray->getIvCornerVoltage() or (backVoltage and SHORT == mState)) {
            if (SHORT == mState) {
                if (not mStateUpmodeLatch) {
                    mStateUpmodeLatch = true;
                    mState = SAG;
                }
            } else {
                mState = SAG;
            }

        } else {
            mState = SHORT;
        }
    }

    return (mState != lastState);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] convergedStep (--) The count of minor steps since the network last converged.
/// @param[in] absoluteStep  (--) Not used.
///
/// @returns  SolutionResult  (--)  Whether this link confirms or rejects the network solution.
///
/// @details  This updates the state after every minor step, and rejects if it changed on a
///           converged step.  Updates sensors and trips on converged steps.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult GunnsElectPvRegConv::confirmSolutionAcceptable(
        const int convergedStep, const int absoluteStep __attribute__((unused)))
{
    SolutionResult result = CONFIRM;

    if (updateRegulatorState() and (convergedStep > 0)) {
        result = REJECT;
    }

    /// - Update sensors & trips.
    if (convergedStep > 0) {
        /// - Sensors are optional; if a sensor exists then the trip uses its sensed value of the
        ///   truth parameter, otherwise the trip looks directly at the truth parameter.
        float sensedVin  = static_cast<float>(mArray->getTerminal().mVoltage);
        float sensedIin  = static_cast<float>(mArray->getTerminal().mCurrent);
        float sensedVout = static_cast<float>(mPotentialVector[0]);
        float sensedIout = static_cast<float>(mFlux);

        /// - Note that since we step the sensors without a time-step, its drift malfunction isn't
        ///   integrated.  This is because we don't have the time-step in this function, and we must
        ///   update the sensor multiple times per major network step, which would repeat the drift
        ///   integration too many times.  The result of all this is that drift lags behind by one
        ///   major step for causing trips.
        if (mSensors.mInVoltage) {
            sensedVin = mSensors.mInVoltage->sense(0.0, mPowered, sensedVin);
        }
        if (mSensors.mInCurrent) {
            sensedIin = mSensors.mInCurrent->sense(0.0, mPowered, sensedIin);
        }
        if (mSensors.mOutVoltage) {
            sensedVout = mSensors.mOutVoltage->sense(0.0, mPowered, sensedVout);
        }
        if (mSensors.mOutCurrent) {
            sensedIout = mSensors.mOutCurrent->sense(0.0, mPowered, sensedIout);
        }

        /// - Check all trip logics for trips.  If any trip, reject the solution and mode to OFF.
        if (mPowered and mEnabled) {
            if (mTrips.mInOverVoltage  .checkForTrip(result, sensedVin,  convergedStep)) mState = OFF;
            if (mTrips.mInOverCurrent  .checkForTrip(result, sensedIin,  convergedStep)) mState = OFF;
            if (mTrips.mOutOverVoltage .checkForTrip(result, sensedVout, convergedStep)) mState = OFF;
            if (mTrips.mOutOverCurrent .checkForTrip(result, sensedIout, convergedStep)) mState = OFF;
            if (mTrips.mOutUnderVoltage.checkForTrip(result, sensedVout, convergedStep)) mState = OFF;
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Not used.
///
/// @details  Computes the flows and final outputs resulting from the network solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegConv::computeFlows(const double dt __attribute__((unused)))
{
    mPotentialDrop = mArray->getTerminal().mVoltage - mPotentialVector[0];
    mPower         = mOutputPower - mInputPower;
    mWasteHeat     = -mPower;

    /// - Transport supply current from the array to the downstream circuit node.
    mNodes[0]->collectInflux(mFlux);
}
