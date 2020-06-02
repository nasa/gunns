#ifndef TsToggleValveController_EXISTS
#define TsToggleValveController_EXISTS

/**
@file
@brief     TS21 Fluid Controller Toggle Valve Controller declarations.

@defgroup  TSM_CONTROLLER_FLUID_TOGGLE_VALVE_CONTROLLER   Toggle Valve Controller
@ingroup   TSM_CONTROLLER_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Class for TS21 Fluid Controller toggle commanded valve controllers with command, power and malfunction.)

@details
 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - ((TsToggleValveController.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2012-10)

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

#include "TsPoweredValveController.hh"
#include "TsToggleValveCmd.hh"
#include "TsOpenCloseValveSensed.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Toggle Valve Controller Model
///
/// @details  Provides the class for a toggle commanded (motor or solenoid) valve controller. \n
///           Parameters may be set prior to the update call:
///               \code
///           // one at a time:
///               setSupplyVoltage(supplyVoltageFlag)
///               setEnabledFlag(enableFlag);
///               setToggle(toggle);
///               setOpenSensed(openSensed);    // with EOT interruption
///               setClosedSensed(closeSensed); // with EOT interruption
///               update(dt)
///           // or via simbus at the same rate or (with queuing) at a higher rate
///               update(dt)
/// \endcode
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsToggleValveController : public TsPoweredValveController
{
    TS_MAKE_SIM_COMPATIBLE(TsToggleValveController);
    public:
        /// @brief    Default constructs this Toggle Valve Controller model.
        TsToggleValveController();
        /// @brief    Default destructs this Toggle Valve Controller model.
        virtual ~TsToggleValveController();
        /// @brief    Initializes this Toggle Valve Controller model.
        /// @callgraph
        void initialize(const TsPoweredValveControllerConfigData& configData,
                        const TsPoweredValveControllerInputData&  initData,
                        const std::string&                        name);
        /// @brief    Updates the position and power consumption for this Toggle Valve Controller model.
        /// @callgraph
        virtual void update(const double dt);
        /// @brief    Sets the enabled flag for this Toggle Valve Controller model.
        virtual void setEnabledFlag(const bool enabledFlag);
        /// @brief    Sets/resets toggle flag for this Toggle Valve Controller model.
        void setToggle(const bool toggle);
        /// @brief    Sets the open sensed flag for this Toggle Valve Controller model.
        void setOpenSensed(const bool open);
        /// @brief    Sets the close sensed flag for this Toggle Valve Controller model.
        void setCloseSensed(const bool close);
        /// @brief    Sets the composite sensed position for this Toggle Valve Controller model.
        void setSensed(const TsOpenCloseValveSensed& sensed);
    protected:
        TsToggleValveCmd        mCommand;      /**< (--)  Composite command.  */
        TsOpenCloseValveSensed  mSensed;       /**< (--)  Composite sensed position.  */
        double                  mMidPosition;  /**< (--)  Midpoint valve position. */
        /// @brief    Updates the position and power consumption for this Toggle Valve Controller model.
        void updateWithoutEotInterrupt(const double dt);
        /// @brief    Updates the position and power consumption with end of travel interruption for this Toggle Valve Controller model.
        void updateWithEotInterrupt(const double dt);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsToggleValveController(const TsToggleValveController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsToggleValveController& operator= (const TsToggleValveController&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   enabledFlag  (--)  Enabled flag.
///
/// @return   void
///
/// @details  Sets the enabled flag for this Toggle Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsToggleValveController::setEnabledFlag(const bool enabledFlag)
{
    mCommand.mEnable = enabledFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  toggle  (--)  Toggle flag.
///
/// @return   void
///
/// @details  Sets the toggle flag for this Toggle Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsToggleValveController::setToggle(const bool toggle)
{
    mCommand.mToggle = toggle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   open  (--)  Open sensed flag.
///
/// @return   void
///
/// @details  Sets the open sensed flag for this Toggle Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsToggleValveController::setOpenSensed(const bool open)
{
    mSensed.mOpen = open;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   close  (--)  Close sensed flag.
///
/// @return   void
///
/// @details  Sets the close sensed flag for this Toggle Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsToggleValveController::setCloseSensed(const bool close)
{
    mSensed.mClose = close;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   sensed  (--)  Composite sensed position.
///
/// @return   void
///
/// @details  Sets the composite sensed position (open and close flags) for this Toggle Valve
///           Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsToggleValveController::setSensed(const TsOpenCloseValveSensed& sensed)
{
    mSensed = sensed;
}

#endif
