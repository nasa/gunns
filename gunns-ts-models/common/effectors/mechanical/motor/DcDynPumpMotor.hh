#ifndef DcDynPumpMotor_EXISTS
#define DcDynPumpMotor_EXISTS

#include <string>
#include "software/SimCompatibility/TsSimCompatibility.hh"

/**
@file
@brief     DC Dynamic Pump Motor Model declarations.

@defgroup  TSM_MECH_MOTOR_DC_DYN_PUMP    DC Dynamic Pump Motor Model
@ingroup   TSM_MECH_MOTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:  (This is a generic model of a DC electric motor.  It balances torque created by the motor
           with torques from external loads and internal friction to update a dynamic motor speed.
           Voltage input from a motor controller model can be used to dynamically control the motor
           speed.  The model accepts up to 4 external load torques.  It calculates electrical power,
           current and resistance for output to the power supply.  Current and resistance are
           relative to the input voltage.  Power generation when coasting is not modeled, and all
           electrical loads are always positive.  It calculates waste heat for output to a thermal
           aspect.  The model has malfunctions for motor jam, degrade, waste heat bias and speed
           override.  This model was developed for use by TS21 fluid pump/fan models, but could be
           used with other torque load models.  Note the units for speed used by this model are in
           rev/min, rather than r/s.)

@details
 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - ((DcDynPumpMotor.o))

 PROGRAMMERS:
 - ((Jason Harvey) (L-3 Communications) (2014-03) (Install))

@{
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    DC Dynamic Pump Motor Model Configuration Data
///
/// @details  This class provides a data structure for the DC Dynamic Pump Motor Model config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class DcDynPumpMotorConfigData
{
    public:
        double mWindingResistance;      /**< (ohm)                trick_chkpnt_io(**) Electrical resistance of motor winding */
        double mTorqueConstant;         /**< (N*m/amp)            trick_chkpnt_io(**) Torque and back-EMF constant           */
        double mFrictionConstant;       /**< (N*m*min/revolution) trick_chkpnt_io(**) Dynamic friction torque constant       */
        double mFrictionMinSpeed;       /**< (revolution/min)     trick_chkpnt_io(**) Minimum speed for dynamic friction     */
        double mInertia;                /**< (kg*m2)              trick_chkpnt_io(**) Inertia of the motor+load mass         */
        /// @brief    Default constructs this Motor configuration data.
        DcDynPumpMotorConfigData(const double windingResistance = 0.0,
                                 const double torqueConstant    = 0.0,
                                 const double frictionConstant  = 0.0,
                                 const double frictionMinSpeed  = 0.0,
                                 const double inertia           = 0.0);
        /// @brief    Copy constructs this Motor configuration data.
        DcDynPumpMotorConfigData(const DcDynPumpMotorConfigData& that);
        /// @brief    Assignment operator for this Motor configuration data.
        DcDynPumpMotorConfigData& operator =(const DcDynPumpMotorConfigData& that);
        /// @brief    Default destructs this Motor configuration data.
        virtual ~DcDynPumpMotorConfigData();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    DC Dynamic Pump Motor Model Input Data
///
/// @details  This class provides a data structure for the DC Dynamic Pump Motor Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class DcDynPumpMotorInputData
{
    public:
        double mVoltage;                /**< (V)                  Initial control voltage                */
        double mCurrentLimit;           /**< (amp)                Initial current limit                  */
        double mMotorSpeed;             /**< (revolution/min)     Initial motor speed                    */
        double mLoadTorque1;            /**< (N*m)                1st initial external load torque       */
        double mLoadTorque2;            /**< (N*m)                2nd initial external load torque       */
        double mLoadTorque3;            /**< (N*m)                3rd initial external load torque       */
        double mLoadTorque4;            /**< (N*m)                4th initial external load torque       */
        bool   mMalfDegradeFlag;        /**< (--)                 Initial Degrade malf flag              */
        double mMalfDegradeValue;       /**< (--)                 Initial (0-1) Degrade malf value       */
        bool   mMalfJamFlag;            /**< (--)                 Initial Jam malf flag                  */
        double mMalfJamValue;           /**< (--)                 Initial (>0) Jam malf value            */
        bool   mMalfSpeedOverrideFlag;  /**< (--)                 Initial Speed override malf flag       */
        double mMalfSpeedOverrideValue; /**< (revolution/min)     Initial Speed override malf value      */
        bool   mMalfHeatRateBiasFlag;   /**< (--)                 Initial Heat rate bias malf flag       */
        double mMalfHeatRateBiasValue;  /**< (W)                  Initial Heat rate bias malf value      */
        /// @brief    Default constructs this Motor input data.
        DcDynPumpMotorInputData(const double voltage      = 0.0,
                                const double currentLimit = 1.0E15,
                                const double motorSpeed   = 0.0,
                                const double loadTorque1  = 0.0,
                                const double loadTorque2  = 0.0,
                                const double loadTorque3  = 0.0,
                                const double loadTorque4  = 0.0);
        /// @brief    Copy constructs this Motor input data.
        DcDynPumpMotorInputData(const DcDynPumpMotorInputData& that);
        /// @brief    Assignment operator for this Motor input data.
        DcDynPumpMotorInputData& operator =(const DcDynPumpMotorInputData& that);
        /// @brief    Default destructs this Motor input data.
        virtual ~DcDynPumpMotorInputData();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    DC Dynamic Pump Motor Model
///
/// @details  Refer to class Purpose.
////////////////////////////////////////////////////////////////////////////////////////////////////
class DcDynPumpMotor
{
    TS_MAKE_SIM_COMPATIBLE(DcDynPumpMotor);
    public:
        /// @name    Malfunction targets.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfDegradeFlag;        /**< (--)                 Degrade malf flag                      */
        double mMalfDegradeValue;       /**< (--)                 (0-1) Degrade malf value               */
        bool   mMalfJamFlag;            /**< (--)                 Jam malf flag                          */
        double mMalfJamValue;           /**< (--)                 (>0) Jam malf value                    */
        bool   mMalfSpeedOverrideFlag;  /**< (--)                 Speed override malf flag               */
        double mMalfSpeedOverrideValue; /**< (revolution/min)     Speed override malf value              */
        bool   mMalfHeatRateBiasFlag;   /**< (--)                 Heat rate bias malf flag               */
        double mMalfHeatRateBiasValue;  /**< (W)                  Heat rate bias malf value              */
        /// @}

        /// @brief    Default constructs this Motor.
        DcDynPumpMotor();
        /// @brief    Default destructs this Motor.
        virtual ~DcDynPumpMotor();
        /// @brief    Initializes this Motor with configuration and input data.
        void   initialize(const DcDynPumpMotorConfigData& configData,
                          const DcDynPumpMotorInputData&  inputData,
                          const std::string&              name);
        /// @brief    Updates the state of the Motor.
        virtual void step(const double dt);
        /// @brief    Sets the control voltage to the Motor.
        void   setVoltage(const double voltage);
        /// @brief    Sets the current limit to the Motor.
        void   setCurrentLimit(const double limit);
        /// @brief    Sets the external load torques to the Motor.
        void   setLoadTorques(const double torque1 = 0.0,
                              const double torque2 = 0.0,
                              const double torque3 = 0.0,
                              const double torque4 = 0.0);
        /// @brief Returns the initialized status of the Motor.
        bool   isInitialized() const;
        /// @brief    Returns the Motor electrical current draw.
        double getCurrent() const;
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
        std::string mName;           /**< ** (--)                 trick_chkpnt_io(**) Name of the instance for messages      */
        double mWindingResistance;   /**<    (ohm)                trick_chkpnt_io(**) Electrical resistance of motor winding */
        double mTorqueConstant;      /**<    (N*m/amp)            trick_chkpnt_io(**) Torque and back-EMF constant           */
        double mFrictionConstant;    /**<    (N*m*min/revolution) trick_chkpnt_io(**) Dynamic friction torque constant       */
        double mFrictionMinSpeed;    /**<    (revolution/min)     trick_chkpnt_io(**) Minimum speed for dynamic friction     */
        double mInertia;             /**<    (kg*m2)              trick_chkpnt_io(**) Inertia of the motor+load mass         */
        double mVoltage;             /**<    (V)                                      Input control voltage                  */
        double mCurrentLimit;        /**<    (amp)                                    Input current limit                    */
        double mMotorSpeed;          /**<    (revolution/min)                         Motor speed                            */
        double mLoadTorques[4];      /**<    (N*m)                                    External load torques                  */
        double mPower;               /**<    (W)                                      Motor electrical power demand          */
        double mCurrent;             /**<    (amp)                                    Motor electrical current draw          */
        double mResistance;          /**<    (ohm)                                    Motor total electrical resistive load  */
        double mWasteHeat;           /**<    (W)                                      Waste heat generated by the motor      */
        double mEfficiency;          /**<    (--)                                     (0-1) Motor power efficiency           */
        double mStallTorque;         /**<    (N*m)                                    Maximum torque at current voltage      */
        double mDriveTorque;         /**<    (N*m)                                    Torque produced by the motor           */
        double mFrictionTorque;      /**<    (N*m)                                    Dynamic friction torque                */
        double mTotalExternalLoad;   /**<    (N*m)                                    Total external torque load on shaft    */
        double mDegradedResistance;  /**<    (ohm)                                    Degraded motor winding resistance      */
        bool   mInitFlag;            /**<    (--)                 trick_chkpnt_io(**) Instance has been initialized          */
        /// @brief    Validates the initialization of this Motor.
        void validate(const DcDynPumpMotorConfigData& configData,
                      const DcDynPumpMotorInputData&  inputData) const;
        /// @brief    Updates electrical current, power, resistance.
        virtual void computeElectricalOutputs();
        /// @brief    Computes the motor stall and drives torques.
        virtual void generateMotorTorque();
        /// @brief    Updates the total external load on the motor shaft.
        virtual void gatherExternalLoads();
        /// @brief    Updates the motor speed.
        virtual void computeMotorSpeed(const double dt);
        /// @brief    Updates efficiency and waste heat.
        virtual void computeWasteHeat();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        DcDynPumpMotor(const DcDynPumpMotor&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        DcDynPumpMotor& operator =(const DcDynPumpMotor&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] voltage (V) Input supply voltage.
///
/// @details  Sets the supply voltage to the Motor.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void DcDynPumpMotor::setVoltage(const double voltage)
{
    mVoltage = voltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] limit (amp) Input current limit.
///
/// @details  Sets the limited input current to the Motor, which is the maximum current the motor
///           can draw.  This will most often be set by the controller to limit the motor in-rush
///           current during startup.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void DcDynPumpMotor::setCurrentLimit(const double limit)
{
    mCurrentLimit = limit;
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
inline void DcDynPumpMotor::setLoadTorques(const double torque1,
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
inline bool DcDynPumpMotor::isInitialized() const
{
    return mInitFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (amp) Motor electrical current draw.
///
/// @details  Returns the Motor electrical current draw.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double DcDynPumpMotor::getCurrent() const
{
    return mCurrent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (revolution/min) Motor rotational speed.
///
/// @details  Returns the Motor rotational speed.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double DcDynPumpMotor::getSpeed() const
{
    return mMotorSpeed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (ohm) Motor total electrical resistance.
///
/// @details  Returns the Motor total electrical resistance.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double DcDynPumpMotor::getResistance() const
{
    return mResistance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) Motor electrical power demand.
///
/// @details  Returns the Motor electrical power demand.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double DcDynPumpMotor::getPower() const
{
    return mPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) Motor waste heat.
///
/// @details  Returns the Motor waste heat.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double DcDynPumpMotor::getWasteHeat() const
{
    return mWasteHeat;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (--) (0-1) Motor total efficiency.
//
/// @details  Returns the Motor total efficiency.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double DcDynPumpMotor::getEfficiency() const
{
    return mEfficiency;
}

#endif
