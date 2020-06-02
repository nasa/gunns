#ifndef SensorBooleanAi_EXISTS
#define SensorBooleanAi_EXISTS

/**
@defgroup  TSM_SENSORS_ANALOG_INPUT_BOOLEAN_SENSOR    Generic Analog Input Boolean Sensor
@ingroup   TSM_SENSORS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Generic model for a boolean sensor with analog input.)

REQUIREMENTS:
- (R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise, drift and bias
                 malfunctions.
   R.TS210-0021: TS21 vehicle subsystem models shall provide sensed and actual data.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((SensorBooleanAi.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2012-04) (Initial))

@{
*/

#include "SensorBooleanBi.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Generic Analog Input Boolean Sensor Configuration Data
///
/// @details  This class provides a data structure for the Analog Input Boolean Sensor
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SensorBooleanAiConfigData : public SensorBooleanBiConfigData
{
    public:
        float mTarget;       /**< (--) trick_chkpnt_io(**) Target value of truth input to trigger sensor              */
        float mTolerance;    /**< (--) trick_chkpnt_io(**) Tolerance range of truth input to target to trigger sensor */

        /// @brief Default constructs this Analog Input Boolean Sensor configuration data.
        SensorBooleanAiConfigData(const bool  offValue  = false,
                                  const float target    = 0.0,
                                  const float tolerance = 0.0);

        /// @brief Default destructs this Analog Input Boolean Sensor configuration data.
        virtual ~SensorBooleanAiConfigData();

        /// @brief Copy constructs this Analog Input Boolean Sensor configuration data.
        SensorBooleanAiConfigData(const SensorBooleanAiConfigData& that);

        /// @brief Assignment operator for this Analog Input Boolean Sensor configuration data.
        SensorBooleanAiConfigData& operator =(const SensorBooleanAiConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Generic Analog Input Boolean Sensor Input Data
///
/// @details  This class provides a data structure for the Analog Input Boolean Sensor input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SensorBooleanAiInputData : public SensorBooleanBiInputData
{
    public:
        float mTruthInputAnalog;        /**< (--) Initial value of the input truth value to sense */

        /// @brief    Default constructs this Analog Input Boolean Sensor input data.
        SensorBooleanAiInputData(const bool  powerFlag         = false,
                                 const bool  truthInput        = false,
                                 const float truthInputAnalog  = 0.0);

        /// @brief    Default destructs this Analog Input Boolean Sensor input data.
        virtual ~SensorBooleanAiInputData();

        /// @brief    Copy constructs this Analog Input Boolean Sensor input data.
        SensorBooleanAiInputData(const SensorBooleanAiInputData& that);

        /// @brief Assignment operator for this Analog Input Boolean Sensor input data.
        SensorBooleanAiInputData& operator =(const SensorBooleanAiInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Analog Input Boolean Sensor
///
/// @details  This is the generic model for a boolean sensor with analog input.  It outputs a
///           boolean based on a floating-point input.  The floating-point input value is
///           compared to a target value for triggering the boolean output of the sensor.  As an
///           example, this can be used to model a limit switch that outputs true when the physical
///           switch contactor is within 90% of a target position, given by the floating-point
///           input.  The sensor outputs true when the input is within a tolerance of the target.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SensorBooleanAi : public SensorBooleanBi
{
    TS_MAKE_SIM_COMPATIBLE(SensorBooleanAi);

    public:
        /// @brief Default constructor.
        SensorBooleanAi();

        /// @brief Default destructor.
        virtual     ~SensorBooleanAi();

        /// @brief Initializes the Analog Input Boolean Sensor object.
        void         initialize(const SensorBooleanAiConfigData& configData,
                                const SensorBooleanAiInputData&  inputData,
                                const std::string&               name);

        /// @brief Updates and returns the sensed output of the sensor given inputs.
        bool         sense(const double timeStep,
                           const bool   powerFlag,
                           const double truthInput);

        /// @brief Setter method to set the input truth analog value to sense.
        void         setTruthInput(const double truthInput);

        /// @brief Getter method to get the input truth analog value to sense.
        double       getTruthInput() const;

    protected:
        float  mTarget;                 /**< (--) trick_chkpnt_io(**) Target value of truth input to trigger sensor   */
        float  mTolerance;              /**< (--) trick_chkpnt_io(**) Tolerance range of truth input to target        */
        double mTruthInputAnalog;       /**< (--) Initial value of the input truth value to sense */

        /// @brief Validates the initial conditions.
        void         validate() const;

        /// @brief Performs transformations on the truth input as needed.
        virtual void processInput();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        SensorBooleanAi(const SensorBooleanAi&);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        SensorBooleanAi& operator =(const SensorBooleanAi&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] truthInput (--) The truth input analog value to the sensor.
///
/// @details  The sensor's truth input value will usually be set by the simbus, but this method
///           allows other objects to set it directly.  This overloaded method is used for the
///           analog input of the class, rather than the boolean input in the SensorBooleanBi
///           base class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void SensorBooleanAi::setTruthInput(const double truthInput)
{
    mTruthInputAnalog = truthInput;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  float (--) The truth input analog value to the sensor.
///
/// @details  Allows other objects to directly see the truth input value to the sensor without
///           needing to access the simbus.  This overloaded method is used for the analog input of
///           the class, rather than the boolean input in the SensorBooleanBi base class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double SensorBooleanAi::getTruthInput() const
{
    return mTruthInputAnalog;
}

#endif
