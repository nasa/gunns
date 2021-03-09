#ifndef ConstantPowerLoad_EXISTS
#define ConstantPowerLoad_EXISTS

/*********************** TRICK HEADER *************************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the Constant Power Load model.
   If the electrical aspect of your component needs to have a constant power supply then they will
   use the constant power load class.
   If your equipment needs to calculate the power based on the ON/OFF/STANDBY modes then you can
   conveniently use this class for your electrical aspect. The power is calculated based on the
   voltage consumed by the load. Voltage value is passed to the step method and the power is
   calculated based on the mode of operation of the component and if the power is valid. PowerValid
   flag is the simBus variable that is updated and written onto the simbus.
   The overWritePower and the overWritePowerFlag values can overwrite the power via malfunctions.
   )

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- (
   (aspects/electrical/UserLoad/UserLoadBase.o)
   (aspects/electrical/UserLoad/ConstantPowerLoad.o)
  )

PROGRAMMERS:
- (
   ((Shailaja Janapati) (L3 Communications) (Initial) (10-2011))
)
@{
****************************************************************************************************/

#include "aspects/electrical/UserLoad/UserLoadBase.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Check the Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Constant Power
///           Load configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ConstantPowerLoadConfigData : public UserLoadBaseConfigData
{
public:
    /// @brief The data value for constant power load.
    double mPowerNormal;  /**< (W) trick_chkpnt_io(**) power for Normal Operation */
    double mPowerStandby; /**< (W) trick_chkpnt_io(**) power for standby operation */

    /// @brief Default constructs this constant power load configuration data.
    ConstantPowerLoadConfigData(const std::string &name     = "Unnamed Load",
            const int loadType = CONSTANT_POWER_LOAD,
            const double underVoltageLimit = 98.0,
            const double powerNormal = 0.0,
            const double powerStandby = 0.0,
            const double fuseCurrentLimit = 0.0);

    /// @brief Default destructs this constant power load configuration data.
    virtual ~ConstantPowerLoadConfigData();

    /// @brief Copy constructs this constant power load configuration data
    ConstantPowerLoadConfigData(const ConstantPowerLoadConfigData& that);

protected:

private:
    /// @brief Assignment operator unavailable since declared private and not implemented.
    ConstantPowerLoadConfigData& operator =(const ConstantPowerLoadConfigData&);

};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    aspects/electrical/ User Load class Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the user load
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ConstantPowerLoadInputData : public UserLoadBaseInputData
{
public:
    /// @brief Default Constructor
    ConstantPowerLoadInputData(const bool   lMalfOverrideCurrentFlag  = false,
                               const double lMalfOverrideCurrentValue = 0.0,
                               const int    loadOperMode              = LoadON,
                               const double initialVoltage            = 0.0);

    /// @brief Default Destructor
    virtual ~ConstantPowerLoadInputData();

    /// @brief Copy constructs this constant power Load Input data.
    ConstantPowerLoadInputData(const ConstantPowerLoadInputData& that);

protected:

private:
    /// @brief Assignment operator unavailable since declared private and not implemented.
    ConstantPowerLoadInputData& operator =(const ConstantPowerLoadInputData&);
};



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Constant Power Load electrical class
///
/// @details  \verbatim
///                                ohm
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class ConstantPowerLoad : public UserLoadBase
{
    TS_MAKE_SIM_COMPATIBLE(ConstantPowerLoad);
public:
    /// @brief Default constructs this constant power user load
    ConstantPowerLoad();

    /// @brief Default destructs this constant power user load
    virtual ~ConstantPowerLoad();

    /// @brief Initializes this constant power load electrical with configuration
    void initialize(const ConstantPowerLoadConfigData& configData,
            const ConstantPowerLoadInputData& inputData,
            std::vector<UserLoadBase *>&  networkLoads,
            const int   cardId,
            const int   loadSwitchId);

    /// @brief
    virtual void step(double voltage);

    /// @brief Sets the power for normal ops.
    void setPowerNormal(const double power);

protected:
    /// @brief power valve supplied for this user load
    double mPowerNormal;  /**< (W) trick_chkpnt_io(**) power for normal mode of operation */

    double mPowerStandby; /**< (W) trick_chkpnt_io(**) power for standby mode of operation */

    /// @brief
    virtual void calculateConstantPowerLoad();

    /// @brief Validates the initialization of this user load.
    void validate(const ConstantPowerLoadConfigData& configData);

    /// @brief Updates the state
    virtual void updateLoad();

    /// @brief Updates the state
    virtual void computeActualPower();


private:
    ////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Copy constructor unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////
    ConstantPowerLoad(const ConstantPowerLoad &);

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Assignment operator unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////
    ConstantPowerLoad& operator =(const ConstantPowerLoad &);
};

#endif
/// @}
