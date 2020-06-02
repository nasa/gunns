#ifndef TsPositionValveController_EXISTS
#define TsPositionValveController_EXISTS

/**
@file
@brief     TS21 Fluid Controller Position Valve Controller declarations.

@defgroup  TSM_CONTROLLER_FLUID_POSITION_VALVE_CONTROLLER    Position Valve Controller
@ingroup   TSM_CONTROLLER_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Class for TS21 Fluid Controller position commanded valve controllers with command, power and malfunction.)

@details
 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - ((TsPositionValveController.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2012-10)

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

#include "TsPoweredValveController.hh"
#include "TsPositionValveCmd.hh"
#include "TsOpenCloseValveSensed.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Position Valve Controller Model configuration data
///
/// @details  The sole purpose of this class is to provide a data structure for the Position Valve
///           Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsPositionValveControllerConfigData : public TsPoweredValveControllerConfigData
{
    public:
        double        mTolerance;  /**< (--) trick_chkpnt_io(**)  Position command tolerance */
        /// @brief    Default constructs this Powered Valve Controller model configuration data.
        TsPositionValveControllerConfigData(const double                               minCmdPosition   = 0.0,
                                            const double                               maxCmdPosition   = 0.0,
                                            const double                               minFluidPosition = 0.0,
                                            const double                               maxFluidPosition = 0.0,
                                            const double                               transitTime      = 0.0,
                                            const double                               refCmd           = 0.0,
                                            const double                               holdPower        = 0.0,
                                            const double                               movePower        = 0.0,
                                            const double                               stuckPower       = 0.0,
                                            const double                               refVoltage       = 0.0,
                                            const bool                                 eotInterrupt     = false,
                                            const TsPoweredValveController::LatchType  latch            = TsPoweredValveController::LATCHING,
                                            const double                               tolerance        = 0.0);
        /// @brief    Copy constructs this Powered Valve Controller model configuration data.
        TsPositionValveControllerConfigData(const TsPositionValveControllerConfigData& that);
        /// @brief    Default destructs this Powered Valve Controller model configuration data.
        virtual ~TsPositionValveControllerConfigData();
        /// @brief    Assigns this Powered Valve Controller model configuration data.
        TsPositionValveControllerConfigData& operator=(const TsPositionValveControllerConfigData& that);
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Position Valve Controller Model
///
/// @details  Provides the class for a position commanded (motor or solenoid) valve controller. \n
///           Parameters may be set prior to the update call or as arguments in an update call:
///           \code
///           // one at a time:
///               setSupplyVoltage(supplyVoltageFlag)
///               setCommand(TsPositionValveCmd(enable, position));
///               setSensed(TsOpenCloseValveSensed(openSensed, closeSensed)); // with EOT interruption
///               update(dt)
///           // or all together without EOT interruption:
///               update(supplyVoltageFlag, TsPositionValveCmd(enable, position), dt)
///           // or all together with EOT interruption:
///               update(supplyVoltageFlag, TsPositionValveCmd(enable, position), TsOpenCloseValveSensed(openSensed, closeSensed), dt)
/// \endcode
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsPositionValveController : public TsPoweredValveController
{
    TS_MAKE_SIM_COMPATIBLE(TsPositionValveController);
    public:
        /// @brief    Default constructs this Position Valve Controller model.
        TsPositionValveController();
        /// @brief    Default destructs this Position Valve Controller model.
        virtual ~TsPositionValveController();
        /// @brief    Initializes this Position Valve Controller model.
        /// @callgraph
        void initialize(const TsPositionValveControllerConfigData& configData,
                        const TsPoweredValveControllerInputData&   initData,
                        const std::string&                         name);
        /// @brief    Updates the position and power consumption this Position Valve Controller model.
        /// @callgraph
        virtual void update(const double dt);
        /// @brief    Updates the position and power consumption this Position Valve Controller model.
        void update(const bool                    supplyVoltageFlag,
                    const TsPositionValveCmd&     command,
                    const double                  sensed,
                    const double                  dt);
        /// @brief    Sets the enabled flag for this Position Valve Controller model.
        virtual void setEnabledFlag(const bool enabledFlag);
        /// @brief    Sets the command position for this Position Valve Controller model.
        void setPositionCommand(const double position);
        /// @brief    Sets the composite command for this Position Valve Controller model.
        void setCommand(const TsPositionValveCmd& cmd);
        /// @brief    Sets the composite sensed position for this Position Valve Controller model.
        void setSensed(const double sensed);
    protected:
        double              mTolerance;  /**< (--)  trick_chkpnt_io(**) Position command tolerance */
        TsPositionValveCmd  mCommand;    /**< (--)  Composite command.  */
        double              mSensed;     /**< (--)  Sensed position.  */
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsPositionValveController(const TsPositionValveController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsPositionValveController& operator= (const TsPositionValveController&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   enabledFlag  (--)  Enabled flag.
///
/// @return   void
///
/// @details  Sets the enabled flag for this Position Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsPositionValveController::setEnabledFlag(const bool enabledFlag)
{
    mCommand.mEnable = enabledFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   position  (--)  Position command.
///
/// @return   void
///
/// @details  Sets the position command for this Position Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsPositionValveController::setPositionCommand(const double position)
{
    mCommand.mPosition = position;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   command  (--)  Composite command.
///
/// @return   void
///
/// @details  Sets the composite command (enabled, open and close command flags) for this Position
///           Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsPositionValveController::setCommand(const TsPositionValveCmd& command)
{
    mCommand = command;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   sensed  (--)  Composite sensed position.
///
/// @return   void
///
/// @details  Sets the composite sensed position (open and close flags) for this Position Valve
///           Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsPositionValveController::setSensed(const double sensed)
{
    mSensed = sensed;
}

#endif
