#ifndef ResistiveLoad_EXISTS
#define ResistiveLoad_EXISTS

/*********************************************************************************************
@ingroup TSM_GUNNS_ELECTRICAL_USERLOADBASE
@defgroup TSM_GUNNS_ELECTRICAL_USERLOAD_RESISTIVELOAD

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the Resistive Load model. Most components modeling a user load would be of resistive load
   type, and will be using or deriving from this class. If the electrical aspect of your component is simple and
   needs to calculate the power based on the ON/OFF/STANDBY modes then you can conveniently use this class to model your electrical
   aspect. The power is calculated based on the voltage consumed by the load. Voltage value is passed to the step method and
   the power is calculated based on the mode of operation of the component and if the power is valid. PowerValid flag is the simBus
   variable that is updated and written onto the simbus.If the load is complicated and has other requirements for the loads operations then
   you can derive from this class and override the step method and the calculateResistiveLoad methods as needed.
)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- (
   (ResistiveLoad.o)
  )

PROGRAMMERS:
- (
   ((Shailaja Janapati) (L3 Communications) (Initial) (10-2011))
 )
@{
 ***************************************************************************************************/
#include "aspects/electrical/UserLoad/UserLoadBase.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Constant Resistance User Load Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Resistive Load
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ResistiveLoadConfigData: public UserLoadBaseConfigData
{
public:
    /// @brief The data value for the resistive load. Default value set very high.
    double mResistanceNormal;  /**<  (ohm) trick_chkpnt_io(**) Resistance for normal operation mode */
    double mResistanceStandby; /**<  (ohm) trick_chkpnt_io(**) Resistance for standby operation mode. Standby resistance
                                           is greater than normal resistance. Standby draws less current than normal. */

    /// @brief Default constructs this resistive user load configuration data.
    ResistiveLoadConfigData(const std::string &name     = "Unnamed Load",
            const int loadType = RESISTIVE_LOAD,
            const double underVoltageLimit = 98.0,
            const double resistanceNormal = 1.0E6,
            const double resistanceStandby = 1.0E8,
            const double fuseCurrentLimit  = 0.0);

    /// @brief Default destructor for this resistive load configuration data.
    virtual ~ResistiveLoadConfigData();

    /// @brief Copy constructs this resistive load configuration data
    ResistiveLoadConfigData(const ResistiveLoadConfigData& that);

protected:

private:
    /// @brief Assignment operator unavailable since declared private and not implemented.
    ResistiveLoadConfigData& operator =(const ResistiveLoadConfigData&);

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Resistive Load class Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the resistive load
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ResistiveLoadInputData : public UserLoadBaseInputData
{
public:
    /// @brief Default Constructor
    ResistiveLoadInputData(const bool   lMalfOverrideCurrentFlag  = false,
                           const double lMalfOverrideCurrentValue = 0.0,
                           const int    loadOperMode              = LoadON,
                           const double initialVoltage            = 0.0);

    /// @brief Default Destructor
    virtual ~ResistiveLoadInputData();

    /// @brief Copy constructs this Constant Resistance Load Elect Input data.
    ResistiveLoadInputData(const ResistiveLoadInputData& that);

protected:

private:
    /// @brief Assignment operator unavailable since declared private and not implemented.
    ResistiveLoadInputData& operator =(const ResistiveLoadInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Resistive Load electrical class
///
/// @details  \verbatim
///                                ohm
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class ResistiveLoad : public UserLoadBase
{
    TS_MAKE_SIM_COMPATIBLE(ResistiveLoad);
public:
    /// @brief Default constructs this resistive user load
    ResistiveLoad();

    /// @brief Default destructs this resistive user load
    virtual ~ResistiveLoad();

    /// @brief Initializes this resistive load electrical with configuration
    void initialize(const ResistiveLoadConfigData& configData,
            const ResistiveLoadInputData& inputData,
            std::vector<UserLoadBase*>&    networkLoads,
            const int  cardId,
            const int  loadSwitchId);

    /// @brief
    virtual void step(double voltage);


protected:
    /// @brief normal resistance value supplied for this user load
    double mResistanceNormal;   /**< (ohm) trick_chkpnt_io(**) resistance for normal operation */

    /// @brief standby resistance value should be greater normal operation
    /// since standby draws less current than normal.
    double mResistanceStandby; /**< (ohm) trick_chkpnt_io(**) resistance for standby operation */

    bool mPrintMessageOnce;    /**< (--) trick_chkpnt_io(**) print a info message */
    /// @brief This method calculates the current and power values
    /// based on the input voltage
    virtual void calculateResistiveLoad();

    /// @brief Validates the initialization of this user load.
    void validate(const ResistiveLoadConfigData& configData);

    /// @brief Updates the state
    virtual void updateLoad();

    /// @brief Updates the state
    virtual void computeActualPower();

private:
    ////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Copy constructor unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////
    ResistiveLoad(const ResistiveLoad&);

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Assignment operator unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////
    ResistiveLoad& operator =(const ResistiveLoad&);
};

/// @}

#endif

