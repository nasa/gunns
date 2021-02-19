#ifndef GunnsGasCooledTurbine_EXISTS
#define GunnsGasCooledTurbine_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_GAS_COOLED_TURBINE Cooled Turbine Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Gas Cooled Turbine link model used to model the fluid aspect of an actively
   cooled gas turbine.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsGasTurbine.o))

PROGRAMMERS:
- ((Chris Brownschidle) (CACI) (Install) (2016-09))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsFluidPotential.hh"
#include "aspects/fluid/conductor/GunnsGasTurbine.hh"
#include "aspects/fluid/conductor/GunnsFluidHeatExchanger.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Gas Cooled Turbine Model Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Gas Cooled
///           Turbine link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsGasCooledTurbineConfigData : public GunnsFluidLinkConfigData
{
public:
    double mHXMaxConductivity;
    double mHXExpansionFactor;
    int    mNumSegs;           /**< (--)      trick_chkpnt_io(**) Number of segments for this Heat Exchanger */
    double mTurbMaxConductivity;
    double mTurbExpansionFactor;
    double mReferenceTemp;     /**< (K)       trick_chkpnt_io(**) Temperature for reference performance curve                   */
    double mReferencePress;    /**< (kPa)     trick_chkpnt_io(**) Pressure for reference performance curve                      */
    double mMaxFlowLowSpeed;   /**< (kg/s)    trick_chkpnt_io(**) Low Speed maximum corrected flow rate                         */
    double mMaxFlowHighSpeed;  /**< (kg/s)    trick_chkpnt_io(**) High Speed maximum corrected flow rate                        */
    double mCoeffLowSpeed1;    /**< (--)      trick_chkpnt_io(**) Low Speed curve fit 1st coefficient                           */
    double mCoeffHighSpeed1;   /**< (--)      trick_chkpnt_io(**) High Speed curve fit 1st coefficient                          */
    double mCoeffLowSpeed2;    /**< (--)      trick_chkpnt_io(**) Low Speed curve fit 2nd coefficient                           */
    double mCoeffHighSpeed2;   /**< (--)      trick_chkpnt_io(**) High Speed curve fit 2nd coefficient                          */
    double mEffCoeffLowSpeed0; /**< (--)      trick_chkpnt_io(**) Low Speed Efficiency performance curve 0th-order coefficient  */
    double mEffCoeffLowSpeed1; /**< (--)      trick_chkpnt_io(**) Low Speed Efficiency performance curve 1st-order coefficient  */
    double mEffCoeffLowSpeed2; /**< (--)      trick_chkpnt_io(**) Low Speed Efficiency performance curve 2nd-order coefficient  */
    double mEffCoeffLowSpeed3; /**< (--)      trick_chkpnt_io(**) Low Speed Efficiency performance curve 3rd-order coefficient  */
    double mEffCoeffLowSpeed4; /**< (--)      trick_chkpnt_io(**) Low Speed Efficiency performance curve 4th-order coefficient  */
    double mEffCoeffLowSpeed5; /**< (--)      trick_chkpnt_io(**) Low Speed Efficiency performance curve 5th-order coefficient  */
    double mEffCoeffHighSpeed0;/**< (--)      trick_chkpnt_io(**) High Speed Efficiency performance curve 0th-order coefficient */
    double mEffCoeffHighSpeed1;/**< (--)      trick_chkpnt_io(**) High Speed Efficiency performance curve 1st-order coefficient */
    double mEffCoeffHighSpeed2;/**< (--)      trick_chkpnt_io(**) High Speed Efficiency performance curve 2nd-order coefficient */
    double mEffCoeffHighSpeed3;/**< (--)      trick_chkpnt_io(**) High Speed Efficiency performance curve 3rd-order coefficient */
    double mEffCoeffHighSpeed4;/**< (--)      trick_chkpnt_io(**) High Speed Efficiency performance curve 4th-order coefficient */
    double mEffCoeffHighSpeed5;/**< (--)      trick_chkpnt_io(**) High Speed Efficiency performance curve 5th-order coefficient */
    double mMinEffLimLowSpeed; /**< (--)      trick_chkpnt_io(**) Lower limit on low speed efficiency curve.                    */
    double mMinEffLimHighSpeed;/**< (--)      trick_chkpnt_io(**) Lower limit on high speed efficiency curve.                   */
    double mMaxEffLimLowSpeed; /**< (--)      trick_chkpnt_io(**) Higher limit on low speed efficiency curve.                   */
    double mMaxEffLimHighSpeed;/**< (--)      trick_chkpnt_io(**) Higher limit on high speed efficiency curve.                  */
    double mCorrectedSpeedLow; /**< (rev/min) trick_chkpnt_io(**) Lowest corrected speed on turbine map.                        */
    double mCorrectedSpeedHigh;/**< (rev/min) trick_chkpnt_io(**) Highest corrected speed on turbine map.                       */
    /// @details Recommend 0.5, less if more stability is needed.  Should not go below 0.1.
    double mFilterGain;        /**< (--)      trick_chkpnt_io(**) (0-1) Low pass filter gain, for stability                     */
    double mDriveRatio;        /**< (--)      trick_chkpnt_io(**) Gear ratio of motor to impeller speed                         */
    double mThermalLength;     /**< (m)       trick_chkpnt_io(**) Impeller length for thermal convection                        */
    double mThermalDiameter;   /**< (m)       trick_chkpnt_io(**) Impeller inner diameter for thermal convection                */
    double mSurfaceRoughness;  /**< (m)       trick_chkpnt_io(**) Impeller wall surface roughness for thermal convection        */
    /// @}
    /// @brief  Default constructs this Gas Cooled Turbine configuration data.
    GunnsGasCooledTurbineConfigData(const std::string& name                     = "",
                                    GunnsNodeList*     nodes                    = 0,
                                    const double       HXMaxConductivity        = 0.0,
                                    const double       HXExpansionScaleFactor   = 0.0,
                                    const int          HXNumSegs                = 0,
                                    const double       TurbMaxConductivity      = 0.0,
                                    const double       TurbexpansionScaleFactor = 0.0,
                                    const double       referenceTemp            = 0.0,
                                    const double       referencePress           = 0.0,
                                    const double       MaxFlowLowSpeed          = 0.0,
                                    const double       MaxFlowHighSpeed         = 0.0,
                                    const double       coeffLowSpeed1           = 0.0,
                                    const double       coeffHighSpeed1          = 0.0,
                                    const double       coeffLowSpeed2           = 0.0,
                                    const double       coeffHighSpeed2          = 0.0,
                                    const double       efficiencyCoeffLow0      = 0.0,
                                    const double       efficiencyCoeffLow1      = 0.0,
                                    const double       efficiencyCoeffLow2      = 0.0,
                                    const double       efficiencyCoeffLow3      = 0.0,
                                    const double       efficiencyCoeffLow4      = 0.0,
                                    const double       efficiencyCoeffLow5      = 0.0,
                                    const double       efficiencyCoeffHigh0     = 0.0,
                                    const double       efficiencyCoeffHigh1     = 0.0,
                                    const double       efficiencyCoeffHigh2     = 0.0,
                                    const double       efficiencyCoeffHigh3     = 0.0,
                                    const double       efficiencyCoeffHigh4     = 0.0,
                                    const double       efficiencyCoeffHigh5     = 0.0,
                                    const double       minEffLimitLowSpeed      = 0.0,
                                    const double       minEffLimitHighSpeed     = 0.0,
                                    const double       maxEffLimitLowSpeed      = 0.0,
                                    const double       maxEffLimitHighSpeed     = 0.0,
                                    const double       correctedSpeedLow        = 0.0,
                                    const double       correctedSpeedHigh       = 0.0,
                                    const double       filterGain               = 1.0,
                                    const double       driveRatio               = 1.0,
                                    const double       thermalLength            = 0.0,
                                    const double       thermalDiameter          = 0.0,
                                    const double       surfaceRoughness         = 0.0);
    /// @brief  Copy constructs this Gas Cooled Turbine configuration data.
    GunnsGasCooledTurbineConfigData(const GunnsGasCooledTurbineConfigData& that);
    /// @brief  Default destructs this Gas Cooled Turbine configuration data.
    virtual ~GunnsGasCooledTurbineConfigData();
protected:
private:
    ////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Assignment operator unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////
    GunnsGasCooledTurbineConfigData& operator =(const GunnsGasCooledTurbineConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Gas Cooled Turbine Model Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Gas Cooled
///           Turbine link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////

class GunnsGasCooledTurbineInputData : public GunnsFluidLinkInputData
{
public:
    bool    mTurbMalfFlag;
    double  mTurbMalfValue;
    bool    mHXMalfFlag;
    double  mHXMalfValue;
    double  mMotorSpeed;                 /**< (rev/min) Initial speed of the motor        */
    double  mTurbWallTemperature;        /**< (K)       Initial impeller wall temperature */
    double  mHeatTransferCoefficient;    /**< (W/K) Default overall Heat Transfer coefficient */
    double  mInitialSegmentTemperature;  /**< (K)   Default segment wall temperature          */
    double* mSegmentHtcOverrides;        /**< (W/K) Segment heat transfer coeffs (optional)   */
    double  mTemperatureOverride;        /**< (K)   Fluid temperature override (optional)     */
    /// @brief  Default constructs this Gas Cooled Turbine input data.
    GunnsGasCooledTurbineInputData(const bool   TurbMalfBlockageFlag      = false,
                                   const double TurbMalfBlockageValue     = 0.0,
                                   const bool   HXMalfBlockageFlag        = false,
                                   const double HXMalfBlockageValue       = 0.0,
                                   const double motorSpeed                = 0.0,
                                   const double TurbWallTemperature       = 0.0,
                                   const double heatTransferCoefficient   = 0.0,
                                   const double initialSegmentTemperature = 0.0);
    /// @brief  Copy constructs this Gas Cooled Turbine input data.
    GunnsGasCooledTurbineInputData(const GunnsGasCooledTurbineInputData& that);
    /// @brief  Default destructs this Gas Cooled Turbine input data.
    virtual ~GunnsGasCooledTurbineInputData();
protected:
private:
    ////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Assignment operator unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////
    GunnsGasCooledTurbineInputData& operator =(const GunnsGasCooledTurbineInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gas Cooled Turbine Model
///
/// @details
////////////////////////////////////////////////////////////////////////////////////////////////////

class GunnsGasCooledTurbine : public GunnsFluidLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsGasCooledTurbine);
    public:
        /// @brief  Default constructs this Gas Cooled Turbine.
        GunnsGasCooledTurbine();
        /// @brief  Default destructs this Gas Cooled Turbine.
        virtual ~GunnsGasCooledTurbine();
        /// @brief  Initializes this Gas Cooled Turbine with configuration and input data.
        void initialize(const GunnsGasCooledTurbineConfigData&    configData,
                        const GunnsGasCooledTurbineInputData&     inputData,
                        std::vector<GunnsBasicLink*>&             links,
                        const int                                 port0,
                        const int                                 port1,
                        const int                                 port2,
                        const int                                 port3);
    protected:
        GunnsFluidHeatExchanger mHeatExchanger;
        GunnsGasTurbine         mTurbine;

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsGasCooledTurbine(const GunnsGasCooledTurbine&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsGasCooledTurbine& operator =(const GunnsGasCooledTurbine&);
};
/// @}
