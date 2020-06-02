#ifndef TsOpenCloseValveController_EXISTS
#define TsOpenCloseValveController_EXISTS

/**
@file
@brief     TS21 Fluid Controller Open/Close Valve Controller declarations.

@defgroup  TSM_CONTROLLER_FLUID_OPEN_CLOSE_VALVE_CONTROLLER  Open/Close Valve Controller
@ingroup   TSM_CONTROLLER_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Class for the TS21 Fluid Controller open/close commanded valve controllers with command, power and malfunction.)

@details
 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - ((TsOpenCloseValveController.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2012-10)

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

#include "TsPoweredValveController.hh"
#include "TsOpenCloseValveCmd.hh"
#include "TsOpenCloseValveSensed.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Open/Close Valve Controller Model
///
/// @details  Provides the class for an open/close commanded (motor or solenoid) valve controller. \n
///           Parameters may be set prior to the update call or as arguments in an update call:
///           \code
///           // one at a time:
///               setSupplyVoltage(supplyVoltageFlag)
///               setCommand(TsOpenCloseValveCmd(enableCmd, openCmd, closeCmd));
///               setSensed(TsOpenCloseValveSensed(openSensed, closeSensed)); // with EOT interruption
///               update(dt)
///           // or all together without EOT interruption:
///               update(supplyVoltageFlag, TsOpenCloseValveCmd(enableCmd, openCmd, closeCmd), dt)
///           // or all together with EOT interruption:
///               update(supplyVoltageFlag, TsOpenCloseValveCmd(enableCmd, openCmd, closeCmd), TsOpenCloseValveSensed(openSensed, closeSensed), dt)
/// \endcode
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsOpenCloseValveController : public TsPoweredValveController
{
    TS_MAKE_SIM_COMPATIBLE(TsOpenCloseValveController);
    public:

        /// @brief  Enumeration for handle stowing positions
        enum Handle
        {
            STOWED   = 0,   ///< Stowed
            UNSTOWED = 1    ///< Unstowed
        };

        /// @brief    Default constructs this Open/Close Valve Controller model.
        TsOpenCloseValveController();
        /// @brief    Default destructs this Open/Close Valve Controller model.
        virtual ~TsOpenCloseValveController();
        /// @brief    Initializes this Open/Close Valve Controller model.
        /// @callgraph
        void initialize(const TsPoweredValveControllerConfigData& configData,
                        const TsPoweredValveControllerInputData&  initData,
                        const std::string&                        name);
        /// @brief    Updates the position and power consumption for this Open/Close Valve Controller model.
        /// @callgraph
        virtual void update(const double dt);
        /// @brief    Updates the position and power consumption for this Open/Close Valve Controller model.
        void update(const bool                 supplyVoltageFlag,
                    const TsOpenCloseValveCmd& command,
                    const double               dt);
        /// @brief    Updates the position and power consumption with end of travel interruption for this Open/Close Valve Controller model.
        void update(const bool                    supplyVoltageFlag,
                    const TsOpenCloseValveCmd&    command,
                    const TsOpenCloseValveSensed& sensed,
                    const double                  dt);
        /// @brief    Sets the enabled flag for this Open/Close Valve Controller model.
        virtual void setEnabledFlag(const bool enabledFlag);
        /// @brief    Sets the open command flag for this Open/Close Valve Controller model.
        void setOpenCommand(const bool open);
        /// @brief    Sets the close command flag for this Open/Close Valve Controller model.
        void setCloseCommand(const bool close);
        /// @brief    Sets the composite command for this Open/Close Valve Controller model.
        void setCommand(const TsOpenCloseValveCmd& command);
        /// @brief    Sets the open sensed flag for this Open/Close Valve Controller model.
        void setOpenSensed(const bool open);
        /// @brief    Sets the close sensed flag for this Open/Close Valve Controller model.
        void setCloseSensed(const bool close);
        /// @brief    Sets the composite sensed position for this Open/Close Valve Controller model.
        void setSensed(const TsOpenCloseValveSensed& sensed);
    protected:
        TsOpenCloseValveCmd     mCommand;       /**< (--) Composite command.  */
        TsOpenCloseValveSensed  mSensed;        /**< (--) Composite sensed position.  */
        Handle                  mHandleStowed;  /**< (--) Handle stowed status from crew station. */
        float                   mHandlePosition; /**< (--) 0..1 from crew satation, used if mHandleStowed is UNSTOWED. */

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsOpenCloseValveController(const TsOpenCloseValveController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsOpenCloseValveController& operator= (const TsOpenCloseValveController&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   enabledFlag  (--)  Enabled flag.
///
/// @return   void
///
/// @details  Sets the enabled flag for this Open/Close Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsOpenCloseValveController::setEnabledFlag(const bool enabledFlag)
{
    mCommand.mEnable = enabledFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   open  (--)  Open command flag.
///
/// @return   void
///
/// @details  Sets the open command flag for this Open/Close Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsOpenCloseValveController::setOpenCommand(const bool open)
{
    mCommand.mOpen = open;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   close  (--)  Close command flag.
///
/// @return   void
///
/// @details  Sets the close command flag for this Open/Close Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsOpenCloseValveController::setCloseCommand(const bool close)
{
    mCommand.mClose = close;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   command  (--)  Composite command.
///
/// @return   void
///
/// @details  Sets the composite command (enabled, open and close command flags) for this Open/Close
///           Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsOpenCloseValveController::setCommand(const TsOpenCloseValveCmd& command)
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
inline void TsOpenCloseValveController::setOpenSensed(const bool open)
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
inline void TsOpenCloseValveController::setCloseSensed(const bool close)
{
    mSensed.mClose = close;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   sensed  (--)  Composite sensed position.
///
/// @return   void
///
/// @details  Sets the composite sensed position (open and close flags) for this Open/Close Valve
///           Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsOpenCloseValveController::setSensed(const TsOpenCloseValveSensed& sensed)
{
    mSensed = sensed;
}

#endif
