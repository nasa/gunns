#ifndef TsOpenCloseValveAssembly_EXISTS
#define TsOpenCloseValveAssembly_EXISTS

/**
@file
@brief     Open/Close Valve Assembly Model declarations.

@defgroup  TSM_CONTROLLER_VALVE_ASSEMBLIES_OPEN_CLOSE_VALVE_ASSEMBLY  Open/Close Valve Assembly
@ingroup   TSM_CONTROLLER_VALVE_ASSEMBLIES

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE: (Provides the classes for an open/close commanded (motor or solenoid) valve controller with
          a single power supply and open/close boolean sensors.)

@details
 REFERENCE:
 - (JSC-36432, TCS Console Handbook, Volume 13)

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
  (TsOpenCloseValveAssembly.o)

 PROGRAMMERS:
- ((Tim Caldwell) (GHG) (Initial) (2012-09))
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "common/sensors/SensorBooleanAi.hh"
#include "common/controllers/fluid/TsOpenCloseValveController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Open/Close Valve Assembly Model Configuration Data.
///
/// @details  This class provides a data structure for the Open/Close Valve Assembly config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsOpenCloseValveAssemblyConfigData {
    public:
        TsPoweredValveControllerConfigData mController;             /**< (--) trick_chkpnt_io(**) Valve controller    */
        SensorBooleanAiConfigData          mSensorOpen;             /**< (--) trick_chkpnt_io(**) Valve open sensor   */
        SensorBooleanAiConfigData          mSensorClosed;           /**< (--) trick_chkpnt_io(**) Valve closed sensor */
        /// @brief    Default constructs this Open/Close Valve Config Data.
        TsOpenCloseValveAssemblyConfigData(
                const TsPoweredValveControllerConfigData &controller   = TsPoweredValveControllerConfigData(),
                const SensorBooleanAiConfigData          &sensorOpen   = SensorBooleanAiConfigData(),
                const SensorBooleanAiConfigData          &sensorClosed = SensorBooleanAiConfigData());
        /// @brief    Default destructs this Open/Close Valve Config Data.
        ~TsOpenCloseValveAssemblyConfigData();
        /// @brief    Copy constructs this Open/Close Valve Config Data.
        TsOpenCloseValveAssemblyConfigData(const TsOpenCloseValveAssemblyConfigData& that);
        /// @brief    Assignment operator for this Open/Close Valve Config Data.
        TsOpenCloseValveAssemblyConfigData& operator=(const TsOpenCloseValveAssemblyConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Open/Close Valve Assembly Model Input Data.
///
/// @details  This class provides a data structure for the Open/Close Valve Assembly input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsOpenCloseValveAssemblyInputData {
    public:
        TsPoweredValveControllerInputData mController;              /**< (--) Valve controller    */
        SensorBooleanAiInputData          mSensorOpen;              /**< (--) Valve open sensor   */
        SensorBooleanAiInputData          mSensorClosed;            /**< (--) Valve closed sensor */
        /// @brief    Default constructs this Open/Close Valve Input Data.
        TsOpenCloseValveAssemblyInputData(
                const TsPoweredValveControllerInputData &controller   = TsPoweredValveControllerInputData(),
                const SensorBooleanAiInputData          &sensorOpen   = SensorBooleanAiInputData(),
                const SensorBooleanAiInputData          &sensorClosed = SensorBooleanAiInputData());
        /// @brief    Default destructs this Open/Close Valve Input Data.
        ~TsOpenCloseValveAssemblyInputData();
        /// @brief    Copy constructs this Open/Close Valve Input Data.
        TsOpenCloseValveAssemblyInputData(const TsOpenCloseValveAssemblyInputData& that);
        /// @brief    Assignment operator for this Open/Close Valve Input Data.
        TsOpenCloseValveAssemblyInputData& operator=(const TsOpenCloseValveAssemblyInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Open/Close Valve Assembly Model.
///
/// @details  Packages an open/close commanded (motor or solenoid) valve controller with a single
///           power supply and open/close boolean sensors.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsOpenCloseValveAssembly {
    TS_MAKE_SIM_COMPATIBLE(TsOpenCloseValveAssembly);
    public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        TsOpenCloseValveController mController;              /**< (--) Valve controller           */
        SensorBooleanAi            mSensorOpen;              /**< (--) Valve open sensor          */
        SensorBooleanAi            mSensorClosed;            /**< (--) Valve closed sensor        */
        /// @}
        /// @brief  Default constructs this Open/Close Valve Assembly.
        TsOpenCloseValveAssembly();
        /// @brief  Default destructs this Open/Close Valve Assembly.
        virtual ~TsOpenCloseValveAssembly();
        /// @brief  Returns true if this Open/Close Valve Assembly has been properly initialized.
        bool isInitialized() const;
        /// @brief  Initializes this Open/Close Valve model.
        void initialize(const TsOpenCloseValveAssemblyConfigData &configData,
                        const TsOpenCloseValveAssemblyInputData  &inputData,
                        const std::string                        &name);
        /// @brief  Updates the Valve Controller and position sensors.
        virtual void update(const double dt);
        /// @brief  Updates the Valve Controller and position sensors with arguments.
        virtual void update(const bool                supplyVoltageFlag,
                            const TsOpenCloseValveCmd &command,
                            const double              dt);
        /// @brief  Returns open sensor's sensed output.
        bool getOpenSensed() const;
        /// @brief  Returns closed sensor's sensed output.
        bool getCloseSensed() const;
        /// @brief  Gets this Valve position.
        double getPosition() const;
        /// @brief  Sets the enabled flag for the Valve Controller.
        void setEnabledFlag(const bool enabledFlag);
        /// @brief  Sets the open command flag for the Valve Controller.
        void setOpenCommand(const bool open);
        /// @brief  Sets the close command flag for the Valve Controller.
        void setCloseCommand(const bool close);
        /// @brief  Sets the composite command for the Valve Controller.
        void setCommand(const TsOpenCloseValveCmd& command);
        /// @brief  Sets the supply voltage flag for the Valve Controller.
        void setSupplyVoltageFlag(const bool supplyVoltageFlag);

    protected:
        bool                       mInitialized; /**< (--) trick_chkpnt_io(**) Object initialization flag */

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        TsOpenCloseValveAssembly(const TsOpenCloseValveAssembly&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        TsOpenCloseValveAssembly& operator=(const TsOpenCloseValveAssembly&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  (--)  True if the model initialized properly.
///
/// @details  Returns true if the model initialized properly.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsOpenCloseValveAssembly::isInitialized() const
{
    return mInitialized;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  Open sensor sensed output.
///
/// @details  Returns open sensor's sensed output.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsOpenCloseValveAssembly::getOpenSensed() const
{
    return mSensorOpen.getSensedOutput();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  Close sensor sensed output.
///
/// @details  Returns close sensor's sensed output.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsOpenCloseValveAssembly::getCloseSensed() const
{
    return mSensorClosed.getSensedOutput();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (--)  Valve position.
///
/// @details  Gets the valve position.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsOpenCloseValveAssembly::getPosition() const
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
inline void TsOpenCloseValveAssembly::setEnabledFlag(const bool enabledFlag)
{
    mController.setEnabledFlag(enabledFlag);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  open  (--)  Open command flag.
///
/// @return   void
///
/// @details  Sets the open command flag for the Valve Controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsOpenCloseValveAssembly::setOpenCommand(const bool open)
{
    mController.setOpenCommand(open);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  close  (--)  Close command flag.
///
/// @return   void
///
/// @details  Sets the close command flag for the Valve Controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsOpenCloseValveAssembly::setCloseCommand(const bool close)
{
    mController.setCloseCommand(close);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  command  (--)  Composite command.
///
/// @return   void
///
/// @details  Sets the composite command (enabled, open and close command flags) for the Valve
///           Controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsOpenCloseValveAssembly::setCommand(const TsOpenCloseValveCmd& command)
{
    mController.setCommand(command);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  supplyVoltageFlag  (--)  Supply voltage flag.
///
/// @return   void
///
/// @details  Sets the supply voltage flag for the Valve Controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsOpenCloseValveAssembly::setSupplyVoltageFlag(const bool supplyVoltageFlag)
{
    mController.setSupplyVoltageFlag(supplyVoltageFlag);
}

#endif
