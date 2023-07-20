/**
@file
@brief    GUNNS Electrical Converter Output Link implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsBasicLink.o)
    (aspects/electrical/Converter/GunnsElectConverterInput.o)
    (core/GunnsSensorAnalogWrapper.o)
    (aspects/electrical/TripLogic/GunnsTripLogic.o)
    (software/exceptions/TsInitializationException.o))
*/

#include "GunnsElectConverterOutput.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                        (--)    Link name.
/// @param[in] nodes                       (--)    Network nodes array.
/// @param[in] regulatorType               (--)    The type of output regulation.
/// @param[in] outputConductance           (1/ohm) Conductance of the output channel.
/// @param[in] converterEfficiency         (--)    The voltage conversion efficiency (0-1).
/// @param[in] outputVoltageSensor         (--)    Pointer to the output voltage sensor spotter.
/// @param[in] outputCurrentSensor         (--)    Pointer to the output current sensor spotter.
/// @param[in] tripPriority                (--)    Priority of trips in the network.
/// @param[in] outputOverVoltageTripLimit  (V)     Output over-voltage trip limit.
/// @param[in] outputOverCurrentTripLimit  (amp)   Output over-current trip limit.
/// @param[in] inputLink                   (--)    Pointer to the input side link.
/// @param[in] enableLimiting              (--)    Limits output current or voltage instead of tripping.
/// @param[in] outputUnderVoltageTripLimit (V)     Output under-voltage trip limit.
///
/// @details  Default Electrical Converter Output link config data constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterOutputConfigData::GunnsElectConverterOutputConfigData(
        const std::string&                             name,
        GunnsNodeList*                                 nodes,
        const GunnsElectConverterOutput::RegulatorType regulatorType,
        const double                                   outputConductance,
        const double                                   converterEfficiency,
        GunnsSensorAnalogWrapper*                      outputVoltageSensor,
        GunnsSensorAnalogWrapper*                      outputCurrentSensor,
        const unsigned int                             tripPriority,
        const float                                    outputOverVoltageTripLimit,
        const float                                    outputOverCurrentTripLimit,
        GunnsElectConverterInput*                      inputLink,
        const bool                                     enableLimiting,
        const float                                    outputUnderVoltageTripLimit)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mRegulatorType(regulatorType),
    mEnableLimiting(enableLimiting),
    mOutputConductance(outputConductance),
    mConverterEfficiency(converterEfficiency),
    mOutputVoltageSensor(outputVoltageSensor),
    mOutputCurrentSensor(outputCurrentSensor),
    mTripPriority(tripPriority),
    mOutputOverVoltageTripLimit(outputOverVoltageTripLimit),
    mOutputUnderVoltageTripLimit(outputUnderVoltageTripLimit),
    mOutputOverCurrentTripLimit(outputOverCurrentTripLimit),
    mInputLink(inputLink),
    mStateFlipsLimit(4)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Electrical Converter Output link config data destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterOutputConfigData::~GunnsElectConverterOutputConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy.
///
/// @details  Electrical Converter Output link config data copy constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterOutputConfigData::GunnsElectConverterOutputConfigData(
        const GunnsElectConverterOutputConfigData& that)
    :
    GunnsBasicLinkConfigData(that),
    mRegulatorType(that.mRegulatorType),
    mEnableLimiting(that.mEnableLimiting),
    mOutputConductance(that.mOutputConductance),
    mConverterEfficiency(that.mConverterEfficiency),
    mOutputVoltageSensor(that.mOutputVoltageSensor),
    mOutputCurrentSensor(that.mOutputCurrentSensor),
    mTripPriority(that.mTripPriority),
    mOutputOverVoltageTripLimit(that.mOutputOverVoltageTripLimit),
    mOutputUnderVoltageTripLimit(that.mOutputUnderVoltageTripLimit),
    mOutputOverCurrentTripLimit(that.mOutputOverCurrentTripLimit),
    mInputLink(that.mInputLink),
    mStateFlipsLimit(that.mStateFlipsLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Initial blockage malfunction activation flag.
/// @param[in] malfBlockageValue (--) Initial blockage maflunction value.
/// @param[in] enabled           (--) Initial operation enabled state.
/// @param[in] inputVoltage      (V)  Initial input voltage.
/// @param[in] inputPower        (W)  Initial input power load.
/// @param[in] setpoint          (--) Initial commanded setpoint.
///
/// @details  Default Electrical Converter Output link input data constructor.  The base class
///           blockage malfunction is disabled because it isn't used in this link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterOutputInputData::GunnsElectConverterOutputInputData(const bool   malfBlockageFlag,
                                                                       const double malfBlockageValue,
                                                                       const bool   enabled,
                                                                       const double inputVoltage,
                                                                       const double inputPower,
                                                                       const double setpoint)
    :
    GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue),
    mEnabled(enabled),
    mInputVoltage(inputVoltage),
    mInputPower(inputPower),
    mSetpoint(setpoint)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Electrical Converter Output link input data destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterOutputInputData::~GunnsElectConverterOutputInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy.
///
/// @details  Electrical Converter Output link input data copy constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterOutputInputData::GunnsElectConverterOutputInputData(
        const GunnsElectConverterOutputInputData& that)
    :
    GunnsBasicLinkInputData(that),
    mEnabled(that.mEnabled),
    mInputVoltage(that.mInputVoltage),
    mInputPower(that.mInputPower),
    mSetpoint(that.mSetpoint)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Electrical Converter Output link default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterOutput::GunnsElectConverterOutput()
    :
    GunnsBasicLink(NPORTS),
    mRegulatorType(),
    mOutputConductance(0.0),
    mConverterEfficiency(0.0),
    mOutputVoltageSensor(0),
    mOutputCurrentSensor(0),
    mInputLink(0),
    mEnableLimiting(false),
    mStateFlipsLimit(0),
    mEnabled(false),
    mInputVoltage(0.0),
    mInputVoltageValid(false),
    mOutputPowerAvailable(false),
    mSetpoint(0.0),
    mResetTrips(false),
    mInputPower(0.0),
    mInputPowerValid(false),
    mOutputChannelLoss(0.0),
    mLoadResistance(0.0),
    mTotalPowerLoss(0.0),
    mOutputOverVoltageTrip(),
    mOutputUnderVoltageTrip(),
    mOutputOverCurrentTrip(),
    mLeadsInterface(false),
    mReverseBiasState(false),
    mReverseBiasFlips(0),
    mLimitState(NO_LIMIT),
    mLimitStateFlips(0),
    mSourceVoltage(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Electrical Converter Output link default destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterOutput::~GunnsElectConverterOutput()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--) Reference to the link config data.
/// @param[in]      inputData    (--) Reference to the link input data.
/// @param[in,out]  networkLinks (--) Reference to the network links vector.
/// @param[in]      port0        (--) Port 0 default node mapping.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Electrical Converter Output link with configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterOutput::initialize(      GunnsElectConverterOutputConfigData& configData,
                                           const GunnsElectConverterOutputInputData&  inputData,
                                           std::vector<GunnsBasicLink*>&              networkLinks,
                                           const int                                  port0)
{
    /// - Initialize the parent class.
    int ports[1] = {port0};
    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Validate initialization.
    validate(configData, inputData);

    /// - Initialize from configuration and input data.
    mRegulatorType       = configData.mRegulatorType;
    mOutputConductance   = configData.mOutputConductance;
    mConverterEfficiency = configData.mConverterEfficiency;
    mEnableLimiting      = configData.mEnableLimiting;
    mStateFlipsLimit     = configData.mStateFlipsLimit;
    if (configData.mInputLink) {
        mInputLink = configData.mInputLink;
        mInputLink->registerOutputLink(this);
        mInputLink->checkNodeList(mNodeList);
    } else {
        mInputLink = 0;
    }
    if (configData.mOutputVoltageSensor) {
        mOutputVoltageSensor = &configData.mOutputVoltageSensor->mSensor;
        configData.mOutputVoltageSensor->setStepPreSolverFlag(false);
        configData.mOutputVoltageSensor->setStepPostSolverFlag(true);
    }
    if (configData.mOutputCurrentSensor) {
        mOutputCurrentSensor = &configData.mOutputCurrentSensor->mSensor;
        configData.mOutputCurrentSensor->setStepPreSolverFlag(false);
        configData.mOutputCurrentSensor->setStepPostSolverFlag(true);
    }
    mOutputOverVoltageTrip .initialize(configData.mOutputOverVoltageTripLimit,  configData.mTripPriority, false);
    mOutputUnderVoltageTrip.initialize(configData.mOutputUnderVoltageTripLimit, configData.mTripPriority, false);
    mOutputOverCurrentTrip .initialize(configData.mOutputOverCurrentTripLimit,  configData.mTripPriority, false);

    mEnabled      = inputData.mEnabled;
    mInputVoltage = inputData.mInputVoltage;
    mInputPower   = inputData.mInputPower;
    mSetpoint     = inputData.mSetpoint;

    /// - Initialize remaining state.
    mOutputPowerAvailable = (mInputVoltage > 0.0);
    mResetTrips           = false;
    mOutputChannelLoss    = 0.0;
    mTotalPowerLoss       = 0.0;
    mLeadsInterface       = false;
    mInputVoltageValid    = true;
    mInputPowerValid      = true;
    mReverseBiasState     = (VOLTAGE == mRegulatorType) and (mSetpoint < mNodes[0]->getPotential());
    mLimitState           = NO_LIMIT;
    mLimitStateFlips      = 0;

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  nodeList  (1)  Pointer to a node list.
///
/// @throws   TsInitializationException
///
/// @details  If this link has been initialized, then compare the given node list nodes array to
///           ours, and throw an exception if they differ.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterOutput::checkNodeList(GunnsNodeList* nodeList)
{
    if (mInitFlag) {
        if (nodeList->mNodes != mNodeList->mNodes) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "input and output converter links are not in the same network.");
        }
        /// - Since we've already finished our initialization when this method was called, we lead
        ///   the interface.
        mLeadsInterface = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--) Reference to the link config data.
/// @param[in]      inputData    (--) Reference to the link input data.
///
/// @throws   TsInitializationException
///
/// @details  Validates this Electrical Converter Output link configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterOutput::validate(const GunnsElectConverterOutputConfigData& configData,
                                         const GunnsElectConverterOutputInputData&  inputData) const
{
    /// - Issue an error on output conductance too low.
    if (configData.mOutputConductance < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "output conductance < DBL_EPSILON.");
    }

    /// - Issue an error on efficiency too low.
    if (configData.mConverterEfficiency < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "converter efficiency < DBL_EPSILON.");
    }

    /// - Issue an error on efficiency too high.
    if (configData.mConverterEfficiency > 1.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "converter efficiency > 1.");
    }

    /// - Issue an error on input voltage too low.
    if (inputData.mInputVoltage < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "input voltage < 0.");
    }

    /// - Issue an error on setpoint too low.
    if (inputData.mSetpoint < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "setpoint < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterOutput::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-checkpointed and non-config data.
    mInputVoltageValid = true;
    mResetTrips        = false;
    mInputPowerValid   = true;
    mOutputChannelLoss = 0.0;
    mReverseBiasState  = false;
    mReverseBiasFlips  = 0;
    mLimitStateFlips   = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step.
///
/// @details  Updates the link admittance matrix and source vector contributions to the system of
///           equations based on converter state, load and load type.  Update sensors with the
///           timestep to advance their drift malfunction.  Then call minorStep for the main update
///           functions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterOutput::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    if (mResetTrips) {
        mResetTrips = false;
        resetTrips();
    }
    mReverseBiasFlips = 0;
    mLimitStateFlips  = 0;

    minorStep(0.0, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt         (s)   Not used.
/// @param[in]  minorStep  (--)  Not used.
///
/// @details  Repeats the step function for minor steps in a non-linear network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterOutput::minorStep(const double dt __attribute__((unused)), const int minorStep __attribute__((unused)))
{
    mSourceVoltage = 0.0;
    if (mNodeMap[0] == getGroundNodeIndex()) {
        /// - Skip processing when on the Ground node.
        mInputVoltage        = 0.0;
        mInputPower          = 0.0;
        mAdmittanceMatrix[0] = 0.0;
        mSourceVector[0]     = 0.0;

    } else {
        computeInputPower(mInputPower);

        /// - If we precede the pointed-to input link, drive the interface with it.  Otherwise we
        ///   expect the interface to be driven by the input link or by other means.
        if (mLeadsInterface) {
            mInputVoltageValid = mInputLink->computeInputVoltage(mInputVoltage);
            mInputLink->setInputPower(mInputPower);
        } else if (mInputLink) {
            mInputVoltageValid = mInputLink->getInputVoltageValid();
        } else {
            mInputVoltageValid = true;
        }

        /// - Update the power available flag on the first minor step of each major step.  This is
        ///   used for all subsequent minor steps in this major step.  If we lead the interface,
        ///   then we're getting the input voltage from the input channel from the end of the last
        ///   major step.  If we don't lead the interface, then the input channel has already set
        ///   our input voltage before getting here in minor step 1.  Either way, we use this to
        ///   to determine whether this output channel is powered for this entire major step.
        if (1 == minorStep) {
            mOutputPowerAvailable = (mInputVoltage > 0.0);
        }

        /// - Set link conductance and source effects based on the load type.
        /// - Blockage malfunction reduces conductance in voltage source modes, and current in
        ///   current source modes.
        /// - The current and power source modes include a small conductance to Ground to help
        ///   the network converge when the downstream circuit is open-circuited.  This conductance
        ///   is not blocked by the blockage malfunction.
        estimateLoad();
        double conductance   = 0.0;
        double sourceCurrent = 0.0;
        mSourceVoltage       = 0.0;
        if (mEnabled and mOutputPowerAvailable and not isAnyTrips()) {
            computeRegulationSources(conductance, mSourceVoltage, sourceCurrent);
        }

        /// - When in the reverse bias state, zero conductance to prevent negative current.
        if (mReverseBiasState) {
            conductance = 0.0;
        }

        /// - Build the admittance matrix and source vector.
        conductance = MsMath::limitRange(0.0, conductance, mConductanceLimit);
        if (fabs(mAdmittanceMatrix[0] - conductance) > 0.0) {
            mAdmittanceMatrix[0] = conductance;
            mAdmittanceUpdate    = true;
        }
        mSourceVector[0] = mSourceVoltage * conductance + sourceCurrent;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] conductance (1/ohm) Returned value of link conductance.
/// @param[out] voltage     (V)     Returned value of source voltage.
/// @param[out] current     (amp)   Returned value of source current.
///
/// @details  Computes the source effects of the link (conductance, voltage, current) based on
///           regulation type and limiting state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterOutput::computeRegulationSources(double& conductance, double& voltage, double& current)
{
    if (isVoltageRegulator()) {
        if (LIMIT_OC == mLimitState) {
            current     = mOutputOverCurrentTrip.getLimit();
            conductance = FLT_EPSILON;
        } else {
            conductance    = applyBlockage(mOutputConductance);
            mSourceVoltage = mSetpoint;
            if (TRANSFORMER == mRegulatorType) {
                mSourceVoltage *= mInputVoltage;
            }
        }
    } else {
        if (LIMIT_OV == mLimitState) {
            conductance    = applyBlockage(mOutputConductance);
            mSourceVoltage = mOutputOverVoltageTrip.getLimit();
        } else if (LIMIT_UV == mLimitState) {
            conductance    = applyBlockage(mOutputConductance);
            mSourceVoltage = mOutputUnderVoltageTrip.getLimit();
        } else {
            conductance = FLT_EPSILON;
            if (CURRENT == mRegulatorType) {
                if (LIMIT_OC == mLimitState) {
                    current = applyBlockage(mOutputOverCurrentTrip.getLimit());
                } else {
                    current = applyBlockage(mSetpoint);
                }
            } else if (mSetpoint > 0.0 and mLoadResistance > 0.0) {
                current = applyBlockage(sqrt(mSetpoint / mLoadResistance));
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Not used.
///
/// @details  Method for computing the flows across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterOutput::computeFlows(const double dt __attribute__((unused)))
{
    if (mNodeMap[0] == getGroundNodeIndex()) {
        /// - Skip processing when on the Ground node.
        mFlux          = 0.0;
        mPotentialDrop = 0.0;
    } else {
        computeFlux();
        mPotentialDrop = -mPotentialVector[0];
        if (mFlux > 0.0) {
            mNodes[0]->collectInflux(mFlux);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] convergedStep (--) The count of minor steps since the network last converged.
/// @param[in] absoluteStep  (--) Not used.
///
/// @returns  SolutionResult  (--)  Whether this link confirms or rejects the network solution.
///
/// @details  This method determines whether to accept or reject the network solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult GunnsElectConverterOutput::confirmSolutionAcceptable(
        const int convergedStep, const int absoluteStep __attribute__((unused)))
{
    ///   Always confirm the solution and reset power valid flag when on the Ground node.
    GunnsBasicLink::SolutionResult result = CONFIRM;
    if (mNodeMap[0] == getGroundNodeIndex()) {
        mInputPowerValid = false;
    } else {

        updateBias(result, convergedStep, false);

        /// - After the network as converged, compute currents, powers and check for trips.
        if ((convergedStep > 0) and (REJECT != result)) {
            computeInputPower(mInputPower);

            /// - Sensors are optional; if a sensor exists then the trip uses its sensed value of
            ///   the truth parameter, otherwise the trip looks directly at the truth parameter.
            float sensedVout = MsMath::limitRange(-FLT_MAX, mPotentialVector[0], FLT_MAX);
            float sensedIout = MsMath::limitRange(-FLT_MAX, mFlux,               FLT_MAX);

            /// - Note that since we step the sensors without a time-step, its drift malfunction
            ///   isn't integrated.  This is because we don't have the time-step in this function,
            ///   and we must update the sensor multiple times per major network step, which would
            ///   repeat the drift integration too many times.  The result of all this is that drift
            ///   lags behind by one major step for causing trips.
            if (mOutputVoltageSensor) {
                sensedVout = mOutputVoltageSensor->sense(0.0, true, sensedVout);
            }
            if (mOutputCurrentSensor) {
                sensedIout = mOutputCurrentSensor->sense(0.0, true, sensedIout);
            }

            /// - Check all trip logics for trips.  If any trip, reject the solution.  Note that
            ///   a trip priority of 1 should not be used whenever limiting is enabled, otherwise
            ///   you can get a false trip along with changing the limiting state.
            if (mEnabled) {
                if (not (mEnableLimiting and not isVoltageRegulator())) {
                    mOutputOverVoltageTrip .checkForTrip(result, sensedVout, convergedStep);
                    mOutputUnderVoltageTrip.checkForTrip(result, sensedVout, convergedStep);
                }
                if (not mEnableLimiting) {
                    mOutputOverCurrentTrip.checkForTrip(result, sensedIout, convergedStep);
                }
            }
            const bool noReverseBias = updateLimitState(result, sensedVout, sensedIout);

            updateBias(result, convergedStep, noReverseBias);
        }

        mInputPowerValid = (REJECT != result);

        /// - Reject the solution if the voltage value from the input link is invalid.  This
        ///   happens when we lead the interface, the input link rejected the network solution on
        ///   the previous minor step and hasn't computed a valid voltage for this minor step yet.
        if (mInputLink) {
            mInputVoltageValid = mInputLink->getInputVoltageValid();
        }
        if (not mInputVoltageValid) {
            result = REJECT;
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] result        (--) The output solution confirm or reject result.
/// @param[in]  convergedStep (--) The converged minor step number.
/// @param[in]  noReverseBias (--) When true, prevents entering reverse bias state.
///
/// @details  Updates the forward/reverse bias state of this converter and rejects the solution
///           result if it changed.  Bias is only allowed to flip from forward to reverse on a
///           converged minor, but it can flip from reverse to forward on any minor step.  Flips
///           to reverse bias can only happen a limited number of times per major step, to prevent
///           repeating oscillation between bias states that could prevent network converging.
///           Flips to reverse bias are also prevented if the input argument is set.  This is used
///           when we are current limiting, so that the reverse bias logic does not interfere with
///           the limiting mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterOutput::updateBias(GunnsBasicLink::SolutionResult& result,
                                           const int                       convergedStep,
                                           const bool                      noReverseBias)
{
    const bool lastBias = mReverseBiasState;

    if (LIMIT_OC == mLimitState or (NO_LIMIT == mLimitState and not isVoltageRegulator())) {
        /// - Reverse bias is always false for any regulator type or limit state that is not acting
        ///   as a voltage regulator.
        mReverseBiasState = false;
    } else if (mPotentialVector[0] <= mSourceVoltage) {
        /// - Normally, reverse bias is false when the source voltage is greater than or equal to
        ///   the output node voltage.
        mReverseBiasState = false;
    } else if ((convergedStep > 0) and (mReverseBiasFlips < mStateFlipsLimit)
            and not (mReverseBiasState or noReverseBias)) {
        /// - Flip to reverse bias if all conditions are met, and increment the counter.
        mReverseBiasState = true;
        mReverseBiasFlips++;
    }

    if (lastBias != mReverseBiasState) {
        mInputPower = 0.0;
        result      = REJECT;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] result  (--)  Network solution result.
/// @param[in]  state   (--)  Limit state value to set.
///
/// @details  This always rejects the solution result.  This sets the new limit state to the given
///           value, and sets the limit state flipped flag if the given state is a limiting state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterOutput::rejectWithLimitState(GunnsBasicLink::SolutionResult& result,
                                                     const LimitStates               state)
{
    result      = REJECT;
    mLimitState = state;
    if (NO_LIMIT != state) {
        mLimitStateFlips++;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] result  (--)  Network solution result.
/// @param[in]  voltage (V)   Output electrical voltage value.
/// @param[in]  current (amp) Output electrical current value.
///
/// @returns  bool (--) Whether transition to reverse bias state is prevented following this update.
///
/// @details  Updates the limit state and rejects the network solution when limit state changes.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsElectConverterOutput::updateLimitState(GunnsBasicLink::SolutionResult& result,
                                                 const float                     voltage,
                                                 const float                     current)
{
    bool noReverseBias = false;
    if (mEnabled and mEnableLimiting) {
        if (isVoltageRegulator()) {
            updateCurrentLimitState(result, voltage, current);
        } else {
            noReverseBias = updateVoltageLimitState(result, voltage, current);
        }
    } else {
        mLimitState = NO_LIMIT;
    }
    return noReverseBias;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] result  (--)  Network solution result.
/// @param[in]  voltage (V)   Output electrical voltage value.
/// @param[in]  current (amp) Output electrical current value.
///
/// @details Updates the current-limiting state for voltage-type regulators and rejects the network
///          solution when the state changes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterOutput::updateCurrentLimitState(GunnsBasicLink::SolutionResult& result,
                                                        const float                     voltage,
                                                        const float                     current)
{
    /// - Voltage regulators can only LIMIT_OC.
    const bool canOcLimit = mOutputOverCurrentTrip.getLimit() > 0.0 and mLimitStateFlips < mStateFlipsLimit;

    if (LIMIT_OC == mLimitState and voltage > computeVoltageControlSetpoint()) {
        rejectWithLimitState(result, NO_LIMIT); /// - LIMIT_OC to NO_LIMIT transition.
    } else if (NO_LIMIT == mLimitState and canOcLimit and current > mOutputOverCurrentTrip.getLimit()) {
        rejectWithLimitState(result, LIMIT_OC); /// - NO_LIMIT to LIMIT_OC transition.
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] result  (--)  Network solution result.
/// @param[in]  voltage (V)   Output electrical voltage value.
/// @param[in]  current (amp) Output electrical current value.
///
/// @returns  bool (--) Whether transition to reverse bias state is prevented following this update.
///
/// @details Updates the current-limiting state for voltage-type regulators and rejects the network
///          solution when the state changes.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsElectConverterOutput::updateVoltageLimitState(GunnsBasicLink::SolutionResult& result,
                                                        const float                     voltage,
                                                        const float                     current)
{
    bool noReverseBias = false;

    /// - Current regulators can only LIMIT_OC from the LIMIT_UV state.  In this case, they go from
    ///   their normal current regulation (NO_LIMIT) to LIMIT_UV when the output voltage drops too
    ///   low, then if demand keeps pulling output voltage down and current up, we can reach the OC
    ///   limit.
    const bool canOvLimit = mOutputOverVoltageTrip.getLimit()  > 0.0 and mLimitStateFlips < mStateFlipsLimit;
    const bool canUvLimit = mOutputUnderVoltageTrip.getLimit() > 0.0 and mLimitStateFlips < mStateFlipsLimit;
    const bool canOcLimit = mOutputOverCurrentTrip.getLimit()  > 0.0 and mLimitStateFlips < mStateFlipsLimit
                            and canUvLimit;

    if (LIMIT_OC == mLimitState) {
        if (canOvLimit and voltage > mOutputOverVoltageTrip.getLimit()) {
            /// - LIMIT_OC to LIMIT_OV transition due to output over-voltage.
            rejectWithLimitState(result, LIMIT_OV);
        } else if (voltage > mOutputUnderVoltageTrip.getLimit()) {
            /// - LIMIT_OC to NO_LIMIT transition due to output voltage restored.
            rejectWithLimitState(result, NO_LIMIT);
        }
    } else if (LIMIT_UV == mLimitState) {
        if (canOcLimit and current > mOutputOverCurrentTrip.getLimit()) {
            /// - LIMIT_UV to LIMIT_OC transition due to output over-current.
            rejectWithLimitState(result, LIMIT_OC);
        } else if (canOvLimit and voltage > mOutputOverVoltageTrip.getLimit()) {
            /// - LIMIT_UV to LIMIT_OV transition due to output over-voltage.
            rejectWithLimitState(result, LIMIT_OV);
        } else if ((voltage > mOutputUnderVoltageTrip.getLimit()) or (current < computeCurrentControlSetpoint())) {
            /// - LIMIT_UV to NO_LIMIT transition due to output voltage or current restored.
            rejectWithLimitState(result, NO_LIMIT);
        }
    } else if (LIMIT_OV == mLimitState) {
        if (canUvLimit and voltage < mOutputUnderVoltageTrip.getLimit()) {
            /// - LIMIT_OV to LIMIT_UV transition due to output under-voltage.
            rejectWithLimitState(result, LIMIT_UV);
        } else {
            const float setpoint = computeCurrentControlSetpoint();
            if ((current > setpoint) or (0.0 == setpoint)) {
                /// - LIMIT_OV to LIMIT_UV transition due to output current exceeds setpoint.
                ///   Rather than going directly to NO_LIMIT, we transition to LIMIT_UV first, and
                ///   afterwards may transition to NO_LIMIT.
                rejectWithLimitState(result, LIMIT_UV);
            }
        }
    } else if (NO_LIMIT == mLimitState) {
        /// - When transitioning from a current source to a voltage-limiting source, we return a
        ///   flag to prevent us from going into the reverse bias state immediately after the
        ///   transition, to allow the voltage source to attempt control the output voltage on the
        ///   next minor step.  We may still switch to reverse bias after that attempt.
        if (canOvLimit and voltage > mOutputOverVoltageTrip.getLimit()) {
            /// - NO_LIMIT to LIMIT_OV transition due to output over-voltage.
            rejectWithLimitState(result, LIMIT_OV);
            noReverseBias = true;
        } else if (canUvLimit and voltage < mOutputUnderVoltageTrip.getLimit()) {
            /// - NO_LIMIT to LIMIT_UV transition due to output under-voltage.
            rejectWithLimitState(result, LIMIT_UV);
            noReverseBias = true;
        }
    }

    return noReverseBias;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  float (V) The effective output voltage control setpoint.
///
/// @details  Computes and returns what the effective output voltage control setpoint would be for
///           a voltage regulator or transformer that is not current limiting.
///
/// @note     This should only be called if regulator type is VOLTAGE or TRANSFORMER.
////////////////////////////////////////////////////////////////////////////////////////////////////
float GunnsElectConverterOutput::computeVoltageControlSetpoint()
{
    float setpoint = mSetpoint;
    if (TRANSFORMER == mRegulatorType) setpoint *= mInputVoltage;
    return setpoint;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  float (amp) The effective output current control setpoint.
///
/// @details  Computes and returns what the effective output current control setpoint would be for
///           a current or power regulator that is not voltage limiting.
///
/// @note     This should only be called if regulator type is CURRENT or POWER.
////////////////////////////////////////////////////////////////////////////////////////////////////
float GunnsElectConverterOutput::computeCurrentControlSetpoint()
{
    float setpoint = 0.0;
    if (CURRENT == mRegulatorType) {
        setpoint = applyBlockage(mSetpoint);
    } else if (mSetpoint > 0.0 and mLoadResistance > 0.0) {
        setpoint = applyBlockage(sqrt(mSetpoint / mLoadResistance));
    }
    return setpoint;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out]  inputPower  (W)  Input channel power load based on load value type.
///
/// @returns  bool  (--)  Whether the returned input power value is valid.
///
/// @details  Updates output current, output power, power losses, input power and input load value
///           based on the load value type.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsElectConverterOutput::computeInputPower(double& inputPower)
{
    /// - Zero outputs if the last minor step solution was invalid or we are on the Ground node.
    if (mNodeMap[0] == getGroundNodeIndex() or not mInputPowerValid) {
        mPower             = 0.0;
        mOutputChannelLoss = 0.0;
        inputPower         = 0.0;
        mTotalPowerLoss    = 0.0;

    } else {
        /// - mPower is the power delivered to the downstream node, and doesn't include losses
        ///   in the voltage converter or through converter output channel resistance.
        computeFlux();
        mPower = mFlux * mPotentialVector[0];

        /// - Power dissipated due to the output current through the output channel resistance.
        mOutputChannelLoss = mFlux * mFlux / fmax(DBL_EPSILON, mOutputConductance);

        /// - Input power due to efficiency of the voltage conversion.
        inputPower = (mPower + mOutputChannelLoss) / MsMath::limitRange(DBL_EPSILON, mConverterEfficiency, 1.0);

        /// - Total power lost due to conversion and output channel resistance; this can also be
        ///   used as the waste heat generated by the converter.
        mTotalPowerLoss = inputPower - mPower;
    }
    return mInputPowerValid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (V)  The controlled voltage value.
///
/// @details  Returns what would be the ideal regulated voltage in this regulator's current state.
///           Returns zero if for any reason, that this couldn't regulate the node voltage,
///           Returns zero if a voltage regulator is current-limiting.  Returns the effective
///           voltage control point of a current regulator that is voltage-limiting.  Ignores the
///           reverse bias state for voltage regulators, as we want the voltage control point of a
///           voltage regulator regardless of forward/reverse bias.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsElectConverterOutput::getControlVoltage() const
{
    double result = 0.0;
    if (applyBlockage(mOutputConductance) >= DBL_EPSILON) {
        /// - mSource voltage already accounts for enabled, trips, regulator type and limit state,
        ///   output power available, so we only have to account for the output conductance affected
        ///   by blockage malf.
        result = mSourceVoltage;

        /// - This accounts for a current reg that could undervolt limit.  If it is actually in the
        ///   overvolt limit state now, then its source voltage is already accounted for above.
        if (not isVoltageRegulator() and mEnableLimiting and LIMIT_OV != mLimitState) {
            result = std::max(mSourceVoltage, static_cast<double>(mOutputUnderVoltageTrip.getLimit()));
        }
    }
    return result;
}
