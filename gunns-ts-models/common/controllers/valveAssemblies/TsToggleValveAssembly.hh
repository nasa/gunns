#ifndef TsToggleValveAssembly_EXISTS
#define TsToggleValveAssembly_EXISTS

/**
@file
@brief     Toggle Valve Assembly Model declarations.

@defgroup  TSM_CONTROLLER_VALVE_ASSEMBLIES_TOGGLE_VALVE_ASSEMBLY  Toggle Valve Assembly
@ingroup   TSM_CONTROLLER_VALVE_ASSEMBLIES

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for a toggle commanded (motor or solenoid) valve controller with a
            single power supply and open/close boolean sensors.)

@details
 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - (TsToggleValveAssembly.o)

 PROGRAMMERS:
 - ((Jason Harvey) (L-3 Communications) (Initial) (2013-06))
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "common/sensors/SensorBooleanAi.hh"
#include "common/controllers/fluid/TsToggleValveController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Toggle Valve Assembly Model Configuration Data.
///
/// @details  This class provides a data structure for the Toggle Valve Assembly config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsToggleValveAssemblyConfigData  {
    public:
        TsPoweredValveControllerConfigData mController;             /**< (--) trick_chkpnt_io(**) Valve controller    */
        SensorBooleanAiConfigData          mSensorOpen;             /**< (--) trick_chkpnt_io(**) Valve open sensor   */
        SensorBooleanAiConfigData          mSensorClosed;           /**< (--) trick_chkpnt_io(**) Valve closed sensor */
        /// @brief  Default constructs this Toggle Valve Assembly configuration data.
        TsToggleValveAssemblyConfigData(
                const TsPoweredValveControllerConfigData& controller   = TsPoweredValveControllerConfigData(),
                const SensorBooleanAiConfigData&          sensorOpen   = SensorBooleanAiConfigData(),
                const SensorBooleanAiConfigData&          sensorClosed = SensorBooleanAiConfigData());
        /// @brief  Default destructs this Toggle Valve Assembly configuration data.
        virtual ~TsToggleValveAssemblyConfigData();
        /// @brief  Copy constructs this Toggle Valve Assembly configuration data.
        TsToggleValveAssemblyConfigData(const TsToggleValveAssemblyConfigData& that);
        /// @brief  Assignment operator for this Toggle Valve Assembly configuration data.
        TsToggleValveAssemblyConfigData& operator=(const TsToggleValveAssemblyConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Toggle Valve Assembly Model Input Data.
///
/// @details  This class provides a data structure for the Toggle Valve Assembly input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsToggleValveAssemblyInputData {
    public:
        TsPoweredValveControllerInputData mController;              /**< (--) Valve controller    */
        SensorBooleanAiInputData          mSensorOpen;              /**< (--) Valve open sensor   */
        SensorBooleanAiInputData          mSensorClosed;            /**< (--) Valve closed sensor */
        /// @brief  Default constructs this Toggle Valve Assembly input data.
        TsToggleValveAssemblyInputData(
                const TsPoweredValveControllerInputData& controller   = TsPoweredValveControllerInputData(),
                const SensorBooleanAiInputData&          sensorOpen   = SensorBooleanAiInputData(),
                const SensorBooleanAiInputData&          sensorClosed = SensorBooleanAiInputData());
        /// @brief  Default destructs this Toggle Valve Assembly input data.
        virtual ~TsToggleValveAssemblyInputData();
        /// @brief  Copy constructs this Toggle Valve Assembly input data.
        TsToggleValveAssemblyInputData(const TsToggleValveAssemblyInputData& that);
        /// @brief  Assignment operator for this Toggle Valve Assembly input data.
        TsToggleValveAssemblyInputData& operator=(const TsToggleValveAssemblyInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Toggle Valve Assembly Model.
///
/// @details  Packages a toggle commanded (motor or solenoid) valve controller with a single power
///           supply and open/close boolean sensors.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsToggleValveAssembly  {
    TS_MAKE_SIM_COMPATIBLE(TsToggleValveAssembly);
    public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        TsToggleValveController mController;                 /**< (--) Valve controller           */
        SensorBooleanAi         mSensorOpen;                 /**< (--) Valve open sensor          */
        SensorBooleanAi         mSensorClosed;               /**< (--) Valve closed sensor        */
        /// @}
        /// @brief  Default constructs this Toggle Valve Assembly.
        TsToggleValveAssembly();
        /// @brief  Default destructs this Toggle Valve Assembly.
        virtual ~TsToggleValveAssembly();
        /// @brief  Returns true if this Toggle Valve Assembly has been properly initialized.
        bool isInitialized() const;
        /// @brief  Initializes this Toggle Valve Assembly.
        void initialize(const TsToggleValveAssemblyConfigData &configData,
                        const TsToggleValveAssemblyInputData  &inputData,
                        const std::string                     &name);
        /// @brief  Updates the Valve Controller and position sensors.
        void update(const double dt);
        /// @brief  Updates the Valve Controller and position sensors with arguments.
        virtual void update(const bool   supplyVoltageFlag,
                            const bool   enable,
                            const bool   toggle,
                            const double dt);
        /// @brief  Returns open sensor's sensed output.
        bool getOpenSensed() const;
        /// @brief  Returns closed sensor's sensed output.
        bool getCloseSensed() const;
        /// @brief  Gets this Valve position.
        double getPosition() const;
        /// @brief  Sets the enabled flag for the Valve Controller.
        void setEnabledFlag(const bool enabledFlag);
        /// @brief  Sets/resets the toggle command for the Valve Controller.
        void setToggle(const bool toggle);
        /// @brief  Sets the supply voltage flag for the Valve Controller.
        void setSupplyVoltageFlag(const bool supplyVoltageFlag);

    protected:
        bool                    mInitialized;   /**< (--) trick_chkpnt_io(**) Object initialization flag */

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        TsToggleValveAssembly(const TsToggleValveAssembly&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        TsToggleValveAssembly& operator= (const TsToggleValveAssembly&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  (--)  True if the model initialized properly.
///
/// @details  Returns true if the model initialized properly.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsToggleValveAssembly::isInitialized() const
{
    return mInitialized;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  Open sensor sensed output.
///
/// @details  Returns the open sensor's sensed output.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsToggleValveAssembly::getOpenSensed() const
{
    return mSensorOpen.getSensedOutput();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  Close sensor sensed output.
///
/// @details  Returns the close sensor's sensed output.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsToggleValveAssembly::getCloseSensed() const
{
    return mSensorClosed.getSensedOutput();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (--)  Valve position.
///
/// @details  Gets the valve position.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsToggleValveAssembly::getPosition() const
{
    return mController.getPosition();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  enabledFlag  (--)  Enabled flag.
///
/// @return   void
///
/// @details  Sets the enabled flag for the Valve Controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsToggleValveAssembly::setEnabledFlag(const bool enabledFlag)
{
    mController.setEnabledFlag(enabledFlag);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  toggle  (--)  Toggle command flag.
///
/// @return   void
///
/// @details  Sets/resets the toggle command for the Valve Controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsToggleValveAssembly::setToggle(const bool toggle)
{
    mController.setToggle(toggle);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  supplyVoltageFlag  (--)  Supply voltage flag.
///
/// @return   void
///
/// @details  Sets the supply voltage flag for the Valve Controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsToggleValveAssembly::setSupplyVoltageFlag(const bool supplyVoltageFlag)
{
    mController.setSupplyVoltageFlag(supplyVoltageFlag);
}

#endif
