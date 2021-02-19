/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Classes for the GUNNS Gas Cooled Turbine Model.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((software/exceptions/TsNumericalException.o)
    (aspects/fluid/conductor/GunnsGasCooledTurbine.o))

PROGRAMMERS:
   ((Chris Brownschidle) (CACI) (Install) (2016-09))
***************************************************************************************************/
#include "../conductor/GunnsGasCooledTurbine.hh"

#include <complex>
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsNumericalException.hh"
#include "math/MsMath.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name                 (--)      Name of object.
/// @param[in,out] nodes                (--)      Pointer to nodes.
/// @param[in]     maxConductivity      (m2)      Max conductivity.
/// @param[in]     expansionScaleFactor (--)      Scale factor for isentropic gas cooling.
/// @param[in]     referenceTemp        (K)       Reference fluid temperature for corrected mass flow
/// @param[in]     referencePress       (--)      Reference fluid pressure for corrected mass flow
/// @param[in]     MaxFlowLowSpeed      (kg/s)    Low Speed maximum corrected flow rate
/// @param[in]     MaxFlowHighSpeed     (kg/s)    High Speed maximum corrected flow rate
/// @param[in]     coeffLowSpeed1       (--)      Low speed 1st curve fit coefficient
/// @param[in]     coeffHighSpeed1      (--)      High speed 1st curve fit coefficient
/// @param[in]     coeffLowSpeed2       (--)      Low speed 2nd curve fit coefficient
/// @param[in]     coeffHighSpeed2      (--)      High speed 2nd curve fit coefficient
/// @param[in]     efficiencyCoeffLow0  (--)      Low Speed Reference efficiency curve 0th-order coefficient
/// @param[in]     efficiencyCoeffLow1  (--)      Low Speed Reference efficiency curve 1st-order coefficient
/// @param[in]     efficiencyCoeffLow2  (--)      Low Speed Reference efficiency curve 2nd-order coefficient
/// @param[in]     efficiencyCoeffLow3  (--)      Low Speed Reference efficiency curve 3rd-order coefficient
/// @param[in]     efficiencyCoeffLow4  (--)      Low Speed Reference efficiency curve 4th-order coefficient
/// @param[in]     efficiencyCoeffLow5  (--)      Low Speed Reference efficiency curve 5th-order coefficient
/// @param[in]     efficiencyCoeffHigh0 (--)      High Speed Reference efficiency curve 0th-order coefficient
/// @param[in]     efficiencyCoeffHigh1 (--)      High Speed Reference efficiency curve 1st-order coefficient
/// @param[in]     efficiencyCoeffHigh2 (--)      High Speed Reference efficiency curve 2nd-order coefficient
/// @param[in]     efficiencyCoeffHigh3 (--)      High Speed Reference efficiency curve 3rd-order coefficient
/// @param[in]     efficiencyCoeffHigh4 (--)      High Speed Reference efficiency curve 4th-order coefficient
/// @param[in]     efficiencyCoeffHigh5 (--)      High Speed Reference efficiency curve 5th-order coefficient
/// @param[in]     minEffLimitLowSpeed  (--)      Min PR that the low speed efficiency curve can be evaluated at
/// @param[in]     minEffLimitHighSpeed (--)      Min PR that the high speed efficiency curve can be evaluated at
/// @param[in]     maxEffLimitLowSpeed  (--)      Max PR that the low speed efficiency curve can be evaluated at
/// @param[in]     maxEffLimitHighSpeed (--)      Max PR that the high speed efficiency curve can be evaluated at
/// @param[in]     correctedSpeedLow    (rev/min) Lowest corrected speed on turbine map.
/// @param[in]     correctedSpeedHigh   (rev/min) Highest corrected speed on turbine map.
/// @param[in]     filterGain           (--)      Low pass filter gain to ensure stability.
/// @param[in]     driveRatio           (--)      Gear ratio of motor to impeller speed.
/// @param[in]     thermalLength        (m)       Impeller length for thermal convection.
/// @param[in]     thermalDiameter      (m)       Impeller inner diameter for thermal convection.
/// @param[in]     surfaceRoughness     (m)       Impeller wall surface roughness for convection.
///
/// @details  Default constructs this GUNNS Gas Turbine link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasCooledTurbineConfigData::GunnsGasCooledTurbineConfigData(const std::string& name,
                                                     GunnsNodeList*     nodes,
                                                     const double       HXMaxConductivity,
                                                     const double       HXExpansionScaleFactor,
                                                     const int          HXNumSegs,
                                                     const double       TurbMaxConductivity,
                                                     const double       TurbExpansionScaleFactor,
                                                     const double       referenceTemp,
                                                     const double       referencePress,
                                                     const double       MaxFlowLowSpeed,
                                                     const double       MaxFlowHighSpeed,
                                                     const double       coeffLowSpeed1,
                                                     const double       coeffHighSpeed1,
                                                     const double       coeffLowSpeed2,
                                                     const double       coeffHighSpeed2,
                                                     const double       efficiencyCoeffLow0,
                                                     const double       efficiencyCoeffLow1,
                                                     const double       efficiencyCoeffLow2,
                                                     const double       efficiencyCoeffLow3,
                                                     const double       efficiencyCoeffLow4,
                                                     const double       efficiencyCoeffLow5,
                                                     const double       efficiencyCoeffHigh0,
                                                     const double       efficiencyCoeffHigh1,
                                                     const double       efficiencyCoeffHigh2,
                                                     const double       efficiencyCoeffHigh3,
                                                     const double       efficiencyCoeffHigh4,
                                                     const double       efficiencyCoeffHigh5,
                                                     const double       minEffLimitLowSpeed,
                                                     const double       minEffLimitHighSpeed,
                                                     const double       maxEffLimitLowSpeed,
                                                     const double       maxEffLimitHighSpeed,
                                                     const double       correctedSpeedLow,
                                                     const double       correctedSpeedHigh,
                                                     const double       filterGain,
                                                     const double       driveRatio,
                                                     const double       thermalLength,
                                                     const double       thermalDiameter,
                                                     const double       surfaceRoughness)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mHXMaxConductivity  (HXMaxConductivity),
    mHXExpansionFactor  (HXExpansionScaleFactor),
    mNumSegs            (HXNumSegs),
    mTurbMaxConductivity(TurbMaxConductivity),
    mTurbExpansionFactor(TurbExpansionScaleFactor),
    mReferenceTemp      (referenceTemp),
    mReferencePress     (referencePress),
    mMaxFlowLowSpeed    (MaxFlowLowSpeed),
    mMaxFlowHighSpeed   (MaxFlowHighSpeed),
    mCoeffLowSpeed1     (coeffLowSpeed1),
    mCoeffHighSpeed1    (coeffHighSpeed1),
    mCoeffLowSpeed2     (coeffLowSpeed2),
    mCoeffHighSpeed2    (coeffHighSpeed2),
    mEffCoeffLowSpeed0  (efficiencyCoeffLow0),
    mEffCoeffLowSpeed1  (efficiencyCoeffLow1),
    mEffCoeffLowSpeed2  (efficiencyCoeffLow2),
    mEffCoeffLowSpeed3  (efficiencyCoeffLow3),
    mEffCoeffLowSpeed4  (efficiencyCoeffLow4),
    mEffCoeffLowSpeed5  (efficiencyCoeffLow5),
    mEffCoeffHighSpeed0 (efficiencyCoeffHigh0),
    mEffCoeffHighSpeed1 (efficiencyCoeffHigh1),
    mEffCoeffHighSpeed2 (efficiencyCoeffHigh2),
    mEffCoeffHighSpeed3 (efficiencyCoeffHigh3),
    mEffCoeffHighSpeed4 (efficiencyCoeffHigh4),
    mEffCoeffHighSpeed5 (efficiencyCoeffHigh5),
    mMinEffLimLowSpeed  (minEffLimitLowSpeed),
    mMinEffLimHighSpeed (minEffLimitHighSpeed),
    mMaxEffLimLowSpeed  (maxEffLimitLowSpeed),
    mMaxEffLimHighSpeed (maxEffLimitHighSpeed),
    mCorrectedSpeedLow  (correctedSpeedLow),
    mCorrectedSpeedHigh (correctedSpeedHigh),
    mFilterGain         (filterGain),
    mDriveRatio         (driveRatio),
    mThermalLength      (thermalLength),
    mThermalDiameter    (thermalDiameter),
    mSurfaceRoughness   (surfaceRoughness)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this GUNNS Gas Turbine link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasCooledTurbineConfigData::GunnsGasCooledTurbineConfigData(
                                                        const GunnsGasCooledTurbineConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mHXMaxConductivity  (that.mHXMaxConductivity),
    mHXExpansionFactor  (that.mHXExpansionFactor),
    mNumSegs            (that.mNumSegs),
    mTurbMaxConductivity(that.mTurbMaxConductivity),
    mTurbExpansionFactor(that.mTurbExpansionFactor),
    mReferenceTemp      (that.mReferenceTemp),
    mReferencePress     (that.mReferencePress),
    mMaxFlowLowSpeed    (that.mMaxFlowLowSpeed),
    mMaxFlowHighSpeed   (that.mMaxFlowHighSpeed),
    mCoeffLowSpeed1     (that.mCoeffLowSpeed1),
    mCoeffHighSpeed1    (that.mCoeffHighSpeed1),
    mCoeffLowSpeed2     (that.mCoeffLowSpeed2),
    mCoeffHighSpeed2    (that.mCoeffHighSpeed2),
    mEffCoeffLowSpeed0  (that.mEffCoeffLowSpeed0),
    mEffCoeffLowSpeed1  (that.mEffCoeffLowSpeed1),
    mEffCoeffLowSpeed2  (that.mEffCoeffLowSpeed2),
    mEffCoeffLowSpeed3  (that.mEffCoeffLowSpeed3),
    mEffCoeffLowSpeed4  (that.mEffCoeffLowSpeed4),
    mEffCoeffLowSpeed5  (that.mEffCoeffLowSpeed5),
    mEffCoeffHighSpeed0 (that.mEffCoeffHighSpeed0),
    mEffCoeffHighSpeed1 (that.mEffCoeffHighSpeed1),
    mEffCoeffHighSpeed2 (that.mEffCoeffHighSpeed2),
    mEffCoeffHighSpeed3 (that.mEffCoeffHighSpeed3),
    mEffCoeffHighSpeed4 (that.mEffCoeffHighSpeed4),
    mEffCoeffHighSpeed5 (that.mEffCoeffHighSpeed5),
    mMinEffLimLowSpeed  (that.mMinEffLimLowSpeed),
    mMinEffLimHighSpeed (that.mMinEffLimHighSpeed),
    mMaxEffLimLowSpeed  (that.mMaxEffLimLowSpeed),
    mMaxEffLimHighSpeed (that.mMaxEffLimHighSpeed),
    mCorrectedSpeedLow  (that.mCorrectedSpeedLow),
    mCorrectedSpeedHigh (that.mCorrectedSpeedHigh),
    mFilterGain         (that.mFilterGain),
    mDriveRatio         (that.mDriveRatio),
    mThermalLength      (that.mThermalLength),
    mThermalDiameter    (that.mThermalDiameter),
    mSurfaceRoughness   (that.mSurfaceRoughness)
{
    // nothing to do
}////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Gas Turbine link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasCooledTurbineConfigData::~GunnsGasCooledTurbineConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--)      Blockage malfunction flag.
/// @param[in] malfBlockageValue (--)      Blockage malfunction fractional value (0-1).
/// @param[in] sourcePressure    (kPa)     Initial pressure drop of the link.
/// @param[in] motorSpeed        (rev/min) Initial speed of the motor.
/// @param[in] wallTemperature   (K)       Initial impeller wall temperature.
///
/// @details  Default constructs this GUNNS Gas Turbine link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasCooledTurbineInputData::GunnsGasCooledTurbineInputData(const bool   TurbMalfBlockageFlag,
                                                               const double TurbMalfBlockageValue,
                                                               const bool   HXMalfBlockageFlag,
                                                               const double HXMalfBlockageValue,
                                                               const double motorSpeed,
                                                               const double TurbWallTemperature,
                                                               const double heatTransferCoefficient,
                                                               const double initialSegmentTemperature)
    :
    GunnsFluidLinkInputData     (false, 0.0),
    mTurbMalfFlag               (TurbMalfBlockageFlag),
    mTurbMalfValue              (TurbMalfBlockageValue),
    mHXMalfFlag                 (HXMalfBlockageFlag),
    mHXMalfValue                (HXMalfBlockageValue),
    mMotorSpeed                 (motorSpeed),
    mTurbWallTemperature        (TurbWallTemperature),
    mHeatTransferCoefficient    (heatTransferCoefficient),
    mInitialSegmentTemperature  (initialSegmentTemperature),
    mSegmentHtcOverrides        (0),
    mTemperatureOverride        (0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this GUNNS Gas Turbine link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasCooledTurbineInputData::GunnsGasCooledTurbineInputData(const GunnsGasCooledTurbineInputData& that)
    :
    GunnsFluidLinkInputData     (that),
    mTurbMalfFlag               (that.mTurbMalfFlag),
    mTurbMalfValue              (that.mTurbMalfValue),
    mHXMalfFlag                 (that.mHXMalfFlag),
    mHXMalfValue                (that.mHXMalfValue),
    mMotorSpeed                 (that.mMotorSpeed),
    mTurbWallTemperature        (that.mTurbWallTemperature),
    mHeatTransferCoefficient    (that.mHeatTransferCoefficient),
    mInitialSegmentTemperature  (that.mInitialSegmentTemperature),
    mSegmentHtcOverrides        (that.mSegmentHtcOverrides),
    mTemperatureOverride        (that.mTemperatureOverride)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Gas Turbine link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasCooledTurbineInputData::~GunnsGasCooledTurbineInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Gas Turbine link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasCooledTurbine::GunnsGasCooledTurbine()
    :
    GunnsGasTurbine(),
    mHeatExchanger()
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Gas Turbine link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasTurbine::~GunnsGasTurbine()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData (--) Configuration data.
/// @param[in]     inputData  (--) Input data.
/// @param[in,out] links      (--) Link vector.
/// @param[in]     port0      (--) Nominal inlet port map index.
/// @param[in]     port1      (--) Nominal outlet port map index.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Gas Turbine link model with configuration and input data. Some
///           validation of config data is needed before state data can be derived from it.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasCooledTurbine::initialize(const GunnsGasCooledTurbineConfigData&  configData,
                                 const GunnsGasCooledTurbineInputData&   inputData,
                                 std::vector<GunnsBasicLink*>&     links,
                                 const int                         port0,
                                 const int                         port1,
                                 const int                         port2,
                                 const int                         port3)
{
    /// - Initialize parent
    int ports[4] = {port0, port1, port2, port3};
    GunnsFluidLink::initialize(configData, inputData, links, ports);

    /// - Initialize member objects
    mTurbine.initialize(GunnsGasTurbineConfigData(configData.mName + ".mTurbine",
                                                  configData.mNodeList,
                                                  configData.mTurbMaxConductivity,
                                                  configData.mTurbExpansionFactor,
                                                  configData.mReferenceTemp,
                                                  configData.mReferencePress,
                                                  configData.mMaxFlowLowSpeed,
                                                  configData.mMaxFlowHighSpeed,
                                                  configData.mCoeffLowSpeed1,
                                                  configData.mCoeffHighSpeed1,
                                                  configData.mCoeffLowSpeed2,
                                                  configData.mCoeffHighSpeed2,
                                                  configData.mEffCoeffLowSpeed0,
                                                  configData.mEffCoeffLowSpeed1,
                                                  configData.mEffCoeffLowSpeed2,
                                                  configData.mEffCoeffLowSpeed3,
                                                  configData.mEffCoeffLowSpeed4,
                                                  configData.mEffCoeffLowSpeed5,
                                                  configData.mEffCoeffHighSpeed0,
                                                  configData.mEffCoeffHighSpeed1,
                                                  configData.mEffCoeffHighSpeed2,
                                                  configData.mEffCoeffHighSpeed3,
                                                  configData.mEffCoeffHighSpeed4,
                                                  configData.mEffCoeffHighSpeed5,
                                                  configData.mMinEffLimLowSpeed,
                                                  configData.mMinEffLimHighSpeed,
                                                  configData.mMaxEffLimLowSpeed,
                                                  configData.mMaxEffLimHighSpeed,
                                                  configData.mCorrectedSpeedLow,
                                                  configData.mCorrectedSpeedHigh,
                                                  configData.mFilterGain,
                                                  configData.mDriveRatio,
                                                  configData.mThermalLength,
                                                  configData.mThermalDiameter,
                                                  configData.mSurfaceRoughness),
                        GunnsGasTurbineInputData( inputData.mMalfBlockageFlag,
                                                  inputData.mMalfBlockageValue,
                                                  inputData.mMotorSpeed,
                                                  inputData.mTurbWallTemperature),
                        links,
                        port0,
                        port1);

    mHeatExchanger.initialize(GunnsFluidHeatExchangerConfigData(configData.mName + ".mHX",
                                                                configData.mNodeList,
                                                                configData.mHXMaxConductivity,
                                                                configData.mHXExpansionFactor,
                                                                configData.mNumSegs),
                               GunnsFluidHeatExchangerInputData(inputData.mHXMalfFlag,
                                                                inputData.mHXMalfValue,
                                                                inputData.mHeatTransferCoefficient,
                                                                inputData.mInitialSegmentTemperature),
                               links,
                               port2,
                               port3);
}
