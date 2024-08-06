/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
 (Converter.o)
 (common/sensors/SensorAnalog.o)
 (core/GunnsBasicLink.o)
 (EpsTripMgr.o)
 (software/exceptions/TsInitializationException.o)
 )

 PROGRAMMERS:
 (
 (Shailaja Janapati) (L-3 Comm) (May 2013) (Initial Prototype)
 )

 */

#include "ConverterElect.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "math/MsMath.hh"
#include "aspects/electrical/EpsMacros.hh"

/// @details    -- Minimum Conductance
const double  ConverterElect::MIN_CONDUCTANCE = 1.0e-5;
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @verbatim
///                                   _____________
///    (Input Side) Port0 ___________|            |____________ Port2 (Output Side)
///    (Source Side)                 |  Converter |                   (Load Side)
///                 Port1 ___________|            |____________ Port3
///                      |           |____________|           |
///                      |                                    |
///                  ____|____                            ____|____
///                    _____                                _____
///
///
///    The Converter is a four port link and the Input side Port0 is connected to the node where the
///    voltage source for the converter will be. Port1 is always grounded.
///    Port2 is connected to the node where the loads for the converter will be on the Output side . Port3 is
///    always grounded.
///
/// @endverbatim

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @param[in]   name                      (--)    Name of the link being created
/// @param[in]   nodes                     (--)    Pointer to  nodes
/// @param[in]   outVoltageSensorConfig    (--)    output voltage sensor configuration data
/// @param[in]   outCurrentSensorConfig    (--)    output current sensor configuration data
/// @param[in]   outputConductance         (1/ohm) Converter's ON conductance on the load side of the link
/// @param[in]   converterOffConductance   (1/ohm) Converter's OFF conductance of the link
/// @param[in]   tripPriority              (--)    Trip tier for this link in gunns network
/// @param[in]   standbyPower              (W)     Standby Power when converter disables its output
///
/// @details  ConverterElect Configuration Data constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
ConverterElectConfigData::ConverterElectConfigData(const std::string& name,
                                         GunnsNodeList* nodes,
                                         const SensorAnalogConfigData* outVoltageSensorConfig,
                                         const SensorAnalogConfigData* outCurrentSensorConfig,
                                         const double outputConductance,
                                         const double converterOffConductance,
                                         const int    tripPriority,
                                         const double standbyPower)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mOutVoltageSensorConfig(outVoltageSensorConfig),
    mOutCurrentSensorConfig(outCurrentSensorConfig),
    mOutputConductance(outputConductance),
    mConverterOffConductance(converterOffConductance),
    mTripPriority(tripPriority),
    mConverterConfig(standbyPower)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @param[in]   that     --    ConverterElect Config Data to be copied
/// @details ConverterElect Configuration Data copy constructor.
///
//////////////////////////////////////////////////////////////////////////////////////////////////
ConverterElectConfigData::ConverterElectConfigData(const ConverterElectConfigData& that)
    :
    GunnsBasicLinkConfigData(that),
    mOutVoltageSensorConfig(that.mOutVoltageSensorConfig),
    mOutCurrentSensorConfig(that.mOutCurrentSensorConfig),
    mOutputConductance(that.mOutputConductance),
    mConverterOffConductance(that.mConverterOffConductance),
    mTripPriority(that.mTripPriority),
    mConverterConfig(that.mConverterConfig)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @details ConverterElect Input Data destructor.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
ConverterElectConfigData::~ConverterElectConfigData() {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @param[in]    malfBlockageFlag           (--)  Flag to enable blockage in the link
/// @param[in]    malfBlockageValue          (--)  Value to use for blockage calculation - basic link data
/// @param[in]    outVoltageSensorInput      (--)  output voltage sensor data to set the instance input data
/// @param[in]    outCurrentSensorInput      (--)  output current sensor data to set the instance input data
/// @param[in]    malfOpOverCurrentFlag      (--)  Malfunction flag to override the output over current Limit
/// @param[in]    malfOpOverVoltageFlag      (--)  Malfunction flag to override the output over voltage limit
/// @param[in]    malfRegulatedVoltageFlag   (--)  Malfunction flag to override the regulated voltage Limit
/// @param[in]    inputVoltage               (V)   Input voltage to the converter
/// @param[in]    regulatedVoltage           (V)   Regualted voltage coming out from the converter
/// @param[in]    efficiency                 (--)  Efficiency of the converter conversion process
/// @param[in]    opOverCurrentLimit         (amp)  Output over current limit
/// @param[in]    opOverVoltageLimit         (V)    Output over voltage limit (hardware)
/// @param[in]    opOverCurrentTripActive    (--)   Output over current trip active
/// @param[in]    opOverVoltageTripActive    (--)   Output over voltage trip active
/// @param[in]    inputOverVoltageLimit      (V)    Input over voltage limit of the converter
/// @param[in]    inputUnderVoltageLimit     (V)    Input under voltage limit of the converter
/// @param[in]    inOverVoltageTripActive    (--)   Input over voltage trip active
/// @param[in]    inUnderVoltageTripActive   (--)   Input under voltage trip active
/// @details ConverterElect Input Data constructor
///
////////////////////////////////////////////////////////////////////////////////////////////////////
ConverterElectInputData::ConverterElectInputData(
                                    const bool   malfBlockageFlag,
                                    const double malfBlockageValue,
                                    const SensorAnalogInputData* outVoltageSensorInput,
                                    const SensorAnalogInputData* outCurrentSensorInput,
                                    const bool malfOpOverCurrentFlag,
                                    const bool malfOpOverVoltageFlag,
                                    const bool malfRegulatedVoltageFlag,
                                    const double inputVoltage,
                                    const double regulatedVoltage,
                                    const double efficiency,
                                    const double opOverCurrentLimit,
                                    const double opOverVoltageLimit,
                                    const bool opOverCurrentTripActive,
                                    const bool opOverVoltageTripActive,
                                    const double inputOverVoltageLimit,
                                    const double inputUnderVoltageLimit,
                                    const bool inOverVoltageTripActive,
                                    const bool inUnderVoltageTripActive)
    :
    GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue),
    mOutVoltageSensorInput(outVoltageSensorInput),
    mOutCurrentSensorInput(outCurrentSensorInput),
    mMalfOpOverCurrentFlag(malfOpOverCurrentFlag),
    mMalfOpOverVoltageFlag(malfOpOverVoltageFlag),
    mMalfRegulatedVoltageFlag(malfRegulatedVoltageFlag),
    mOpOverCurrentLimit(opOverCurrentLimit),
    // input data to pass to the Converter class
    mConverterInput(inputVoltage,
            regulatedVoltage,
            efficiency,
            opOverCurrentLimit,
            opOverVoltageLimit,
            opOverCurrentTripActive,
            opOverVoltageTripActive,
            inputOverVoltageLimit,
            inputUnderVoltageLimit,
            inOverVoltageTripActive,
            inUnderVoltageTripActive)
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @param[in]    that    --    ConverterElect Input Data to be copied
/// @details ConverterElect Input Data copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
ConverterElectInputData::ConverterElectInputData(
        const ConverterElectInputData& that)
    :
    GunnsBasicLinkInputData(that),
    mOutVoltageSensorInput(that.mOutVoltageSensorInput),
    mOutCurrentSensorInput(that.mOutCurrentSensorInput),
    mMalfOpOverCurrentFlag(that.mMalfOpOverCurrentFlag),
    mMalfOpOverVoltageFlag(that.mMalfOpOverVoltageFlag),
    mMalfRegulatedVoltageFlag(that.mMalfRegulatedVoltageFlag),
    mOpOverCurrentLimit(that.mOpOverCurrentLimit),
    mConverterInput(that.mConverterInput)
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    ConverterElect Input Data destructor
///
////////////////////////////////////////////////////////////////////////////////////////////////////
ConverterElectInputData::~ConverterElectInputData()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @details ConverterElect constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
ConverterElect::ConverterElect()
    :
    GunnsBasicLink(CONV_NUMPORTS),
    mConverter(),
    mOutVoltageSensor(),
    mOutCurrentSensor(),
    mMalfOpOverCurrentFlag(false),
    mMalfOpOverCurrentValue(0.0),
    mMalfOpOverVoltageFlag(false),
    mMalfOpOverVoltageValue(0.0),
    mMalfRegulatedVoltageFlag(false),
    mMalfRegulatedVoltageValue(0.0),
    mMalfFailConverterFlag(false),
    mConverterOffConductance(0.0),
    mOutputConductance(0.0),
    mInputActiveConductance(0.0),
    mOutputActiveConductance(0.0),
    mOpOverCurrentLimit(0.0),
    mOutputOverVoltageLimit(0.0),
    mRegulatedVoltage(0.0),
    mOutputCurrent(0.0),
    mOutputCurrentSensed(0.0),
    mOutputVoltageSensed(0.0),
    mConverterOnCmd(false),
    mTlmPowerSupplyValid(false),
    mConverterTripMgr(),
    mResetTrips(false),
    mOutputSource(0.0),
    mDeltaPotentialOutputSide(0.0),
    mDeltaPotentialInputSide(0.0)
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @details ConverterElect destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
ConverterElect::~ConverterElect()
{
    // Nothing to do
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @param[in]    configDataIn    (--)    Configuration data for ConverterElect model
/// @param[in]    inputDataIn     (--)    Input data for ConverterElect model
/// @param[in]    networkLinks    (--)    Vector off newtowrk link used by gunns solver
/// @param[in]     ports0         (--)    Port0 is on the input side of the converter, connected to the node at the source
/// @param[in]     ports1         (--)    Port1 is on the input side of the converter, connected to the ground
/// @param[in]     ports2         (--)    Port2 is on the output side of the converter, connected to the node at the loads
/// @param[in]     ports3         (--)    Port3 is on the output side of the converter, connected to the ground
/// @throws   TsInitializationException
/// @return   void
/// @details Initialize the configuration and input data for the converter utility as well as the link
///
///////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterElect::initialize(const ConverterElectConfigData& configDataIn,
                           const ConverterElectInputData& inputDataIn,
                           std::vector<GunnsBasicLink*>& networkLinks,
                           const int  port0,
                           const int  port1,
                           const int  port2,
                           const int  port3)
{
    /// - Resets init flag.
    mInitFlag = false;

    int lports[4] = {port0, port1, port2, port3};

    GunnsBasicLink::initialize(configDataIn, inputDataIn, networkLinks, lports);

    /// - Validate configuration and input data for this link
    validate(configDataIn);

    /// - Initialize from input data.
    mOpOverCurrentLimit        = inputDataIn.mOpOverCurrentLimit;
    mOutputOverVoltageLimit    = inputDataIn.mConverterInput.mOutputOverVoltageLimit;
    mRegulatedVoltage          = inputDataIn.mConverterInput.mRegulatedVoltage;

    mMalfOpOverCurrentFlag     = inputDataIn.mMalfOpOverCurrentFlag;
    mMalfOpOverVoltageFlag     = inputDataIn.mMalfOpOverVoltageFlag;
    mMalfRegulatedVoltageFlag  = inputDataIn.mMalfRegulatedVoltageFlag;

    /// -- Initialize from config data
    mOutputConductance  = configDataIn.mOutputConductance;
    mConverterOffConductance = configDataIn.mConverterOffConductance;
    mConverterTripMgr.initialize(configDataIn.mName + "_TripMgr", configDataIn.mTripPriority);

    std::string convName(configDataIn.mName);

    /// - Initialize output current and voltage sensor
    mOutVoltageSensor.initialize(*configDataIn.mOutVoltageSensorConfig, *inputDataIn.mOutVoltageSensorInput,
            convName + "_OutputVoltageSensor");
    mOutCurrentSensor.initialize(*configDataIn.mOutCurrentSensorConfig, *inputDataIn.mOutCurrentSensorInput,
            convName + "_OutputCurrentSensor");

    // initialize fail converter flag to false
    mMalfFailConverterFlag = false;

    // set delta potential and input power values for the first time
    mDeltaPotentialOutputSide = getDeltaPotential(OUT_POSITIVE, OUT_NEGITIVE);  // output ports
    mDeltaPotentialInputSide = getDeltaPotential(IN_POSITIVE, IN_NEGITIVE);   // input ports

    /// @todo Shailaja - kludge the term for now
    mTlmPowerSupplyValid = true;

    /// - Pass on the converter input and config data to the Converter's initialize function
    /// Sets init flag on successful initialization of converter
    mInitFlag = mConverter.initialize(&configDataIn.mConverterConfig, &inputDataIn.mConverterInput);

    // converter should trip on sensed value
    mConverter.setTripOnSensedValue(true);

    /// - Warn of deprecation due to obsolescence by GunnsElectConverterInput and -Output.
    GUNNS_WARNING("this link is deprecated!  It is obsoleted by GunnsElectConverterInput and -Output.");
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @return   bool  --  Returns true if all link-specific rules checks pass
///
/// @details  Check that ports 2 and 4 are connected to ground. Method overwritten for link specific rules
////////////////////////////////////////////////////////////////////////////////////////////////////
bool ConverterElect::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;
    /// - Check if the proper port number is assigned to the ground node.
    if ((IN_NEGITIVE == port || OUT_NEGITIVE == port) && (node != getGroundNodeIndex())) { // ports at these indexes must be grounded
        GUNNS_WARNING("aborted setting a port: " << port << " should be set to Ground for the Converter "
                <<  mName << ".");
        result = false;
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configDataIn  (--)  Configuration data.
/// @throws  TsInitializationException
/// @return  void
/// @details  Method to validate link's configuration and input data
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterElect::validate(const ConverterElectConfigData& configDataIn)
{
    /// - Check to make sure on conductance are greater than 0.0
    TS_EPS_IF_ERREX(configDataIn.mOutputConductance < 0.0, TsInitializationException,
                "Invalid Initialization Data", "Tried to set Output Conductance < 0.0");


    /// - Check to make sure off conductance are greater than 0.0
    TS_EPS_IF_ERREX(configDataIn.mConverterOffConductance < 0.0, TsInitializationException,
                    "Invalid Initialization Data", "Tried to set Converter Off Conductance < 0.0");


    /// - Check to make sure trip priority is greater than 0.0
    TS_EPS_IF_ERREX(configDataIn.mTripPriority <= 0, TsInitializationException,
                        "Invalid Initialization Data", "Tried to set Trip Priority < 0");
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterElect::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt        (s)  Link time step
/// @param[in] minorstep       (--) The minor step number that the network is on (not used)
/// @return void
/// @details  Updates the link during the minor time step
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterElect::minorStep(const double dt, const int __attribute__((unused)) minorstep)
{
    computeFlows(dt);
    step(dt);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   timeStep  (s)  Time step (not used)
/// @return void
/// @details  Method to update admittance and source potential of ConverterElect link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterElect::step(const double __attribute__((unused)) timeStep)
{
    /// - Reset trips if commanded
    if (mResetTrips || !mConverterOnCmd) {
        resetTrips();
    }

    if(mPotentialVector[0] > mConverter.getInputUnderVoltageLimit()) {
        mTlmPowerSupplyValid = true;
    } else {
        mTlmPowerSupplyValid = false;
    }

    /// - Turn on/off converter based on commanded value from signal aspect
    updateConverterState();

    /// - Handled malfunctions if flags are set
    handleOverrideMalfs();

    // Set the Input voltage for the converter.
    mConverter.setInputVoltage(mDeltaPotentialInputSide);

    /// - Updates converter state depending on given inputs
    mConverter.update();

    /// - Update ConverterElect link conductance
    buildConductance();

    /// - recompute the admittance
    buildAdmittance();

    /// - calculate source vector for the nodes based on the admittance and output voltage
    buildSourceVector();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return void
/// @details  Method to build Converter link's conductance into the system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterElect::buildConductance()
{
    // build the conductance on input side / battery side -- buildConductance()
    if (mConverter.isConverterOn() && fabs(mConverter.getOutputVoltage()) > 0.0) {
        // Output Side
        mOutputActiveConductance = mOutputConductance;
        mInputActiveConductance = MIN_CONDUCTANCE;
        // Input Side
        if(mDeltaPotentialInputSide > 0.0) {
            mInputActiveConductance = mConverter.getInputPower() / (mDeltaPotentialInputSide * mDeltaPotentialInputSide);
            mInputActiveConductance = MsMath::limitRange(MIN_CONDUCTANCE, mInputActiveConductance, GunnsBasicLink::mConductanceLimit);
        }
    } else {
        mInputActiveConductance   = mConverterOffConductance;
        mOutputActiveConductance = mConverterOffConductance;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return void
/// @details  Method to set the Converter link's admittance matrix to compute the source vector
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterElect::buildAdmittance()
{
    if (fabs(mAdmittanceMatrix[0]  - mInputActiveConductance)  > DBL_EPSILON or
        fabs(mAdmittanceMatrix[15] - mOutputActiveConductance) > DBL_EPSILON) {
        // set input Side admittance
        // To compute indexes for the input side of the admittance matrix let Row = 0 (port0) and Col = 1 (port1)
        // and numPorts = 4
        // Row * mNumPorts + Row = 0
        // Row * mNumPorts + Col = 1
        // Col * mNumPorts + Row = 2
        // Col * mNumPorts + Col = 3
        mAdmittanceMatrix[0] = mInputActiveConductance;
        mAdmittanceMatrix[1] = -mInputActiveConductance;
        mAdmittanceMatrix[4] = -mInputActiveConductance;
        mAdmittanceMatrix[5] = mInputActiveConductance;

        // set Output Side admittance
        // To recompute indexes for the output side of the admittance matrix let Row = 2 (port2) and Col = 3 (port3)
        // Row * mNumPorts + Row = 10
        // Row * mNumPorts + Col = 11
        // Col * mNumPorts + Row = 14
        // Col * mNumPorts + Col = 15
        mAdmittanceMatrix[10] = mOutputActiveConductance;
        mAdmittanceMatrix[11] = -mOutputActiveConductance;
        mAdmittanceMatrix[14] = -mOutputActiveConductance;
        mAdmittanceMatrix[15] = mOutputActiveConductance;

        mAdmittanceUpdate = true;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return void
/// @details  Method to build Converter link's conductance into the system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterElect::buildSourceVector()
{
    /// - The source vector term is defined as the potential rise created by the potential
    ///   source.  So that a positive source potential creates a flow in the positive direction
    ///   (port 0 to port 1), we reverse the sign in the source vector for the other port.
    /// get the index of the admittance matrix id for the output side positive port
    static const int id = (2 * mNumPorts) + 2; // evaluates to 10 - output side positive port

    // build source vector on output side
    mOutputSource =  mConverter.getOutputVoltage() * mAdmittanceMatrix[id];
    mSourceVector[0] = 0.0;
    mSourceVector[1] = 0.0;
    mSourceVector[2] = mOutputSource;
    mSourceVector[3] = -mSourceVector[2];
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   timeStep  (s)  Time step
/// @return void
/// @details  Method to compute flow across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterElect::computeFlows(const double timeStep)
{
    /// - calculate the deltaPotential on the primary bus side and the battery bus side
    mDeltaPotentialOutputSide = getDeltaPotential(2, 3);
    mDeltaPotentialInputSide = getDeltaPotential(0, 1);

    /// - get the output current value
    computeFlux();

    /// - Update ConverterElect output sensors
    updateOutputSensors(timeStep);

    /// - Trip the converter on output limits on sensed values
    mConverter.handleOutputTrips(mOutputVoltageSensed, mOutputCurrentSensed);

    /// - Wait to trip the first time since your priority will not be up yet
    /// Trip priority affects only the output over current value
    mConverterTripMgr.computeTripState(mConverter.isOutputOverCurrentTrip());

    if (mConverterTripMgr.isNotTimeToTrip()) {
        mConverter.resetStateDueToOverCurrentTrip();
    }

    /// - Converter conversion process, calculates heat dissipation and power consumed by it
    mConverter.doConversion(mOutputCurrent);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return void
/// @details  Compute flux as the value on the output side of the link
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterElect::computeFlux()
{
    // get the output current for the converter
    static const int id = 2 * mNumPorts + 2; // evaluates to 10
    mFlux = -mDeltaPotentialOutputSide * mAdmittanceMatrix[id] + mSourceVector[2];
    mOutputCurrent = mFlux;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return void
/// @details Method to update output sensor values
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterElect::updateOutputSensors(const double dt)
{
    /// - Update output voltage sensor
    // use the power supply valid term from the (signal side simbus) to send to the sensor.
    mOutputVoltageSensed = mOutVoltageSensor.sense(dt, mTlmPowerSupplyValid, mConverter.getOutputVoltage());

    /// - Update output current sensor
    mOutputCurrentSensed = mOutCurrentSensor.sense(dt, mTlmPowerSupplyValid, mOutputCurrent);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return void
/// @details  Method to handle override malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterElect::handleOverrideMalfs()
{
    /// - Malfunction to override output Over Current limit - value should be greater than or equal to zero
    if (mMalfOpOverCurrentFlag) {
        mMalfOpOverCurrentValue = fmax(0.0, mMalfOpOverCurrentValue);
        mConverter.setOutputOverCurrentLimit(mMalfOpOverCurrentValue);
    } else {
        mConverter.setOutputOverCurrentLimit(mOpOverCurrentLimit);
    }

    /// - Malfunction to override output Over Voltage limit - value should be greater than or equal to zero
    if (mMalfOpOverVoltageFlag) {
        mMalfOpOverVoltageValue = fmax(0.0, mMalfOpOverVoltageValue);
        mConverter.setOutputOverVoltageLimit(mMalfOpOverVoltageValue);
    } else {
        mConverter.setOutputOverVoltageLimit(mOutputOverVoltageLimit);
    }

    /// - Malfunction to override Regulated Voltage - value should be greater than or equal to zero
    if (mMalfRegulatedVoltageFlag) {
        mMalfRegulatedVoltageValue = fmax(0.0, mMalfRegulatedVoltageValue);
        mConverter.setRegulatedVoltage(mMalfRegulatedVoltageValue);
    } else {
        mConverter.setRegulatedVoltage(mRegulatedVoltage);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return void
/// @details  Method to clear trip flags at the converter as well as the priority flags for the link
///           used for tiered trip logic.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConverterElect::resetTrips(void) {
    /// - Resets all the trips
    mConverter.setResetTrips(true);
    mResetTrips = false;
    mConverterTripMgr.resetTrips();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] convergedStep    (--)    converged step of gunns minor step
/// @param[in] absoluteStep     (--)    absolute step of gunns minor step
/// @return (--) SolutionResult returns either REJECT, CONFIRM, DELAY
/// @details Method for for telling the gunns solver that link has an acceptable solution or not
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult ConverterElect::confirmSolutionAcceptable (
        const int convergedStep, const int absoluteStep __attribute__((unused)))
{
    if (convergedStep > 0) {
       return mConverterTripMgr.verifyTimeToTrip(convergedStep);
    } else {
       return GunnsBasicLink::DELAY;
    }
}
