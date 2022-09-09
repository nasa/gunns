/**
@file
@brief    GUNNS Electrical Converter Output Link implementation

@copyright Copyright 2022 United States Government as represented by the Administrator of the
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
/// @param[in] enableCurrentLimiting       (--)    Limits output current instead of over-current tripping.
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
        const bool                                     enableCurrentLimiting,
        const float                                    outputUnderVoltageTripLimit)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mRegulatorType(regulatorType),
    mOutputConductance(outputConductance),
    mConverterEfficiency(converterEfficiency),
    mOutputVoltageSensor(outputVoltageSensor),
    mOutputCurrentSensor(outputCurrentSensor),
    mTripPriority(tripPriority),
    mOutputOverVoltageTripLimit(outputOverVoltageTripLimit),
    mOutputOverCurrentTripLimit(outputOverCurrentTripLimit),
    mInputLink(inputLink),
    mEnableCurrentLimiting(enableCurrentLimiting),
    mOutputUnderVoltageTripLimit(outputUnderVoltageTripLimit)
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
    mOutputConductance(that.mOutputConductance),
    mConverterEfficiency(that.mConverterEfficiency),
    mOutputVoltageSensor(that.mOutputVoltageSensor),
    mOutputCurrentSensor(that.mOutputCurrentSensor),
    mTripPriority(that.mTripPriority),
    mOutputOverVoltageTripLimit(that.mOutputOverVoltageTripLimit),
    mOutputOverCurrentTripLimit(that.mOutputOverCurrentTripLimit),
    mInputLink(that.mInputLink),
    mEnableCurrentLimiting(that.mEnableCurrentLimiting),
    mOutputUnderVoltageTripLimit(that.mOutputUnderVoltageTripLimit)
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
    mEnableCurrentLimiting(false),
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
    mSolutionReset(false),
    mBiasFlippedReverse(false),
    mCurrentLimitingState(false),
    mCurrentLimitFlipped(false),
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
    mRegulatorType         = configData.mRegulatorType;
    mOutputConductance     = configData.mOutputConductance;
    mConverterEfficiency   = configData.mConverterEfficiency;
    mEnableCurrentLimiting = configData.mEnableCurrentLimiting;
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
    mCurrentLimitingState = false;
    mCurrentLimitFlipped  = false;

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
    mInputVoltageValid   = true;
    mResetTrips          = false;
    mInputPowerValid     = true;
    mOutputChannelLoss   = 0.0;
    mReverseBiasState    = false;
    mSolutionReset       = false;
    mCurrentLimitFlipped = false;
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
    mBiasFlippedReverse  = false;
    mCurrentLimitFlipped = false;

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
        mInputPowerValid = true;
        computeInputPower(mInputPower);
        mSolutionReset = false;

        /// - If we precede the pointed-to input link, drive the interface with it.  Otherwise we
        ///   expect the interface to be driven by the input link or by other means.
        if (mLeadsInterface) {
            mInputVoltageValid = mInputLink->computeInputVoltage(mInputVoltage);
            mInputLink->setInputPower(mInputPower);
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
        if (mEnabled and mOutputPowerAvailable
                and not (mOutputOverVoltageTrip.isTripped() or mOutputOverCurrentTrip.isTripped() or
                         mOutputUnderVoltageTrip.isTripped())) {
            switch (mRegulatorType) {
                case (CURRENT) :
                    sourceCurrent  = applyBlockage(mSetpoint);
                    conductance    = FLT_EPSILON;
                    break;
                case (POWER) :
                    if (mSetpoint > 0.0 and mLoadResistance > 0.0) {
                        sourceCurrent = applyBlockage(sqrt(mSetpoint / mLoadResistance));
                    }
                    conductance = FLT_EPSILON;
                    break;
                case (TRANSFORMER) :
                    if (mCurrentLimitingState) {
                        sourceCurrent = mOutputOverCurrentTrip.getLimit();
                        conductance   = FLT_EPSILON;
                    } else {
                        conductance    = applyBlockage(mOutputConductance);
                        mSourceVoltage = mInputVoltage * mSetpoint;
                    }
                    break;
                default :    // VOLTAGE
                    if (mCurrentLimitingState) {
                        sourceCurrent = mOutputOverCurrentTrip.getLimit();
                        conductance   = FLT_EPSILON;
                    } else {
                        conductance    = applyBlockage(mOutputConductance);
                        mSourceVoltage = mSetpoint;
                    }
                    break;
            }
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
    GunnsBasicLink::SolutionResult result = CONFIRM;
    mSolutionReset = false;

    ///   Always confirm when on the Ground node.
    if (mNodeMap[0] != getGroundNodeIndex()) {

        /// - If the voltage bias switched states in either direction, reject the solution and
        ///   start over.  Zero the input power for the next minor step because this power value
        ///   was invalid.
        if (updateBias()) {
            mInputPower = 0.0;
            result      = REJECT;
        }

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
            ///   a trip priority of 1 should not be used whenever current limiting is enabled.
            ///   If it is set to 1, then you'll get an over-voltage trip along with leaving
            ///   the current limiting state.
            if (mEnabled) {
                mOutputOverVoltageTrip .checkForTrip(result, sensedVout, convergedStep);
                mOutputUnderVoltageTrip.checkForTrip(result, sensedVout, convergedStep);
                if (not mEnableCurrentLimiting) {
                    mOutputOverCurrentTrip.checkForTrip(result, sensedIout, convergedStep);
                }
            }

            /// - Reject the solution when current limiting state changes.  Exit the limiting
            ///   state if output voltage is over the setpoint.  Enter the limiting state if
            ///   output current is over the limit.  To prevent endless state oscillating in
            ///   feedback with the rest of the circuit, and to prevent getting stuck in the
            ///   current limiting (current source) state when the circuit is over-volted,
            ///   only allow one flip to the current limiting state per major step.
            if (mEnableCurrentLimiting and not mCurrentLimitFlipped) {
                if (mCurrentLimitingState) {
                    if (sensedVout > mSetpoint) {
                        mCurrentLimitingState = false;
                        result                = REJECT;
                    }
                } else {
                    if (sensedIout > mOutputOverCurrentTrip.getLimit()) {
                        mCurrentLimitingState = true;
                        mCurrentLimitFlipped  = true;
                        result                = REJECT;
                    }
                }
            }
        }
        mInputPowerValid = (REJECT != result);

        /// - Reject the solution if the voltage value from the input link is invalid.  This
        ///   happens when we lead the interface, the input link rejected the network solution on
        ///   the previous minor step and hasn't computed a valid voltage for this minor step yet.
        if (not mInputVoltageValid) {
            result = REJECT;
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) True if the bias changed direction during this update.
///
/// @details  Updates the forward/reverse bias state of this converter and returns true if it
///           changed.  Bias is only allowed to flip from forward to reverse at most once per major
///           network step, to avoid repeating oscillation between bias states that could prevent
///           network converging.  Also, we do not flip to reverse bias state if we are current
///           limiting, so that the reverse bias logic does not interferew with the limiting mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsElectConverterOutput::updateBias()
{
    const bool lastBias = mReverseBiasState;
    if (CURRENT == mRegulatorType or POWER == mRegulatorType or
            mSourceVoltage >= mPotentialVector[0]) {
        mReverseBiasState = false;
    } else if (not mBiasFlippedReverse and not mCurrentLimitingState) {
        mReverseBiasState   = true;
        mBiasFlippedReverse = true;
    }
    return (lastBias != mReverseBiasState);
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
    if (mSolutionReset or (mNodeMap[0] == getGroundNodeIndex())) {
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
