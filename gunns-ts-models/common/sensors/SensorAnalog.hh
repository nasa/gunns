#ifndef SensorAnalog_EXISTS
#define SensorAnalog_EXISTS

/**
@defgroup  TSM_SENSORS_ANALOG_SENSOR    Generic Analog Sensor
@ingroup   TSM_SENSORS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Generic model for an analog sensor.)

REQUIREMENTS:
- (R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise, drift and bias
                 malfunctions.
   R.TS210-0021: TS21 vehicle subsystem models shall provide sensed and actual data.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((SensorAnalog.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2012-04) (Initial))

@{
*/

#include "SensorBase.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "math/UnitConversion.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Generic Analog Sensor Configuration Data
///
/// @details  This class provides a data structure for the Analog Sensor configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SensorAnalogConfigData : public SensorBaseConfigData
{
    public:
        /// @brief A function pointer type of signature: double function(), used by the sensor class
        ///        and this config data class to store pointers to external noise functions.
        typedef double (*fctnPtr)();

        float                mMinRange;          /**<    (--) trick_chkpnt_io(**) Minimum sensed output value, in output units */
        float                mMaxRange;          /**<    (--) trick_chkpnt_io(**) Maximum sensed output value, in output units */
        float                mOffValue;          /**<    (--) trick_chkpnt_io(**) Sensed output value when the sensor is off, in output units */
        float                mNominalBias;       /**<    (--) trick_chkpnt_io(**) Nominal bias in the sensed output, in output units */
        float                mNominalScale;      /**<    (--) trick_chkpnt_io(**) Nominal scale factor applied to the sensed output */
        float                mNominalNoiseScale; /**<    (--) trick_chkpnt_io(**) Nominal constant noise amplitude in the sensed output, in output units */
        float                mNominalResolution; /**<    (--) trick_chkpnt_io(**) Nominal sensor resolution, in output units */
        fctnPtr              mNoiseFunction;     /**< ** (--) trick_chkpnt_io(**) Pointer to noise function for this sensor */
        UnitConversion::Type mUnitConversion;    /**<    (--) trick_chkpnt_io(**) Unit conversion type to convert truth input to output units */
        float                mNominalNoiseMult;  /**<    (--) trick_chkpnt_io(**) Nominal input-scaling noise amplitude in the sensed output, in fraction of total sensor output range */

        /// @brief Default constructs this Analog Sensor configuration data.
        SensorAnalogConfigData(const float                minRange          = 0.0,
                               const float                maxRange          = 0.0,
                               const float                offValue          = 0.0,
                               const float                nominalBias       = 0.0,
                               const float                nominalScale      = 1.0,
                               const float                nominalNoiseScale = 0.0,
                               const float                nominalResolution = 0.0,
                               fctnPtr                    noiseFunction     = 0,
                               const UnitConversion::Type unitConversion    = UnitConversion::NO_CONVERSION,
                               const float                nominalNoiseMult  = 0.0);

        /// @brief Default destructs this Analog Sensor configuration data.
        virtual ~SensorAnalogConfigData();

        /// @brief Copy constructs this Analog Sensor configuration data.
        SensorAnalogConfigData(const SensorAnalogConfigData& that);

        /// @brief Assignment operator for this Analog Sensor configuration data.
        SensorAnalogConfigData& operator =(const SensorAnalogConfigData& that);

        /// @brief Sets the noise function pointer to the given function.
        void setNoiseFunction(double (*function)());
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Generic Analog Sensor Input Data
///
/// @details  This class provides a data structure for the Analog Sensor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SensorAnalogInputData : public SensorBaseInputData
{
    public:
        double mTruthInput;             /**< (--) Initial value of the input truth value to sense */
        float  mMalfFailToValue;        /**< (--) Fail-to-value malfunction value                 */
        bool   mMalfScaleFlag;          /**< (--) Scale malfunction activation flag               */
        float  mMalfScaleValue;         /**< (--) Scale malfunction value                         */
        bool   mMalfBiasFlag;           /**< (--) Bias malfunction activation flag                */
        float  mMalfBiasValue;          /**< (--) Bias malfunction value                          */
        bool   mMalfDriftFlag;          /**< (--) Drift malfunction activation flag               */
        float  mMalfDriftRate;          /**< (--) Drift malfunction rate value                    */
        bool   mMalfNoiseFlag;          /**< (--) Noise malfunction activation flag               */
        float  mMalfNoiseScale;         /**< (--) Noise malfunction constant amplitude value      */
        float  mMalfNoiseMult;          /**< (--) Noise malfunction input-scaling amplitude value */
        bool   mMalfResolutionFlag;     /**< (--) Resolution malfunction activation flag          */
        float  mMalfResolutionValue;    /**< (--) Resolution malfunction amount                   */
        float  mDrift;                  /**< (--) Current drift amount during drift malfunction   */

        /// @brief    Default constructs this Analog Sensor input data.
        SensorAnalogInputData(const bool   powerFlag           = false,
                              const double truthInput          = 0.0);

        /// @brief    Default destructs this Analog Sensor input data.
        virtual ~SensorAnalogInputData();

        /// @brief    Copy constructs this Analog Sensor input data.
        SensorAnalogInputData(const SensorAnalogInputData& that);

        /// @brief Assignment operator for this Analog Sensor configuration data.
        SensorAnalogInputData& operator =(const SensorAnalogInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Analog Sensor
///
/// @details  This is the generic model for an analog sensor.  It outputs a floating-point (analog)
///           modeled sensed value based on a floating-point input.  This sensor is designed to
///           work on the same engineering units as provided in its input, without converting the
///           units.  All conversions between desired units and the units in the supplied input are
///           the responsibility of the user and interfacing systems.
///
///           In addition to the power fail on, power fail off, stuck, fail-to and perfect sensor
///           malfunctions common with the other sensor classes, this class also models nominal and
///           malfunction sensor scale, bias, drift, noise, and resolution.  All of these effects
///           are optional.  The noise logic can use any external function supplied by the config
///           data as long as it has the correct method signature.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SensorAnalog : public SensorBase
{
    TS_MAKE_SIM_COMPATIBLE(SensorAnalog);

    public:
        /// @name     Malfunctions.
        /// @{
        /// @details  Malfunctions are declared public to allow the sim's malfunction hooks sim
        ///           object to only require a pointer to this sensor object, and not individual
        ///           pointers to each malfunction term.
        float  mMalfFailToValue;        /**< (--) Fail-to-value malfunction value                 */
        bool   mMalfScaleFlag;          /**< (--) Scale malfunction activation flag               */
        float  mMalfScaleValue;         /**< (--) Scale malfunction value                         */
        bool   mMalfBiasFlag;           /**< (--) Bias malfunction activation flag                */
        float  mMalfBiasValue;          /**< (--) Bias malfunction value                          */
        bool   mMalfDriftFlag;          /**< (--) Drift malfunction activation flag               */
        float  mMalfDriftRate;          /**< (--) Drift malfunction rate value                    */
        bool   mMalfNoiseFlag;          /**< (--) Noise malfunction activation flag               */
        float  mMalfNoiseScale;         /**< (--) Noise malfunction constant amplitude value      */
        float  mMalfNoiseMultiplier;    /**< (--) Noise malfunction input-scaling amplitude value */
        bool   mMalfResolutionFlag;     /**< (--) Resolution malfunction activation flag          */
        float  mMalfResolutionValue;    /**< (--) Resolution malfunction amount                   */
        float  mDrift;                  /**< (--) Current drift amount during drift malfunction   */
        /// @}

        /// @brief Default constructor.
        SensorAnalog();

        /// @brief Default destructor.
        virtual     ~SensorAnalog();

        /// @brief Initializes the Analog Sensor object.
        void         initialize(const SensorAnalogConfigData& configData,
                                      const SensorAnalogInputData&  inputData,
                                      const std::string&            name);

        /// @brief Updates the sensed output of the sensor.
        virtual void update(const double timeStep);

        /// @brief Updates and returns the sensed output of the sensor given inputs.
        float        sense(const double timeStep,
                                 const bool   powerFlag,
                                 const double truthInput);

        /// @brief Setter method to set the input truth value to sense.
        void         setTruthInput(const double truthInput);

        /// @brief Getter method to get the input truth value to sense.
        double       getTruthInput() const;

        /// @brief Getter method to get the input truth value converted to output units.
        double       getTruthOutput() const;

        /// @brief Getter method to get the sensed output value.
        float        getSensedOutput() const;

        /// @brief Determines the degradation state of the sensor.
        virtual bool determineDegradation() const;

    protected:
        float  mMinRange;          /**< (--) trick_chkpnt_io(**) Minimum sensed output value, in output units */
        float  mMaxRange;          /**< (--) trick_chkpnt_io(**) Maximum sensed output value, in output units */
        float  mOffValue;          /**< (--) trick_chkpnt_io(**) Sensed output value when the sensor is off, in output units */
        float  mNominalBias;       /**< (--) trick_chkpnt_io(**) Nominal bias in the sensed output, in output units */
        float  mNominalScale;      /**< (--) trick_chkpnt_io(**) Nominal scale factor applied to the sensed output */
        float  mNominalNoiseScale; /**< (--) trick_chkpnt_io(**) Nominal constant noise amplitude in the sensed output, in output units */
        float  mNominalNoiseMult;  /**< (--) trick_chkpnt_io(**) Nominal input-scaling noise amplitude in the sensed output, in fraction of total sensor output range */
        float  mNominalResolution; /**< (--) trick_chkpnt_io(**) Nominal sensor resolution, in output units */
        SensorAnalogConfigData::fctnPtr mNoiseFunction; /**< ** (--) trick_chkpnt_io(**) Pointer to noise function */
        UnitConversion::Type mUnitConversion;           /**< (--) trick_chkpnt_io(**) Unit conversion type to convert truth input to output units */
        double mTruthInput;        /**< (--) Input truth value to sense in input units */
        double mTruthOutput;       /**< (--) Input truth value converted to output units */
        float  mSensedOutput;      /**< (--) Sensed output value of the sensor, in output units */

        /// @brief Validates the initial conditions.
        void         validate() const;

        /// @brief Performs transformations on the truth input as needed.
        virtual void processInput();

        /// @brief Performs transformations on the sensed output as needed.
        virtual void processOutput();

        /// @brief Applies nominal and malfunction scaling to the sensor output.
        void         applyScale();

        /// @brief Applies nominal and malfunction bias to the sensor output.
        void         applyBias();

        /// @brief Applies drift malfunction to the sensor output.
        void         applyDrift(const double timeStep);

        /// @brief Applies nominal and malfunction noise to the sensor output.
        void         applyNoise();

        /// @brief Applies nominal and malfunction resolution to the sensor output.
        void         applyResolution();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        SensorAnalog(const SensorAnalog&);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        SensorAnalog& operator =(const SensorAnalog&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] function (--) Pointer to function of signature: double function().
///
/// @details  Sets the mNoiseFunction attribute to the given function pointer.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void SensorAnalogConfigData::setNoiseFunction(double (*function)())
{
    mNoiseFunction = function;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] truthInput (--) The truth input analog value to the sensor.
///
/// @details  The sensor's truth input value will usually be set by the simbus, but this method
///           allows other objects to set it directly.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void SensorAnalog::setTruthInput(const double truthInput)
{
    mTruthInput = truthInput;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (--) The truth input analog value to the sensor.
///
/// @details  Allows other objects to directly see the truth input value to the sensor without
///           needing to access the simbus.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double SensorAnalog::getTruthInput() const
{
    return mTruthInput;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (--) The truth input analog value to the sensor, converted to output units.
///
/// @details  Allows other objects to directly see the truth output value to the sensor without
///           needing to access the simbus.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double SensorAnalog::getTruthOutput() const
{
    return mTruthOutput;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  float (--) The sensed output value.
///
/// @details  Allows other objects to directly see the sensed output value of the sensor without
///           needing to access the simbus.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline float SensorAnalog::getSensedOutput() const
{
    return mSensedOutput;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The sensor is considered degraded if any malfunction that is usually used to degrade
///           the sensor output is active (regardless of whether the malfunction is actually
///           degrading the output at this time), but not if the perfect sensor malfunction is
///           active.  This method should be overridden to include further malfunctions in the
///           derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool SensorAnalog::determineDegradation() const
{
    return ((mMalfFailToFlag or mMalfFailOffFlag or mMalfFailStuckFlag or mMalfScaleFlag or
             mMalfBiasFlag   or mMalfDriftFlag   or mMalfNoiseFlag     or mMalfResolutionFlag)
             and not mMalfPerfectSensor);
}

#endif
