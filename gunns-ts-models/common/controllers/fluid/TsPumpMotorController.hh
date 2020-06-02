#ifndef TsPumpMotorController_EXISTS
#define TsPumpMotorController_EXISTS

/**
@file
@brief     Pump Motor Controller Model declarations.

@defgroup  TSM_CONTROLLER_FLUID_PUMP_MOTOR    Pump Motor Controller Model
@ingroup   TSM_CONTROLLER_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:  (This is a generic pump/fan motor controller model.  It is designed to work with, and
           includes a DcDynPumpMotor model.  It uses a simple control filter with forcing & damping
           functions to control error between sensed motor speed and a commanded speed.  Optional
           features that can be configured are: trips for over-temperature and under/over-voltage,
           a start-up motor current limiter, and a sinusoidal speed noise function.)

@details
 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - ((TsPumpMotorController.o))

 PROGRAMMERS:
 - ((Jason Harvey) (L-3 Communications) (2014-03) (Install))
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "common/effectors/mechanical/motor/DcDynPumpMotor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Pump Motor Controller Model Configuration Data
///
/// @details  This class provides a data structure for the Pump Motor Controller Model config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsPumpMotorControllerConfigData
{
    public:
        DcDynPumpMotorConfigData mMotor;     /**< (--)             trick_chkpnt_io(**) Motor configuration data              */
        /// @details Recommend 0.1 - 0.5.  Higher values produce faster motor response times, but
        ///          with higher risk of instability.
        double mForcingGain;                 /**< (--)             trick_chkpnt_io(**) (0-1) Control filter forcing gain     */
        /// @details Recommend 0.05 - 0.2, and never higher than mForcingGain.  Higher values reduce
        ///          motor speed overshoot, but with higher risk of instability.
        double mDampingGain;                 /**< (--)             trick_chkpnt_io(**) (0-1) Control filter damping gain     */
        /// @details Recommend 0.01.  This is the percent speed error within which the damping cuts
        ///          out.
        double mDampingCutoff;               /**< (--)             trick_chkpnt_io(**) (0-1) Damping cutoff error            */
        double mTemperatureTripLimit;        /**< (K)              trick_chkpnt_io(**) Temperature trip limit                */
        double mTemperatureTripReset;        /**< (K)              trick_chkpnt_io(**) Temperature trip reset                */
        double mMinVoltage;                  /**< (V)              trick_chkpnt_io(**) Minimum voltage for operation         */
        double mMaxVoltage;                  /**< (V)              trick_chkpnt_io(**) Maximum voltage for operation         */
        double mControllerPowerLoad;         /**< (W)              trick_chkpnt_io(**) Controller circuitry power load       */
        double mNoiseAmplitude;              /**< (revolution/min) trick_chkpnt_io(**) Amplitude of speed bias function      */
        double mNoiseFrequency;              /**< (Hz)             trick_chkpnt_io(**) Frequency of speed bias function      */
        double mStartupCurrentLimit;         /**< (amp)            trick_chkpnt_io(**) Limited motor current during startup  */
        /// @brief    Default constructs this Controller configuration data.
        TsPumpMotorControllerConfigData(const DcDynPumpMotorConfigData& motor = DcDynPumpMotorConfigData(),
                                        const double forcingGain              = 0.0,
                                        const double dampingGain              = 0.0,
                                        const double dampingCutoff            = 0.01,
                                        const double temperatureTripLimit     = 0.0,
                                        const double temperatureTripReset     = 0.0,
                                        const double minVoltage               = 0.0,
                                        const double maxVoltage               = 0.0,
                                        const double controllerPowerLoad      = 0.0,
                                        const double noiseAmplitude           = 0.0,
                                        const double noiseFrequency           = 0.0,
                                        const double startupCurrentLimit      = 1.0E15);
        /// @brief    Copy constructs this Controller configuration data.
        TsPumpMotorControllerConfigData(const TsPumpMotorControllerConfigData& that);
        /// @brief    Assignment operator for this Controller configuration data.
        TsPumpMotorControllerConfigData& operator =(const TsPumpMotorControllerConfigData& that);
        /// @brief    Default destructs this Controller configuration data.
        virtual ~TsPumpMotorControllerConfigData();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Pump Motor Controller Model Input Data
///
/// @details  This class provides a data structure for the Pump Motor Controller Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsPumpMotorControllerInputData
{
    public:
        DcDynPumpMotorInputData mMotor;  /**< (--)             Motor input data                          */
        double mVoltage;                 /**< (V)              Initial supply voltage                    */
        double mSensedSpeed;             /**< (revolution/min) Initial sensed speed                      */
        double mSensedTemperature;       /**< (K)              Initial sensed temperature                */
        bool   mStartupState;            /**< (--)             Initially limiting startup current        */
        bool   mCommandEnable;           /**< (--)             Initial enable command                    */
        double mCommandSpeed;            /**< (revolution/min) Initial commanded speed                   */
        double mNoisePhase;              /**< (rad)            Initial phase of speed bias function      */
        bool   mMalfFailPower;           /**< (--)             Initial Fail power malf flag              */
        bool   mMalfPowerOverrideFlag;   /**< (--)             Initial Power override malf flag          */
        double mMalfPowerOverrideValue;  /**< (V)              Initial Power override malf voltage       */
        bool   mMalfCommandSpeedFlag;    /**< (--)             Initial Speed command override malf flag  */
        double mMalfCommandSpeedValue;   /**< (revolution/min) Initial Speed command override malf value */
        /// @brief    Default constructs this Controller input data.
        TsPumpMotorControllerInputData(const DcDynPumpMotorInputData& motor = DcDynPumpMotorInputData(),
                                       const double voltage                 = 0.0,
                                       const double sensedSpeed             = 0.0,
                                       const double sensedTemperature       = 0.0,
                                       const double startupState            = false,
                                       const bool   commandEnable           = false,
                                       const double commandSpeed            = 0.0,
                                       const double noisePhase              = 0.0);
        /// @brief    Copy constructs this Controller input data.
        TsPumpMotorControllerInputData(const TsPumpMotorControllerInputData& that);
        /// @brief    Assignment operator for this Controller input data.
        TsPumpMotorControllerInputData& operator =(const TsPumpMotorControllerInputData& that);
        /// @brief    Default destructs this Controller input data.
        virtual ~TsPumpMotorControllerInputData();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Pump Motor Controller Model
///
/// @details  Refer to class Purpose.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsPumpMotorController
{
    TS_MAKE_SIM_COMPATIBLE(TsPumpMotorController);
    public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        DcDynPumpMotor mMotor;          /**< (--)             The motor                                  */
        /// @}

        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfFailPower;          /**< (--)             Fail power malf flag                       */
        bool   mMalfPowerOverrideFlag;  /**< (--)             Power override malf flag                   */
        double mMalfPowerOverrideValue; /**< (V)              Power override malf voltage                */
        bool   mMalfCommandSpeedFlag;   /**< (--)             Speed command override malf flag           */
        double mMalfCommandSpeedValue;  /**< (revolution/min) Speed command override malf value          */
        /// @}

        /// @brief    Default constructs this Controller.
        TsPumpMotorController();
        /// @brief    Default destructs this Controller.
        virtual ~TsPumpMotorController();
        /// @brief    Initializes this Controller with configuration and input data.
        void   initialize(const TsPumpMotorControllerConfigData& configData,
                          const TsPumpMotorControllerInputData&  inputData,
                          const std::string&              name);
        /// @brief    Updates the state of the Controller.
        virtual void step(const double dt);
        /// @brief    Updates the state of the Controller and provides all needed inputs.
        virtual void stepWithArgs(const double dt,
                                  const double voltage,
                                  const double speed,
                                  const double temperature,
                                  const double speedCommand,
                                  const bool   enableCommand);
        /// @brief    Sets the power supply voltage to the Controller.
        void   setVoltage(const double voltage);
        /// @brief    Sets the sensed motor speed to the Controller.
        void   setSensedSpeed(const double speed);
        /// @brief    Sets the sensed motor temperature to the Controller.
        void   setSensedTemperature(const double temperature);
        /// @brief    Sets the speed command to the Controller.
        void   setCommandSpeed(const double speedCommand);
        /// @brief    Sets the enable command to the Controller.
        void   setCommandEnable(const bool enableCommand);
        /// @brief    Sets the external load torques to the Motor.
        void   setMotorLoadTorques(const double torque1 = 0.0,
                                   const double torque2 = 0.0,
                                   const double torque3 = 0.0,
                                   const double torque4 = 0.0);
        /// @brief    Returns the initialization status of the Controller.
        bool   isInitialized() const;
        /// @brief    Returns the voltage tripped flag.
        bool   isTemperatureTripped() const;
        /// @brief    Returns the controller bus powered flag.
        bool   isControllerBusPowered() const;
        /// @brief    Returns the motor bus powered flag.
        bool   isMotorBusPowered() const;
        /// @brief    Returns the pulse width on-time fraction.
        double getPulseWidth() const;
        /// @brief    Returns the total electrical resistance of the motor + controller.
        double getTotalResistance() const;
        /// @brief    Returns the total waste heat of the motor + controller.
        double getTotalWasteHeat() const;
        /// @brief    Returns the total electrical current drawn by the motor + controller.
        double getTotalCurrent() const;
        /// @brief    Returns the electrical current drawn by the motor.
        double getMotorCurrent() const;
        /// @brief    Returns the electrical power load of the motor.
        double getMotorPower() const;
        /// @brief    Returns the electrical power load of the motor + controller.
        double getTotalPower() const;
        /// @brief    Returns the true motor shaft speed.
        double getMotorSpeed() const;

    protected:
        std::string            mName;                    /**< ** (--)             trick_chkpnt_io(**) Name of the controller for output messages */
        bool                   mInitFlag;                /**<    (--)             trick_chkpnt_io(**) Initialization successful flag             */
        double                 mForcingGain;             /**<    (--)             trick_chkpnt_io(**) (0-1) Control filter forcing gain          */
        double                 mDampingGain;             /**<    (--)             trick_chkpnt_io(**) (0-1) Control filter damping gain          */
        double                 mDampingCutoff;           /**<    (--)             trick_chkpnt_io(**) (0-1) Damping cutoff error                 */
        double                 mTemperatureTripLimit;    /**<    (K)              trick_chkpnt_io(**) Temperature trip limit                     */
        double                 mTemperatureTripReset;    /**<    (K)              trick_chkpnt_io(**) Temperature trip reset                     */
        double                 mMinVoltage;              /**<    (V)              trick_chkpnt_io(**) Minimum voltage for operation              */
        double                 mMaxVoltage;              /**<    (V)              trick_chkpnt_io(**) Maximum voltage for operation              */
        double                 mControllerPowerLoad;     /**<    (W)              trick_chkpnt_io(**) Controller circuitry power load            */
        double                 mNoiseAmplitude;          /**<    (revolution/min) trick_chkpnt_io(**) Amplitude of speed bias function           */
        double                 mNoiseFrequency;          /**<    (Hz)             trick_chkpnt_io(**) Frequency of speed bias function           */
        double                 mStartupCurrentLimit;     /**<    (amp)            trick_chkpnt_io(**) Limited motor current during startup       */
        double                 mVoltage;                 /**<    (V)                                  Input supply voltage                       */
        double                 mSensedSpeed;             /**<    (revolution/min)                     Input sensed speed                         */
        double                 mSensedTemperature;       /**<    (K)                                  Input sensed temperature                   */
        bool                   mStartupState;            /**<    (--)                                 Controller is limiting startup current     */
        bool                   mCommandEnable;           /**<    (--)                                 Input enable command                       */
        double                 mCommandSpeed;            /**<    (revolution/min)                     Input commanded speed                      */
        double                 mNoisePhase;              /**<    (rad)                                Phase of speed bias function               */
        double                 mPulseWidth;              /**<    (--)                                 (0-1) Pulse width fraction to motor        */
        double                 mSpeedError;              /**<    (revolution/min)                     Error between sensed & commanded speed     */
        bool                   mTrippedTemperature;      /**<    (--)                                 Controller tripped due to bad temperature  */
        bool                   mControllerPowerBus;      /**<    (--)                                 Controller bus is powered flag             */
        bool                   mMotorPowerBus;           /**<    (--)                                 Motor bus is powered flag                  */
        double                 mTotalResistiveLoad;      /**<    (ohm)                                Total resistance of controller + motor     */
        double                 mTotalWasteHeat;          /**<    (W)                                  Total waste heat of controller + motor     */
        double                 mTotalCurrent;            /**<    (amp)                                Total elect. current of controller + motor */
        static const double    MAX_RESISTANCE;           /**<    (ohm)            trick_chkpnt_io(**) Maximum resistance. */
        /// @brief    Validates the initialization of this Controller.
        void validate(const TsPumpMotorControllerConfigData& configData,
                      const TsPumpMotorControllerInputData&  inputData) const;
        /// @brief    Determines controller and motor powered states.
        virtual void determinePower();
        /// @brief    Checks for controller tips due to parameters out of bounds.
        virtual void checkTrips();
        /// @brief    Updates the control filter to produce a new pulse width.
        virtual void updateControlFilter(const double dt);
        /// @brief    Steps the motor with the latest power and pulse width.
        virtual void updateMotor(const double dt);
        /// @brief    Calculates total resistance and waste heat for the motor + controller.
        virtual void updatePowerLoad();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsPumpMotorController(const TsPumpMotorController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsPumpMotorController& operator =(const TsPumpMotorController&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] voltage (V) Input supply voltage.
///
/// @details  Sets the supply voltage to the Controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsPumpMotorController::setVoltage(const double voltage)
{
    mVoltage = voltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] speed (revolution/min) Input speed.
///
/// @details  Sets the sensed motor speed to the Controller.  The actual motor speed can be used if
///           modeling a sensor is not required.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsPumpMotorController::setSensedSpeed(const double speed)
{
    mSensedSpeed = speed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] temperature (K) Input temperature.
///
/// @details  Sets the sensed temperature to the Controller.  The actual temperature can be used if
///           modeling a sensor is not required.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsPumpMotorController::setSensedTemperature(const double temperature)
{
    mSensedTemperature = temperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] speedCommand (revolution/min) Input commanded speed.
///
/// @details  Sets the commanded speed to the Controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsPumpMotorController::setCommandSpeed(const double speedCommand)
{
    mCommandSpeed = speedCommand;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] enableCommand (--) Input command enable flag.
///
/// @details  Sets the speed command enable flag to the Controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsPumpMotorController::setCommandEnable(const bool enableCommand)
{
    mCommandEnable = enableCommand;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] torque1 (N*m) 1st external torque load.
/// @param[in] torque2 (N*m) 2nd external torque load.
/// @param[in] torque3 (N*m) 3rd external torque load.
/// @param[in] torque4 (N*m) 4th external torque load.
///
/// @details  Sets the external load torques to the Motor.  The input arguments default to zero, for
///           convenience when not all of them are needed.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsPumpMotorController::setMotorLoadTorques(const double torque1,
                                                const double torque2,
                                                const double torque3,
                                                const double torque4)
{
    mMotor.setLoadTorques(torque1, torque2, torque3, torque4);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) True if the controller is successfully initialized.
///
/// @details  Returns the initialization status of the Controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsPumpMotorController::isInitialized() const
{
    return mInitFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) Temperature tripped flag.
///
/// @details  Returns the temperature tripped flag.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsPumpMotorController::isTemperatureTripped() const
{
    return mTrippedTemperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) Controller bus is powered flag.
///
/// @details  Returns the controller bus is powered flag.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsPumpMotorController::isControllerBusPowered() const
{
    return mControllerPowerBus;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) Motor bus is powered flag.
///
/// @details  Returns the motor bus is powered flag.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsPumpMotorController::isMotorBusPowered() const
{
    return mMotorPowerBus;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (--) (0-1) Pulse width on-time fraction.
///
/// @details  Returns the pulse width on-time fraction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsPumpMotorController::getPulseWidth() const
{
    return mPulseWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (ohm) The total electrical resistance of the motor + controller.
///
/// @details  Returns the total electrical resistance of the motor + controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsPumpMotorController::getTotalResistance() const
{
    return mTotalResistiveLoad;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) The total waste heat of the motor + controller.
///
/// @details  Returns the total waste heat of the motor + controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsPumpMotorController::getTotalWasteHeat() const
{
    return mTotalWasteHeat;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (amp) The total electrical current drawn by the motor + controller.
///
/// @details  Returns the total electrical current drawn by the motor + controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsPumpMotorController::getTotalCurrent() const
{
    return mTotalCurrent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (amp) The electrical current drawn by the motor.
///
/// @details  Returns the electrical current drawn by the motor.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsPumpMotorController::getMotorCurrent() const
{
    return mMotor.getCurrent();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  (W) The electrical power load of the motor.
///
/// @details  Returns the electrical power load of the motor.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsPumpMotorController::getMotorPower() const
{
    return mMotor.getPower();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  (revolution/min) The true motor shaft speed.
///
/// @details  Returns the true motor shaft speed.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsPumpMotorController::getMotorSpeed() const
{
    return mMotor.getSpeed();
}

#endif
