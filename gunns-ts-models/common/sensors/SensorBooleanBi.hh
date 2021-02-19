#ifndef SensorBooleanBi_EXISTS
#define SensorBooleanBi_EXISTS

/**
@defgroup  TSM_SENSORS_BOOLEAN_INPUT_BOOLEAN_SENSOR    Generic Boolean Input Boolean Sensor
@ingroup   TSM_SENSORS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Generic model for a boolean sensor with boolean input.)

REQUIREMENTS:
- (R.TS210-0009: TS21 vehicle subsystem sensor models shall have fail, scale, noise, drift and bias
                 malfunctions.
   R.TS210-0021: TS21 vehicle subsystem models shall provide sensed and actual data.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((SensorBooleanBi.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2012-04) (Initial))

@{
*/

#include "SensorBase.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Generic Boolean Input Boolean Sensor Configuration Data
///
/// @details  This class provides a data structure for the Boolean Input Boolean Sensor
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SensorBooleanBiConfigData : public SensorBaseConfigData
{
    public:
        bool mOffValue;                      /**< (--) trick_chkpnt_io(**) Sensed output value of the sensor when off */

        /// @brief Default constructs this Boolean Input Boolean Sensor configuration data.
        explicit SensorBooleanBiConfigData(const bool offValue = false);

        /// @brief Default destructs this Boolean Input Boolean Sensor configuration data.
        virtual ~SensorBooleanBiConfigData();

        /// @brief Copy constructs this Boolean Input Boolean Sensor configuration data.
        SensorBooleanBiConfigData(const SensorBooleanBiConfigData& that);

        /// @brief Assignment operator for this Boolean Input Boolean Sensor configuration data.
        SensorBooleanBiConfigData& operator =(const SensorBooleanBiConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Generic Boolean Input Boolean Sensor Input Data
///
/// @details  This class provides a data structure for the Boolean Input Boolean Sensor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SensorBooleanBiInputData : public SensorBaseInputData
{
    public:
        bool mTruthInput;               /**< (--) Initial value of the input truth value to sense */
        bool mMalfFailToValue;          /**< (--) Fail-to-value malfunction value                 */

        /// @brief    Default constructs this Boolean Input Boolean Sensor input data.
        SensorBooleanBiInputData(const bool powerFlag         = false,
                                 const bool truthInput        = false);

        /// @brief    Default destructs this Boolean Input Boolean Sensor input data.
        virtual ~SensorBooleanBiInputData();

        /// @brief    Copy constructs this Boolean Input Boolean Sensor input data.
        SensorBooleanBiInputData(const SensorBooleanBiInputData& that);

        /// @brief Assignment operator for this Boolean Input Boolean Sensor input data.
        SensorBooleanBiInputData& operator =(const SensorBooleanBiInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Boolean Input Boolean Sensor
///
/// @details  This is the generic model for a boolean sensor with boolean input.  It outputs a
///           boolean based on an input boolean.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SensorBooleanBi : public SensorBase
{
    TS_MAKE_SIM_COMPATIBLE(SensorBooleanBi);

    public:
        /// @name     Malfunctions.
        /// @{
        /// @details  Malfunctions are declared public to allow the sim's malfunction hooks sim
        ///           object to only require a pointer to this sensor object, and not individual
        ///           pointers to each malfunction term.
        bool mMalfFailToValue;               /**< (--) Fail-to-value malfunction value            */
        /// @}

        /// @brief Default constructor.
        SensorBooleanBi();

        /// @brief Default destructor.
        virtual     ~SensorBooleanBi();

        /// @brief Initializes the Boolean Input Boolean Sensor object.
        void         initialize(const SensorBooleanBiConfigData& configData,
                                const SensorBooleanBiInputData&  inputData,
                                const std::string&               name);

        /// @brief Updates the sensed output of the sensor.
        virtual void update(const double timeStep);

        /// @brief Updates and returns the sensed output of the sensor given inputs.
        bool         sense(const double timeStep,
                           const bool   powerFlag,
                           const bool   truthInput);

        /// @brief Setter method to set the input truth value to sense.
        void         setTruthInput(const bool truthInput);

        /// @brief Getter method to get the input truth value to sense.
        bool         getTruthInput() const;

        /// @brief Getter method to get the sensed output value.
        bool         getSensedOutput() const;

    protected:
        bool mOffValue;                      /**< (--) trick_chkpnt_io(**) Sensed output value of the sensor when off */
        bool mTruthInput;                    /**< (--) Input truth value to sense                 */
        bool mSensedOutput;                  /**< (--) Sensed output value of the sensor          */

        /// @brief Validates the initial conditions.
        void         validate() const;

        /// @brief Performs transformations on the truth input as needed.
        virtual void processInput();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        SensorBooleanBi(const SensorBooleanBi&);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        SensorBooleanBi& operator =(const SensorBooleanBi&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] truthInput (--) The truth input boolean value to the sensor.
///
/// @details  The sensor's truth input value will usually be set by the simbus, but this method
///           allows other objects to set it directly.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void SensorBooleanBi::setTruthInput(const bool truthInput)
{
    mTruthInput = truthInput;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) The truth input boolean value to the sensor.
///
/// @details  Allows other objects to directly see the truth input value to the sensor without
///           needing to access the simbus.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool SensorBooleanBi::getTruthInput() const
{
    return mTruthInput;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) The sensed output value.
///
/// @details  Allows other objects to directly see the sensed output value of the sensor without
///           needing to access the simbus.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool SensorBooleanBi::getSensedOutput() const
{
    return mSensedOutput;
}

#endif
