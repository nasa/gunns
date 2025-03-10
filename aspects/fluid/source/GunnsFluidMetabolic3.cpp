/**
@file
@brief    GUNNS Improved Human Metabolics Model implementation (Updated BVAD)

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Provides the classes for modeling human metabolics within a GUNNS fluid network.)

LIBRARY DEPENDENCY:
   ((core/GunnsFluidSource.o)
    (aspects/fluid/fluid/GunnsFluidTraceCompounds.o))
*/

#include "GunnsFluidMetabolic3.hh"
#include "aspects/fluid/fluid/GunnsFluidTraceCompounds.hh"
#include "properties/ChemicalCompound.hh"
#include "math/MsMath.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "math/UnitConversion.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name                (--)      Name of object
/// @param[in,out] nodes               (--)      Pointer to nodes.
///
/// @details  Default constructs this GUNNS Fluid Metabolic link model configuration data with
///           arguments and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolic3ConfigData::GunnsFluidMetabolic3ConfigData(const std::string& name,
                                                               GunnsNodeList*     nodes)
    :
    GunnsFluidSourceConfigData(name, nodes),
    mO2ConsumptionRate_Nominal(0.0),
    mO2ConsumptionRate_Sleep(0.0),
    mO2ConsumptionRate_Recovery0(0.0),
    mO2ConsumptionRate_Recovery1(0.0),
    mO2ConsumptionRate_Recovery2(0.0),
    mO2ConsumptionRate_Recovery3(0.0),
    mO2ConsumptionRate_Aerobic0(0.0),
    mO2ConsumptionRate_Aerobic1(0.0),
    mO2ConsumptionRate_Resistive0(0.0),
    mO2ConsumptionRate_Resistive1(0.0),
    mO2ConsumptionRate_Resistive2(0.0),
    mO2ConsumptionRate_Resistive3(0.0),
    mCO2ProductionRate_Nominal(0.0),
    mCO2ProductionRate_Sleep(0.0),
    mCO2ProductionRate_Recovery0(0.0),
    mCO2ProductionRate_Recovery1(0.0),
    mCO2ProductionRate_Recovery2(0.0),
    mCO2ProductionRate_Recovery3(0.0),
    mCO2ProductionRate_Aerobic0(0.0),
    mCO2ProductionRate_Aerobic1(0.0),
    mCO2ProductionRate_Resistive0(0.0),
    mCO2ProductionRate_Resistive1(0.0),
    mCO2ProductionRate_Resistive2(0.0),
    mCO2ProductionRate_Resistive3(0.0),
    mH2OProductionRate_Nominal(0.0),
    mH2OProductionRate_Sleep(0.0),
    mH2OProductionRate_Recovery0(0.0),
    mH2OProductionRate_Recovery1(0.0),
    mH2OProductionRate_Recovery2(0.0),
    mH2OProductionRate_Recovery3(0.0),
    mH2OProductionRate_Aerobic0(0.0),
    mH2OProductionRate_Aerobic1(0.0),
    mH2OProductionRate_Resistive0(0.0),
    mH2OProductionRate_Resistive1(0.0),
    mH2OProductionRate_Resistive2(0.0),
    mH2OProductionRate_Resistive3(0.0),
    mHeatProductionRate_Nominal(0.0),
    mHeatProductionRate_Sleep(0.0),
    mHeatProductionRate_Recovery0(0.0),
    mHeatProductionRate_Recovery1(0.0),
    mHeatProductionRate_Recovery2(0.0),
    mHeatProductionRate_Recovery3(0.0),
    mHeatProductionRate_Aerobic0(0.0),
    mHeatProductionRate_Aerobic1(0.0),
    mHeatProductionRate_Resistive0(0.0),
    mHeatProductionRate_Resistive1(0.0),
    mHeatProductionRate_Resistive2(0.0),
    mHeatProductionRate_Resistive3(0.0),
    mCH4OProductionRate(0.0),
    mC2H6OProductionRate(0.0),
    mC4H10OProductionRate(0.0),
    mCH2OProductionRate(0.0),
    mC2H4OProductionRate(0.0),
    mC6H6ProductionRate(0.0),
    mC7H8ProductionRate(0.0),
    mC8H10ProductionRate(0.0),
    mCH2CL2ProductionRate(0.0),
    mC3H6OProductionRate(0.0),
    mNH3ProductionRate(0.0),
    mCOProductionRate(0.0),
    mH2ProductionRate(0.0),
    mCH4ProductionRate(0.0)
{
    /// - Initialize default rates to "Life Support Baseline Values and Assumptions Document"
    ///   Table 3.25, column 8.  Convert (g/min to kg/s).
    mO2ConsumptionRate_Nominal     =  0.59 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mO2ConsumptionRate_Sleep       =  0.37 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mO2ConsumptionRate_Recovery0   =  0.59 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mO2ConsumptionRate_Recovery1   =  0.59 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mO2ConsumptionRate_Recovery2   =  0.59 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mO2ConsumptionRate_Recovery3   =  0.59 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mO2ConsumptionRate_Aerobic0    =  3.99 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mO2ConsumptionRate_Aerobic1    =  3.99 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mO2ConsumptionRate_Resistive0  =  1.43 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mO2ConsumptionRate_Resistive1  =  1.43 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mO2ConsumptionRate_Resistive2  =  1.43 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mO2ConsumptionRate_Resistive3  =  1.43 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;

    /// - Initialize default rates to "Life Support Baseline Values and Assumptions Document"
    ///   Table 3.25, column 9.  Convert (g/min to kg/s).
    mCO2ProductionRate_Nominal     =  0.69 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mCO2ProductionRate_Sleep       =  0.44 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mCO2ProductionRate_Recovery0   =  0.69 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mCO2ProductionRate_Recovery1   =  0.69 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mCO2ProductionRate_Recovery2   =  0.69 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mCO2ProductionRate_Recovery3   =  0.69 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mCO2ProductionRate_Aerobic0    =  5.22 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mCO2ProductionRate_Aerobic1    =  5.22 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mCO2ProductionRate_Resistive0  =  1.89 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mCO2ProductionRate_Resistive1  =  1.89 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mCO2ProductionRate_Resistive2  =  1.89 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mCO2ProductionRate_Resistive3  =  1.89 * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;

    /// - Initialize default rates to "Life Support Baseline Values and Assumptions Document"
    ///   Table 3.25, columns 6 & 7.  Convert (g/min to kg/s).
    mH2OProductionRate_Nominal     = (1.33  + 0.00)  * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mH2OProductionRate_Sleep       = (1.08  + 0.00)  * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mH2OProductionRate_Recovery0   = (2.07  + 0.00)  * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mH2OProductionRate_Recovery1   = (1.79  + 0.00)  * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mH2OProductionRate_Recovery2   = (2.20  + 0.00)  * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mH2OProductionRate_Recovery3   = (1.86  + 0.00)  * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mH2OProductionRate_Aerobic0    = (6.99  + 1.34)  * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mH2OProductionRate_Aerobic1    = (13.48 + 11.82) * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mH2OProductionRate_Resistive0  = (11.60 + 11.86) * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mH2OProductionRate_Resistive1  = (8.44  + 2.18)  * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mH2OProductionRate_Resistive2  = (6.66  + 0.78)  * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;
    mH2OProductionRate_Resistive3  = (5.93  + 0.45)  * UnitConversion::KILO_PER_UNIT / UnitConversion::SEC_PER_MIN;

    /// - Initialize default rates to "Life Support Baseline Values and Assumptions Document"
    ///   Table 3.25, column 3.  Convert (kJ/hr to W).
    mHeatProductionRate_Nominal     = 306.0 * UnitConversion::UNIT_PER_KILO / UnitConversion::SEC_PER_HR;
    mHeatProductionRate_Sleep       = 160.0 * UnitConversion::UNIT_PER_KILO / UnitConversion::SEC_PER_HR;
    mHeatProductionRate_Recovery0   = 298.0 * UnitConversion::UNIT_PER_KILO / UnitConversion::SEC_PER_HR;
    mHeatProductionRate_Recovery1   = 301.0 * UnitConversion::UNIT_PER_KILO / UnitConversion::SEC_PER_HR;
    mHeatProductionRate_Recovery2   = 295.0 * UnitConversion::UNIT_PER_KILO / UnitConversion::SEC_PER_HR;
    mHeatProductionRate_Recovery3   = 300.0 * UnitConversion::UNIT_PER_KILO / UnitConversion::SEC_PER_HR;
    mHeatProductionRate_Aerobic0    = 482.0 * UnitConversion::UNIT_PER_KILO / UnitConversion::SEC_PER_HR;
    mHeatProductionRate_Aerobic1    = 467.0 * UnitConversion::UNIT_PER_KILO / UnitConversion::SEC_PER_HR;
    mHeatProductionRate_Resistive0  = 417.0 * UnitConversion::UNIT_PER_KILO / UnitConversion::SEC_PER_HR;
    mHeatProductionRate_Resistive1  = 347.0 * UnitConversion::UNIT_PER_KILO / UnitConversion::SEC_PER_HR;
    mHeatProductionRate_Resistive2  = 338.0 * UnitConversion::UNIT_PER_KILO / UnitConversion::SEC_PER_HR;
    mHeatProductionRate_Resistive3  = 340.0 * UnitConversion::UNIT_PER_KILO / UnitConversion::SEC_PER_HR;

    /// - Initialize trace compound rates to "Life Support Baseline Values and Assumptions
    /// - Document" Table 4.3.  Convert (mg/d to kg/s).
    mCH4OProductionRate   =   0.9  * UnitConversion::MEGA_PER_UNIT / UnitConversion::SEC_PER_DAY;
    mC2H6OProductionRate  =   4.3  * UnitConversion::MEGA_PER_UNIT / UnitConversion::SEC_PER_DAY;
    mC4H10OProductionRate =   0.5  * UnitConversion::MEGA_PER_UNIT / UnitConversion::SEC_PER_DAY;
    mCH2OProductionRate   =   0.4  * UnitConversion::MEGA_PER_UNIT / UnitConversion::SEC_PER_DAY;
    mC2H4OProductionRate  =   0.6  * UnitConversion::MEGA_PER_UNIT / UnitConversion::SEC_PER_DAY;
    mC6H6ProductionRate   =   2.2  * UnitConversion::MEGA_PER_UNIT / UnitConversion::SEC_PER_DAY;
    mC7H8ProductionRate   =   0.6  * UnitConversion::MEGA_PER_UNIT / UnitConversion::SEC_PER_DAY;
    mC8H10ProductionRate  =   0.2  * UnitConversion::MEGA_PER_UNIT / UnitConversion::SEC_PER_DAY;
    mCH2CL2ProductionRate =   0.09 * UnitConversion::MEGA_PER_UNIT / UnitConversion::SEC_PER_DAY;
    mC3H6OProductionRate  =  19.0  * UnitConversion::MEGA_PER_UNIT / UnitConversion::SEC_PER_DAY;
    mNH3ProductionRate    =  50.0  * UnitConversion::MEGA_PER_UNIT / UnitConversion::SEC_PER_DAY;
    mCOProductionRate     =  18.0  * UnitConversion::MEGA_PER_UNIT / UnitConversion::SEC_PER_DAY;
    mH2ProductionRate     =  42.0  * UnitConversion::MEGA_PER_UNIT / UnitConversion::SEC_PER_DAY;
    mCH4ProductionRate    = 329.0  * UnitConversion::MEGA_PER_UNIT / UnitConversion::SEC_PER_DAY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that   --  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Metabolic link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolic3ConfigData::GunnsFluidMetabolic3ConfigData(
                                                         const GunnsFluidMetabolic3ConfigData& that)
    :
    GunnsFluidSourceConfigData(that),
    mO2ConsumptionRate_Nominal(that.mO2ConsumptionRate_Nominal),
    mO2ConsumptionRate_Sleep(that.mO2ConsumptionRate_Sleep),
    mO2ConsumptionRate_Recovery0(that.mO2ConsumptionRate_Recovery0),
    mO2ConsumptionRate_Recovery1(that.mO2ConsumptionRate_Recovery1),
    mO2ConsumptionRate_Recovery2(that.mO2ConsumptionRate_Recovery2),
    mO2ConsumptionRate_Recovery3(that.mO2ConsumptionRate_Recovery3),
    mO2ConsumptionRate_Aerobic0(that.mO2ConsumptionRate_Aerobic0),
    mO2ConsumptionRate_Aerobic1(that.mO2ConsumptionRate_Aerobic1),
    mO2ConsumptionRate_Resistive0(that.mO2ConsumptionRate_Resistive0),
    mO2ConsumptionRate_Resistive1(that.mO2ConsumptionRate_Resistive1),
    mO2ConsumptionRate_Resistive2(that.mO2ConsumptionRate_Resistive2),
    mO2ConsumptionRate_Resistive3(that.mO2ConsumptionRate_Resistive3),
    mCO2ProductionRate_Nominal(that.mCO2ProductionRate_Nominal),
    mCO2ProductionRate_Sleep(that.mCO2ProductionRate_Sleep),
    mCO2ProductionRate_Recovery0(that.mCO2ProductionRate_Recovery0),
    mCO2ProductionRate_Recovery1(that.mCO2ProductionRate_Recovery1),
    mCO2ProductionRate_Recovery2(that.mCO2ProductionRate_Recovery2),
    mCO2ProductionRate_Recovery3(that.mCO2ProductionRate_Recovery3),
    mCO2ProductionRate_Aerobic0(that.mCO2ProductionRate_Aerobic0),
    mCO2ProductionRate_Aerobic1(that.mCO2ProductionRate_Aerobic1),
    mCO2ProductionRate_Resistive0(that.mCO2ProductionRate_Resistive0),
    mCO2ProductionRate_Resistive1(that.mCO2ProductionRate_Resistive1),
    mCO2ProductionRate_Resistive2(that.mCO2ProductionRate_Resistive2),
    mCO2ProductionRate_Resistive3(that.mCO2ProductionRate_Resistive3),
    mH2OProductionRate_Nominal(that.mH2OProductionRate_Nominal),
    mH2OProductionRate_Sleep(that.mH2OProductionRate_Sleep),
    mH2OProductionRate_Recovery0(that.mH2OProductionRate_Recovery0),
    mH2OProductionRate_Recovery1(that.mH2OProductionRate_Recovery1),
    mH2OProductionRate_Recovery2(that.mH2OProductionRate_Recovery2),
    mH2OProductionRate_Recovery3(that.mH2OProductionRate_Recovery3),
    mH2OProductionRate_Aerobic0(that.mH2OProductionRate_Aerobic0),
    mH2OProductionRate_Aerobic1(that.mH2OProductionRate_Aerobic1),
    mH2OProductionRate_Resistive0(that.mH2OProductionRate_Resistive0),
    mH2OProductionRate_Resistive1(that.mH2OProductionRate_Resistive1),
    mH2OProductionRate_Resistive2(that.mH2OProductionRate_Resistive2),
    mH2OProductionRate_Resistive3(that.mH2OProductionRate_Resistive3),
    mHeatProductionRate_Nominal(that.mHeatProductionRate_Nominal),
    mHeatProductionRate_Sleep(that.mHeatProductionRate_Sleep),
    mHeatProductionRate_Recovery0(that.mHeatProductionRate_Recovery0),
    mHeatProductionRate_Recovery1(that.mHeatProductionRate_Recovery1),
    mHeatProductionRate_Recovery2(that.mHeatProductionRate_Recovery2),
    mHeatProductionRate_Recovery3(that.mHeatProductionRate_Recovery3),
    mHeatProductionRate_Aerobic0(that.mHeatProductionRate_Aerobic0),
    mHeatProductionRate_Aerobic1(that.mHeatProductionRate_Aerobic1),
    mHeatProductionRate_Resistive0(that.mHeatProductionRate_Resistive0),
    mHeatProductionRate_Resistive1(that.mHeatProductionRate_Resistive1),
    mHeatProductionRate_Resistive2(that.mHeatProductionRate_Resistive2),
    mHeatProductionRate_Resistive3(that.mHeatProductionRate_Resistive3),
    mCH4OProductionRate(that.mCH4OProductionRate),
    mC2H6OProductionRate(that.mC2H6OProductionRate),
    mC4H10OProductionRate(that.mC4H10OProductionRate),
    mCH2OProductionRate(that.mCH2OProductionRate),
    mC2H4OProductionRate(that.mC2H4OProductionRate),
    mC6H6ProductionRate(that.mC6H6ProductionRate),
    mC7H8ProductionRate(that.mC7H8ProductionRate),
    mC8H10ProductionRate(that.mC8H10ProductionRate),
    mCH2CL2ProductionRate(that.mCH2CL2ProductionRate),
    mC3H6OProductionRate(that.mC3H6OProductionRate),
    mNH3ProductionRate(that.mNH3ProductionRate),
    mCOProductionRate(that.mCOProductionRate),
    mH2ProductionRate(that.mH2ProductionRate),
    mCH4ProductionRate(that.mCH4ProductionRate)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Metabolic link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolic3ConfigData::~GunnsFluidMetabolic3ConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    malfBlockageFlag   (--)    Blockage malfunction flag.
/// @param[in]    malfBlockageValue  (--)    Blockage malfunction fractional value (0-1).
/// @param[in]    flowDemand         (kg/s)  Initial demanded mass flow rate of the link.
/// @param[in]    nNominal           (--)    Number of crew members in NOMINAL state.
/// @param[in]    nSleep             (--)    Number of crew members in SLEEP state.
/// @param[in]    nRecovery0         (--)    Number of crew members in RECOVERY_0 state.
/// @param[in]    nRecovery1         (--)    Number of crew members in RECOVERY_1 state.
/// @param[in]    nRecovery2         (--)    Number of crew members in RECOVERY_2 state.
/// @param[in]    nRecovery3         (--)    Number of crew members in RECOVERY_3 state.
/// @param[in]    nAerobic0          (--)    Number of crew members in AEROBIC_0 state.
/// @param[in]    nAerobic1          (--)    Number of crew members in AEROBIC_1 state.
/// @param[in]    nResistive0        (--)    Number of crew members in RESISTIVE_0 state.
/// @param[in]    nResistive1        (--)    Number of crew members in RESISTIVE_1 state.
/// @param[in]    nResistive2        (--)    Number of crew members in RESISTIVE_2 state.
/// @param[in]    nResistive3        (--)    Number of crew members in RESISTIVE_3 state.
///
/// @details  Default constructs this GUNNS Fluid Metabolic link model input data with arguments
///           and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolic3InputData::GunnsFluidMetabolic3InputData(const bool   malfBlockageFlag,
                                                             const double malfBlockageValue,
                                                             const double flowDemand,
                                                             const double nNominal,
                                                             const double nSleep,
                                                             const double nRecovery0,
                                                             const double nRecovery1,
                                                             const double nRecovery2,
                                                             const double nRecovery3,
                                                             const double nAerobic0,
                                                             const double nAerobic1,
                                                             const double nResistive0,
                                                             const double nResistive1,
                                                             const double nResistive2,
                                                             const double nResistive3)
    :
    GunnsFluidSourceInputData(malfBlockageFlag, malfBlockageValue, flowDemand),
    mNNominal(nNominal),
    mNSleep(nSleep),
    mNRecovery0(nRecovery0),
    mNRecovery1(nRecovery1),
    mNRecovery2(nRecovery2),
    mNRecovery3(nRecovery3),
    mNAerobic0(nAerobic0),
    mNAerobic1(nAerobic1),
    mNResistive0(nResistive0),
    mNResistive1(nResistive1),
    mNResistive2(nResistive2),
    mNResistive3(nResistive3)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  (--)   Source to copy from.
///
/// @details  Copy constructs this GUNNS Fluid Metabolic link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolic3InputData::GunnsFluidMetabolic3InputData(const GunnsFluidMetabolic3InputData& that)
    :
    GunnsFluidSourceInputData(that),
    mNNominal(that.mNNominal),
    mNSleep(that.mNSleep),
    mNRecovery0(that.mNRecovery0),
    mNRecovery1(that.mNRecovery1),
    mNRecovery2(that.mNRecovery2),
    mNRecovery3(that.mNRecovery3),
    mNAerobic0(that.mNAerobic0),
    mNAerobic1(that.mNAerobic1),
    mNResistive0(that.mNResistive0),
    mNResistive1(that.mNResistive1),
    mNResistive2(that.mNResistive2),
    mNResistive3(that.mNResistive3)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Metabolic link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolic3InputData::~GunnsFluidMetabolic3InputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Metabolic link model with obviously invalid data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolic3::GunnsFluidMetabolic3()
    :
    GunnsFluidSource(),
    mNCrew(),
    mO2ConsumptionRate(),
    mCO2ProductionRate(),
    mH2OProductionRate(),
    mHeatProductionRate(),
    mCH4OProductionRate(0.0),
    mC2H6OProductionRate(0.0),
    mC4H10OProductionRate(0.0),
    mCH2OProductionRate(0.0),
    mC2H4OProductionRate(0.0),
    mC6H6ProductionRate(0.0),
    mC7H8ProductionRate(0.0),
    mC8H10ProductionRate(0.0),
    mCH2CL2ProductionRate(0.0),
    mC3H6OProductionRate(0.0),
    mNH3ProductionRate(0.0),
    mCOProductionRate(0.0),
    mH2ProductionRate(0.0),
    mCH4ProductionRate(0.0),
    mConsumedO2(0.0),
    mProducedCO2(0.0),
    mProducedH2O(0.0),
    mProducedHeat(0.0),
    mProducedCH4O(0.0),
    mProducedC2H6O(0.0),
    mProducedC4H10O(0.0),
    mProducedCH2O(0.0),
    mProducedC2H4O(0.0),
    mProducedC6H6(0.0),
    mProducedC7H8(0.0),
    mProducedC8H10(0.0),
    mProducedCH2CL2(0.0),
    mProducedC3H6O(0.0),
    mProducedNH3(0.0),
    mProducedCO(0.0),
    mProducedH2(0.0),
    mProducedCH4(0.0),
    mO2(-1),
    mCO2(-1),
    mH2O(-1),
    mNH3(-1),
    mCO(-1),
    mH2(-1),
    mCH4(-1),
    mTcCH4O(-1),
    mTcC2H6O(-1),
    mTcC4H10O(-1),
    mTcCH2O(-1),
    mTcC2H4O(-1),
    mTcC6H6(-1),
    mTcC7H8(-1),
    mTcC8H10(-1),
    mTcCH2CL2(-1),
    mTcC3H6O(-1),
    mTcNH3(-1),
    mTcCO(-1),
    mTcH2(-1),
    mTcCH4(-1)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Metabolic link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolic3::~GunnsFluidMetabolic3()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
/// @param[in]      inputData    (--)  Input data.
/// @param[in,out]  links        (--)  Link vector.
/// @param[in]      port0        (--)  Nominal inlet port map index.
/// @param[in]      port1        (--)  Nominal outlet port map index.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Metabolic link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic3::initialize(const GunnsFluidMetabolic3ConfigData& configData,
                                      const GunnsFluidMetabolic3InputData&  inputData,
                                      std::vector<GunnsBasicLink*>&         links,
                                      const int                             port0,
                                      const int                             port1)
{
    /// - First initialize & validate parent.
    GunnsFluidSource::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Validate input data.
    validate(inputData);

    /// - Initialize derived attributes from configuration and input data.
    derive(configData, inputData);

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      inputData    (--)  Input data.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Fluid Metabolic link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic3::validate(const GunnsFluidMetabolic3InputData&  inputData) const
{
    /// - Throw an exception on negative number of crew members in NOMINAL state.
    if (inputData.mNNominal < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in NOMINAL state < 0.0.");
    }

    /// - Throw an exception on negative number of crew members in SLEEP state.
    if (inputData.mNSleep < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in SLEEP state < 0.0.");
    }

    /// - Throw an exception on negative number of crew members in RECOVERY_0 state.
    if (inputData.mNRecovery0 < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in RECOVERY_0 state < 0.0.");
    }

    /// - Throw an exception on negative number of crew members in RECOVERY_1 state.
    if (inputData.mNRecovery1 < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in RECOVERY_1 state < 0.0.");
    }

    /// - Throw an exception on negative number of crew members in RECOVERY_2 state.
    if (inputData.mNRecovery2 < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in RECOVERY_2 state < 0.0.");
    }

    /// - Throw an exception on negative number of crew members in RECOVERY_3 state.
    if (inputData.mNRecovery3 < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in RECOVERY_3 state < 0.0.");
    }

    /// - Throw an exception on negative number of crew members in AEROBIC_0 state.
    if (inputData.mNAerobic0 < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in AEROBIC_0 state < 0.0.");
    }

    /// - Throw an exception on negative number of crew members in AEROBIC_1 state.
    if (inputData.mNAerobic1 < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in AEROBIC_1 state < 0.0.");
    }

    /// - Throw an exception on negative number of crew members in RESISTIVE_0 state.
    if (inputData.mNResistive0 < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in RESISTIVE_0 state < 0.0.");
    }

    /// - Throw an exception on negative number of crew members in RESISTIVE_1 state.
    if (inputData.mNResistive1 < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in RESISTIVE_1 state < 0.0.");
    }

    /// - Throw an exception on negative number of crew members in RESISTIVE_2 state.
    if (inputData.mNResistive2 < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in RESISTIVE_2 state < 0.0.");
    }

    /// - Throw an exception on negative number of crew members in RESISTIVE_3 state.
    if (inputData.mNResistive3 < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in RESISTIVE_3 state < 0.0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic3::restartModel()
{
    /// - Reset the base class.
    GunnsFluidSource::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    mConsumedO2     = 0.0;
    mProducedCO2    = 0.0;
    mProducedH2O    = 0.0;
    mProducedHeat   = 0.0;
    mProducedCH4O   = 0.0;
    mProducedC2H6O  = 0.0;
    mProducedC4H10O = 0.0;
    mProducedCH2O   = 0.0;
    mProducedC2H4O  = 0.0;
    mProducedC6H6   = 0.0;
    mProducedC7H8   = 0.0;
    mProducedC8H10  = 0.0;
    mProducedCH2CL2 = 0.0;
    mProducedC3H6O  = 0.0;
    mProducedNH3    = 0.0;
    mProducedCO     = 0.0;
    mProducedH2     = 0.0;
    mProducedCH4    = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
/// @param[in]      inputData    (--)  Input data.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Initializes the derived attributes of this GUNNS Fluid Metabolic link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic3::derive(const GunnsFluidMetabolic3ConfigData& configData,
                                  const GunnsFluidMetabolic3InputData&  inputData)
{
    mO2ConsumptionRate[GunnsFluidMetabolic3::NOMINAL]  = configData.mO2ConsumptionRate_Nominal;
    mO2ConsumptionRate[GunnsFluidMetabolic3::SLEEP]  = configData.mO2ConsumptionRate_Sleep;
    mO2ConsumptionRate[GunnsFluidMetabolic3::RECOVERY_0]  = configData.mO2ConsumptionRate_Recovery0;
    mO2ConsumptionRate[GunnsFluidMetabolic3::RECOVERY_1]  = configData.mO2ConsumptionRate_Recovery1;
    mO2ConsumptionRate[GunnsFluidMetabolic3::RECOVERY_2]  = configData.mO2ConsumptionRate_Recovery2;
    mO2ConsumptionRate[GunnsFluidMetabolic3::RECOVERY_3]  = configData.mO2ConsumptionRate_Recovery3;
    mO2ConsumptionRate[GunnsFluidMetabolic3::AEROBIC_0]   = configData.mO2ConsumptionRate_Aerobic0;
    mO2ConsumptionRate[GunnsFluidMetabolic3::AEROBIC_1]   = configData.mO2ConsumptionRate_Aerobic1;
    mO2ConsumptionRate[GunnsFluidMetabolic3::RESISTIVE_0] = configData.mO2ConsumptionRate_Resistive0;
    mO2ConsumptionRate[GunnsFluidMetabolic3::RESISTIVE_1] = configData.mO2ConsumptionRate_Resistive1;
    mO2ConsumptionRate[GunnsFluidMetabolic3::RESISTIVE_2] = configData.mO2ConsumptionRate_Resistive2;
    mO2ConsumptionRate[GunnsFluidMetabolic3::RESISTIVE_3] = configData.mO2ConsumptionRate_Resistive3;

    mCO2ProductionRate[GunnsFluidMetabolic3::NOMINAL]  = configData.mCO2ProductionRate_Nominal;
    mCO2ProductionRate[GunnsFluidMetabolic3::SLEEP]  = configData.mCO2ProductionRate_Sleep;
    mCO2ProductionRate[GunnsFluidMetabolic3::RECOVERY_0]  = configData.mCO2ProductionRate_Recovery0;
    mCO2ProductionRate[GunnsFluidMetabolic3::RECOVERY_1]  = configData.mCO2ProductionRate_Recovery1;
    mCO2ProductionRate[GunnsFluidMetabolic3::RECOVERY_2]  = configData.mCO2ProductionRate_Recovery2;
    mCO2ProductionRate[GunnsFluidMetabolic3::RECOVERY_3]  = configData.mCO2ProductionRate_Recovery3;
    mCO2ProductionRate[GunnsFluidMetabolic3::AEROBIC_0]   = configData.mCO2ProductionRate_Aerobic0;
    mCO2ProductionRate[GunnsFluidMetabolic3::AEROBIC_1]   = configData.mCO2ProductionRate_Aerobic1;
    mCO2ProductionRate[GunnsFluidMetabolic3::RESISTIVE_0] = configData.mCO2ProductionRate_Resistive0;
    mCO2ProductionRate[GunnsFluidMetabolic3::RESISTIVE_1] = configData.mCO2ProductionRate_Resistive1;
    mCO2ProductionRate[GunnsFluidMetabolic3::RESISTIVE_2] = configData.mCO2ProductionRate_Resistive2;
    mCO2ProductionRate[GunnsFluidMetabolic3::RESISTIVE_3] = configData.mCO2ProductionRate_Resistive3;

    mH2OProductionRate[GunnsFluidMetabolic3::NOMINAL]  = configData.mH2OProductionRate_Nominal;
    mH2OProductionRate[GunnsFluidMetabolic3::SLEEP]  = configData.mH2OProductionRate_Sleep;
    mH2OProductionRate[GunnsFluidMetabolic3::RECOVERY_0]  = configData.mH2OProductionRate_Recovery0;
    mH2OProductionRate[GunnsFluidMetabolic3::RECOVERY_1]  = configData.mH2OProductionRate_Recovery1;
    mH2OProductionRate[GunnsFluidMetabolic3::RECOVERY_2]  = configData.mH2OProductionRate_Recovery2;
    mH2OProductionRate[GunnsFluidMetabolic3::RECOVERY_3]  = configData.mH2OProductionRate_Recovery3;
    mH2OProductionRate[GunnsFluidMetabolic3::AEROBIC_0]   = configData.mH2OProductionRate_Aerobic0;
    mH2OProductionRate[GunnsFluidMetabolic3::AEROBIC_1]   = configData.mH2OProductionRate_Aerobic1;
    mH2OProductionRate[GunnsFluidMetabolic3::RESISTIVE_0] = configData.mH2OProductionRate_Resistive0;
    mH2OProductionRate[GunnsFluidMetabolic3::RESISTIVE_1] = configData.mH2OProductionRate_Resistive1;
    mH2OProductionRate[GunnsFluidMetabolic3::RESISTIVE_2] = configData.mH2OProductionRate_Resistive2;
    mH2OProductionRate[GunnsFluidMetabolic3::RESISTIVE_3] = configData.mH2OProductionRate_Resistive3;

    mHeatProductionRate[GunnsFluidMetabolic3::NOMINAL]  = configData.mHeatProductionRate_Nominal;
    mHeatProductionRate[GunnsFluidMetabolic3::SLEEP]  = configData.mHeatProductionRate_Sleep;
    mHeatProductionRate[GunnsFluidMetabolic3::RECOVERY_0]  = configData.mHeatProductionRate_Recovery0;
    mHeatProductionRate[GunnsFluidMetabolic3::RECOVERY_1]  = configData.mHeatProductionRate_Recovery1;
    mHeatProductionRate[GunnsFluidMetabolic3::RECOVERY_2]  = configData.mHeatProductionRate_Recovery2;
    mHeatProductionRate[GunnsFluidMetabolic3::RECOVERY_3]  = configData.mHeatProductionRate_Recovery3;
    mHeatProductionRate[GunnsFluidMetabolic3::AEROBIC_0]   = configData.mHeatProductionRate_Aerobic0;
    mHeatProductionRate[GunnsFluidMetabolic3::AEROBIC_1]   = configData.mHeatProductionRate_Aerobic1;
    mHeatProductionRate[GunnsFluidMetabolic3::RESISTIVE_0] = configData.mHeatProductionRate_Resistive0;
    mHeatProductionRate[GunnsFluidMetabolic3::RESISTIVE_1] = configData.mHeatProductionRate_Resistive1;
    mHeatProductionRate[GunnsFluidMetabolic3::RESISTIVE_2] = configData.mHeatProductionRate_Resistive2;
    mHeatProductionRate[GunnsFluidMetabolic3::RESISTIVE_3] = configData.mHeatProductionRate_Resistive3;

    mCH4OProductionRate     = configData.mCH4OProductionRate;
    mC2H6OProductionRate    = configData.mC2H6OProductionRate;
    mC4H10OProductionRate   = configData.mC4H10OProductionRate;
    mCH2OProductionRate     = configData.mCH2OProductionRate;
    mC2H4OProductionRate    = configData.mC2H4OProductionRate;
    mC6H6ProductionRate     = configData.mC6H6ProductionRate;
    mC7H8ProductionRate     = configData.mC7H8ProductionRate;
    mC8H10ProductionRate    = configData.mC8H10ProductionRate;
    mCH2CL2ProductionRate   = configData.mCH2CL2ProductionRate;
    mC3H6OProductionRate    = configData.mC3H6OProductionRate;
    mNH3ProductionRate      = configData.mNH3ProductionRate;
    mCOProductionRate       = configData.mCOProductionRate;
    mH2ProductionRate       = configData.mH2ProductionRate;
    mCH4ProductionRate      = configData.mCH4ProductionRate;

    mNCrew[GunnsFluidMetabolic3::NOMINAL]               = inputData.mNNominal;
    mNCrew[GunnsFluidMetabolic3::SLEEP]                 = inputData.mNSleep;
    mNCrew[GunnsFluidMetabolic3::RECOVERY_0]            = inputData.mNRecovery0;
    mNCrew[GunnsFluidMetabolic3::RECOVERY_1]            = inputData.mNRecovery1;
    mNCrew[GunnsFluidMetabolic3::RECOVERY_2]            = inputData.mNRecovery2;
    mNCrew[GunnsFluidMetabolic3::RECOVERY_3]            = inputData.mNRecovery3;
    mNCrew[GunnsFluidMetabolic3::AEROBIC_0]             = inputData.mNAerobic0;
    mNCrew[GunnsFluidMetabolic3::AEROBIC_1]             = inputData.mNAerobic1;
    mNCrew[GunnsFluidMetabolic3::RESISTIVE_0]           = inputData.mNResistive0;
    mNCrew[GunnsFluidMetabolic3::RESISTIVE_1]           = inputData.mNResistive1;
    mNCrew[GunnsFluidMetabolic3::RESISTIVE_2]           = inputData.mNResistive2;
    mNCrew[GunnsFluidMetabolic3::RESISTIVE_3]           = inputData.mNResistive3;

    resetRates();

    mO2           = mNodes[1]->getContent()->find(FluidProperties::GUNNS_O2);
    mCO2          = mNodes[1]->getContent()->find(FluidProperties::GUNNS_CO2);
    mH2O          = mNodes[1]->getContent()->find(FluidProperties::GUNNS_H2O);
    mNH3          = findFluidIndex(FluidProperties::GUNNS_NH3);
    mCO           = findFluidIndex(FluidProperties::GUNNS_CO);
    mH2           = findFluidIndex(FluidProperties::GUNNS_H2);
    mCH4          = findFluidIndex(FluidProperties::GUNNS_CH4);

    mTcCH4O       = findTraceCompoundIndex(ChemicalCompound::CH4O);
    mTcC2H6O      = findTraceCompoundIndex(ChemicalCompound::C2H6O);
    mTcC4H10O     = findTraceCompoundIndex(ChemicalCompound::C4H10O);
    mTcCH2O       = findTraceCompoundIndex(ChemicalCompound::CH2O);
    mTcC2H4O      = findTraceCompoundIndex(ChemicalCompound::C2H4O);
    mTcC6H6       = findTraceCompoundIndex(ChemicalCompound::C6H6);
    mTcC7H8       = findTraceCompoundIndex(ChemicalCompound::C7H8);
    mTcC8H10      = findTraceCompoundIndex(ChemicalCompound::C8H10);
    mTcCH2CL2     = findTraceCompoundIndex(ChemicalCompound::CH2CL2);
    mTcC3H6O      = findTraceCompoundIndex(ChemicalCompound::C3H6O);
    mTcNH3        = findTraceCompoundIndex(ChemicalCompound::NH3);
    mTcCO         = findTraceCompoundIndex(ChemicalCompound::CO);
    mTcH2         = findTraceCompoundIndex(ChemicalCompound::H2);
    mTcCH4        = findTraceCompoundIndex(ChemicalCompound::CH4);

    /// - Create the internal fluid.
    createInternalFluid();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type  (--)  Fluid constituent type to find the index of.
///
/// @return   int (--) Index of the constituent type in this network's fluid config.
///
/// @details  Finds and returns the index of the given fluid constituent in this network's fluid
///           config, or -1 if the constituent is not in this network.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsFluidMetabolic3::findFluidIndex(const FluidProperties::FluidType type)
{
    int result = -1;

    /// - We could use the fluid's find method, but it throws an H&S error message when an optional
    ///   type is not present - this is a nuisance for this application so we do our own search.
    const PolyFluidConfigData* config = mNodes[0]->getFluidConfig();
    for (int i = 0; i < config->mNTypes; ++i) {
        if (type == config->mTypes[i]) {
            result = i;
            break;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type  (--)  Trace compound type to find the index of.
///
/// @return   int (--) Index of the compound type in this network's trace compounds config.
///
/// @details  Finds and returns the index of the given compound in this network's trace compounds
///           config, or -1 if the trace compound is not in this network.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsFluidMetabolic3::findTraceCompoundIndex(const ChemicalCompound::Type type)
{
    int result = -1;

    /// - We could use the trace compound's find method, but it throws an H&S error message when an
    ///   optional type is not present - this is a nuisance for this application so we do our own
    ///   search.
    if (GunnsFluidTraceCompounds* tc = mNodes[0]->getContent()->getTraceCompounds()) {
        const GunnsFluidTraceCompoundsConfigData* config = tc->getConfig();
        for (int i = 0; i < config->mNTypes; ++i) {
            if(type != ChemicalCompound::NO_COMPOUND){
                if (config->mCompounds[i]->mType == type) {
                    result = i;
                    break;
                }
            }
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    dt    (s)    Time step.
///
/// @return   void
///
/// @details  Updates the flow demand for this GUNNS Fluid Metabolic link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic3::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Call the virtual updateState method so that any derived class may modify the behavior.
    updateState(dt);

    if (dt < DBL_EPSILON) {
        /// - Zero out the flow demand if time step is negligible.
       mFlowDemand = 0.0;
    } else {

        /// - Compute produced/consumed metabolic rates of the primary fluids & heat.
        resetRates();
        double totalCrew = 0.0;
        for (int i = 0; i < GunnsFluidMetabolic3::NO_METABOLIC; ++i) {
            totalCrew += mNCrew[i];
            mConsumedO2   += mNCrew[i] * mO2ConsumptionRate[i];
            mProducedHeat += mNCrew[i] * mHeatProductionRate[i];
            mProducedCO2  += computeProductionRate(mCO2, -1, mNCrew[i], mCO2ProductionRate[i]);
            mProducedH2O  += computeProductionRate(mH2O, -1, mNCrew[i], mH2OProductionRate[i]);
        }
        /// - O2 consumption rate is limited by the O2 mass in the node.
        mConsumedO2 = fmin((mNodes[1]->getContent()->getMassFraction(mO2) *
                            mNodes[1]->getContent()->getMass()) / dt, mConsumedO2);

        /// - Compute produced trace contaminant metabolic rates.
        mProducedNH3    += computeProductionRate(mNH3, mTcNH3,    totalCrew, mNH3ProductionRate);
        mProducedCO     += computeProductionRate(mCO,  mTcCO,     totalCrew, mCOProductionRate);
        mProducedH2     += computeProductionRate(mH2,  mTcH2,     totalCrew, mH2ProductionRate);
        mProducedCH4    += computeProductionRate(mCH4, mTcCH4,    totalCrew, mCH4ProductionRate);
        mProducedCH4O   += computeProductionRate(-1,   mTcCH4O,   totalCrew, mCH4OProductionRate);
        mProducedC2H6O  += computeProductionRate(-1,   mTcC2H6O,  totalCrew, mC2H6OProductionRate);
        mProducedC4H10O += computeProductionRate(-1,   mTcC4H10O, totalCrew, mC4H10OProductionRate);
        mProducedCH2O   += computeProductionRate(-1,   mTcCH2O,   totalCrew, mCH2OProductionRate);
        mProducedC2H4O  += computeProductionRate(-1,   mTcC2H4O,  totalCrew, mC2H4OProductionRate);
        mProducedC6H6   += computeProductionRate(-1,   mTcC6H6,   totalCrew, mC6H6ProductionRate);
        mProducedC7H8   += computeProductionRate(-1,   mTcC7H8,   totalCrew, mC7H8ProductionRate);
        mProducedC8H10  += computeProductionRate(-1,   mTcC8H10,  totalCrew, mC8H10ProductionRate);
        mProducedCH2CL2 += computeProductionRate(-1,   mTcCH2CL2, totalCrew, mCH2CL2ProductionRate);
        mProducedC3H6O  += computeProductionRate(-1,   mTcC3H6O,  totalCrew, mC3H6OProductionRate);

        /// - Those fluid types that can also be trace compounds are only added to the fluid flow
        ///   rate if they are present as fluid constituents in the network.
        double flowDemand = mProducedH2O + mProducedCO2 - mConsumedO2;
        if (mNH3 > -1) flowDemand += mProducedNH3;
        if (mCO  > -1) flowDemand += mProducedCO;
        if (mH2  > -1) flowDemand += mProducedH2;
        if (mCH4 > -1) flowDemand += mProducedCH4;

        if (flowDemand <= DBL_EPSILON) {
            /// - Zero out the flow demand if total production rate is negligible or negative.
            mFlowDemand = 0.0;
        } else {
            /// - Set the flow demand to the computed total production rate.
            mFlowDemand = flowDemand;
        }
    }
    
    /// - Call the virtual getFlowDemand method so that any derived class can control the flow rate.
    mFlowRate = getFlowDemand();

    /// - Reduce the actual flow rate by the blockage malfunction if it is active.
    if (mMalfBlockageFlag) {
        mFlowRate *= (1.0 - mMalfBlockageValue);
    }

    /// - Convert the mass flow rate demand to molar flow rate based on the molecular weight of the
    ///   source fluid.  Molar flow rate is zeroed in TC-only mode since there will be no affect on
    ///   bulk fluid in the node.
    const double sourceMWeight = mInternalFluid->getMWeight();
    if (sourceMWeight < DBL_EPSILON) {
        mFlux = 0.0;
    } else {
        mFlux = mFlowRate / sourceMWeight;
    }

    buildSource();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Computes the potential drop and port direction across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic3::computeFlows(const double dt __attribute__((unused)))
{
    /// - Calculate potential drop.  Since the source of the node is assumed to be Ground with
    ///   potential = 0, flows into the node cause a potential rise and therefore negative potential
    ///   drop.
    if (mFlux < 0.0) {
        mPotentialDrop =  mPotentialVector[0];
    } else {
        mPotentialDrop = -mPotentialVector[0];
    }

    /// - Set port flow directions and schedule flow from source nodes.
    if (fabs(mFlux) > DBL_EPSILON) {
        mPortDirections[0] = SINK;
    } else {
        mPortDirections[0] = NONE;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  transports the flows across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic3::transportFlows(const double dt)
{
    /// - Calculate true volumetric flow rate from the mass flow rate, using the density of the
    ///   internal fluid.
    const double sourceDensity = mInternalFluid->getDensity();
    if (sourceDensity > DBL_EPSILON) {
        mVolFlowRate = mFlowRate / sourceDensity;
    } else {
        mVolFlowRate = 0.0;
    }

    /// - Calculate hydraulic power.
    computePower();

    /// - Call the virtual updateFluid method to allow derived classes to further modify the
    ///   internal fluid before it is transported.
    updateFluid(dt, mFlowRate);

    /// - Transport the internal fluid to/from the attached node.
    transportFluid(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    dt         (s)    Time step.
/// @param[in]    flowrate   (kg/s) Mass flow rate (not used).
///
/// @return   void
///
/// @details  Updates the atmosphere for the consumed O2 and produced CO2, H20 and heat based on
///           the number of crew members in each metabolic state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic3::updateFluid(const double dt       __attribute__((unused)),
                                       const double flowrate __attribute__((unused)))
{
    /// - Skip jf flow demand is negligible.
    if (mFlowDemand > DBL_EPSILON) {

        /// - Reset the internal fluid state.
        mInternalFluid->resetState();

        /// - Update output atmosphere constituents using mass rate instead of mass since it is
        ///   only the mass fractions that are of interest.
        mInternalFluid->setMass(mO2,  -mConsumedO2);
        mInternalFluid->setMass(mCO2,  mProducedCO2);
        mInternalFluid->setMass(mH2O,  mProducedH2O);
        if (mNH3 > -1) {
            mInternalFluid->setMass(mNH3,  mProducedNH3);
        }
        if (mCO > -1) {
            mInternalFluid->setMass(mCO,   mProducedCO);
        }
        if (mH2 > -1) {
            mInternalFluid->setMass(mH2,   mProducedH2);
        }
        if (mCH4 > -1) {
            mInternalFluid->setMass(mCH4,  mProducedCH4);
        }

        /// - Update output atmosphere mass, moles & fractions from constituent mass.
        mInternalFluid->updateMass();
        updateTraceCompounds();

        /// - Update output atmosphere temperature.
        mNodes[1]->collectHeatFlux(mProducedHeat);
        mInternalFluid->setTemperature(mNodes[1]->getContent()->getTemperature());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the internal fluid's trace compounds object with all produced trace compounds
///           for transport to the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic3::updateTraceCompounds()
{
    if (GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds()) {
        setTraceMass(tc, mTcCH4O,   mProducedCH4O);
        setTraceMass(tc, mTcC2H6O,  mProducedC2H6O);
        setTraceMass(tc, mTcC4H10O, mProducedC4H10O);
        setTraceMass(tc, mTcCH2O,   mProducedCH2O);
        setTraceMass(tc, mTcC2H4O,  mProducedC2H4O);
        setTraceMass(tc, mTcC6H6,   mProducedC6H6);
        setTraceMass(tc, mTcC7H8,   mProducedC7H8);
        setTraceMass(tc, mTcC8H10,  mProducedC8H10);
        setTraceMass(tc, mTcCH2CL2, mProducedCH2CL2);
        setTraceMass(tc, mTcC3H6O,  mProducedC3H6O);
        setTraceMass(tc, mTcNH3,    mProducedNH3);
        setTraceMass(tc, mTcCO,     mProducedCO);
        setTraceMass(tc, mTcH2,     mProducedH2);
        setTraceMass(tc, mTcCH4,    mProducedCH4);
        tc->updateMoleFractions();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    number     (--)  Number of crew members (> 0) to transition.
/// @param[in]    fromState  (--)  Initial metabolic state.
/// @param[in]    toState    (--)  Final metabolic state.
///
/// @return   void
///
/// @details  Transitions the specified crew members from one metabolic state to another, subject
///           to the constraint that the number of crew members in any state must be non-negative.
///           NO_METABOLIC in the fromState or toState arguments can be used to increase or decrease
///           the total # of crew.  If NO_METABOLIC is given as the fromState, then the total # of
///           crew is increased by number, all in the toState.  Likewise, if NO_METABOLIC is given
///           as the toState, then the total # of crew are decreased by number, all from the
///           fromState.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic3::transition(const double        number,
                                      const MetabolicType fromState,
                                      const MetabolicType toState)
{
    if (number < 0) {
        /// - Do nothing on negative number of crew members.
        GUNNS_WARNING("rejecting request to transition a negative number of crew members.");
    } else {
        /// - Transition as many as requested or are available.
        double n = number;
        if (NO_METABOLIC != fromState) {
            n = fmin(number, mNCrew[fromState]);
            mNCrew[fromState] -= n;
        }
        if (NO_METABOLIC != toState) {
            mNCrew[toState]   += n;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    port  (--)  The port to be assigned.
/// @param[in]    node  (--)  The desired node to assign the port to.
///
/// @returns  bool (--) True if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           a Gunns Fluid Metabolic link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidMetabolic3::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 1 is the vacuum boundary node.
    if ((1 == port) && (node == mNodeList->mNumNodes - 1)) {
        TsHsMsg msg(TS_HS_WARNING, "GUNNS");
        msg << mName << " aborted setting a port: cannot assign port 1 to the boundary node.";
        hsSendMsg(msg);
        result = false;
    }

    /// - Fail if port 0 is not the vacuum boundary node.
    else if ((0 == port) && (node != mNodeList->mNumNodes - 1)) {
        TsHsMsg msg(TS_HS_WARNING, "GUNNS");
        msg << mName << " aborted setting a port: must assign port 0 to the boundary node.";
        hsSendMsg(msg);
        result = false;
    }

    return result;
}
