#ifndef TsDcPwmDynMotor_EXISTS
#define TsDcPwmDynMotor_EXISTS

/**
@defgroup  TSM_MECH_MOTOR_DC_PWM_DYN  PWM-Controlled Dynamic DC Motor Model
@ingroup   TSM_MECH_MOTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (This is a generic model of a DC motor that is speed controlled via Pulse-Width Modulation.  It
   balances torque created by the motor with torques from external loads and internal friction to
   update a dynamic motor speed.  PWM and voltage inputs from a motor controller model are used to
   dynamically control the motor speed, and the model also limits the speed to the maximum the motor
   can reach based on the available torque.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((TsDcPwmDynMotor.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Install) (2012-07))

@{
*/

#include <string>
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    PWM-Controlled Dynamic DC Motor Model Configuration Data
///
/// @details  This class provides a data structure for the PWM-Controlled Dynamic DC Motor Model
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsDcPwmDynMotorConfigData
{
    public:
        double mPulseWasteFraction;  /**< (--)                 trick_chkpnt_io(**) (0-1) Fraction of power wasted during PWM */
        double mStallTorqueCoeff1;   /**< (N*m/V)              trick_chkpnt_io(**) (0-1) Stall torque curve 1st-order coefficient  */
        double mStallTorqueCoeff2;   /**< (N*m/V)              trick_chkpnt_io(**) (0-1) Stall torque curve 2nd-order coefficient  */
        double mFrictionTorque;      /**< (N*m)                trick_chkpnt_io(**) (0-1) Friction torque (constant)                */
        double mBemfConstant;        /**< (V*min/revolution)   trick_chkpnt_io(**) (0-1) Back-EMF constant                         */
        double mArmatureResistance;  /**< (ohm)                trick_chkpnt_io(**) (0-1) Electrical resistance of motor armature   */
        double mInertia;             /**< (kg*m2)              trick_chkpnt_io(**) (0-1) Inertia of the motor+load mass            */
        double mSpeedLoadRatio;      /**< (revolution/min/N/m) trick_chkpnt_io(**) (0-1) Slope of motor speed/torque line          */
        /// @brief    Default constructs this Motor configuration data.
        TsDcPwmDynMotorConfigData(const double pulseWasteFraction = 0.0,
                                  const double stallTorqueCoeff1  = 0.0,
                                  const double stallTorqueCoeff2  = 0.0,
                                  const double frictionTorque     = 0.0,
                                  const double bemfConstant       = 0.0,
                                  const double armatureResistance = 0.0,
                                  const double inertia            = 0.0,
                                  const double speedLoadRatio     = 0.0);
        /// @brief    Copy constructs this Motor configuration data.
        TsDcPwmDynMotorConfigData(const TsDcPwmDynMotorConfigData& that);
        /// @brief    Assignment operator for this Motor configuration data.
        TsDcPwmDynMotorConfigData& operator =(const TsDcPwmDynMotorConfigData& that);
        /// @brief    Default destructs this Motor configuration data.
        virtual ~TsDcPwmDynMotorConfigData();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    PWM-Controlled Dynamic DC Motor Model Input Data
///
/// @details  This class provides a data structure for the PWM-Controlled Dynamic DC Motor Model
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsDcPwmDynMotorInputData
{
    public:
        double mVoltage;                         /**< (V)              Initial supply voltage            */
        double mPulseWidth;                      /**< (--)             (0-1) Initial pulse width         */
        double mMotorSpeed;                      /**< (revolution/min) Initial motor speed               */
        double mLoadTorque1;                     /**< (N*m)            1st initial external load torque  */
        double mLoadTorque2;                     /**< (N*m)            2nd initial external load torque  */
        double mLoadTorque3;                     /**< (N*m)            3rd initial external load torque  */
        double mLoadTorque4;                     /**< (N*m)            4th initial external load torque  */
        bool   mMalfDegradeFlag;                 /**< (--)             Initial degrade malf flag         */
        double mMalfDegradeValue;                /**< (--)             (0-1) Initial degrade malf value  */
        bool   mMalfJamFlag;                     /**< (--)             Initial jam malf flag             */
        double mMalfJamValue;                    /**< (--)             (0-1) Initial jam malf value      */
        bool   mMalfSpeedOverrideFlag;           /**< (--)             Initial speed override malf flag  */
        double mMalfSpeedOverrideValue;          /**< (revolution/min) Initial speed override malf value */
        bool   mMalfHeatRateBiasFlag;            /**< (--)             Initial heat rate bias malf flag  */
        double mMalfHeatRateBiasValue;           /**< (W)              Initial heat rate bias malf value */
        /// @brief    Default constructs this Motor input data.
        TsDcPwmDynMotorInputData(const double voltage     = 0.0,
                                 const double pulseWidth  = 0.0,
                                 const double motorSpeed  = 0.0,
                                 const double loadTorque1 = 0.0,
                                 const double loadTorque2 = 0.0,
                                 const double loadTorque3 = 0.0,
                                 const double loadTorque4 = 0.0);
        /// @brief    Copy constructs this Motor input data.
        TsDcPwmDynMotorInputData(const TsDcPwmDynMotorInputData& that);
        /// @brief    Assignment operator for this Motor input data.
        TsDcPwmDynMotorInputData& operator =(const TsDcPwmDynMotorInputData& that);
        /// @brief    Default destructs this Motor input data.
        virtual ~TsDcPwmDynMotorInputData();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    PWM-Controlled Dynamic DC Motor Model
///
/// @details  Refer to class Purpose.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsDcPwmDynMotor
{
    TS_MAKE_SIM_COMPATIBLE(TsDcPwmDynMotor);
    public:
        /// @name    Malfunction targets.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfDegradeFlag;        /**< (--)             Degrade malf flag                        */
        double mMalfDegradeValue;       /**< (--)             (0-1) Degrade malf value                 */
        bool   mMalfJamFlag;            /**< (--)             Jam malf flag                            */
        double mMalfJamValue;           /**< (--)             (0-1) Jam malf value                     */
        bool   mMalfSpeedOverrideFlag;  /**< (--)             Speed override malf flag                 */
        double mMalfSpeedOverrideValue; /**< (revolution/min) Speed override malf value                */
        bool   mMalfHeatRateBiasFlag;   /**< (--)             Heat rate bias malf flag                 */
        double mMalfHeatRateBiasValue;  /**< (W)              Heat rate bias malf value                */
        /// @}

        /// @brief    Default constructs this Motor.
        TsDcPwmDynMotor();
        /// @brief    Default destructs this Motor.
        virtual ~TsDcPwmDynMotor();
        /// @brief    Initializes this Motor with configuration and input data.
        void   initialize(const TsDcPwmDynMotorConfigData& configData,
                          const TsDcPwmDynMotorInputData&  inputData,
                          const std::string&               name);
        /// @brief    Updates the state of the Motor.
        virtual void step(const double dt);
        /// @brief    Sets the supply voltage to the Motor.
        void   setVoltage(const double voltage);
        /// @brief    Sets the pulse width command to the Motor.
        void   setPulseWidth(const double pulseWidth);
        /// @brief    Sets the external load torques to the Motor.
        void   setLoadTorques(const double torque1 = 0.0,
                              const double torque2 = 0.0,
                              const double torque3 = 0.0,
                              const double torque4 = 0.0);
        /// @brief Returns the initialized status of the Motor.
        bool   isInitialized() const;
        /// @brief    Returns the Motor electrical current draw.
        double getCurrent() const;
        /// @brief    Returns the Motor generated electrical current.
        double getGeneratedCurrent() const;
        /// @brief    Returns the Motor rotational speed.
        double getSpeed() const;
        /// @brief    Returns the Motor total electrical resistance.
        double getResistance() const;
        /// @brief    Returns the Motor electrical power demand.
        double getPower() const;
        /// @brief    Returns the Motor waste heat.
        double getWasteHeat() const;
        /// @brief    Returns the Motor total efficiency.
        double getEfficiency() const;

    protected:
        std::string mName;          /**< ** (--)                 trick_chkpnt_io(**) Name of the motor for output messages     */
        double mPulseWasteFraction; /**<    (--)                 trick_chkpnt_io(**) (0-1) Fraction of power wasted during PWM */
        double mStallTorqueCoeff1;  /**<    (N*m/V)              trick_chkpnt_io(**) Stall torque curve 1st-order coefficient  */
        double mStallTorqueCoeff2;  /**<    (N*m/V)              trick_chkpnt_io(**) Stall torque curve 2nd-order coefficient  */
        double mFrictionTorque;     /**<    (N*m)                trick_chkpnt_io(**) Friction torque (constant)                */
        double mBemfConstant;       /**<    (V*min/revolution)   trick_chkpnt_io(**) Back-EMF constant                         */
        double mArmatureResistance; /**<    (ohm)                trick_chkpnt_io(**) Electrical resistance of motor armature   */
        double mInertia;            /**<    (kg*m2)              trick_chkpnt_io(**) Inertia of the motor+load mass            */
        double mSpeedLoadRatio;     /**<    (revolution/min/N/m) trick_chkpnt_io(**) Slope of motor speed/torque line          */
        double mVoltage;            /**<    (V)                                      Input supply voltage                      */
        double mPulseWidth;         /**<    (--)                                     (0-1) Input pulse width                   */
        double mMotorSpeed;         /**<    (revolution/min)                         Motor speed                               */
        double mLoadTorques[4];     /**<    (N*m)                                    External load torques                     */
        double mPower;              /**<    (W)                                      Motor electrical power demand             */
        double mCurrent;            /**<    (amp)                                    Motor electrical current draw             */
        double mGeneratedCurrent;   /**<    (amp)                                    Motor electrical current generated        */
        double mResistance;         /**<    (ohm)                                    Motor total electrical resistance         */
        double mWasteHeat;          /**<    (W)                                      Waste heat generated by the motor         */
        double mEfficiency;         /**<    (--)                                     (0-1) Motor power efficiency              */
        double mStallTorque;        /**<    (N*m)                                    Maximum torque at current voltage         */
        double mDriveTorque;        /**<    (N*m)                                    Torque produced by the motor              */
        double mTotalExternalLoad;  /**<    (N*m)                                    Total external torque load on shaft       */
        bool   mInitFlag;           /**<    (--)                 trick_chkpnt_io(**) Motor has been initialized                */
        /// @brief    Validates the initialization of this Motor.
        void validate(const TsDcPwmDynMotorConfigData& configData,
                      const TsDcPwmDynMotorInputData&  inputData) const;
        /// @brief    Computes the motor stall and drives torques.
        virtual void generateMotorTorque();
        /// @brief    Updates the total external load on the motor shaft.
        virtual void gatherExternalLoads();
        /// @brief    Updates the motor speed.
        virtual void computeMotorSpeed(const double dt);
        /// @brief    Updates electrical current, power, resistance.
        virtual void computeElectricalOutputs();
        /// @brief    Updates efficiency and waste heat.
        virtual void computeWasteHeat();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsDcPwmDynMotor(const TsDcPwmDynMotor&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsDcPwmDynMotor& operator =(const TsDcPwmDynMotor&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] voltage (V) Input supply voltage.
///
/// @details  Sets the supply voltage to the Motor.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsDcPwmDynMotor::setVoltage(const double voltage)
{
    mVoltage = voltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] pulseWidth (--) (0-1) Input pulse width.
///
/// @details  Sets the pulse width command to the Motor.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsDcPwmDynMotor::setPulseWidth(const double pulseWidth)
{
    mPulseWidth = pulseWidth;
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
inline void TsDcPwmDynMotor::setLoadTorques(const double torque1,
                                            const double torque2,
                                            const double torque3,
                                            const double torque4)
{
    mLoadTorques[0] = torque1;
    mLoadTorques[1] = torque2;
    mLoadTorques[2] = torque3;
    mLoadTorques[3] = torque4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns bool (--) True if the motor has been properly initialized, false if not.
///
/// @details  Returns the initialized status of the motor.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsDcPwmDynMotor::isInitialized() const
{
    return mInitFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (amp) Motor electrical current draw.
///
/// @details  Returns the Motor electrical current draw.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsDcPwmDynMotor::getCurrent() const
{
    return mCurrent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (amp) Motor generated electrical current.
///
/// @details  Returns the Motor generated electrical current.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsDcPwmDynMotor::getGeneratedCurrent() const
{
    return mGeneratedCurrent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (revolution/min) Motor rotational speed.
///
/// @details  Returns the Motor rotational speed.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsDcPwmDynMotor::getSpeed() const
{
    return mMotorSpeed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (ohm) Motor total electrical resistance.
///
/// @details  Returns the Motor total electrical resistance.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsDcPwmDynMotor::getResistance() const
{
    return mResistance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) Motor electrical power demand.
///
/// @details  Returns the Motor electrical power demand.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsDcPwmDynMotor::getPower() const
{
    return mPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) Motor waste heat.
///
/// @details  Returns the Motor waste heat.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsDcPwmDynMotor::getWasteHeat() const
{
    return mWasteHeat;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (--) (0-1) Motor total efficiency.
//
/// @details  Returns the Motor total efficiency.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsDcPwmDynMotor::getEfficiency() const
{
    return mEfficiency;
}

#endif
