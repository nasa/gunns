#ifndef TsSpeedValveController_EXISTS
#define TsSpeedValveController_EXISTS

/**
@file
@brief     TS21 Fluid Controller Speed Valve Controller declarations.

@defgroup  TSM_CONTROLLER_FLUID_SPEED_VALVE_CONTROLLER    Speed Valve Controller
@ingroup   TSM_CONTROLLER_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Class for TS21 Fluid Controller speed commanded valve controllers with command, power and malfunction.)

@details
 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - ((TsSpeedValveController.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2012-10)

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

#include "TsPoweredValveController.hh"
#include "TsSpeedValveCmd.hh"
#include "TsOpenCloseValveSensed.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Speed Valve Controller Model
///
/// @details  Provides the class for a speed commanded (motor or solenoid) valve controller. \n
///           Parameters may be set prior to the update call or as arguments in an update call:
///           \code
///           // one at a time:
///               setSupplyVoltage(supplyVoltageFlag)
///               setCommand(TsSpeedValveCmd(enable, speed));
///               setSensed(TsOpenCloseValveSensed(openSensed, closeSensed)); // with EOT interruption
///               update(dt)
///           // or all together without EOT interruption:
///               update(supplyVoltageFlag, TsSpeedValveCmd(enable, speed), dt)
///           // or all together with EOT interruption:
///               update(supplyVoltageFlag, TsSpeedValveCmd(enable, speed), TsOpenCloseValveSensed(openSensed, closeSensed), dt)
/// \endcode
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsSpeedValveController : public TsPoweredValveController
{
    TS_MAKE_SIM_COMPATIBLE(TsSpeedValveController);
    public:
        /// @brief    Default constructs this Speed Valve Controller model.
        TsSpeedValveController();
        /// @brief    Default destructs this Speed Valve Controller model.
        virtual ~TsSpeedValveController();
        /// @brief    Initializes this Speed Valve Controller model.
        /// @callgraph
        void initialize(const TsPoweredValveControllerConfigData& configData,
                        const TsPoweredValveControllerInputData&  initData,
                        const std::string&                        name);
        /// @brief    Updates the position and power consumption for this Speed Valve Controller model.
        /// @callgraph
        virtual void update(const double dt);
        /// @brief    Updates the position and power consumption for this Speed Valve Controller model.
        void update(const bool             supplyVoltageFlag,
                    const TsSpeedValveCmd& command,
                    const double           dt);
        /// @brief    Updates the position and power consumption with end of travel interruption for this Speed Valve Controller model.
        void update(const bool                    supplyVoltageFlag,
                    const TsSpeedValveCmd&        command,
                    const TsOpenCloseValveSensed& sensed,
                    const double                  dt);
        /// @brief    Sets the enabled flag for this Speed Valve Controller model.
        virtual void setEnabledFlag(const bool enabledFlag);
        /// @brief    Sets the command speed for this Speed Valve Controller model.
        void setSpeedCommand(const double speed);
        /// @brief    Sets the composite command for this Speed Valve Controller model.
        void setCommand(const TsSpeedValveCmd& command);
        /// @brief    Sets the open sensed flag for this Open/Close Valve Controller model.
        void setOpenSensed(const bool open);
        /// @brief    Sets the close sensed flag for this Open/Close Valve Controller model.
        void setCloseSensed(const bool close);
        /// @brief    Sets the composite sensed position for this Speed Valve Controller model.
        void setSensed(const TsOpenCloseValveSensed& sensed);
    protected:
        TsSpeedValveCmd         mCommand;  /**< (--)  Composite command.  */
        TsOpenCloseValveSensed  mSensed;   /**< (--)  Composite sensed position.  */
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsSpeedValveController(const TsSpeedValveController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsSpeedValveController& operator= (const TsSpeedValveController&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   enabledFlag  (--)  Enabled flag.
///
/// @return   void
///
/// @details  Sets the enabled flag for this Speed Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsSpeedValveController::setEnabledFlag(const bool enabledFlag)
{
    mCommand.mEnable = enabledFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   speed  (--)  Speed command.
///
/// @return   void
///
/// @details  Sets the speed command for this Speed Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsSpeedValveController::setSpeedCommand(const double speed)
{
    mCommand.mSpeed = speed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   command  (--)  Composite command.
///
/// @return   void
///
/// @details  Sets the composite command (enabled flag and speed command) for this Speed Valve
///           Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsSpeedValveController::setCommand(const TsSpeedValveCmd& command)
{
    mCommand = command;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   open  (--)  Open sensed flag.
///
/// @return   void
///
/// @details  Sets the open sensed flag for this Open/Close Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsSpeedValveController::setOpenSensed(const bool open)
{
    mSensed.mOpen = open;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   close  (--)  Close sensed flag.
///
/// @return   void
///
/// @details  Sets the close sensed flag for this Open/Close Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsSpeedValveController::setCloseSensed(const bool close)
{
    mSensed.mClose = close;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   sensed  (--)  Composite sensed position.
///
/// @return   void
///
/// @details  Sets the composite sensed position (open and close flags) for this Speed Valve
///           Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsSpeedValveController::setSensed(const TsOpenCloseValveSensed& sensed)
{
    mSensed = sensed;
}

#endif
