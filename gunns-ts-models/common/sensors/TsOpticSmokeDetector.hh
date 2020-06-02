#ifndef TsOpticSmokeDetector_EXISTS
#define TsOpticSmokeDetector_EXISTS

/**
@defgroup  TSM_SENSORS_TsOpticSmokeDetector Photoelectric Smoke Detector
@ingroup   TSM_SENSORS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
 - (Model the behavior of an Optic Photoelectric Smoke Detector.)

 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - (common/sensors/TsOpticSmokeDetector.o)

 PROGRAMMERS:
 - ((Jose A. Perez) (L3-Comm) (Mar 23, 2012) (TS21) (initial))
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "common/sensors/SensorAnalog.hh"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TSM Optic Smoke Detector Config Data class.
///////////////////////////////////////////////////////////////////////////////////////////////////
class TsOpticSmokeDetectorConfigData
{
public:
    float obsMaxPercentage;     /**< (--)   trick_chkpnt_io(**) The maximum obscuration percentage SD can output.*/
    float obsMinPercentage;     /**< (--)   trick_chkpnt_io(**) The minimum obscuration percentage SD can output.*/
    float obsSmokeContributer;  /**< (--)   trick_chkpnt_io(**) This is the smoke attribute that effects the obscuration value.*/
    float scatterMaxPercentage; /**< (--)   trick_chkpnt_io(**) The maximum scatter percentage SD can output.*/
    float scatterMinPercentage; /**< (--)   trick_chkpnt_io(**) The minimum scatter percentage SD can output.*/
    float bitOnMaxScatterValue; /**< (--)   trick_chkpnt_io(**) The maximum scatter value when performing bit test. */
    float nominalObscuration;   /**< (--)   trick_chkpnt_io(**) The nominal obscuration percentage SD runs at.*/
    float nominalScatter;       /**< (--)   trick_chkpnt_io(**) The nominal scatter percentage SD runs at.*/
    float bitRecoveryTime;      /**< (s)    trick_chkpnt_io(**) The total time Active Bit should perform.*/
    float obsOffValue;          /**< (--)   trick_chkpnt_io(**) Obscuration smoke detector off value.*/
    float scatOffValue;         /**< (1/ft) trick_chkpnt_io(**) Scatter smoke detector off value.*/
    float obsPercentVoltSlope;  /**< (V)    trick_chkpnt_io(**) Obscuration Percent voltage slope for conversion.*/
    float obsVoltIntercept;     /**< (V)    trick_chkpnt_io(**) Obscuration Voltage slope y-intercept for conversion.*/
    float scatPercentVoltSlope; /**< (V)    trick_chkpnt_io(**) Scatter Percent voltage slope for conversion.*/
    float scatVoltIntercept;    /**< (V)    trick_chkpnt_io(**) Scatter Voltage slope y-intercept for conversion.*/

    SensorAnalogConfigData obsSensorConfigD;     /**< (--) trick_chkpnt_io(**) Obscuration Sensor config data.*/
    SensorAnalogConfigData scatterSensorConfigD; /**< (--) trick_chkpnt_io(**) Scatter Sensor config data.*/

    /// @brief Default constructor for Config Data class.
    TsOpticSmokeDetectorConfigData();
    /// @brief Default destructor for Config Data class.
    virtual ~TsOpticSmokeDetectorConfigData();

protected:
private:
    /// @brief Unimplemented
    TsOpticSmokeDetectorConfigData(const TsOpticSmokeDetectorConfigData &);
    /// @brief Unimplemented
    TsOpticSmokeDetectorConfigData& operator =(const TsOpticSmokeDetectorConfigData &);
};


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TSM Optic Smoke Detector Input Data class.
///////////////////////////////////////////////////////////////////////////////////////////////////
class TsOpticSmokeDetectorInputData
{
public:
    int  isLoadOn;           /**< (--)  1 if model is consuming power, 0 if not. */
    bool isPowered;          /**< (--)  True if model is powered. */
    bool isBitEnabled;       /**< (--)  True if Active Bit test has been enabled. */
    float smokeLevelFactor;  /**< (--)  Smoke Factor value from 0.0 to 1.0. */
    float bitTimer;          /**< (--)  Time counter that keeps time when isBitEnabled is true. */

    SensorAnalogInputData obsSensorInputD;     /**< (--) Obscuration Sensor input data.*/
    SensorAnalogInputData scatterSensorInputD; /**< (--) Scatter Sensor input data.*/

    /// @brief Default constructor for Input Data class.
    TsOpticSmokeDetectorInputData();
    /// @brief Default destructor for Input Data class.
    virtual ~TsOpticSmokeDetectorInputData();
protected:
private:
    /// @brief Unimplemented
    TsOpticSmokeDetectorInputData(const TsOpticSmokeDetectorInputData &);
    /// @brief Unimplemented
    TsOpticSmokeDetectorInputData& operator =(const TsOpticSmokeDetectorInputData &);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief TsOpticSmokeDetector Class
/// @details  The Optic Smoke Detector consist of two photoelectric sensors, the obscuration and
///           scatter sensors. The obscuration sensor measures the light intensity the light source
///           is producing. The scatter sensor measures the 'light scattering' of the light source
///           due to smoke or any other type of light obstruction. The smoke detector is capable of
///           performing an Active Bit test when ever it is commanded to do so. This is done to
///           verify the smoke detector is functioning properly. The smoke detector outputs are
///           obscuration and scatter percentages. Its inputs are a power command,a bit enable
///           command, and malfunctions. The malfunctions include malfunctioning the scatter and
///           obscuration telemetry, a power malfunction and a malfunction of the Smoke Detector
///           as a whole.
///////////////////////////////////////////////////////////////////////////////////////////////////
class TsOpticSmokeDetector
{
    TS_MAKE_SIM_COMPATIBLE(TsOpticSmokeDetector);

public:
    /// @name    Embedded objects.
    /// @{
    /// @details Embedded objects are public to allow access from the Trick events processor.
    SensorAnalog obscurationSensor;         /**< (--) Obscuration sensor. */
    SensorAnalog scatterSensor;             /**< (--) Scatter sensor. */
    float smokeLevelFactor;       /**< (--)    A 0.0 to 1.0 value to represent the quantity of smoke on the testing volume. */
    /// @}

    /// @name    Malfunction targets.
    /// @{
    /// @details Malfunction targets are public to allow access from the Trick events processor.
    bool malfTotalFail;           /**< (--)    True if want to fail the smoke detector as a whole. */
    bool malfPowerToOn;           /**< (--)    True if want to want the load to be on even though isPowered is false. */
    /// @}

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief TsOpticSmokeDetector Default Class constructor.
    ///////////////////////////////////////////////////////////////////////////////////////////////
    TsOpticSmokeDetector();

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief TsOpticSmokeDetector Class destructor.
    ///////////////////////////////////////////////////////////////////////////////////////////////
    virtual ~TsOpticSmokeDetector();

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// @param[in] configData   (--) Model's Config data.
    /// @param[in] inputData    (--) Model's Input Data.
    /// @param[in] name         (--) Instance name for self-identification in messages.
    ///
    /// @throws TsConfigurationException if model not configurized properly.
    ///
    /// @brief Method that sets config and input data for the smoke detector model.
    ///
    /// @return void
    ///////////////////////////////////////////////////////////////////////////////////////////////
    void initialize(
        const TsOpticSmokeDetectorConfigData &configData,
        const TsOpticSmokeDetectorInputData  &inputData,
        const std::string                    &name);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// @param[in] dt  (s) Job cycle model gets called at.
    ///
    /// @throws   TsInitializationException If model has not been initialized.
    ///
    /// @brief Method that executes the smoke detector's behavior, that is generates obscuration
    ///        and scatter percentage values. Calls the method performBitTest.
    ///
    /// @return void
    ///////////////////////////////////////////////////////////////////////////////////////////////
    void update(
        const float dt);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Method that returns the obscuration sensed percentage value smoke detector has
    ///        generated.
    ///
    /// @return sensedValue (--) Obscuration sensed value from the obscuration sensor.
    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline float getSensedObscurationValue() const
    {
        return obscurationSensor.getSensedOutput();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Method that returns the scatter sensed percentage value smoke detector has
    ///        generated.
    ///
    /// @return sensedValue (--) scatter sensed value from the scatter sensor.
    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline float getSensedScatterValue() const
    {
        return scatterSensor.getSensedOutput();
    }

    /// @brief Returns the initialized status of the smoke detector.
    bool               isInitialized() const;

private:
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// @param[in] dt  (s) Job cycle model gets called at.
    ///
    /// @brief Method that executes the smoke detector's Active Bit Test when ever the isBitEnable
    ///        flag is set to true.
    ///
    /// @return void
    ///////////////////////////////////////////////////////////////////////////////////////////////
    void performBitTest(
        const float dt);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Method that checks if input and config data is set properly, else throws config
    ///        exceptions.
    ///
    /// @throws TsInitializationException if model is not configurized properly.
    ///
    /// @return void
    ///////////////////////////////////////////////////////////////////////////////////////////////
    void validate();

protected:
    int   isLoadOn;              /**<    (--)                       1 if smoke detector is consuming power, 0 if is not. */
    bool  isPowered;             /**<    (--)                       True if smoke detector is being powered by EPS. */
    bool  isBitEnabled;          /**<    (--)                       True if smoke detector has been commanded to perform Active Bit Test. */
    bool  isBitInProcess;        /**<    (--)                       True if smoke detector is performing the Active Bit Test. */
    float obscurationPercentage; /**<    (--)                       The true obscuration percentage telemetry. */
    float scatterPercentage;     /**<    (1/ft)                     The true scatter percentage telemetry. */
    float bitTimer;              /**<    (s)                        Time counter that keeps time when isBitEnabled is true. */
    float obsSensedVoltage;      /**<    (V)                        Obscuration sensed voltage. */
    float scatSensedVoltage;     /**<    (V)                        Scatter sensed voltage. */
    std::string mName;           /**< *o (--)   trick_chkpnt_io(**) Smoke detector's name. */
    bool  mInitialized;          /**<    (--)   trick_chkpnt_io(**) True if smoke detector model has been initialized. */

    TsOpticSmokeDetectorConfigData configD; /**< (--) trick_chkpnt_io(**) Smoke Detector Config Data instance. */

private:
    /// @brief keep private, never used
    TsOpticSmokeDetector(const TsOpticSmokeDetector &);

    /// @brief keep private, never used
    TsOpticSmokeDetector& operator= (const TsOpticSmokeDetector &);
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns bool (--) True if the sensor has been properly initialized, false if not.
///
/// @details  Returns the initialized status of the sensor.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsOpticSmokeDetector::isInitialized() const
{
    return mInitialized;
}

/// @}

#endif //TsOpticSmokeDetector_EXISTS
