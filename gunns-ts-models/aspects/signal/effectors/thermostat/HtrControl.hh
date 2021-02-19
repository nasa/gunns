#ifndef HtrControl_EXISTS
#define HtrControl_EXISTS
/***************************************************************************************************
@defgroup TSM_SIGNAL_EFFECTORS_HTRCONTROL HtrControl model
@ingroup  TSM_SIGNAL_EFFECTORS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
   (The HtrControl is an optional signal aspect within a heater system. Its purpose is to
    determine an ON/OFF status (mCommandActual) that it passes to the electrical aspect.

    The HtrControl is an abstract class that should never be instantiated directly.
    It serves as the base for the Thermostat model and any model-specific heater control classes.

    The HtrControl class is pre-loaded with one malfunction type. Note that if the malf
    is active, the mCommandNominal member will continue to be maintained, just the
    mCommandActual member will be overwritten.

        -- Command:ON and Command:OFF
                These malfunctions, set by activating mMalfCmdOnFlag or mMalfCmdOffFlag,
                force the HtrControl to command status ON or OFF, respectively. The command OFF
                malfunction takes precedence, and will automatically de-activate the ON malf
                if both are active at once. Once both malfunctions are no longer active, the
                HtrControl returns to command status based on its sensor logic, as if no malf
                had ever happened at all.

    The HtrControl class also contains a kill-switch boolean that can be set by the simbus.
    If the kill-switch is engaged, the command will be forced to OFF regardless of temperature
    inputs. Malfunctions, however, will still take precedence over the kill-switch setting.

REQUIREMENTS:
    (R.TS210-0002: TS21 vehicle subsystem models shall provide component values within spec limits.)
    (R.TS210-0010: TS21 vehicle subsystem models shall respond to thermal inputs.)
    (R.TS228-0006: Spacecraft thermal models shall perform heater control.)
    (R.TS228-0031: Spacecraft thermal models shall provide heater malfunctions.)

REFERENCES:
    (MDC-95H0225E, Boeing Heater System Notebook)
    (ISS Thermal Control Training Manual, 01.07.05(0)T0012)

ASSUMPTIONS AND LIMITATIONS:
    ()

LIBRARY DEPENDENCY:
    (HtrControl.o)

PROGRAMMERS:
    (
    (Joe Valerioti) (L3) (Jun 2013) (prototype, parent for Thermostat)
    )
@{
***************************************************************************************************/
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "aspects/electrical/UserLoad/UserLoadBase.hh"   //needed only for UserLoadMode enum typedef

/// Forward declarations
//class std::string;   //<string> is currently #included in UserLoadBase.hh

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    HtrControl Configuration Data.
///
/// @details  The sole purpose of this class is to provide a data structure for the HtrControl
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class HtrControlConfigData
{
    protected:
        /// @brief  Default constructs this HtrControl configuration data.
        HtrControlConfigData();

        /// @brief  Copy constructs this HtrControl configuration data.
        HtrControlConfigData(const HtrControlConfigData& that);

        /// @brief  Default destructs this HtrControl configuration data.
        virtual ~HtrControlConfigData();

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        HtrControlConfigData& operator=(const HtrControlConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    HtrControl Input Data.
///
/// @details  The sole purpose of this class is to provide a data structure for the HtrControl
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class HtrControlInputData
{
    public:
        bool iMalfCmdOnFlag;         /**< (--)  Malfunction flag, heater command:on */
        bool iMalfCmdOffFlag;        /**< (--)  Malfunction flag, heater command:off */
        bool iIsKillSwitchEngaged;   /**< (--)  Flag to open thermostat switch and kill the heater (set by simbus) */

    protected:
        /// @brief  Default constructs this HtrControl input data.
        HtrControlInputData(const bool malfCmdOnFlag  = false,
                            const bool malfCmdOffFlag = false,
                            const bool isKillSwitchEngaged = false);

        /// @brief  Copy constructs this HtrControl input data.
        HtrControlInputData(const HtrControlInputData& that);

        /// @brief  Default destructs this HtrControl input data.
        virtual ~HtrControlInputData();

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        HtrControlInputData& operator=(const HtrControlInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    HtrControl class
///
/// @details  The HtrControl class is abstract and cannot be instantiated directly. It is designed
///           as a base-class for Thermostat and any other device or circuit used for heater
///           control.
////////////////////////////////////////////////////////////////////////////////////////////////////
class HtrControl
{
    TS_MAKE_SIM_COMPATIBLE(HtrControl);

    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool mMalfCmdOnFlag;    /**< (--)  Malfunction flag, command ON */
        bool mMalfCmdOffFlag;   /**< (--)  Malfunction flag, command OFF (takes precedence over ON)*/
        /// @}

        /// @brief  Default constructs the object.
        HtrControl();

        /// @brief  Default destructs this HtrControl.
        virtual ~HtrControl();

        /// @brief  Loads object with config & input data and gives the object a name.
        void initialize(const HtrControlConfigData& config,
                        const HtrControlInputData& input,
                        const std::string& name = "Unnamed");

        /// @brief  Pure virtual simulation update task.
        virtual void update(const double timeStep) = 0;

        /// @brief  Verify that object has been initialized.
        void verifyInitialization();

        /// @brief  Gets the initialization complete flag of this HtrControl.
        bool isInitialized() const;

        /// @brief  Gets the actual command status of the HtrControl.
        UserLoadMode getCommandActual() const;

   protected:
        std::string mName;         /**< *o (--) trick_chkpnt_io(**) Object name */
        bool        mInitialized;  /**< *o (--) trick_chkpnt_io(**) Initialization complete flag */

        bool mIsKillSwitchEngaged;    /**< (--)  Flag to open thermostat switch and kill the heater (set by simbus)*/
        UserLoadMode mCommandActual;  /**< (--)  Actual command on/off (simbus output to elec aspect)*/
        UserLoadMode mCommandNominal; /**< (--)  Nominal heater command, regardless of malfs */

        /// @brief  Validates this HtrControl input data.
        void validate(const HtrControlInputData& input) const;

        /// @brief  Applies the Command:ON/OFF malfunction if active.
        UserLoadMode considerCommandMalfs(const UserLoadMode commandStatus);

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        HtrControl(const HtrControl&);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        HtrControl& operator=(const HtrControl&);
};
/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     bool  (--)  initialization complete status
///
/// @details    Gets the initialization complete status of this HtrControl.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool HtrControl::isInitialized() const
{
    return mInitialized;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     UserLoadMode  (--)  actual command status of the HtrControl
///
/// @details    Gets the actual command status of the HtrControl.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline UserLoadMode HtrControl::getCommandActual() const
{
    return mCommandActual;
}

#endif // HtrControl_EXISTS
