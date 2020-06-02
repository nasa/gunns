#ifndef DerivedResistanceLoadExample_EXISTS
#define DerivedResistanceLoadExample_EXISTS

/************************* TRICK HEADER *********************
@defgroup  TSM_GUNNS_ELECTRICAL_USERLOAD_TEST_DERIVED_RESISTANCE_LOAD_EXAMPLE   Derived Resistance Load Example
@ingroup   TSM_GUNNS_ELECTRICAL_USERLOADBASE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the Derived Resistance Load example.)


LIBRARY DEPENDENCY:
- (
   (aspects/electrical/UserLoad/test/DerivedResistanceLoadExample.o)
  )

PROGRAMMERS:
- (Shailaja Janapati) (L3 Communications) (Initial) (2011-10))

@{
 ********/


#include "aspects/electrical/UserLoad/ResistiveLoad.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Check Valve Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Check Valve
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class DerivedResistanceLoadExampleConfigData : public ResistiveLoadConfigData
{
public:

    double mPrllResistance1; // normal resistance1 for this parallel load
    double mPrllResistance2; // normal resistance2 for this parallel load

    /// @brief Default constructs this resistive user load configuration data.
    DerivedResistanceLoadExampleConfigData(const std::string &name     = "Unnamed Load",
            const int loadType = RESISTIVE_LOAD,
            const double underVoltageLimit = 98,
            const double resistanceNormal = 1.0E6,
            const double resistanceStandby = 1.0E8,
            const double prllResistance1 = 5.0E5,
            const double prllResistance2 = 5.0E5);

    /// @brief Default destructor for this resistive user load configuration data.
    virtual ~DerivedResistanceLoadExampleConfigData();

    /// @brief Copy constructs this constant resistance user load configuration data
    DerivedResistanceLoadExampleConfigData(const DerivedResistanceLoadExampleConfigData& that);

protected:

private:
    /// @brief Assignment operator unavailable since declared private and not implemented.
    DerivedResistanceLoadExampleConfigData& operator =(const DerivedResistanceLoadExampleConfigData&);

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    aspects/electrical/ User Load class Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the user load
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class DerivedResistanceLoadExampleInputData : public ResistiveLoadInputData
{
public:
    /// @brief Default Constructor
    DerivedResistanceLoadExampleInputData(const bool lMalfOverrideCurrentFlag = false,
            const double lMalfOverrideCurrentValue = 0.0,
            int loadOperMode = LoadON);

    /// @brief Default Destructor
    virtual ~DerivedResistanceLoadExampleInputData();

    /// @brief Copy constructs this User Load Elect Input data.
    DerivedResistanceLoadExampleInputData(const DerivedResistanceLoadExampleInputData& that);

protected:

private:
    /// @brief Assignment operator unavailable since declared private and not implemented.
    DerivedResistanceLoadExampleInputData& operator =(const DerivedResistanceLoadExampleInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Constant Resistance User Load electrical node
///
/// @details  \verbatim
///                                ohm
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class DerivedResistanceLoadExample : public ResistiveLoad
{
    TS_MAKE_SIM_COMPATIBLE(DerivedResistanceLoadExample);
public:
    /// @brief Default constructs this resistance user load
    DerivedResistanceLoadExample();

    /// @brief Default destructs this resistance user load
    virtual ~DerivedResistanceLoadExample();

    /// @brief Initializes this resistance user load electrical with configuration
    virtual void initialize(const DerivedResistanceLoadExampleConfigData& configData,
            const DerivedResistanceLoadExampleInputData& inputData,
            std::vector<UserLoadBase*>&    networkLoads,
            const int                  cardId,
            const int                  loadSwitchId);

    /// @brief
    /// virtual void step(double voltage);

protected:
    // none

    /// @brief Updates the state
    void updateLoad();

private:
    ////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Copy constructor unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////
    DerivedResistanceLoadExample(const DerivedResistanceLoadExample&);

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Assignment operator unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////
    DerivedResistanceLoadExample& operator =(const DerivedResistanceLoadExample&);

    /// @details power dissipation is handled here
    double mPowerdissipated;

    /// @details efficiency percentage is set here
    double mEfficiency;

    // parallel resistance values set for resistances
    double mPrllResistance1;
    double mPrllResistance2;
};

/// @}

#endif  /* DerivedResistanceLoadExample_EXISTS */

