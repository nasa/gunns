#ifndef SensorBase_EXISTS
#define SensorBase_EXISTS

/**
@defgroup  TSM_SENSORS_SENSOR_BASE  Generic Sensor Base
@ingroup   TSM_SENSORS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (This is the base class for generic sensors.  It is a pure virtual class and cannot be directly
   instantiated.  It implements some functionality common to all derived sensor classes.)

REQUIREMENTS:
- (R.TS210-0004: TS21 vehicle subsystem models shall provide an override for nominal component
                 functionality regardless of power availability.
   R.TS210-0005: TS21 vehicle subsystem models shall provide an override for preventing nominal
                 component functionality regardless of power availability.
   R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise, drift and bias
                 malfunctions.
   R.TS210-0011: TS21 vehicle subsystem models shall receive the current state of the electrical
                 network system.
   R.TS210-0021: TS21 vehicle subsystem models shall provide sensed and actual data.)

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

LIBRARY_DEPENDENCY:
- ((SensorBase.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2012-04) (Initial))

@{
*/

#include <string>
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Generic Sensor Base Configuration Data
///
/// @details  This class provides a data structure for the Sensor Base configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SensorBaseConfigData
{
    public:
        /// @brief Default constructs this Sensor Base configuration data.
        SensorBaseConfigData();

        /// @brief Default destructs this Sensor Base configuration data.
        virtual ~SensorBaseConfigData();

        /// @brief Copy constructs this Sensor Base configuration data.
        SensorBaseConfigData(const SensorBaseConfigData& that);

        /// @brief    Assignment operator for this Sensor Base configuration data.
        SensorBaseConfigData& operator =(const SensorBaseConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Generic Sensor Base Input Data
///
/// @details  This class provides a data structure for the Sensor Base input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SensorBaseInputData
{
    public:
        bool mPowerFlag;                /**< (--) Initial powered state of the sensor             */
        bool mMalfFailToFlag;           /**< (--) Fail-to-value malfunction activation flag       */
        bool mMalfFailOffFlag;          /**< (--) Fail-off malfunction activation flag            */
        bool mMalfFailStuckFlag;        /**< (--) Fail-stuck malfunction activation flag          */
        bool mMalfIgnorePower;          /**< (--) Flag causes sensor to be on regardless of power */
        bool mMalfPerfectSensor;        /**< (--) Flag causes sensor to be perfect                */

        /// @brief    Default constructs this Sensor Base input data.
        explicit SensorBaseInputData(const bool powerFlag = false);

        /// @brief    Default destructs this Sensor Base input data.
        virtual ~SensorBaseInputData();

        /// @brief    Copy constructs this Sensor Base input data.
        SensorBaseInputData(const SensorBaseInputData& that);

        /// @brief    Assignment operator for this Sensor Base input data.
        SensorBaseInputData& operator =(const SensorBaseInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sensor Base
///
/// @details  This is the generic sensor base class for any derived generic sensors.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SensorBase
{
    TS_MAKE_SIM_COMPATIBLE(SensorBase);

    public:
        /// @name     Malfunctions.
        /// @{
        /// @details  Malfunctions are declared public to allow the sim's malfunction hooks sim
        ///           object to only require a pointer to this sensor object, and not individual
        ///           pointers to each malfunction term.
        bool  mMalfFailToFlag;          /**< (--) Fail-to-value malfunction activation flag       */
        bool  mMalfFailOffFlag;         /**< (--) Fail-off malfunction activation flag            */
        bool  mMalfFailStuckFlag;       /**< (--) Fail-stuck malfunction activation flag          */
        bool  mMalfIgnorePower;         /**< (--) Flag causes sensor to be on regardless of power */
        bool  mMalfPerfectSensor;       /**< (--) Flag causes sensor to be perfect                */
        /// @}

        /// @brief Default destructor.
        virtual           ~SensorBase();

        /// @brief Initializes the sensor base object.
        void               initialize(const SensorBaseConfigData& configData,
                                      const SensorBaseInputData&  inputData,
                                      const std::string&          name);

        /// @brief Updates the sensed output of the sensor.
        ///
        /// @details This class implementation is pure virtual and must be implemented in a derived
        ///          class.
        virtual void       update(const double timeStep) = 0;

        /// @brief Returns the initialized status of the sensor.
        bool               isInitialized() const;

        /// @brief Sets the sensor's powered state.
        virtual void       setPowerFlag(const bool powerFlag);

        /// @brief Returns the sensor's name.
        const char*        getName() const;

    protected:
        std::string mName;   /**< *o (--) trick_chkpnt_io(**) Name of the sensor for output messages */
        bool  mPowerFlag;    /**<    (--)                     Powered state of the sensor            */
        bool  mDegradedFlag; /**<    (--) trick_chkpnt_io(**) Sensor is degraded by malfunctions     */
        bool  mInitFlag;     /**< *o (--) trick_chkpnt_io(**) Sensor object has been initialized     */

        /// @brief Default constructor is protected because this class is never directly
        ///        instantiated.
        SensorBase();

        /// @brief Validates the initial conditions.
        void               validate() const;

        /// @brief Performs transformations on the truth input as needed.
        ///
        /// @details This class implementation is pure virtual and must be implemented in a derived
        ///          class.
        virtual void       processInput() = 0;

        /// @brief Determines the powered state of the sensor.
        virtual bool       determinePower() const;

        /// @brief Determines the degradation state of the sensor.
        virtual bool       determineDegradation() const;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        SensorBase(const SensorBase&);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        SensorBase& operator =(const SensorBase&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns bool (--) True if the sensor has been properly initialized, false if not.
///
/// @details  Returns the initialized status of the sensor.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool SensorBase::isInitialized() const
{
    return mInitFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] powerFlag (--) Desired power state, true = powered, false = unpowered.
///
/// @details  Sets the sensor's powered state to the desired value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void SensorBase::setPowerFlag(const bool powerFlag)
{
    mPowerFlag = powerFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns char* (--) Pointer to the sensor's name.
///
/// @details  Returns a pointer to the sensor's name.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char* SensorBase::getName() const
{
    return mName.c_str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The sensor is on if either the input power is available or the malfunction to ignore
///           power supply is active, and then if the fail-off malfunction is not active.  The fail-
///           off malf overrides the ignore power malf.  This method can be overridden to add
///           voltage & temperature effects on powered state as required.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool SensorBase::determinePower() const
{
    return ((mPowerFlag or mMalfIgnorePower) and not mMalfFailOffFlag);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The sensor is considered degraded if any malfunction that is usually used to degrade
///           the sensor output is active (regardless of whether the malfunction is actually
///           degrading the output at this time), but not if the perfect sensor malfunction is
///           active.  This method should be overridden to include further malfunctions in the
///           derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool SensorBase::determineDegradation() const
{
    return ((mMalfFailToFlag or mMalfFailOffFlag or mMalfFailStuckFlag) and not mMalfPerfectSensor);
}

#endif
