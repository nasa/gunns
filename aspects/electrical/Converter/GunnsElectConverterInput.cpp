/**
@file
@brief    GUNNS Electrical Converter Input Link implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsBasicLink.o)
    (core/GunnsSensorAnalogWrapper.o)
    (aspects/electrical/Converter/GunnsElectConverterOutput.o)
    (aspects/electrical/TripLogic/GunnsTripLogic.o)
    (software/exceptions/TsInitializationException.o))
*/

#include "GunnsElectConverterInput.hh"
#include "aspects/electrical/Converter/GunnsElectConverterOutput.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "math/MsMath.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                       (--) Link name
/// @param[in] nodes                      (--) Network nodes array
/// @param[in] inputVoltageSensor         (--) Pointer to the input voltage sensor spotter.
/// @param[in] inputCurrentSensor         (--) Pointer to the input current sensor spotter.
/// @param[in] tripPriority               (--) Priority of trips in the network.
/// @param[in] inputUnderVoltageTripLimit (--) Input under-voltage trip limit.
/// @param[in] inputOverVoltageTripLimit  (--) Input over-voltage trip limit.
/// @param[in] efficiencyTable            (--) Pointer to the converter efficiency vs. power fraction table.
///
/// @details  Default Electrical Converter Input link config data constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterInputConfigData::GunnsElectConverterInputConfigData(
        const std::string&        name,
        GunnsNodeList*            nodes,
        GunnsSensorAnalogWrapper* inputVoltageSensor,
        GunnsSensorAnalogWrapper* inputCurrentSensor,
        const unsigned int        tripPriority,
        const float               inputUnderVoltageTripLimit,
        const float               inputOverVoltageTripLimit,
        TsLinearInterpolator*     efficiencyTable)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mInputVoltageSensor(inputVoltageSensor),
    mInputCurrentSensor(inputCurrentSensor),
    mTripPriority(tripPriority),
    mInputUnderVoltageTripLimit(inputUnderVoltageTripLimit),
    mInputOverVoltageTripLimit(inputOverVoltageTripLimit),
    mEfficiencyTable(efficiencyTable)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Electrical Converter Input link config data destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterInputConfigData::~GunnsElectConverterInputConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Electrical Converter Input link config data copy constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterInputConfigData::GunnsElectConverterInputConfigData(
        const GunnsElectConverterInputConfigData& that)
    :
    GunnsBasicLinkConfigData(that),
    mInputVoltageSensor(that.mInputVoltageSensor),
    mInputCurrentSensor(that.mInputCurrentSensor),
    mTripPriority(that.mTripPriority),
    mInputUnderVoltageTripLimit(that.mInputUnderVoltageTripLimit),
    mInputOverVoltageTripLimit(that.mInputOverVoltageTripLimit),
    mEfficiencyTable(that.mEfficiencyTable)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Initial blockage malfunction activation flag.
/// @param[in] malfBlockageValue (--) Initial blockage maflunction value.
/// @param[in] enabled           (--) Initial operation enabled state.
/// @param[in] inputVoltage      (V)  Initial input voltage.
/// @param[in] inputPower        (W)  Initial input power load.
/// @param[in] referencePower    (W)  Initial reference power load for efficiency calculation.
///
/// @details  Default Electrical Converter Input link input data constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterInputInputData::GunnsElectConverterInputInputData(const bool   malfBlockageFlag,
                                                                     const double malfBlockageValue,
                                                                     const bool   enabled,
                                                                     const double inputVoltage,
                                                                     const double inputPower,
                                                                     const double referencePower)
    :
    GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue),
    mEnabled(enabled),
    mInputVoltage(inputVoltage),
    mInputPower(inputPower),
    mReferencePower(referencePower)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Electrical Converter Input link input data destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterInputInputData::~GunnsElectConverterInputInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Electrical Converter Input link input data copy constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterInputInputData::GunnsElectConverterInputInputData(
        const GunnsElectConverterInputInputData& that)
    :
    GunnsBasicLinkInputData(that),
    mEnabled(that.mEnabled),
    mInputVoltage(that.mInputVoltage),
    mInputPower(that.mInputPower),
    mReferencePower(that.mReferencePower)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Electrical Converter Input link default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterInput::GunnsElectConverterInput()
    :
    GunnsBasicLink(NPORTS),
    mInputVoltageSensor(0),
    mInputCurrentSensor(0),
    mEfficiencyTable(0),
    mOutputLink(0),
    mEnabled(false),
    mInputPower(0.0),
    mInputPowerValid(false),
    mResetTrips(false),
    mReferencePower(0.0),
    mInputVoltage(0.0),
    mInputVoltageValid(false),
    mInputUnderVoltageTrip(),
    mInputOverVoltageTrip(),
    mConverterEfficiency(0.0),
    mTotalPowerLoss(0.0),
    mLeadsInterface(false),
    mOverloadedState(false),
    mLastOverloadedState(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Electrical Converter Input link default destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectConverterInput::~GunnsElectConverterInput()
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
/// @details  Initializes this Electrical Converter Input link with configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterInput::initialize(      GunnsElectConverterInputConfigData& configData,
                                          const GunnsElectConverterInputInputData&  inputData,
                                          std::vector<GunnsBasicLink*>&             networkLinks,
                                          const int                                 port0)
{
    /// - Initialize the parent class.
    int ports[1] = {port0};
    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Validate initialization.
    validate(configData, inputData);
    if (mOutputLink) {
        mOutputLink->checkNodeList(mNodeList);
    }

    /// - Initialize from configuration and input data.
    if (configData.mInputVoltageSensor) {
        mInputVoltageSensor = &configData.mInputVoltageSensor->mSensor;
        configData.mInputVoltageSensor->setStepPreSolverFlag(false);
        configData.mInputVoltageSensor->setStepPostSolverFlag(true);
    }
    if (configData.mInputCurrentSensor) {
        mInputCurrentSensor = &configData.mInputCurrentSensor->mSensor;
        configData.mInputCurrentSensor->setStepPreSolverFlag(false);
        configData.mInputCurrentSensor->setStepPostSolverFlag(true);
    }
    mInputUnderVoltageTrip.initialize(configData.mInputUnderVoltageTripLimit, configData.mTripPriority, false);
    mInputOverVoltageTrip .initialize(configData.mInputOverVoltageTripLimit,  configData.mTripPriority, false);
    mEfficiencyTable = configData.mEfficiencyTable;
    mEnabled         = inputData.mEnabled;
    mInputVoltage    = inputData.mInputVoltage;
    mInputPower      = inputData.mInputPower;
    mReferencePower  = inputData.mReferencePower;

    /// - Initialize remaining state.
    mResetTrips          = false;
    mLeadsInterface      = false;
    mOverloadedState     = false;
    mLastOverloadedState = false;
    mInputVoltageValid   = true;
    mInputPowerValid     = true;
    mConverterEfficiency = 1.0;
    mTotalPowerLoss      = 0.0;
    mNodes[0]->setPotential(mInputVoltage);

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
void GunnsElectConverterInput::checkNodeList(GunnsNodeList* nodeList)
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
/// @param[in]      inputData    (--) Reference to the link input data (not used).
///
/// @throws   TsInitializationException
///
/// @details  Validates this Electrical Converter Input link configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterInput::validate(const GunnsElectConverterInputConfigData& configData,
                                        const GunnsElectConverterInputInputData&  inputData) const
{
    /// - Issue an error on backwards trip limits.
    if ( (configData.mInputUnderVoltageTripLimit > configData.mInputOverVoltageTripLimit) and
         (configData.mInputOverVoltageTripLimit != 0.0) ) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "input under-voltage trip limit > over-voltage limit.");
    }

    if (configData.mEfficiencyTable) {
        /// - Issue an error on table limits out of bounds of valid efficiency (DBL_EPSILON-1).
        ///   Check at every 10% power fraction.
        for (int i=0; i<11; ++i) {
            if (not MsMath::isInRange(DBL_EPSILON, configData.mEfficiencyTable->get(0.1 * i), 1.0)) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "some of the efficiency table is not in valid range (DBL_EPSILON-1)");
            }
        }

        /// - Issue an error if reference power not > 0 when the efficiency table is provided.
        if (inputData.mReferencePower < DBL_EPSILON) {
            GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                        "reference power < DBL_EPSILON while efficiency table is provided.");
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterInput::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-checkpointed and non-config data.
    mInputPowerValid     = true;
    mResetTrips          = false;
    mInputVoltageValid   = true;
    mOverloadedState     = false;
    mLastOverloadedState = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step.
///
/// @details  First step in a non-linear network.  We clear the overloaded state from last pass so
///           it can be re-checked this pass.  Update sensors with the timestep to advance their
///           drift malfunction.  Then call minorStep for the main update functions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterInput::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Note that once entering the overloaded state, even though we reset it here, we will
    ///   re-enter overloaded state this pass if the input power load has not been reset to zero.
    mOverloadedState = false;
    if (mResetTrips) {
        mResetTrips = false;
        resetTrips();
    }

    minorStep(0.0, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt         (s)   Not used.
/// @param[in]  minorStep  (--)  Not used.
///
/// @details  Minor step function in a non-linear network.  Updates the link admittance matrix and
///           source vector contributions to the system of equations based on converter state and
///           load.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterInput::minorStep(const double dt __attribute__((unused)), const int minorStep __attribute__((unused)))
{
    if (mNodeMap[0] == getGroundNodeIndex()) {
        /// - Skip processing when on the Ground node.
        mInputVoltage        = 0.0;
        mInputPower          = 0.0;
        mAdmittanceMatrix[0] = 0.0;
        mSourceVector[0]     = 0.0;
        mPotentialDrop       = 0.0;
        mFlux                = 0.0;
        mPower               = 0.0;

    } else {
        computeInputVoltage(mInputVoltage);

        /// - If we precede the pointed-to output link, drive the interface with it.  Otherwise we
        ///   expect the interface to be driven by the output link or by other means.  We get the
        ///   output link's solution reject flag - if set then the input power we got from it is
        ///   invalid, and we will reject this minorStep's solution.
        if (mLeadsInterface) {
            mInputPowerValid = mOutputLink->computeInputPower(mInputPower);
            mOutputLink->setInputVoltage(mInputVoltage);
        } else if (mOutputLink) {
            mInputPowerValid = mOutputLink->getInputPowerValid();
        } else {
            mInputPowerValid = true;
        }

        /// - Apply efficiency loss to the input power load.  This is optional, only applied when
        ///   the optional efficiency table is provided.  Efficiency is a lookup vs. ratio of
        ///   demanded power from the output side (mInputPower) to the reference power value.
        ///   This allows the reference power to change, as more converter cores are tied in
        ///   parallel, etc.
        double efficiency = 1.0;
        if (mEfficiencyTable) {
            const double powerFraction = mInputPower / std::max(DBL_EPSILON, mReferencePower);
            efficiency = mEfficiencyTable->get(powerFraction);
        }

        /// - The link blockage malfunction decreases efficiency towards zero.
        if (mMalfBlockageFlag) {
            efficiency *= 1.0 - MsMath::limitRange(0.0, mMalfBlockageValue, 1.0);
        }
        mConverterEfficiency = MsMath::limitRange(DBL_EPSILON, efficiency, 1.0);
        const double scaledInputLoad = mInputPower / mConverterEfficiency;

        /// - Total power lost due to conversion efficiency; this can also be used as the waste heat
        ///   generated by the converter.
        mTotalPowerLoss = scaledInputLoad - mInputPower;

        double current = 0.0;
        if (mEnabled and not (mOverloadedState or mInputOverVoltageTrip.isTripped()
                or mInputUnderVoltageTrip.isTripped())) {
            if (mPotentialVector[0] < 0.0) {
                /// - If node potential is negative, then hold the current source constant to allow
                ///   the network to converge.  If the network ends up converging at negative
                ///   voltage, then we'll either undervolt trip off, enter the overloaded state, or
                ///   reset this potential in resetLastMinorStep().
                current = -mSourceVector[0];
            } else if (mInputVoltage > DBL_EPSILON) {
                /// - For positive input voltage, set link current source effect to create the input
                ///   power load at the input voltage.
                current = scaledInputLoad / mInputVoltage;
            }
        }

        /// - Build the admittance matrix and source vector.  Admittance is always forced to zero
        ///   since this link is only ever a current source.
        if (mAdmittanceMatrix[0] != 0.0) {
            mAdmittanceMatrix[0] = 0.0;
            mAdmittanceUpdate    = true;
        }
        mSourceVector[0] = -current;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Not used.
///
/// @details  Method for computing the flows across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectConverterInput::computeFlows(const double dt __attribute__((unused)))
{
    if (mNodeMap[0] != getGroundNodeIndex()) {
        computeInputVoltage(mInputVoltage);
        mPotentialDrop = mPotentialVector[0];
        mFlux          = mPotentialVector[0] * mAdmittanceMatrix[0] - mSourceVector[0];
        mPower         = -mFlux * mPotentialVector[0];
        if (mFlux > 0.0) {
            mNodes[0]->collectOutflux(mFlux);
        }
        if (mInputCurrentSensor) {
            mInputCurrentSensor->sense(0.0, true, mFlux);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] convergedStep (--) The count of minor steps since the network last converged.
/// @param[in] absoluteStep  (--) Not used.
///
/// @returns  SolutionResult  (--)  Whether this link confirms or rejects the network solution.
///
/// @details  This method determines whether to accept or reject the converged network solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult GunnsElectConverterInput::confirmSolutionAcceptable(
        const int convergedStep, const int absoluteStep __attribute__((unused)))
{
    /// - Always confirm the solution and reset the voltage valid flag when on the Ground node.
    GunnsBasicLink::SolutionResult result = CONFIRM;
    if (mNodeMap[0] == getGroundNodeIndex()) {
        mInputVoltageValid = false;
    } else if (0 == convergedStep) {
        /// - We only check for solution rejection and state change after the network has converged.
        ///   Until it has converged, we assume hte input voltage is valid.
        mInputVoltageValid = true;
    } else {
        /// - Sensors are optional; if a sensor exists then the trip uses its sensed value of the
        ///   truth parameter, otherwise the trip looks directly at the truth parameter.
        float sensedVin = MsMath::limitRange(-FLT_MAX, mPotentialVector[0], FLT_MAX);

        /// - Note that since we step the sensors without a time-step, its drift malfunction isn't
        ///   integrated.  This is because we don't have the time-step in this function, and we must
        ///   update the sensor multiple times per major network step, which would repeat the drift
        ///   integration too many times.  The result of all this is that drift lags behind by one
        ///   major step for causing trips.
        if (mInputVoltageSensor) {
            sensedVin = mInputVoltageSensor->sense(0.0, true, sensedVin);
        }

        /// - Check all trip logics for trips.  If any trip, reject the solution.
        if (mEnabled) {
            mInputOverVoltageTrip .checkForTrip(result, sensedVin, convergedStep);
            mInputUnderVoltageTrip.checkForTrip(result, sensedVin, convergedStep);
        }

        /// - If the network converged on a negative input voltage but we didn't under-volt trip,
        ///   then reject and go to the overloaded state.
        if (CONFIRM == result and not
                (mInputOverVoltageTrip.isTripped() or mInputUnderVoltageTrip.isTripped() or mOverloadedState)) {
            computeInputVoltage(mInputVoltage);
            if (mInputPower > 0.0 and mPotentialVector[0] <= 0.0) {
                mOverloadedState = true;
                result           = REJECT;
                if (not mLastOverloadedState) {
                    GUNNS_WARNING("entered overloaded state.");
                }
            }
        }
        mLastOverloadedState = mOverloadedState;
        mInputVoltageValid = (REJECT != result);

        /// - Reject the solution if the power value from the output link is invalid.  This happens
        ///   when we lead the interface, the output link rejected the network solution on the
        ///   previous minor step and hasn't computed a valid power for this minor step yet.
        if (mOutputLink) {
            mInputPowerValid = mOutputLink->getInputPowerValid();
        }
        if (not mInputPowerValid) {
            result = REJECT;
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] convergedStep (--) The minor step number since network convergence.
/// @param[in] absoluteStep  (--) Unused.
///
/// @returns  bool (--) Always returns true.
///
/// @details  In the minorStep function, it will hold a constant current pulling negative input
///           potential to help the network converge, so that we (or some other link) can then
///           reject the solution.  However, we need a way to snap us out of that mode once the
///           solution has been rejected.  So, here we reset the potential vector to zero if we are
///           in such a condition.  This function is only called by the solver when the previous
///           minor step solution was rejected.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsElectConverterInput::resetLastMinorStep(const int convergedStep,
                                                  const int absoluteStep __attribute__((unused)))
{
    if (convergedStep > 0 and mPotentialVector[0] < 0.0) {
        mPotentialVector[0] = 0.0;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out]  inputVoltage  (V)  Input channel voltage.
///
/// @returns  bool  (--)  Whether the returned input voltage value is valid.
///
/// @details  Sets the input channel voltage as the input node voltage if the converter isn't
///           disabled, blocked or tripped on the input side, otherwise the input channel voltage is
///           zero.  If the node voltage is negative, the input voltage is set to zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsElectConverterInput::computeInputVoltage(double& inputVoltage)
{
    if (not mEnabled or mInputOverVoltageTrip.isTripped() or mInputUnderVoltageTrip.isTripped()
            or (mMalfBlockageFlag and (mMalfBlockageValue >= 1.0)) ) {
        inputVoltage = 0.0;
    } else {
        inputVoltage = fmax(0.0, mPotentialVector[0]);
    }
    return mInputVoltageValid;
}
