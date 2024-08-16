/*********************** TRICK HEADER ******************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (Generic Converter Model)

 REQUIREMENTS:
 ()

 REFERENCE:
 ()

 ASSUMPTIONS AND LIMITATIONS:
 ()

 LIBRARY DEPENDENCY:
 (
 (Converter.o)
 (software/exceptions/TsInitializationException.o)
 (software/exceptions/TsOutOfBoundsException.o)
 (simulation/hs/TsHsMsg.o)
 )

 PROGRAMMERS:
 (
 (Chetan Patel) (ESCG) (January 2012) (Added inhibit functionality)
 (Nick Kaufmann) (L-3) (May 2011) (Initial Prototype)
 )
 *******************************************************************/

#include "Converter.hh"

#include <string>
#include <iostream>

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "software/exceptions/TsHsException.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "math/MsMath.hh"



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    mStandbyPower    (W)    Stand by power used by converter with no load attached
/// -
/// @brief Converter Configuration Data constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

ConverterConfigData::ConverterConfigData(const double mStandbyPower)
:
mStandbyPower(mStandbyPower) {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that (--)    Converter configuration data to be copied
/// -
/// @brief Converter Configuration Data copy constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

ConverterConfigData::ConverterConfigData(const ConverterConfigData& that)
:
mStandbyPower(that.mStandbyPower) {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that (--)    Converter configuration data to be copied
/// -
/// @brief Converter Configuration Data assignment operator.
////////////////////////////////////////////////////////////////////////////////////////////////////

ConverterConfigData& ConverterConfigData::operator =(const ConverterConfigData& that) {
    if (this != &that) { // Gracefully handle self assignment
        mStandbyPower = that.mStandbyPower;
    }
    return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Converter Configuration Data destructor.
///////////////////////////////////////////////////////////////////////////////////////////////////

ConverterConfigData::~ConverterConfigData() {
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    mInputVoltage     (V)    Input voltage to the converter
/// @param[in]    mRegulatedVoltage (V)    Regulated voltage coming out of the converter
/// @param[in]    mEfficiency    (--)    Efficiency of conversion process by the converter
/// @param[in]    mOutputOverCurrentLimit    (amp)   Output over current limit
/// @param[in]    mOutputOverVoltageLimit    (V)     Output over voltage limit
/// @param[in]    mOutOverCurrentTripActive  (--)    Output over current trip active flag
/// @param[in]    mOutOverVoltageTripActive  (--)    Output over voltage trip active flag
/// @param[in]    mInputOverVoltageLimit     (V)     Input over voltage limit
/// @param[in]    mInputUnderVoltageLimit    (V)     Input under voltage limit
/// @param[in]    mInOverVoltageTripActive   (--)    Input over voltage trip active flag
/// @param[in]    mInUnderVoltageTripActive  (--)    Input under voltage trip active flag
/// -
/// @brief Converter Input Data constructor.
///////////////////////////////////////////////////////////////////////////////////////////////////

ConverterInputData::ConverterInputData(const double mInputVoltage,
                                       const double mRegulatedVoltage,
                                       const double mEfficiency,
                                       const double mOutputOverCurrentLimit,
                                       const double mOutputOverVoltageLimit,
                                       const bool mOutOverCurrentTripActive,
                                       const bool mOutOverVoltageTripActive,
                                       const double mInputOverVoltageLimit,
                                       const double mInputUnderVoltageLimit,
                                       const bool mInOverVoltageTripActive,
                                       const bool mInUnderVoltageTripActive,
                                       const double mOutputOverCurrentFastLimit,
                                       const bool mOutOverCurrentFastTripActive,
                                       const double mInputOverCurrentLimit,
                                       const bool mInputOverCurrentTripActive
                                       )
:
mInputVoltage(mInputVoltage),
mRegulatedVoltage(mRegulatedVoltage),
mEfficiency(mEfficiency),
mOutputOverCurrentLimit(mOutputOverCurrentLimit),
mOutputOverVoltageLimit(mOutputOverVoltageLimit),
mOutOverCurrentTripActive(mOutOverCurrentTripActive),
mOutOverVoltageTripActive(mOutOverVoltageTripActive),
mInputOverVoltageLimit(mInputOverVoltageLimit),
mInputUnderVoltageLimit(mInputUnderVoltageLimit),
mInOverVoltageTripActive(mInOverVoltageTripActive),
mInUnderVoltageTripActive(mInUnderVoltageTripActive),
mOutputOverCurrentFastLimit(mOutputOverCurrentFastLimit),
mOutOverCurrentFastTripActive(mOutOverCurrentFastTripActive),
mInputOverCurrentLimit(mInputOverCurrentLimit),
mInputOverCurrentTripActive(mInputOverCurrentTripActive){
    // nothing to do
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that    (--)    Converter input data to be copied
/// -
/// @brief Converter Input Data copy constructor.
///////////////////////////////////////////////////////////////////////////////////////////////////

ConverterInputData::ConverterInputData(const ConverterInputData& that)
:
mInputVoltage(that.mInputVoltage),
mRegulatedVoltage(that.mRegulatedVoltage),
mEfficiency(that.mEfficiency),
mOutputOverCurrentLimit(that.mOutputOverCurrentLimit),
mOutputOverVoltageLimit(that.mOutputOverVoltageLimit),
mOutOverCurrentTripActive(that.mOutOverCurrentTripActive),
mOutOverVoltageTripActive(that.mOutOverVoltageTripActive),
mInputOverVoltageLimit(that.mInputOverVoltageLimit),
mInputUnderVoltageLimit(that.mInputUnderVoltageLimit),
mInOverVoltageTripActive(that.mInOverVoltageTripActive),
mInUnderVoltageTripActive(that.mInUnderVoltageTripActive),
mOutputOverCurrentFastLimit(that.mOutputOverCurrentFastLimit),
mOutOverCurrentFastTripActive(that.mOutOverCurrentFastTripActive),
mInputOverCurrentLimit(that.mInputOverCurrentLimit),
mInputOverCurrentTripActive(that.mInputOverCurrentTripActive){
    // nothing to do
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that    (--)    Converter input data to be copied
/// -
/// @brief Converter Input Data copy constructor.
///////////////////////////////////////////////////////////////////////////////////////////////////

ConverterInputData& ConverterInputData::operator =(const ConverterInputData& that) {
    if (this != &that) { // Gracefully handle self assignment
        mInputVoltage = that.mInputVoltage;
        mRegulatedVoltage = that.mRegulatedVoltage;
        mEfficiency = that.mEfficiency;
        mOutputOverCurrentLimit = that.mOutputOverCurrentLimit;
        mOutputOverVoltageLimit = that.mOutputOverVoltageLimit;
        mOutOverCurrentTripActive = that.mOutOverCurrentTripActive;
        mOutOverVoltageTripActive = that.mOutOverVoltageTripActive;
        mInputOverVoltageLimit = that.mInputOverVoltageLimit;
        mInputUnderVoltageLimit = that.mInputUnderVoltageLimit;
        mInOverVoltageTripActive = that.mInOverVoltageTripActive;
        mInUnderVoltageTripActive = that.mInUnderVoltageTripActive;
        mOutputOverCurrentFastLimit = that.mOutputOverCurrentFastLimit;
        mOutOverCurrentFastTripActive = that.mOutOverCurrentFastTripActive;
        mInputOverCurrentLimit = that.mInputOverCurrentLimit;
        mInputOverCurrentTripActive = that.mInputOverCurrentTripActive;
    }
    return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Converter Input Data destructor.
///////////////////////////////////////////////////////////////////////////////////////////////////

ConverterInputData::~ConverterInputData() {
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Converter constructor.
///////////////////////////////////////////////////////////////////////////////////////////////////

Converter::Converter()
:
mStandbyPower(0.0),
mInputVoltage(0.0),
mRegulatedVoltage(0.0),
mEfficiency(0.0),
mOutputOverCurrentLimit(0.0),
mOutputOverVoltageLimit(0.0),
mOutOverCurrentTripActive(false),
mOutOverVoltageTripActive(false),
mInputOverVoltageLimit(0.0),
mInputUnderVoltageLimit(0.0),
mInOverVoltageTripActive(false),
mInUnderVoltageTripActive(false),
mOutputOverCurrentTrip(false),
mOutputOverVoltageTrip(false),
mInputOverVoltageTrip(false),
mInputUnderVoltageTrip(false),
mResetTrips(false),
mTripOnSensedValue(false),
mConverterOn(false),
mBadEfficiency(false),
mInputCurrent(0.0),
mOutputCurrent(0.0),
mOutputVoltage(0.0),
mInputPower(0.0),
mOutputPower(0.0),
mHeatDissipation(0.0),
mOutputOverCurrentFastLimit(0.0),
mOutOverCurrentFastTripActive(false),
mOutputOverCurrentFastTrip(false),
mInputOverCurrentLimit(0.0),
mInputOverCurrentTrip(false),
mInputOverCurrentTripActive(false),
mThermalTuningFactor(0.5),
mBiasOutputPower(0.0) {
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Converter destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

Converter::~Converter() {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configDataIn       (--) Converter configuration data
/// @param[in] inputDataIn        (--) Converter input data
/// -
/// @return converterInitFlag (--) returns true if successfully initialized
/// -
/// @throw TsInitializationException
/// -
/// @brief Initialize converter utility with configuration and input data and
/// return init flag (true if successful initializing).
////////////////////////////////////////////////////////////////////////////////////////////////////

bool Converter::initialize(const ConverterConfigData* configDataIn,
                           const ConverterInputData* inputDataIn) {
    /// -- Default init flag is false
    bool converterInitFlag = false;

    /// --  Initialize from configuration data
    mStandbyPower = configDataIn->mStandbyPower;

    /// -- Initialize from input data
    mInputVoltage = inputDataIn->mInputVoltage;
    mRegulatedVoltage = inputDataIn->mRegulatedVoltage;
    mEfficiency = inputDataIn->mEfficiency;
    mOutputOverCurrentLimit = inputDataIn->mOutputOverCurrentLimit;
    mOutputOverVoltageLimit = inputDataIn->mOutputOverVoltageLimit;
    mOutOverCurrentTripActive = inputDataIn->mOutOverCurrentTripActive;
    mOutOverVoltageTripActive = inputDataIn->mOutOverVoltageTripActive;
    mInputOverVoltageLimit = inputDataIn->mInputOverVoltageLimit;
    mInputUnderVoltageLimit = inputDataIn->mInputUnderVoltageLimit;
    mInOverVoltageTripActive = inputDataIn->mInOverVoltageTripActive;
    mInUnderVoltageTripActive = inputDataIn->mInUnderVoltageTripActive;

    mOutputOverCurrentFastLimit = inputDataIn->mOutputOverCurrentFastLimit;
    mOutOverCurrentFastTripActive = inputDataIn->mOutOverCurrentFastTripActive;
    mInputOverCurrentLimit = inputDataIn->mInputOverCurrentLimit;
    mInputOverCurrentTripActive = inputDataIn->mInputOverCurrentTripActive;

    /// -- Validate config and input data
    std::string thrower = "Converter.initialize()";
    try {
        validate();
    } catch (const TsOutOfBoundsException& obe) {
        std::string msg = "Validation resulted in out of bound exception";
        std::string cause = "Either mStandbyPower, Input Limit, or mEfficiency is out of bound";
        TsInitializationException ie(msg, thrower, cause);
        TsHsMsg logger(TS_HS_ERROR, "EPS", ie.toString());
        logger.send(__FILE__, 999, "initialize()");
        throw ie;
    }

    /// -- Set init flag to true on successful initialization. This will not happen if any
    /// exceptions are thrown.
    converterInitFlag = true;

    return converterInitFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Method to validate configuration and input data to the converter.
/// -
/// @throw TsOutOfBoundsException
/// -
////////////////////////////////////////////////////////////////////////////////////////////////////

void Converter::validate() {

    std::string thrower = "Converter::validate()";
    std::string cause = "Invalid Initialization Data";

    /// -- Check if mStandbyPower is greater than 0.0
    if (mStandbyPower < 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "EPS", "Config data standby power less than 0.0",
            TsOutOfBoundsException, cause, thrower);
    }

    /// -- Check that input under voltage is not greater that input over voltage
    if (mInputUnderVoltageLimit >= mInputOverVoltageLimit) {
        std::ostringstream msg;
        msg << "Tried to set Input under voltage limit " << mInputUnderVoltageLimit << " less than Input over voltage limit " << mInputOverVoltageLimit;
        TS_HS_EXCEPTION(TS_HS_ERROR, "EPS", msg.str(),
            TsOutOfBoundsException, cause, thrower);
    }


    /// -- Check that Regulated Voltage is within output Voltage limit
    if (mRegulatedVoltage >= mOutputOverVoltageLimit) {
        std::ostringstream msg;
        msg << "Tried to set Regulated voltage " << mRegulatedVoltage << " less than Output over voltage limit " << mOutputOverVoltageLimit;
        TS_HS_EXCEPTION(TS_HS_ERROR, "EPS", msg.str(),
            TsOutOfBoundsException, cause, thrower);
    }

    /// -- Check for efficiency of the converter is in range (0 <= efficiency <= 1)
    if (!(MsMath::isInRange(0.0, mEfficiency, 1.0))) {
        std::ostringstream msg;
        msg << "Tried to set Efficiency out of bound " << mEfficiency;
        TS_HS_EXCEPTION(TS_HS_ERROR, "EPS", msg.str(),
            TsOutOfBoundsException, cause, thrower);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Update health and status flags using latest values, then check if
/// converter is enabled, is receiving valid voltage, and is not tripped.
////////////////////////////////////////////////////////////////////////////////////////////////////

void Converter::update() {
    if (mResetTrips) {
        resetTrips();
    }

    /// --  If sensor values are used, then input trips will be handled by sensed value passed
    // into the following method from the model itself
    //if (!mTripOnSensedValue) {
    //    handleInputTrips(mInputVoltage,mInputCurrent);
    //}

    /// -- Update health and status
    updateHealthStatus();

    if (0.0 == mInputVoltage) {
        mConverterOn = false;
    }

    /// -- Set output voltage depending on status of the converter
    if (mConverterOn) {
        mOutputVoltage = mRegulatedVoltage;
    } else {
        mOutputVoltage = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] inVoltage    (V)    Input voltage to be used for trip
/// -
/// @details This method will trip based on input voltage given. By default,
/// the method will use truth value, however user can set mTripOnSensedValue to
/// true to bypass truth value and use supplied sensor values by calling this
/// method from their model explicitly.
////////////////////////////////////////////////////////////////////////////////////////////////////

void Converter::handleInputTrips(double inVoltage, double inCurrent) {

    if (((inVoltage <= mInputUnderVoltageLimit) && mInUnderVoltageTripActive && mConverterOn)
        || mInputUnderVoltageTrip) {
        mInputUnderVoltageTrip = true;
        mConverterOn = false;
    } else {
        mInputUnderVoltageTrip = false;
    }


    if (((inVoltage >= mInputOverVoltageLimit) && mInOverVoltageTripActive && mConverterOn)
        || mInputOverVoltageTrip) {
        mInputOverVoltageTrip = true;
        mConverterOn = false;
    } else {
        mInputOverVoltageTrip = false;
    }

    if (((inCurrent >= mInputOverCurrentLimit) && mInputOverCurrentTripActive && mConverterOn)
        || mInputOverCurrentTrip) {
        mInputOverCurrentTrip = true;
        mConverterOn = false;
    } else {
        mInputOverCurrentTrip = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details
/// The Health and Status method checks all state data against the operating
/// limits defined in the configuration and input data. This method will check
/// current data against operating limits defined in the configuration data set.
/// Health flags will be set, and then the converter can doConversion if it is
/// receiving valid voltage, if it is enabled, and if it has no tripped values.
/// Data members marked as "Tripped" values will automatically disable the
/// converter if set to true. All others serve as warning flags which can be
/// handled by the user in some way.
////////////////////////////////////////////////////////////////////////////////////////////////////

void Converter::updateHealthStatus() {
    /// -- Check efficiency is within range
    if ((mEfficiency <= 0.0) || (mEfficiency > 1.0)) {
        mBadEfficiency = true;
    } else {
        mBadEfficiency = false;
    }

    /// -- Check if any trip or  bad efficiency or output disabled occurred
    if (mInputUnderVoltageTrip || mInputOverVoltageTrip ||
        mOutputOverVoltageTrip || mOutputOverCurrentTrip ||
        mOutputOverCurrentFastTrip || mInputOverCurrentTrip ||
        mBadEfficiency) {
        mConverterOn = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] outCurrent (amp) Output current of the converter
/// -
/// @details
/// This method will calculate new data for the converter.
/// It requires current output as an input to the function and will solve for
/// input current based on the equation  (Vin * Iin * Eff. = Vout * Iout).
/// Current output is passed in, voltage output is defined as regulated voltage
/// in input data, mEfficiency is defined as a input parameter, input voltage is
/// supplied as an input parameter to the converter, and Input current is
/// solved from the above equation.
////////////////////////////////////////////////////////////////////////////////////////////////////

void Converter::doConversion(double outCurrent) {
    mOutputCurrent = outCurrent;

    // into the following method from the model itself
    // Already done at this point
//    if (!mTripOnSensedValue) {
//        handleOutputTrips(mOutputVoltage, mOutputCurrent);
//    }

    mOutputPower = (mOutputCurrent * mOutputVoltage) + mBiasOutputPower;
    mOutputPower = std::max(0.0, mOutputPower);     // no negative power loads, plz

    if ((mEfficiency > 0.0) && mConverterOn) {
        if (mOutputPower <= 0.0) {
            mInputPower = mStandbyPower;
        } else {
            mInputPower = mOutputPower / mEfficiency;
        }
    } else {
        mInputPower = 0.0;
    }

    if ((mInputVoltage > DBL_EPSILON) && mConverterOn) {
        mInputCurrent = mInputPower / mInputVoltage;
    } else {
        mInputCurrent = 0.0;
    }

    // If sensor values are used, then input trips will be handled by sensed value passed
    // into the following method from the model itself
    // Already done at this point
    if (!mTripOnSensedValue) {
        handleInputTrips(mInputVoltage,mInputCurrent);
        handleOutputTrips(mOutputVoltage, mOutputCurrent);
    }

    calculateHeatDissipation();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] outVoltage    (V)    Output voltage (truth or sensed value)
/// @param[in] outCurrent    (amp) Output current (truth or sensed value)
/// -
/// @details This method will handle output trips depending on truth or sensed
/// values. By default, the method will use truth value, however, users can set
/// mTripOnSensedValue to true to bypass truth value and use supplied sensor
/// values by calling this method from their model explicitly.
////////////////////////////////////////////////////////////////////////////////////////////////////

void Converter::handleOutputTrips(double outVoltage, double outCurrent) {

    if (((outVoltage >= mOutputOverVoltageLimit) && mOutOverVoltageTripActive && mConverterOn)
        || mOutputOverVoltageTrip) {
        mOutputOverVoltageTrip = true;
        mConverterOn = false;
    } else {
        mOutputOverVoltageTrip = false;
    }

    if (((outCurrent >= mOutputOverCurrentFastLimit) && mOutOverCurrentFastTripActive && mConverterOn)
        || mOutputOverCurrentFastTrip) {
        mOutputOverCurrentFastTrip = true;
        mConverterOn = false;
    } else {
        mOutputOverCurrentFastTrip = false;
    }
    // check the slow current trip only if the fast one has not occurred

    if (((outCurrent >= mOutputOverCurrentLimit) && mOutOverCurrentTripActive && mConverterOn)
        || mOutputOverCurrentTrip) {
        mOutputOverCurrentTrip = true;
        mConverterOn = false;
    } else {
        mOutputOverCurrentTrip = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @details Method to calculate heat dissipated by the conversion process.
////////////////////////////////////////////////////////////////////////////////
inline void Converter::calculateHeatDissipation() {
    // some portion of the output power factors into the heat dissipated to match the behavior
    // in PRB0044091 :
    // DDCU w/ 1kW load = 3 hours 50 minutes to overheat
    // DDCU w/ 3kW load = 2 hours 12 minutes to overheat
    mHeatDissipation = (mInputPower - mOutputPower);

    // system has been tuned so that the heat above matches the 1kW = 4 hours
    // now add a factor to shorten that time with more output load
    double lThermalFactor = mOutputPower / 1000.0;   // less than 1 for smaller loads, greater for larger loads

    // find just the new heat part
    double lHeatFromLoad = (mHeatDissipation * lThermalFactor) - mHeatDissipation;

    // work in a tuning factor so we can control how much faster a greater heat load makes a DDCU warm up
    lHeatFromLoad *= mThermalTuningFactor;

    mHeatDissipation = mHeatDissipation + lHeatFromLoad;
}

////////////////////////////////////////////////////////////////////////////////
/// @param [in] double value (W)   The amount of watts to add to the output load
/// @details Method to set the amount
////////////////////////////////////////////////////////////////////////////////
void Converter::setBiasOutputPower(const double value) {
    mBiasOutputPower = value;
}
