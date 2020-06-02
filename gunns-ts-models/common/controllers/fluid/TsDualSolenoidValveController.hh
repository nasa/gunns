#ifndef TsDualSolenoidValveController_EXISTS
#define TsDualSolenoidValveController_EXISTS

/**
@file
@brief     TS21 Fluid Controller Dual Solenoid Valve Controller Model declarations.

@defgroup  TSM_CONTROLLER_FLUID_DUAL_SOLENOID_VALVE_CONTROLLER  Dual Solenoid Valve Controller
@ingroup   TSM_CONTROLLER_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for the TS21 Fluid Controller Dual Solenoid Valve Controller Model.)

@details
 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - (TsDualSolenoidValveController.o)

 PROGRAMMERS:
 - ((Jason Harvey) (L-3 Communications) (Initial) (2013-06))
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "common/controllers/fluid/TsValveController.hh"
#include "aspects/electrical/UserLoad/UserLoadBase.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Dual Solenoid Valve Controller Model Input Data.
///
/// @details  This class provides a data structure for the Dual Solenoid Valve Controller input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsDualSolenoidValveControllerInputData : public TsValveControllerInputData {
    public:
        bool mOpenSolenoidCmd;                       /**< (--) Initial open channel powered flag  */
        bool mCloseSolenoidCmd;                      /**< (--) Initial close channel powered flag */
        /// @brief  Default constructs this Dual Solenoid Valve Controller input data.
        TsDualSolenoidValveControllerInputData(const double cmdPosition         = 0.0,
                                               const bool   manualPositionFlag  = false,
                                               const double manualPositionValue = 0.0,
                                               const bool   openSolenoidCmd     = false,
                                               const bool   closeSolenoidCmd    = false);
        /// @brief  Default destructs this Dual Solenoid Valve Controller input data.
        virtual ~TsDualSolenoidValveControllerInputData();
        /// @brief  Copy constructs this Dual Solenoid Valve Controller input data.
        TsDualSolenoidValveControllerInputData(const TsDualSolenoidValveControllerInputData& that);
        /// @brief  Assignment operator for this Dual Solenoid Valve Controller input data.
        TsDualSolenoidValveControllerInputData& operator=(const TsDualSolenoidValveControllerInputData& that);
};

// Forward declaration, full declaration is below.
class TsDualSolenoidValveControllerConfigData;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Dual Solenoid Valve Controller Model.
///
/// @details  This class is the signal aspect for a valve that has separate solenoid drivers for the
///           opening & closing directions.  The two solenoids interface with UserLoad objects for
///           drive power (command) and electrical load.  This class can also be used for a valve
///           that has a single solenoid driving in one direction, by configuring it to latch in the
///           other direction, defaulting the unused solenoid command to off, and omitting
///           connecting the unused solenoid to a user load.
///
///           This valve doesn't support intermediate positions - the position is either
///           mMinCmdPosition or mMaxCmdPosition.  There is also no transit time, so the valve flips
///           from one to the other instantly.
///
///           Configure the electrical User Load object (Resistive is recommended) to give the
///           desired currents when the solenoids are energized.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsDualSolenoidValveController : public TsValveController {
    TS_MAKE_SIM_COMPATIBLE(TsDualSolenoidValveController);
    public:
        /// @brief    Enumeration of valve position behavior when power is removed.
        enum LatchType {
            LATCHING        = 0,  ///< Position stays at current value.
            NORMALLY_OPEN   = 1,  ///< Position goes open.
            NORMALLY_CLOSED = 2   ///< Position goes closed.
        };
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool         mMalfOpenSolenoidFail;   /**< (--) Open solenoid power fail malfunction      */
        bool         mMalfCloseSolenoidFail;  /**< (--) Close solenoid power fail malfunction     */
        /// @}
        /// @brief  Default constructs this Dual Solenoid Valve Controller.
        TsDualSolenoidValveController();
        /// @brief  Default destructs this Dual Solenoid Valve Controller.
        virtual ~TsDualSolenoidValveController();
        /// @brief  Initializes this Dual Solenoid Valve Controller.
        /// @callgraph
        void initialize(const TsDualSolenoidValveControllerConfigData &configData,
                        const TsDualSolenoidValveControllerInputData  &inputData,
                        const std::string                             &name = "");
        /// @brief  Updates this Dual Solenoid Valve Controller during run-time.
        /// @callgraph
        virtual void update(const double dt);
        /// @brief  Sets the open solenoid command/power state.
        void setOpenSolenoidCmd(const bool command);
        /// @brief  Sets the close solenoid command/power state.
        void setCloseSolenoidCmd(const bool command);
        /// @brief  Returns the open solenoid user load state.
        UserLoadMode getOpenSolenoidLoad() const;
        /// @brief  Returns the close solenoid user load state.
        UserLoadMode getCloseSolenoidLoad() const;

    protected:
        LatchType    mLatch;                  /**< (--) trick_chkpnt_io(**) Latching type.                            */
        bool         mOpenSolenoidCmd;        /**< (--) Open channel powered input from simbus    */
        bool         mCloseSolenoidCmd;       /**< (--) Close channel powered input from simbus   */
        UserLoadMode mOpenSolenoidLoad;       /**< (--) Open channel load state output to simbus  */
        UserLoadMode mCloseSolenoidLoad;      /**< (--) Close channel load state output to simbus */
        double       mMidCmdPosition;         /**< (--) Mid-point of commandable position range   */
        /// @brief  Validates the object's input data.
        void validate(const TsDualSolenoidValveControllerInputData &inputData);
        /// @brief  Returns a solenoid user load state.
        UserLoadMode calculateSolenoidLoad(const bool cmd, const bool malf) const;

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        TsDualSolenoidValveController(const TsDualSolenoidValveController &rhs);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        TsDualSolenoidValveController& operator= (const TsDualSolenoidValveController &rhs);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Dual Solenoid Valve Controller Model Configuration Data.
///
/// @details  This class provides a data structure for the Dual Solenoid Valve Controller config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsDualSolenoidValveControllerConfigData : public TsValveControllerConfigData {
    public:
        TsDualSolenoidValveController::LatchType mLatch;                  /**< (--) trick_chkpnt_io(**) Latching type */
        /// @brief  Default constructs this Dual Solenoid Valve Controller configuration data.
        TsDualSolenoidValveControllerConfigData(
                const double                                   minCmdPosition   = 0.0,
                const double                                   maxCmdPosition   = 0.0,
                const double                                   minFluidPosition = 0.0,
                const double                                   maxFluidPosition = 0.0,
                const TsDualSolenoidValveController::LatchType latch            = TsDualSolenoidValveController::LATCHING);
        /// @brief  Default destructs this Dual Solenoid Valve Controller configuration data.
        virtual ~TsDualSolenoidValveControllerConfigData();
        /// @brief  Copy constructs this Dual Solenoid Valve Controller configuration data.
        TsDualSolenoidValveControllerConfigData(const TsDualSolenoidValveControllerConfigData& that);
        /// @brief  Assignment operator for this Dual Solenoid Valve Controller configuration data.
        TsDualSolenoidValveControllerConfigData& operator=(const TsDualSolenoidValveControllerConfigData& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  command  (--)  Solenoid power supply flag.
///
/// @return   void
///
/// @details  The command represents whether drive power is supplied to the solenoid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsDualSolenoidValveController::setOpenSolenoidCmd(const bool command)
{
    mOpenSolenoidCmd = command;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  command  (--)  Solenoid power supply flag.
///
/// @return   void
///
/// @details  The command represents whether drive power is supplied to the solenoid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsDualSolenoidValveController::setCloseSolenoidCmd(const bool command)
{
    mCloseSolenoidCmd = command;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   UserLoadMode (--) LoadON if the solenoid is drawing power, otherwise LoadOFF
///
/// @details  The returned state can differ from the power supply/command flag because of
///           malfunctions modeled within this valve model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline UserLoadMode TsDualSolenoidValveController::getOpenSolenoidLoad() const
{
    return mOpenSolenoidLoad;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   UserLoadMode (--) LoadON if the solenoid is drawing power, otherwise LoadOFF
///
/// @details  The returned state can differ from the power supply/command flag because of
///           malfunctions modeled within this valve model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline UserLoadMode TsDualSolenoidValveController::getCloseSolenoidLoad() const
{
    return mCloseSolenoidLoad;
}

#endif
