#ifndef GunnsFluidRegulatorValve_EXISTS
#define GunnsFluidRegulatorValve_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_REGULATOR_VALVE   Pressure Regulator Valve Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Pressure Regulator Valve link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidRegulatorValve.o))

PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (New base) (2012-04))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

#include "GunnsFluidPressureSensitiveValve.hh"
#include "GunnsFluidValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pressure Regulator Valve Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Pressure Regulator Valve link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidRegulatorValveConfigData : public GunnsFluidPressureSensitiveValveConfigData
{
    public:
        double        mReseatPressure;   /**< (kPa)  trick_chkpnt_io(**) Valve close pressure. */
        double        mCrackPressure;    /**< (kPa)  trick_chkpnt_io(**) Valve open pressure. */
        double        mFullOpenPressure; /**< (kPa)  trick_chkpnt_io(**) Valve full open pressure. */
        double        mPopPosition;      /**< (--)   trick_chkpnt_io(**) Initial pop position. */
        double        mPopSlopeScale;    /**< (kPa)  trick_chkpnt_io(**) Initial pop slope scale factor. */
        /// @brief    Default constructs this Pressure Regulator Configuration data.
        GunnsFluidRegulatorValveConfigData(const std::string& name                 = "",
                                           GunnsNodeList*     nodes                = 0,
                                           const double       maxConductivity      = 0.0,
                                           const double       expansionScaleFactor = 0.0,
                                           const double       rateLimit            = 0.0,
                                           const double       thermalLength        = 0.0,
                                           const double       thermalDiameter      = 0.0,
                                           const double       surfaceRoughness     = 0.0,
                                           const double       reseatPressure       = 0.0,
                                           const double       crackPressure        = 0.0,
                                           const double       fullOpenPressure     = 0.0,
                                           const double       popPosition          = 0.0,
                                           const double       popSlopeScale        = 0.0);
        /// @brief    Copy constructs this Pressure Regulator Valve configuration data.
        GunnsFluidRegulatorValveConfigData(const GunnsFluidRegulatorValveConfigData& that);
        /// @brief    Default destructs this Pressure Regulator Valve configuration data.
        virtual ~GunnsFluidRegulatorValveConfigData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidRegulatorValveConfigData& operator =(const GunnsFluidRegulatorValveConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pressure Regulator Valve Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Pressure Regulator link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidRegulatorValveInputData : public GunnsFluidPressureSensitiveValveInputData
{
    public:
        /// @brief    Default constructs this Pressure Regulator Valve input data.
        GunnsFluidRegulatorValveInputData(const bool   malfBlockageFlag      = false,
                                          const double malfBlockageValue     = 0.0,
                                          const double position              = 0.0,
                                          const bool   malfLeakThruFlag      = false,
                                          const double malfLeakThruValue     = 0.0,
                                          const bool   malfPressureBiasFlag  = false,
                                          const double malfPressureBiasValue = 0.0,
                                          const double setPointPressureBias  = 0.0,
                                          const double wallTemperature       = 0.0,
                                          const bool   malfPreviousFlag      = false,
                                          const bool   malfConstantFlag      = false,
                                          const double malfConstantValue     = 0.0);
        /// @brief    Copy constructs this Pressure Regulator Valve input data.
        GunnsFluidRegulatorValveInputData(const GunnsFluidRegulatorValveInputData& that);
        /// @brief    Default destructs this Pressure Regulator Valve input data.
        virtual ~GunnsFluidRegulatorValveInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidRegulatorValveInputData& operator =(const GunnsFluidRegulatorValveInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pressure Regulator Valve Link Model
///
/// @details  The GUNNS Fluid Pressure Regulator Valve link model attempts to maintain the control
///           pressure between the crack pressure and the reseat pressure. The control pressure is
///           the difference in pressure between the node mapped to port 2 (pressure port 0) and
///           the node mapped to port 3 (pressure port 1). As the control pressure decreases below
///           the crack pressure, the valve rapidly opens to the pop position, then continues to
///           open with decreasing control pressure at a more leisurely rate. Once the control
///           pressure begins to increase,  there is a brief valve position hold due to hysteresis
///           and then the valve position closes at a leisurely rate until it reaches the pop
///           position, at which point it closes rapidly with increasing control pressure.
///           For crack, reseat and full open as absolute pressures, map port 3 to the vacuum node.
///           \verbatim
///                 valve position
///                    1 -|      -----
///                       |        -- ----
///                       |          --   ----
///                       |            --     ----
///                       |              --       ----
///                       |                --         ----
///                       |                  --           ----
///                       |                    --             ----
///                       |                      --               ----
///                  pop _|                        --                 ----
///                       |                          \                    |
///           ____________|______|____________________|____________________\___  outlet pressure
///                       |  full open              crack                 reseat
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidRegulatorValve : public  GunnsFluidPressureSensitiveValve
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidRegulatorValve);
    public:
        /// @brief    Default constructs this Pressure Regulator Valve.
        GunnsFluidRegulatorValve();
        /// @brief    Default destructs this Pressure Regulator Valve.
        virtual ~GunnsFluidRegulatorValve();
        /// @brief    Initializes this Pressure Regulator Valve with configuration and input data.
        void initialize(const GunnsFluidRegulatorValveConfigData&  configData,
                        const GunnsFluidRegulatorValveInputData&   inputData,
                        std::vector<GunnsBasicLink*>&              networkLinks,
                        const int                                  port0,
                        const int                                  port1,
                        const int                                  pressurePort0,
                        const int                                  pressurePort1);
        /// @brief    Returns the valve state of the regulator
        GunnsFluidValve::State getState() const;
    protected:
        double        mReseatPressure;   /**< (kPa)   trick_chkpnt_io(**) Valve close pressure. */
        double        mCrackPressure;    /**< (kPa)   trick_chkpnt_io(**) Valve open pressure. */
        double        mFullOpenPressure; /**< (kPa)   trick_chkpnt_io(**) Valve full open pressure. */
        double        mPopPosition;      /**< (--)    trick_chkpnt_io(**) Initial pop position. */
        double        mPopSlopeScale;    /**< (kPa)   trick_chkpnt_io(**) Initial pop slope scale factor. */
        double        mPopIncM;          /**< (kPa)   trick_chkpnt_io(**) Slope for increasing initial pop. */
        double        mPopIncMInv;       /**< (1/kPa) trick_chkpnt_io(**) Slope for increasing initial pop inverse. */
        double        mPopIncB;          /**< (--)    trick_chkpnt_io(**) X intercept for increasing initial pop. */
        double        mPopIncP;          /**< (kPa)   trick_chkpnt_io(**) Increasing transition pressure. */
        double        mPopDecM;          /**< (kPa)   trick_chkpnt_io(**) Slope for decreasing initial pop equation. */
        double        mPopDecMInv;       /**< (1/kPa) trick_chkpnt_io(**) Slope for decreasing initial pop inverse. */
        double        mPopDecB;          /**< (--)    trick_chkpnt_io(**) X intercept for decreasing initial pop. */
        double        mPopDecP;          /**< (kPa)   trick_chkpnt_io(**) Decreasing transition pressure. */
        double        mNomIncM;          /**< (kPa)   trick_chkpnt_io(**) Slope for increasing nominal. */
        double        mNomIncMInv;       /**< (1/kPa) trick_chkpnt_io(**) Slope for increasing nominal inverse. */
        double        mNomIncB;          /**< (--)    trick_chkpnt_io(**) X intercept for increasing nominal. */
        double        mNomDecM;          /**< (kPa)   trick_chkpnt_io(**) Slope for decreasing nominal. */
        double        mNomDecMInv;       /**< (1/kPa) trick_chkpnt_io(**) Slope for decreasing nominal inverse. */
        double        mNomDecB;          /**< (--)    trick_chkpnt_io(**) X intercept for decreasing nominal. */
        double        mPreviousPressure; /**< (kPa)                       Previous inlet pressure. */
        GunnsFluidValve::State mState;   /**< (--)                        Valve state of this Pressure Regulator. */
        /// @brief    Validates the initialization of this Pressure Regulator Valve.
        void validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Computes the derived attributes for this Pressure Regulator Valve.
        void derive();
        /// @brief    Updates the state of this Pressure Regulator Valve.
        virtual void updateState(const double dt);
        /// @brief    Updates the valve state of this Pressure Regulator Valve.
        void updateValveState();
        /// @brief    Updates the valve position of this Pressure Regulator Valve.
        void updatePosition(const double dt);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidRegulatorValve(const GunnsFluidRegulatorValve&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidRegulatorValve& operator =(const GunnsFluidRegulatorValve&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   GunnsFluidValve::State  (--)  The state of the valve.
///
/// @details  Gets the valve state of this GUNNS Fluid Pressure Regulator Valve Link Model.
////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsFluidValve::State GunnsFluidRegulatorValve::getState() const
{
        return mState;
}

#endif
