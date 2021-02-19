#ifndef GunnsGasFan_EXISTS
#define GunnsGasFan_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_POTENTIAL_GAS_FAN Fan Model
@ingroup   TSM_GUNNS_FLUID_POTENTIAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Gas Fan link model used to model the fluid aspect of a gas fan.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsGasFan.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Install) (2012-07))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsFluidPotential.hh"
#include "aspects/fluid/potential/GunnsGasFanCurve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Gas Fan Model Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Gas Fan
///           link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsGasFanConfigData : public GunnsFluidPotentialConfigData
{
    public:
        double mReferenceDensity;  /**< (kg/m3)          trick_chkpnt_io(**) Fluid inlet density for the reference performance curve */
        double mReferenceSpeed;    /**< (revolution/min) trick_chkpnt_io(**) Impeller speed for the reference performance curve      */
        double mReferenceCoeff0;   /**< (kPa)            trick_chkpnt_io(**) Reference performance curve 0th-order coefficient.
                                                                             When all other coefficients are set to zero, this defines P_BEP */
        double mReferenceCoeff1;   /**< (kPa)            trick_chkpnt_io(**) Reference performance curve 1st-order coefficient       */
        double mReferenceCoeff2;   /**< (kPa)            trick_chkpnt_io(**) Reference performance curve 2nd-order coefficient       */
        double mReferenceCoeff3;   /**< (kPa)            trick_chkpnt_io(**) Reference performance curve 3rd-order coefficient       */
        double mReferenceCoeff4;   /**< (kPa)            trick_chkpnt_io(**) Reference performance curve 4th-order coefficient       */
        double mReferenceCoeff5;   /**< (kPa)            trick_chkpnt_io(**) Reference performance curve 5th-order coefficient       */
        /// @name    Best Efficiency Point.
        /// @{
        /// @details These BEP terms are optional.  When flow rate and efficiency are used (both
        ///          non-zero), they model more realistic shaft power and torque load for integration
        ///          with a motor model. They should coincide with the other reference performance
        ///          curve and parameters.
        double mBestEfficiency;    /**< (--)             trick_chkpnt_io(**) (0-1) Efficiency at best efficiency point at reference  */
        double mReferenceQBep;     /**< (m3/s)           trick_chkpnt_io(**) Volume flow rate at best efficiency point at reference  */
        /// @}
        /// @details Recommend 0.5, less if more stability is needed.  Should not go below 0.1.
        double mFilterGain;        /**< (--)             trick_chkpnt_io(**) (0-1) Flow filter gain for system curve estimate        */
        double mDriveRatio;        /**< (--)             trick_chkpnt_io(**) Gear ratio of motor to impeller speed                   */
        double mThermalLength;     /**< (m)              trick_chkpnt_io(**) Impeller length for thermal convection                  */
        double mThermalDiameter;   /**< (m)              trick_chkpnt_io(**) Impeller inner diameter for thermal convection          */
        double mSurfaceRoughness;  /**< (m)              trick_chkpnt_io(**) Impeller wall surface roughness for thermal convection  */
        bool   mCheckValveActive;  /**< (--)             trick_chkpnt_io(**) Flag indicating check valve functionality is active     */
        /// @}
        /// @brief  Default constructs this Gas Fan configuration data.
        //TODO add Bep terms to the constructor arg list and update all drawings
        GunnsGasFanConfigData(const std::string& name                 = "",
                              GunnsNodeList*     nodes                = 0,
                              const double       maxConductivity      = 0.0,
                              const double       expansionScaleFactor = 0.0,
                              const double       referenceDensity     = 0.0,
                              const double       referenceSpeed       = 0.0,
                              const double       referenceCoeff0      = 0.0,
                              const double       referenceCoeff1      = 0.0,
                              const double       referenceCoeff2      = 0.0,
                              const double       referenceCoeff3      = 0.0,
                              const double       referenceCoeff4      = 0.0,
                              const double       referenceCoeff5      = 0.0,
                              const double       bestEfficiency       = 0.0,
                              const double       referenceQBep        = 0.0,
                              const double       filterGain           = 0.5,
                              const double       driveRatio           = 1.0,
                              const double       thermalLength        = 0.0,
                              const double       thermalDiameter      = 0.0,
                              const double       surfaceRoughness     = 0.0,
                              const bool         checkValveActive     = false);
        /// @brief  Copy constructs this Gas Fan configuration data.
        GunnsGasFanConfigData(const GunnsGasFanConfigData& that);
        /// @brief  Default destructs this Gas Fan configuration data.
        virtual ~GunnsGasFanConfigData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsGasFanConfigData& operator =(const GunnsGasFanConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Gas Fan Model Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Gas Fan
///           link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsGasFanInputData : public GunnsFluidPotentialInputData
{
    public:
        double        mMotorSpeed;      /**< (revolution/min) Initial speed of the motor        */
        double        mWallTemperature; /**< (K)              Initial impeller wall temperature */
        /// @brief  Default constructs this Gas Fan input data.
        GunnsGasFanInputData(const bool   malfBlockageFlag  = false,
                             const double malfBlockageValue = 0.0,
                             const double sourcePressure    = 0.0,
                             const double motorSpeed        = 0.0,
                             const double wallTemperature   = 0.0);
        /// @brief  Copy constructs this Gas Fan input data.
        GunnsGasFanInputData(const GunnsGasFanInputData& that);
        /// @brief  Default destructs this Gas Fan input data.
        virtual ~GunnsGasFanInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsGasFanInputData& operator =(const GunnsGasFanInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gas Fan Model
///
/// @details  The GUNNS Gas Fan link model simulates all axial, radial, or mixed-flow gas ducted
///           fans and centrifugal pumps.  It can also be used for liquid centrifugal pumps if
///           liquid cavitation is not desired.  If cavitation is desired, use the
///           GunnsLiquidCentrifugalPump class instead.  This class should NOT be used for positive
///           displacement (constant-volume) pumps such as pistons, etc.  For those types of pumps,
///           use the GunnsGasDisplacementPump class instead.
///
///           This class implements the fan performance curve for produced pressure (kPa) as a
///           function of volumetric flow rate (m3/s), rotational speed (rpm), and inlet density
///           (kg/m3).  Effects of speed and density are governed by the fan Affinity Laws.  We
///           model convection heat transfer between the fluid and the fan/pipe wall, similar to
///           other pipe & HX links.  The curve is a 5th-order polynomial and operations on it are
///           handled by the GunnsGasFanCurve class.  This class maintains coefficients of the
///           polynomial as arrays of size [6].
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsGasFan : public GunnsFluidPotential
{
    TS_MAKE_SIM_COMPATIBLE(GunnsGasFan);
    public:
        /// @brief  Default constructs this Gas Fan.
        GunnsGasFan();
        /// @brief  Default destructs this Gas Fan.
        virtual ~GunnsGasFan();
        /// @brief  Initializes this Gas Fan with configuration and input data.
        void initialize(const GunnsGasFanConfigData&  configData,
                        const GunnsGasFanInputData&   inputData,
                        std::vector<GunnsBasicLink*>& links,
                        const int                     port0,
                        const int                     port1);
        /// @brief  Returns the heat flux from the fluid to the impeller wall.
        double getWallHeatFlux();
        /// @brief  Returns the fluid torque opposing impeller rotation.
        double getImpellerTorque();
        /// @brief  Returns the impeller speed.
        double getImpellerSpeed();
        /// @brief  Returns the impeller fluid power.
        double getImpellerPower();
        /// @brief    Sets the thermal surface area of this Gas Fan.
        void   setThermalSurfaceArea(const double value);
        /// @brief    Sets the wall temperature of this Gas Fan.
        void   setWallTemperature(const double value);
        /// @brief    Sets the motor speed of this Gas Fan.
        void   setMotorSpeed(const double value);
        /// @brief    Turn on or off the check valve functionality
        void   setCheckValveFlag(const bool value);

    protected:
        double mReferenceDensity;   /**< (kg/m3)          trick_chkpnt_io(**) Fluid inlet density for the reference performance curve */
        double mReferenceSpeed;     /**< (revolution/min) trick_chkpnt_io(**) Impeller speed for the reference performance curve      */
        double mReferenceCoeffs[6]; /**< (kPa)            trick_chkpnt_io(**) Reference performance curve polynomial coefficients     */
        double mReferenceQBep;      /**< (m3/s)           trick_chkpnt_io(**) Volume flow rate at best efficiency point at reference  */
        double mFilterGain;         /**< (--)             trick_chkpnt_io(**) (0-1) Flow filter gain for system curve estimate        */
        double mDriveRatio;         /**< (--)             trick_chkpnt_io(**) Gear ratio of motor to impeller speed                   */
        double mThermalDiameter;    /**< (m)              trick_chkpnt_io(**) Impeller inner diameter for thermal convection          */
        double mThermalSurfaceArea; /**< (m2)             trick_chkpnt_io(**) Impeller surface area for thermal convection            */
        double mThermalROverD;      /**< (--)             trick_chkpnt_io(**) Impeller surface roughness over diameter for convection */
        double mReferenceQ;         /**< (m3/s)           trick_chkpnt_io(**) Reference performance curve maximum flow rate           */
        double mReferencePowerBep;  /**< (W)              trick_chkpnt_io(**) Total motor power at best efficiency point at reference */
        double mSpecificSpeed;      /**< (rad)            trick_chkpnt_io(**) Pump specific speed for reference curve at BEP          */
        double mPowerCoeffs[4];     /**< (--)             trick_chkpnt_io(**) Impeller shaft power load polynomial coefficients       */
        double mMotorSpeed;         /**< (revolution/min)                     Motor rotational speed input from simbus                */
        double mWallTemperature;    /**< (K)                                  Impeller wall temperature input from simbus             */
        double mWallHeatFlux;       /**< (W)                                  Heat flux from fluid to wall output to simbus           */
        double mImpellerTorque;     /**< (N*m)                                Impeller fluid torque opposing rotation output to simbus */
        double mImpellerSpeed;      /**< (revolution/min) trick_chkpnt_io(**) Impeller rotational speed                               */
        double mImpellerPower;      /**< (W)              trick_chkpnt_io(**) Power imparted on fluid by the impeller                 */
        double mSystemConstant;     /**< (kPa)                                Estimated system curve coefficient                      */
        double mAffinityCoeffs[6];  /**< (--)             trick_chkpnt_io(**) Performance curve coefficients scaled by affinity laws  */
        double mSourceQ;            /**< (m3/s)                               Adjusted volumetric flow rate used in fan curve         */
        bool   mCheckValveActive;   /**< (--)             trick_chkpnt_io(**) Flag indicating check valve functionality is active     */
        double mCheckValvePosition; /**< (--)             trick_chkpnt_io(**) Check valve current position                            */
        GunnsGasFanCurve mCurve;    /**< (--)             trick_chkpnt_io(**) The fan-system polynomial                               */
        static const double mSpecificSpeedRadial;  /**< (rad) Lower specific speed limit for radial fans    */
        static const double mSpecificSpeedAxial;   /**< (rad) Upper specific speed limit for radial fans    */
        static const double mPowerCoeffsRadial[4]; /**< (--)  Power curve coefficients for radial flow (Ns = 0.2) */
        static const double mPowerCoeffsAxial[4];  /**< (--)  Power curve coefficients for axial flow (Ns = 5.0)  */
        static const double mRefCoeffsRadial[6];   /**< (--)  Fan curve coefficients for radial flow (Ns = 0.2) */
        static const double mRefCoeffsAxial[6];    /**< (--)  Fan curve coefficients for axial flow (Ns = 5.0)  */
        /// @brief  Validates the initialization of this Gas Fan.
        void          validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void  restartModel();
        /// @brief  Updates the state of this Gas Fan.
        virtual void  updateState(const double dt);
        /// @brief  Updates the internal fluid of this Gas Fan.
        virtual void  updateFluid(const double dt, const double flowRate);
        /// @brief  Computes the delta-pressure produced by the impeller.
        virtual void  computeSourcePressure();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsGasFan(const GunnsGasFan&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsGasFan& operator =(const GunnsGasFan&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) The heat flux from the fluid to the impeller wall.
///
/// @details  Returns the heat flux from the fluid to the impeller wall.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsGasFan::getWallHeatFlux()
{
    return mWallHeatFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (N*m) The fluid torque opposing impeller rotation.
///
/// @details  Returns the fluid torque opposing impeller rotation.  This is signed relative to the
///           normal rotational direction of the impeller, so will usually be negative.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsGasFan::getImpellerTorque()
{
    return mImpellerTorque;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (revolution/min) The impeller rotational speed.
///
/// @details  Returns the impeller rotational speed.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsGasFan::getImpellerSpeed()
{
    return mImpellerSpeed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) The power imparted on the fluid by the impeller.
///
/// @details  Returns the power imparted on the fluid by the impeller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsGasFan::getImpellerPower()
{
    return mImpellerPower;
}

#endif
