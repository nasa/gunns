#ifndef TsDualSolenoidValveAssembly_EXISTS
#define TsDualSolenoidValveAssembly_EXISTS

/**
@file
@brief     Dual Solenoid Valve Assembly Model declarations.

@defgroup  TSM_CONTROLLER_VALVE_ASSEMBLIES  Valve Assemblies
@ingroup   TSM_CONTROLLER
@defgroup  TSM_CONTROLLER_VALVE_ASSEMBLIES_DUAL_SOLENOID_VALVE_ASSEMBLY  Dual Solenoid Valve Assembly
@ingroup   TSM_CONTROLLER_VALVE_ASSEMBLIES

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for a dual solenoid valve controller with separate open/close power
            inputs and open/close boolean sensors.)

@details
 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - (TsDualSolenoidValveAssembly.o)

 PROGRAMMERS:
 - ((Jason Harvey) (L-3 Communications) (Initial) (2013-06))
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "common/sensors/SensorBooleanAi.hh"
#include "common/controllers/fluid/TsDualSolenoidValveController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Dual Solenoid Valve Assembly Model Configuration Data.
///
/// @details  This class provides a data structure for the Dual Solenoid Valve Assembly config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsDualSolenoidValveAssemblyConfigData  {
    public:
        TsDualSolenoidValveControllerConfigData mController;        /**< (--) trick_chkpnt_io(**) Valve controller    */
        SensorBooleanAiConfigData               mSensorOpen;        /**< (--) trick_chkpnt_io(**) Valve open sensor   */
        SensorBooleanAiConfigData               mSensorClosed;      /**< (--) trick_chkpnt_io(**) Valve closed sensor */
        /// @brief  Default constructs this Dual Solenoid Valve Assembly configuration data.
        TsDualSolenoidValveAssemblyConfigData(
                const TsDualSolenoidValveControllerConfigData& controller   = TsDualSolenoidValveControllerConfigData(),
                const SensorBooleanAiConfigData&               sensorOpen   = SensorBooleanAiConfigData(),
                const SensorBooleanAiConfigData&               sensorClosed = SensorBooleanAiConfigData());
        /// @brief  Default destructs this Dual Solenoid Valve Assembly configuration data.
        virtual ~TsDualSolenoidValveAssemblyConfigData();
        /// @brief  Copy constructs this Dual Solenoid Valve Assembly configuration data.
        TsDualSolenoidValveAssemblyConfigData(const TsDualSolenoidValveAssemblyConfigData& that);
        /// @brief  Assignment operator for this Dual Solenoid Valve Assembly configuration data.
        TsDualSolenoidValveAssemblyConfigData& operator=(const TsDualSolenoidValveAssemblyConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Dual Solenoid Valve Assembly Model Input Data.
///
/// @details  This class provides a data structure for the Dual Solenoid Valve Assembly input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsDualSolenoidValveAssemblyInputData {
    public:
        TsDualSolenoidValveControllerInputData mController;         /**< (--) Valve controller    */
        SensorBooleanAiInputData               mSensorOpen;         /**< (--) Valve open sensor   */
        SensorBooleanAiInputData               mSensorClosed;       /**< (--) Valve closed sensor */
        /// @brief  Default constructs this Dual Solenoid Valve Assembly input data.
        TsDualSolenoidValveAssemblyInputData(
                const TsDualSolenoidValveControllerInputData& controller   = TsDualSolenoidValveControllerInputData(),
                const SensorBooleanAiInputData&               sensorOpen   = SensorBooleanAiInputData(),
                const SensorBooleanAiInputData&               sensorClosed = SensorBooleanAiInputData());
        /// @brief  Default destructs this Dual Solenoid Valve Assembly input data.
        virtual ~TsDualSolenoidValveAssemblyInputData();
        /// @brief  Copy constructs this Dual Solenoid Valve Assembly input data.
        TsDualSolenoidValveAssemblyInputData(const TsDualSolenoidValveAssemblyInputData& that);
        /// @brief  Assignment operator for this Dual Solenoid Valve Assembly input data.
        TsDualSolenoidValveAssemblyInputData& operator=(const TsDualSolenoidValveAssemblyInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Dual Solenoid Valve Assembly Model.
///
/// @details  Packages a dual solenoid valve controller with separate open/close power inputs and
///           open/close boolean sensors.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsDualSolenoidValveAssembly  {
    TS_MAKE_SIM_COMPATIBLE(TsDualSolenoidValveAssembly);
    public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        TsDualSolenoidValveController mController;           /**< (--) Valve controller           */
        SensorBooleanAi               mSensorOpen;           /**< (--) Valve open limit switch    */
        SensorBooleanAi               mSensorClosed;         /**< (--) Valve closed limit switch  */
        /// @}
        /// @brief  Default constructs this Dual Solenoid Valve Assembly.
        TsDualSolenoidValveAssembly();
        /// @brief  Default destructs this Dual Solenoid Valve Assembly.
        virtual ~TsDualSolenoidValveAssembly();
        /// @brief  Returns true if this Dual Solenoid Valve Assembly has been properly initialized.
        bool isInitialized() const;
        /// @brief  Initializes this Dual Solenoid Valve Assembly.
        void initialize(const TsDualSolenoidValveAssemblyConfigData &configData,
                        const TsDualSolenoidValveAssemblyInputData  &inputData,
                        const std::string                           &name);
        /// @brief  Updates the Valve Controller and position sensors.
        void update(const double dt);
        /// @brief  Updates the Valve Controller and position sensors with arguments.
        virtual void update(const bool   open,
                            const bool   close,
                            const double dt);
        /// @brief  Returns open sensor's sensed output.
        bool getOpenSensed() const;
        /// @brief  Returns closed sensor's sensed output.
        bool getCloseSensed() const;
        /// @brief  Gets this Valve position.
        double getPosition() const;
        /// @brief  Sets the open solenoid command/power flag for the Valve Controller.
        void setOpenSolenoidCmd(const bool open);
        /// @brief  Sets the close solenoid command/power flag for the Valve Controller.
        void setCloseSolenoidCmd(const bool close);

    protected:
        bool                          mInitialized; /**< (--) trick_chkpnt_io(**) Object initialization flag */

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        TsDualSolenoidValveAssembly(const TsDualSolenoidValveAssembly&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        TsDualSolenoidValveAssembly& operator= (const TsDualSolenoidValveAssembly&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  (--)  True if the model initialized properly.
///
/// @details  Returns true if the model initialized properly.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsDualSolenoidValveAssembly::isInitialized() const
{
    return mInitialized;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  Open sensor sensed output.
///
/// @details  Returns open sensor's sensed output.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsDualSolenoidValveAssembly::getOpenSensed() const
{
    return mSensorOpen.getSensedOutput();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  Close sensor sensed output.
///
/// @details  Returns close sensor's sensed output.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsDualSolenoidValveAssembly::getCloseSensed() const
{
    return mSensorClosed.getSensedOutput();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (--)  Valve position.
///
/// @details  Gets the valve position.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsDualSolenoidValveAssembly::getPosition() const
{
    return mController.getPosition();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  open  (--)  Open command/power state.
///
/// @return   void
///
/// @details  Sets the open command/power state for this Open/Close Valve.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsDualSolenoidValveAssembly::setOpenSolenoidCmd(const bool open)
{
    mController.setOpenSolenoidCmd(open);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  close  (--)  Close command/power state.
///
/// @return   void
///
/// @details  Sets the close command/power state for this Open/Close Valve.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsDualSolenoidValveAssembly::setCloseSolenoidCmd(const bool close)
{
    mController.setCloseSolenoidCmd(close);
}

#endif
