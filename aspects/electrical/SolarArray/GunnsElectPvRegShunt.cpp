/**
@file
@brief    GUNNS Electrical Photovoltaic Array Shunting Regulator Model implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
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

#include "GunnsElectPvRegShunt.hh"
#include "GunnsElectPvArray.hh"
#include "core/GunnsSensorAnalogWrapper.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] section (--) Array section number of this load order.
/// @param[in] string  (--) Section string number of this load order.
///
/// @details  Default constructs this Photovoltaic Array Shunting Regulator String Load Order.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvStringLoadOrder::GunnsElectPvStringLoadOrder(const unsigned int section,
                                                         const unsigned int string)
    :
    mSection(section),
    mString(string)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic Array Shunting Regulator String Load Order.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvStringLoadOrder::~GunnsElectPvStringLoadOrder()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to be copied from.
///
/// @details  Copy constructs this Photovoltaic Array Shunting Regulator String Load Order.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvStringLoadOrder::GunnsElectPvStringLoadOrder(const GunnsElectPvStringLoadOrder& that)
    :
    mSection(that.mSection),
    mString(that.mString)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object to be assigned from.
///
/// @returns  GunnsElectPvStringLoadOrder&  (--)  A reference to this object.
///
/// @details  Copies the state of the given Photovoltaic Array Shunting Regulator String Load Order.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvStringLoadOrder& GunnsElectPvStringLoadOrder::operator =(const GunnsElectPvStringLoadOrder& that)
{
    if (&that != this) {
        mSection = that.mSection;
        mString  = that.mString;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object to be checked for equality with this one.
///
/// @returns  bool  (--)  True if the objects are identical.
///
/// @details  Returns true if this is the same object as that, or if all of this objects attributes
///           are equal to that's.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsElectPvStringLoadOrder::operator ==(const GunnsElectPvStringLoadOrder& that) const
{
    return (&that == this) or
           ( (mSection == that.mSection)
         and (mString  == that.mString) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                   (--)    Link name.
/// @param[in] nodes                  (--)    Network nodes array.
/// @param[in] outputConductance      (1/ohm) Conductance of the regulated output.
/// @param[in] shuntConductance       (1/ohm) Conductance of each string shunt.
/// @param[in] array                  (--)    Pointer to the PV array link.
/// @param[in] inCurrentSensor        (--)    Optional pointer to the input current sensor wrapper.
/// @param[in] inVoltageSensor        (--)    Optional pointer to the input voltage sensor wrapper.
/// @param[in] outCurrentSensor       (--)    Optional pointer to the output current sensor wrapper.
/// @param[in] outVoltageSensor       (--)    Optional pointer to the output voltage sensor wrapper.
/// @param[in] inputOverCurrentTrip   (amp)   Optional input over-current trip limit.
/// @param[in] inputOverVoltageTrip   (V)     Optional input over-volt trip limit.
/// @param[in] outputOverCurrentTrip  (amp)   Optional output over-current trip limit.
/// @param[in] outputOverVoltageTrip  (V)     Optional output over-volt trip limit.
/// @param[in] outputUnderVoltageTrip (V)     Optional output under-volt trip limit.
/// @param[in] tripPriority           (--)    Trip network step priority.
///
/// @details  Default constructs this Photovoltaic Array Shunting Regulator configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegShuntConfigData::GunnsElectPvRegShuntConfigData(
        const std::string&        name,
        GunnsNodeList*            nodes,
        const double              outputConductance,
        const double              shuntConductance,
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
    mOutputConductance(outputConductance),
    mShuntConductance(shuntConductance),
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
    mTripPriority(tripPriority),
    mStringLoadOrder()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic Array Shunting Regulator configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegShuntConfigData::~GunnsElectPvRegShuntConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] section (--) Array section number of the load order.
/// @param[in] string  (--) Section string number of the load order.
///
/// @details  Creates a Photovoltaic Array Shunting Regulator String Load Order with the given
///           values and adds it to the string load order vector.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegShuntConfigData::addStringLoadOrder(const unsigned int section,
                                                        const unsigned int string)
{
    mStringLoadOrder.push_back(GunnsElectPvStringLoadOrder(section, string));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] voltageSetpoint  (V)   Initial setpoint for the regulated output voltage.
/// @param[in] powered          (--)  Initial state of power on flag.
/// @param[in] enabled          (--)  Initial state of enabled, powered and commanded on, etc.
/// @param[in] minOperatePower  (W)   Initial minimum bulk power available from array to operate.
///
/// @details  Default constructs this Photovoltaic Array Shunting Regulator input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegShuntInputData::GunnsElectPvRegShuntInputData(const double voltageSetpoint,
                                                             const bool   powered,
                                                             const bool   enabled,
                                                             const double minOperatePower)
    :
    GunnsBasicLinkInputData(false, 0.0),
    mVoltageSetpoint(voltageSetpoint),
    mPowered(powered),
    mEnabled(enabled),
    mMinOperatePower(minOperatePower)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic Array Shunting Regulator input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegShuntInputData::~GunnsElectPvRegShuntInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Gunns Photovoltaic Array Shunting Regulator Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegShunt::GunnsElectPvRegShunt()
    :
    GunnsBasicLink(NPORTS),
    mMalfVoltageBiasFlag(false),
    mMalfVoltageBiasValue(0.0),
    mInputOverVoltTrip(),
    mOutputUnderVoltTrip(),
    mOutputConductance(0.0),
    mShuntConductance(0.0),
    mArray(0),
    mTripPriority(0),
    mStringLoadOrder(),
    mVoltageSetpoint(0.0),
    mPowered(false),
    mEnabled(false),
    mMinOperatePower(0.0),
    mResetTrips(false),
    mSensors(),
    mTrips(),
    mState(OFF),
    mRegulatedVoltage(0.0),
    mInputConductance(0.0),
    mShuntPower(0.0),
    mInputPower(0.0),
    mOutputPower(0.0),
    mWasteHeat(0.0),
    mPvBulkPowerAvail(0.0),
    mMaxRegCurrent(0.0),
    mOffToRegOccurred(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Photovoltaic Array Shunting Regulator.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvRegShunt::~GunnsElectPvRegShunt()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Photovoltaic Array Shunting Regulator Config Data.
/// @param[in]     inputData    (--) Photovoltaic Array Shunting Regulator Input Data.
/// @param[in,out] networkLinks (--) Network links.
/// @param[in]     port0        (--) Network port 0.
/// @param[in]     port1        (--) Network port 1.
///
/// @throws   TsInitializationException
///
/// @details  This method initializes this Photovoltaic Array Shunting Regulator with configuration
///           and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegShunt::initialize(const GunnsElectPvRegShuntConfigData& configData,
                                      const GunnsElectPvRegShuntInputData&  inputData,
                                      std::vector<GunnsBasicLink*>&         networkLinks,
                                      const int                             port0,
                                      const int                             port1)
{
    /// - Initialize the parent class.
    int ports[2] = {port0, port1};
    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag          = false;

    validate(configData, inputData);

    /// - Initialize config and input data.
    mOutputConductance = configData.mOutputConductance;
    mShuntConductance  = configData.mShuntConductance;
    mArray             = configData.mArray;
    mTripPriority      = configData.mTripPriority;
    mStringLoadOrder   = configData.mStringLoadOrder;
    mVoltageSetpoint   = inputData.mVoltageSetpoint;
    mPowered           = inputData.mPowered;
    mEnabled           = inputData.mEnabled;
    mMinOperatePower   = inputData.mMinOperatePower;

    /// - If the string load order is emtpy, then initialize it with a default load order.
    if (mStringLoadOrder.empty()) {
        for (unsigned int section=0; section<mArray->getNumSections(); ++section) {
            for (unsigned int string=0; string<mArray->mSections[section].getNumStrings(); ++string) {
                mStringLoadOrder.push_back(GunnsElectPvStringLoadOrder(section, string));
            }
        }
    }

    /// - Connect to sensors.
    mSensors.connectInCurrent(configData.mInCurrentSensor);
    mSensors.connectInVoltage(configData.mInVoltageSensor);
    mSensors.connectOutCurrent(configData.mOutCurrentSensor);
    mSensors.connectOutVoltage(configData.mOutVoltageSensor);

    /// - Initialize trip functions.
    mTrips.mInOverCurrent.initialize(configData.mInOverCurrentTrip,
                                     configData.mTripPriority, false);
    mTrips.mInOverVoltage.initialize(configData.mInOverVoltageTrip,
                                     configData.mTripPriority, false);
    mTrips.mOutOverCurrent.initialize(configData.mOutOverCurrentTrip,
                                      configData.mTripPriority, false);
    mTrips.mOutOverVoltage.initialize(configData.mOutOverVoltageTrip,
                                      configData.mTripPriority, false);
    mTrips.mOutUnderVoltage.initialize(configData.mOutUnderVoltageTrip,
                                       configData.mTripPriority, false);

    /// - Initialize class attributes.
    mState             = OFF;
    mRegulatedVoltage  = 0.0;
    mInputConductance  = 0.0;
    mShuntPower        = 0.0;
    mInputPower        = 0.0;
    mOutputPower       = 0.0;
    mWasteHeat         = 0.0;
    mPvBulkPowerAvail  = 0.0;
    mMaxRegCurrent     = 0.0;
    mInitFlag          = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--) Photovoltaic Array Shunting Regulator Config Data.
/// @param[in]  inputData   (--) Photovoltaic Array Shunting Regulator Input Data.
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Photovoltaic Array Shunting Regulator configuration & input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegShunt::validate(const GunnsElectPvRegShuntConfigData& configData,
                                    const GunnsElectPvRegShuntInputData&  inputData) const
{
    /// - Throw an exception on output conductance < DBL_EPSILON.
    if (configData.mOutputConductance < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "output conductance < DBL_EPSILON.");
    }

    /// - Throw an exception on shunt conductance < DBL_EPSILON.
    if (configData.mShuntConductance < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "shunt conductance < DBL_EPSILON.");
    }

    /// - Throw an exception on null array pointer.
    if (!configData.mArray) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "null pointer to array link.");
    }

    /// - Throw an exception on trip priority < 1.
    if (configData.mTripPriority < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "trip priority < 1.");
    }

    /// - Throw an exception if the array link has not been initialized already.  This ensures the
    ///   array link is stepped before this link, which is an order dependency that we require.
    if (!configData.mArray->isInitialized()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "array link not initialized before this link.");
    }

    if (!configData.mStringLoadOrder.empty()) {
        /// - Throw an exception if the provided string load order has incorrect length.
        if (configData.mStringLoadOrder.size() != configData.mArray->getNumStrings()) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "string load order number of entries doesn't match array number of strings.");
        }

        for (unsigned int i=0; i<configData.mStringLoadOrder.size(); ++i) {
            const unsigned int section = configData.mStringLoadOrder[i].mSection;
            /// - Throw an exception if a provided string load order entry has invalid section
            ///   number.
            if (section >= configData.mArray->getNumSections()) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "string load order entry has invalid section number.");
            }

            /// - Throw an exception if a provided string load order entry has invalid string
            ///   number.
            if (configData.mStringLoadOrder[i].mString >= configData.mArray->mSections[section].getNumStrings()) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "string load order entry has invalid string number.");
            }

            /// - Throw an exception if the provided string load order has duplicate entries.
            for (unsigned int j=i+1; j<configData.mStringLoadOrder.size(); ++j) {
                if (configData.mStringLoadOrder[i] == configData.mStringLoadOrder[j]) {
                    GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "string load order has duplicate entries.");
                }
            }
        }
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
void GunnsElectPvRegShunt::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-config and non-checkpointed data.
    mState             = OFF;
    mRegulatedVoltage  = 0.0;
    mInputConductance  = 0.0;
    mShuntPower        = 0.0;
    mInputPower        = 0.0;
    mOutputPower       = 0.0;
    mWasteHeat         = 0.0;
    mPvBulkPowerAvail  = 0.0;
    mMaxRegCurrent     = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Not used.
///
/// @details  Computes this link's contributions to the network system of equations prior to the
///           network major step solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegShunt::step(const double dt __attribute__((unused)))
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Step the model.
    updateRegulatedVoltage();
    updateMaxOutputs();
    updateInitialState();
    minorStep(0.0, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt        (s)  Not used.
/// @param[in] minorStep (--) Not used.
///
/// @details  Computes this link's contributions to the network system of equations prior to each
///           network minor step solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegShunt::minorStep(const double dt __attribute__((unused)),
                                     const int    minorStep __attribute__((unused)))
{
    /// - Build the contributions to the network solution.
    buildAdmittanceMatrix();
    buildSourceVector();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes maximum power and current outputs the array can supply this pass, and
///           initially loads all strings at the regulated voltage.  The strings can be reloaded
///           later this pass.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegShunt::updateMaxOutputs()
{
    /// - Maximum power the array's bulk equivalent model can output at the regulated voltage, and
    ///   The corresponding conductance that would load it at that power.
    mArray->predictLoadAtVoltage(mPvBulkPowerAvail, mInputConductance, mRegulatedVoltage);

    /// - Load all strings at the regulated voltage and sum their resulting output currents.
    mMaxRegCurrent = 0.0;
    for (unsigned int section=0; section<mArray->getNumSections(); ++section) {
        for (unsigned int string=0; string<mArray->mSections[section].getNumStrings(); ++string) {
            mArray->mSections[section].mStrings[string].setShunted(false);
            mArray->mSections[section].mStrings[string].loadAtVoltage(mRegulatedVoltage);
            mMaxRegCurrent += mArray->mSections[section].mStrings[string].getTerminal().mCurrent;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Performs initial state transitions in response to the array state this pass.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegShunt::updateInitialState()
{
    if (mResetTrips or not mPowered) {
        mTrips.resetTrips();
        mResetTrips = false;
    }

    mOffToRegOccurred = false;
    if (not (mPowered and mEnabled) or mTrips.isTripped() or
            (mArray->getMpp().mPower < DBL_EPSILON) or
            (mPvBulkPowerAvail < mMinOperatePower) ) {
        mState = OFF;
    } else {
        if (OFF == mState) {
            mOffToRegOccurred = true;
        }
        mState = REG;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the Admittance Matrix for the link.  The behavior depends on state.  In REG and
///           OFF states, the input and output nodes are loaded separately and are not connected.
///           In SAG mode, this link simply connects the nodes like a conductor.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegShunt::buildAdmittanceMatrix()
{
    switch (mState) {
        case REG: {
            mAdmittanceUpdate = (mAdmittanceMatrix[0] != mInputConductance) or
                                (mAdmittanceMatrix[3] != mOutputConductance);
            mAdmittanceMatrix[0] = mInputConductance;
            mAdmittanceMatrix[1] = 0.0;
            mAdmittanceMatrix[2] = 0.0;
            mAdmittanceMatrix[3] = mOutputConductance;
        } break;
        case SAG: {
            mAdmittanceUpdate = (mAdmittanceMatrix[0] != mOutputConductance) or
                                (mAdmittanceMatrix[3] != mOutputConductance);
            mAdmittanceMatrix[0] =  mOutputConductance;
            mAdmittanceMatrix[1] = -mOutputConductance;
            mAdmittanceMatrix[2] = -mOutputConductance;
            mAdmittanceMatrix[3] =  mOutputConductance;
        } break;
        default: {  // OFF or invalid
            mAdmittanceUpdate = (mAdmittanceMatrix[0] != mInputConductance) or
                                (mAdmittanceMatrix[3] != 0.0);
            mAdmittanceMatrix[0] = mInputConductance;
            mAdmittanceMatrix[1] = 0.0;
            mAdmittanceMatrix[2] = 0.0;
            mAdmittanceMatrix[3] = 1.0 / mConductanceLimit;
        } break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] convergedStep (--) The count of minor steps since the network last converged.
/// @param[in] absoluteStep  (--) Not used.
///
/// @returns  SolutionResult  (--)  Whether this link confirms or rejects the network solution.
///
/// @details  This link determines whether to accept or reject the converged network solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult GunnsElectPvRegShunt::confirmSolutionAcceptable(
        const int convergedStep, const int absoluteStep __attribute__((unused)))
{
    GunnsBasicLink::SolutionResult result = CONFIRM;

    /// - Process state changes if the regulator is operating.
    if (isOperating()) {

        /// - On any network minor step, immediately transition to OFF state and reject the network
        ///   solution if there is back voltage on the output.  This is similar to reverse bias on a
        ///   diode or output power converter, and the solution is invalid.
        if ( (REG == mState) and (mPotentialVector[1] > mRegulatedVoltage) ) {
            mState = OFF;
            result = REJECT;
        }
        /// - We only check for solution rejection and state change after the network has converged.
        /// - Further mode changes wait for the network to converge.  This is to avoid oscillations in
        ///   states between this and other regulators in a network.
        if (convergedStep > 0) {
            switch (mState) {
                case REG: {
                    /// - In REG mode, find and load the array strings with the total power demanded by
                    ///   the downstream circuit and the output channel resistive loss.  If the load
                    ///   exceeds what the array can supply, then delay until our trip priority, then
                    ///   transition to OFF mode and reject the solution.  This delay allows downstream
                    ///   elements to possibly shut off and reduce our load before triggering our
                    ///   shutdown.
                    const double powerDemand = mRegulatedVoltage * mAdmittanceMatrix[3] *
                            (mRegulatedVoltage - mPotentialVector[1]);
                    loadArray(powerDemand);
                    if (mInputPower < powerDemand and mPvBulkPowerAvail < powerDemand) {
                        if (convergedStep < static_cast<int>(mTripPriority)) {
                            result = DELAY;
                        } else {
                            mState = OFF;
                            result = REJECT;
                        }
                    }
                } break;
                case SAG: {
                    ; // do nothing, SAG no longer modeled
                } break;
                default: {      // OFF or invalid
                    /// - In OFF mode, if the array can make sufficient power to restart, then delay
                    ///   the network until our trip priority, then reject the solution and transition
                    ///   to REG state.  To avoid oscillating between OFF->REG->OFF indefinitely when
                    ///   powerDemand > mPvBulkPowerAvailable > mMinOperatePower, we limit this
                    ///   transition to once per major step.
                    if (mPvBulkPowerAvail >= mMinOperatePower and not mOffToRegOccurred) {
                        if (convergedStep < static_cast<int>(mTripPriority)) {
                            result = DELAY;
                        } else {
                            mState = REG;
                            result = REJECT;
                            mOffToRegOccurred = true;
                        }
                    } else {
                        /// - Shunt all strings when OFF.
                        loadArray(-1.0);
                    }
                } break;
            }
        }
    }

    /// - Only continue with trip checks if we haven't already rejected due to state changes.
    if (convergedStep > 0 and REJECT != result) {
        computeFlux();

        /// - Sensors are optional; if a sensor exists then the trip uses its sensed value of
        ///   the truth parameter, otherwise the trip looks directly at the truth parameter.
        const unsigned int section = mStringLoadOrder[0].mSection;
        const unsigned int string  = mStringLoadOrder[0].mString;
        float sensedVin  = mArray->mSections[section].mStrings[string].getTerminal().mVoltage;
        float sensedIin  = mArray->mSections[section].mStrings[string].getTerminal().mCurrent;
        float sensedVout = mPotentialVector[1];
        float sensedIout = mFlux;

        /// - Note that since we step the sensors without a time-step, its drift malfunction
        ///   isn't integrated.  This is because we don't have the time-step in this function,
        ///   and we must update the sensor multiple times per major network step, which would
        ///   repeat the drift integration too many times.  The result of all this is that drift
        ///   lags behind by one major step for causing trips.
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

        /// - Check all trip logics for trips. If any trip, reject the solution and mode to OFF.
        if (isOperating()) {
            if (mTrips.mInOverVoltage  .checkForTrip(result, sensedVin,  convergedStep)) mState = OFF;
            if (mTrips.mInOverCurrent  .checkForTrip(result, sensedIin,  convergedStep)) mState = OFF;
            if (mTrips.mOutOverVoltage .checkForTrip(result, sensedVout, convergedStep)) mState = OFF;
            if (mTrips.mOutOverCurrent .checkForTrip(result, sensedIout, convergedStep)) mState = OFF;
            if (mTrips.mOutUnderVoltage.checkForTrip(result, sensedVout, convergedStep)) mState = OFF;
        }
    }

    /// - We only set the array's common strings output flag to false when in REG state because
    ///   that's the only state where array strings are individually shunted.  In OFF state, we keep
    ///   all array arrays strings tied to the node to avoid chatter.
    if (isOperating()) {
        mArray->setCommonStringsOutput(REG != mState);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Not used.
///
/// @details  Computes the final outputs resulting from the network solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegShunt::computeFlows(const double dt __attribute__((unused)))
{
    mPotentialDrop = getDeltaPotential();

    switch (mState) {
        case REG: {
            mPower        = -mFlux * (mRegulatedVoltage - mPotentialVector[1]);
            mOutputPower  = mPotentialVector[1] * mFlux;
        } break;
        case SAG: {
            ; // do nothing, SAG no longer modeled
        } break;
        default: { // OFF or invalid
            mPower        = 0.0;
            mOutputPower  = 0.0;
            mInputPower   = 0.0;
        } break;
    }

    /// - Transport supply current from the array to the downstream circuit nodes.  String shunt
    ///   currents are not transported in the nodes because they are modeled separately from the
    ///   network solution.
    mNodes[1]->collectInflux(mFlux);
    mNodes[0]->collectOutflux(mFlux);

    /// - Waste heat generated in the regulator is the sum of resistive losses through the string
    ///   shunts and the output channel paths.  Note mPower, defined in the base class as power
    ///   created by the link, is subtracted since it is always a power lost in this link.
    mWasteHeat = mShuntPower - mPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  power  (W)  Total power load to be placed on the strings.
///
/// @details  Loops over the array strings and loads each at the regulated voltage until the power
///           argument value has been exceeded.  The remaining strings are shunted.  A negative
///           power argument value will shunt all the strings.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvRegShunt::loadArray(const double power)
{
    mInputConductance = 0.0;
    mInputPower       = 0.0;
    mShuntPower       = 0.0;
    for (unsigned int arrayString=0; arrayString<mArray->getNumStrings(); ++arrayString) {
        const unsigned int section = mStringLoadOrder[arrayString].mSection;
        const unsigned int string  = mStringLoadOrder[arrayString].mString;
        if ( (mInputPower >= power) or (power < 0.0)) {
            /// - Shunt the string by loading it with the shunt conductance, and accumulate its
            ///   returned power into the total.  Note that the excess power from the last
            ///   non-shunted string is not included in the total shunt power, as it is assumed that
            ///   the reg reduces that string's duty cycle to only create the required power.
            mArray->mSections[section].mStrings[string].loadAtConductance(mShuntConductance);
            mArray->mSections[section].mStrings[string].setShunted(true);
            mShuntPower += mArray->mSections[section].mStrings[string].getTerminal().mPower;
        } else {
            /// - Load the string at regulated voltage and accumulate its resulting power and
            ///   conductance values into the totals.
            if (mArray->mSections[section].mStrings[string].isShunted()) {
                /// - Since all strings are always loaded at regulated voltage in step(), we only
                ///   need to reload them here if they had been shunted since then.  This can happen
                ///   if the network converges with this in REG mode twice in one major step at
                ///   different vehicle power demands.  Skipping this saves computation time in the
                ///   string if it's not needed.
                mArray->mSections[section].mStrings[string].loadAtVoltage(mRegulatedVoltage);
                mArray->mSections[section].mStrings[string].setShunted(false);
            }
            mInputConductance += mArray->mSections[section].mStrings[string].getTerminal().mConductance;
            mInputPower       += mArray->mSections[section].mStrings[string].getTerminal().mPower;
        }
    }
}
