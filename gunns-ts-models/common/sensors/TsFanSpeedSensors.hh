#ifndef TsFanSpeedSensors_EXISTS
#define TsFanSpeedSensors_EXISTS

/**
@defgroup  TSM_SENSORS_FAN_SPEED_SENSORS Fan Speed Sensors
@ingroup   TSM_SENSORS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
 - (Iss Fans have 3 speed sensors, not all of which may be used for telemetry. The average
    of the three speed sensors is used by the fan controller. )

 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - (TsFanSpeedSensors.o)

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Update) (2012-12))
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

#include "SensorAnalog.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fan Speed Sensors Model configuration data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fan Speed
///           Sensors Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsFanSpeedSensorsConfigData
{
    public:
        SensorAnalogConfigData  mSensor;  /**< (--)  trick_chkpnt_io(**) Common speed sensor configuration data. */
        /// @brief    Default constructs this Fan Speed Sensors Model configuration data.
        TsFanSpeedSensorsConfigData(const SensorAnalogConfigData& sensor = SensorAnalogConfigData());
        /// @brief    Copy constructs this Fan Speed Sensors Model configuration data.
        TsFanSpeedSensorsConfigData(const TsFanSpeedSensorsConfigData& that);
        /// @brief    Default destructs this Fan Speed Sensors Model configuration data.
        virtual ~TsFanSpeedSensorsConfigData();
        /// @brief    Assigns this Fan Speed Sensors Model configuration data.
        TsFanSpeedSensorsConfigData& operator=(const TsFanSpeedSensorsConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fan Speed Sensors Model input data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fan Speed
///           Sensors Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsFanSpeedSensorsInputData
{
    public:
        SensorAnalogInputData mSensorA;  /**< (--)  Speed sensor A input data. */
        SensorAnalogInputData mSensorB;  /**< (--)  Speed sensor B input data. */
        SensorAnalogInputData mSensorC;  /**< (--)  Speed sensor C input data. */
        /// @brief    Default constructs this Fan Speed Sensors Model input data.
        TsFanSpeedSensorsInputData(const SensorAnalogInputData& sensorA = SensorAnalogInputData(),
                                    const SensorAnalogInputData& sensorB = SensorAnalogInputData(),
                                    const SensorAnalogInputData& sensorC = SensorAnalogInputData());
        /// @brief    Copy constructs this Fan Speed Sensors Model input data.
        TsFanSpeedSensorsInputData(const TsFanSpeedSensorsInputData& that);
        /// @brief    Default destructs this Fan Speed Sensors Model input data.
        virtual ~TsFanSpeedSensorsInputData();
        /// @brief    Assigns this Fan Speed Sensors Model input data.
        TsFanSpeedSensorsInputData& operator=(const TsFanSpeedSensorsInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fan Speed Sensors Model
///
/// @details  Contains the three speed sensors for an ISS fan or pump motor. The individual sensor
///           supply voltage flag and truth value may be set via simbus or by setter calls and then
///           the update method may be called. Alternately, the sense method may be used with
///           arguments for the supply voltage flag and truth value.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsFanSpeedSensors
{
    TS_MAKE_SIM_COMPATIBLE(TsFanSpeedSensors);
    public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        SensorAnalog  mSensorA;       /**< (--)      Speed sensor A. */
        SensorAnalog  mSensorB;       /**< (--)      Speed sensor B. */
        SensorAnalog  mSensorC;       /**< (--)      Speed sensor C. */
        /// @}

        /// @brief    Default constructs this Fan Speed Sensors Model.
        TsFanSpeedSensors();
        /// @brief    Default destructs this Fan Speed Sensors Model.
        virtual ~TsFanSpeedSensors();
        /// @brief    Initializes this Fan Speed Sensors Model.
        void initialize(const TsFanSpeedSensorsConfigData& configData,
                        const TsFanSpeedSensorsInputData&  inputData,
                        const std::string&                  name = "FanSpeed");
        /// @brief    Updates this Fan Speed Sensors Model.
        void update(const double dt);
        /// @brief    Updates this Fan Speed Sensors Model.
        float sense(const double dt, const bool powerFlag, const double truthInput);
        /// @brief    Sets this Fan Speed Sensors Model power flag.
        void setPowerFlag(const bool powerFlag);
        /// @brief    Sets this Fan Speed Sensors Model truth value speed.
        void setTruthInput(const double speed);
        /// @brief    Gets this Fan Speed Sensors Model truth value speed.
        double getTruthInput() const;
        /// @brief    Gets this Fan Speed Sensors Model sensed speed.
        float getSensedOutput() const;
        /// @brief    Returns this PCA Controller model initialization complete flag.
        bool isInitialized() const;
    protected:
        std::string   mName;          /**< *o (--)             trick_chkpnt_io(**) Object name. */
        float         mAverageSpeed;  /**<    (revolution/min)                     Average sensed speed. */
        bool          mInitialized;   /**< *o (--)             trick_chkpnt_io(**) Initialization complete flag. */
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsFanSpeedSensors(const TsFanSpeedSensors&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsFanSpeedSensors operator= (const TsFanSpeedSensors&);
};

/// @}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  powerFlag  (--)  Power flag.
///
/// @return   void
///
/// @details  Sets the power flag for each sensor in this Fan Speed Sensors Model.
///////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsFanSpeedSensors::setPowerFlag(const bool powerFlag)
{
    mSensorA.setPowerFlag(powerFlag);
    mSensorB.setPowerFlag(powerFlag);
    mSensorC.setPowerFlag(powerFlag);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  speed  (revolution/min)  Truth value speed.
///
/// @return   void
///
/// @details  Sets the truth value speed for each sensor in this Fan Speed Sensors Model.
///////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsFanSpeedSensors::setTruthInput(const double speed )
{
    mSensorA.setTruthInput(speed);
    mSensorB.setTruthInput(speed);
    mSensorC.setTruthInput(speed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (revolution/min)  Truth value speed.
///
/// @details  Returns this Fan Speed Sensors Model truth value speed.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline  double TsFanSpeedSensors::getTruthInput() const
{
    return mSensorA.getTruthInput();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (revolution/min)  Sensed speed.
///
/// @details  Returns this Fan Speed Sensors Model average sensed speed.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline float TsFanSpeedSensors::getSensedOutput() const
{
    return mAverageSpeed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  True if initialization completed successfully, false otherwise.
///
/// @details  Returns this Fan Speed Sensors Model initialization complete flag.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsFanSpeedSensors::isInitialized() const
{
    return mInitialized;
}

#endif
