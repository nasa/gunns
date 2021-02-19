#ifndef GunnsGasTurbine_EXISTS
#define GunnsGasTurbine_EXISTS

/**
@file
@brief    GUNNS Gas Turbine Model declarations

@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_GAS_TURBINE Turbine Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Gas Turbine link model used to model the fluid aspect of a gas turbine.)

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

LIBRARY DEPENDENCY:
- ((GunnsGasTurbine.o))

PROGRAMMERS:
- ((Chris Brownschidle) (CACI) (Install) (2016-08))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "aspects/fluid/potential/GunnsGasFanCurve.hh"
#include "core/GunnsFluidConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Gas Turbine Model Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Gas Turbine
///           link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsGasTurbineConfigData : public GunnsFluidConductorConfigData
{
    public:
        double mReferenceTemp;     /**< (K)              trick_chkpnt_io(**) Temperature for reference performance curve                   */
        double mReferencePress;    /**< (kPa)            trick_chkpnt_io(**) Pressure for reference performance curve                      */
        double mMaxFlowLowSpeed;   /**< (kg/s)           trick_chkpnt_io(**) Low Speed maximum corrected flow rate                         */
        double mMaxFlowHighSpeed;  /**< (kg/s)           trick_chkpnt_io(**) High Speed maximum corrected flow rate                        */
        double mCoeffLowSpeed1;    /**< (--)             trick_chkpnt_io(**) Low Speed curve fit 1st coefficient                           */
        double mCoeffHighSpeed1;   /**< (--)             trick_chkpnt_io(**) High Speed curve fit 1st coefficient                          */
        double mCoeffLowSpeed2;    /**< (--)             trick_chkpnt_io(**) Low Speed curve fit 2nd coefficient                           */
        double mCoeffHighSpeed2;   /**< (--)             trick_chkpnt_io(**) High Speed curve fit 2nd coefficient                          */
        double mEffCoeffLowSpeed0; /**< (--)             trick_chkpnt_io(**) Low Speed Efficiency performance curve 0th-order coefficient  */
        double mEffCoeffLowSpeed1; /**< (--)             trick_chkpnt_io(**) Low Speed Efficiency performance curve 1st-order coefficient  */
        double mEffCoeffLowSpeed2; /**< (--)             trick_chkpnt_io(**) Low Speed Efficiency performance curve 2nd-order coefficient  */
        double mEffCoeffLowSpeed3; /**< (--)             trick_chkpnt_io(**) Low Speed Efficiency performance curve 3rd-order coefficient  */
        double mEffCoeffLowSpeed4; /**< (--)             trick_chkpnt_io(**) Low Speed Efficiency performance curve 4th-order coefficient  */
        double mEffCoeffLowSpeed5; /**< (--)             trick_chkpnt_io(**) Low Speed Efficiency performance curve 5th-order coefficient  */
        double mEffCoeffHighSpeed0;/**< (--)             trick_chkpnt_io(**) High Speed Efficiency performance curve 0th-order coefficient */
        double mEffCoeffHighSpeed1;/**< (--)             trick_chkpnt_io(**) High Speed Efficiency performance curve 1st-order coefficient */
        double mEffCoeffHighSpeed2;/**< (--)             trick_chkpnt_io(**) High Speed Efficiency performance curve 2nd-order coefficient */
        double mEffCoeffHighSpeed3;/**< (--)             trick_chkpnt_io(**) High Speed Efficiency performance curve 3rd-order coefficient */
        double mEffCoeffHighSpeed4;/**< (--)             trick_chkpnt_io(**) High Speed Efficiency performance curve 4th-order coefficient */
        double mEffCoeffHighSpeed5;/**< (--)             trick_chkpnt_io(**) High Speed Efficiency performance curve 5th-order coefficient */
        double mMinEffLimLowSpeed; /**< (--)             trick_chkpnt_io(**) Lower limit on low speed efficiency curve.                    */
        double mMinEffLimHighSpeed;/**< (--)             trick_chkpnt_io(**) Lower limit on high speed efficiency curve.                   */
        double mMaxEffLimLowSpeed; /**< (--)             trick_chkpnt_io(**) Higher limit on low speed efficiency curve.                   */
        double mMaxEffLimHighSpeed;/**< (--)             trick_chkpnt_io(**) Higher limit on high speed efficiency curve.                  */
        double mCorrectedSpeedLow; /**< (revolution/min) trick_chkpnt_io(**) Lowest corrected speed on turbine map.                        */
        double mCorrectedSpeedHigh;/**< (revolution/min) trick_chkpnt_io(**) Highest corrected speed on turbine map.                       */
        /// @details Recommend 1.0, less if more stability is needed.  Should not go below 0.1.
        double mFilterGain;        /**< (--)             trick_chkpnt_io(**) (0-1) Low pass filter gain, for stability                     */
        double mDriveRatio;        /**< (--)             trick_chkpnt_io(**) Gear ratio of motor to impeller speed                         */
        double mThermalLength;     /**< (m)              trick_chkpnt_io(**) Impeller length for thermal convection                        */
        double mThermalDiameter;   /**< (m)              trick_chkpnt_io(**) Impeller inner diameter for thermal convection                */
        double mSurfaceRoughness;  /**< (m)              trick_chkpnt_io(**) Impeller wall surface roughness for thermal convection        */
        /// @}
        /// @brief  Default constructs this Gas Turbine configuration data.
        GunnsGasTurbineConfigData(const std::string& name                 = "",
                                  GunnsNodeList*     nodes                = 0,
                                  const double       maxConductivity      = 0.0,
                                  const double       expansionScaleFactor = 0.0,
                                  const double       referenceTemp        = 0.0,
                                  const double       referencePress       = 0.0,
                                  const double       maxFlowLowSpeed      = 0.0,
                                  const double       maxFlowHighSpeed     = 0.0,
                                  const double       coeffLowSpeed1       = 0.0,
                                  const double       coeffHighSpeed1      = 0.0,
                                  const double       coeffLowSpeed2       = 0.0,
                                  const double       coeffHighSpeed2      = 0.0,
                                  const double       efficiencyCoeffLow0  = 0.0,
                                  const double       efficiencyCoeffLow1  = 0.0,
                                  const double       efficiencyCoeffLow2  = 0.0,
                                  const double       efficiencyCoeffLow3  = 0.0,
                                  const double       efficiencyCoeffLow4  = 0.0,
                                  const double       efficiencyCoeffLow5  = 0.0,
                                  const double       efficiencyCoeffHigh0 = 0.0,
                                  const double       efficiencyCoeffHigh1 = 0.0,
                                  const double       efficiencyCoeffHigh2 = 0.0,
                                  const double       efficiencyCoeffHigh3 = 0.0,
                                  const double       efficiencyCoeffHigh4 = 0.0,
                                  const double       efficiencyCoeffHigh5 = 0.0,
                                  const double       minEffLimitLowSpeed  = 0.0,
                                  const double       minEffLimitHighSpeed = 0.0,
                                  const double       maxEffLimitLowSpeed  = 0.0,
                                  const double       maxEffLimitHighSpeed = 0.0,
                                  const double       correctedSpeedLow    = 0.0,
                                  const double       correctedSpeedHigh   = 0.0,
                                  const double       filterGain           = 1.0,
                                  const double       driveRatio           = 1.0,
                                  const double       thermalLength        = 0.0,
                                  const double       thermalDiameter      = 0.0,
                                  const double       surfaceRoughness     = 0.0);
        /// @brief  Copy constructs this Gas Turbine configuration data.
        GunnsGasTurbineConfigData(const GunnsGasTurbineConfigData& that);
        /// @brief  Default destructs this Gas Turbine configuration data.
        virtual ~GunnsGasTurbineConfigData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsGasTurbineConfigData& operator =(const GunnsGasTurbineConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Gas Turbine Model Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Gas Turbine
///           link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsGasTurbineInputData : public GunnsFluidConductorInputData
{
    public:
        double  mMotorSpeed;      /**< (revolution/min) Initial speed of the motor        */
        double  mWallTemperature; /**< (K)              Initial impeller wall temperature */
        /// @brief  Default constructs this Gas Turbine input data.
        GunnsGasTurbineInputData(const bool   malfBlockageFlag  = false,
                                 const double malfBlockageValue = 0.0,
                                 const double motorSpeed        = 0.0,
                                 const double wallTemperature   = 0.0);
        /// @brief  Copy constructs this Gas Turbine input data.
        GunnsGasTurbineInputData(const GunnsGasTurbineInputData& that);
        /// @brief  Default destructs this Gas Turbine input data.
        virtual ~GunnsGasTurbineInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsGasTurbineInputData& operator =(const GunnsGasTurbineInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gas Turbine Model
///
/// @details  This is a turbine model, intended to model turbines used in gas turbine engines. Its
///           purpose is to model pressure drop, flow rate, and power generation.
///
///           This model relies on user supplied turbine performance maps to model turbine
///           efficiency, and the relationship between flow rate and pressure drop. The efficiency
///           map is a 5th order polynomial, while the relationship between pressure and flow rate
///           is a modified power function. The user must supply two of each of these functions, at
///           two operating shaft speeds. These operating speeds should ideally enclose the expected
///           operating conditions of the turbine. Performance at speeds between these references are
///           calculated using interpolation.
///
///           This model can be used by the GunnsDriveShaftSpotter model to simulate a drive shaft
///           connection between a compressor/fan and turbine.
////////////////////////////////////////////////////////////////////////////////////////////////////

class GunnsGasTurbine : public GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsGasTurbine);
    public:
        /// @brief  Default constructs this Gas Turbine.
        GunnsGasTurbine();
        /// @brief  Default destructs this Gas Turbine.
        virtual ~GunnsGasTurbine();
        /// @brief  Initializes this Gas Turbine with configuration and input data.
        void   initialize(const GunnsGasTurbineConfigData&    configData,
                          const GunnsGasTurbineInputData&     inputData,
                          std::vector<GunnsBasicLink*>&       links,
                          const int                           port0,
                          const int                           port1);
        /// @brief  Returns the heat flux from the fluid to the impeller wall.
        double getWallHeatFlux();
        /// @brief  Returns the fluid torque assisting impeller rotation.
        double getImpellerTorque();
        /// @brief  Returns the impeller speed.
        double getImpellerSpeed();
        /// @brief  Returns the impeller shaft power.
        double getImpellerPower();
        /// @brief  Sets the thermal surface area of this Gas Turbine.
        void   setThermalSurfaceArea(const double value);
        /// @brief  Sets the wall temperature of this Gas Turbine.
        void   setWallTemperature(const double value);
        /// @brief  Sets the motor speed of this Gas Fan.
        void   setMotorSpeed(const double value);

    protected:
        double mReferenceTemp;       /**< (K)              trick_chkpnt_io(**) Temperature for reference performance curve             */
        double mReferencePress;      /**< (kPa)            trick_chkpnt_io(**) Pressure for reference performance curve                */
        double mLowSpeedMaxFlow;     /**< (kg/s)           trick_chkpnt_io(**) Low Speed maximum corrected flow rate                   */
        double mHighSpeedMaxFlow;    /**< (kg/s)           trick_chkpnt_io(**) High Speed maximum corrected flow rate                  */
        double mCoeffLowSpeed1;      /**< (--)             trick_chkpnt_io(**) Low Speed curve fit 1st coefficient                     */
        double mCoeffLowSpeed2;      /**< (--)             trick_chkpnt_io(**) Low Speed curve fit 2nd coefficient                     */
        double mCoeffHighSpeed1;     /**< (--)             trick_chkpnt_io(**) High Speed curve fit 1st coefficient                    */
        double mCoeffHighSpeed2;     /**< (--)             trick_chkpnt_io(**) High Speed curve fit 2nd coefficient                    */
        double mCorrectedSpeedLow;   /**< (revolution/min) trick_chkpnt_io(**) Lowest corrected speed on turbine map.                  */
        double mCorrectedSpeedHigh;  /**< (revolution/min) trick_chkpnt_io(**) Highest corrected speed on turbine map.                 */
        double mEffCoeffLowSpeed[6]; /**< (--)             trick_chkpnt_io(**) Low speed reference efficiency curve coefficients       */
        double mEffCoeffHighSpeed[6];/**< (--)             trick_chkpnt_io(**) High speed reference efficiency curve coefficients      */
        double mMinEffLimLowSpeed;   /**< (--)             trick_chkpnt_io(**) Lower limit on Low speed efficiency curve.              */
        double mMinEffLimHighSpeed;  /**< (--)             trick_chkpnt_io(**) Lower limit on high speed efficiency curve.             */
        double mMaxEffLimLowSpeed;   /**< (--)             trick_chkpnt_io(**) Higher limit on Low speed efficiency curve.             */
        double mMaxEffLimHighSpeed;  /**< (--)             trick_chkpnt_io(**) Higher limit on high speed efficiency curve.            */
        double mDriveRatio;          /**< (--)             trick_chkpnt_io(**) Gear ratio of motor to impeller speed                   */
        double mFilterGain;          /**< (--)             trick_chkpnt_io(**) Low pass filter gain to ensure stability                */
        double mThermalDiameter;     /**< (m)              trick_chkpnt_io(**) Impeller inner diameter for thermal convection          */
        double mThermalSurfaceArea;  /**< (m2)             trick_chkpnt_io(**) Impeller surface area for thermal convection            */
        double mThermalROverD;       /**< (--)             trick_chkpnt_io(**) Impeller surface roughness over diameter for convection */
        double mMotorSpeed;          /**< (revolution/min)                     Motor rotational speed input from simbus                */
        double mWallTemperature;     /**< (K)                                  Impeller wall temperature input from simbus             */
        GunnsGasFanCurve mCurve;     /**< (--)             trick_chkpnt_io(**) Polynomial used to evaluate efficiency curve            */
        double mImpellerSpeed;       /**< (revolution/min) trick_chkpnt_io(**) Impeller rotational speed                               */
        double mPressureRatio;       /**< (--)             trick_chkpnt_io(**) Pressure ratio across turbine (Pressure in/Pressure out)*/
        double mPressureDrop;        /**< (kPa)            trick_chkpnt_io(**) Pressure drop across the turbine.                       */
        double mEfficiency;          /**< (--)             trick_chkpnt_io(**) Turbine efficiency                                      */
        double mWallHeatFlux;        /**< (W)                                  Heat flux from fluid to wall output to simbus           */
        double mImpellerTorque;      /**< (N*m)                                Impeller fluid torque opposing rotation output to simbus*/
        double mImpellerPower;       /**< (W)              trick_chkpnt_io(**) Power imparted to the shaft by the fluid                */
        double mPredictedFlowRate;   /**< (kg/s)           trick_chkpnt_io(**) Predicted flow rate based on turbine map and press ratio*/
        /// @brief  Validates the initialization of this Gas Turbine.
        void          validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void  restartModel();
        /// @brief  Updates the state of this Gas Turbine.
        virtual void  updateState(const double dt);
        /// @brief  Updates the internal fluid of this Gas Turbine.
        virtual void  updateFluid(const double dt, const double flowRate);
        /// @brief  Computes the predicted flow rate across the turbine.
        virtual void  computeFlowRate();
        /// @brief  Perform linear interpolation
        double        interpolate(double y1, double y2, double fraction);

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsGasTurbine(const GunnsGasTurbine&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsGasTurbine& operator =(const GunnsGasTurbine&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) The heat flux from the fluid to the impeller wall.
///
/// @details  Returns the heat flux from the fluid to the impeller wall.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsGasTurbine::getWallHeatFlux()
{
    return mWallHeatFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (N*m) The fluid torque producing impeller rotation.
///
/// @details  Returns the fluid torque producing impeller rotation.  This is signed relative to the
///           normal rotational direction of the impeller, so will usually be positive.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsGasTurbine::getImpellerTorque()
{
    return mImpellerTorque;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (revolution/min) The impeller rotational speed.
///
/// @details  Returns the impeller rotational speed.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsGasTurbine::getImpellerSpeed()
{
    return mImpellerSpeed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) The power imparted to the shaft by the fluid.
///
/// @details  Returns the power imparted to the shaft by the fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsGasTurbine::getImpellerPower()
{
    return mImpellerPower;
}

#endif
