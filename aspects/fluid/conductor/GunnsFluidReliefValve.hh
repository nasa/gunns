#ifndef GunnsFluidReliefValve_EXISTS
#define GunnsFluidReliefValve_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_PRELIEF_VALVE      Pressure Relief Valve Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Pressure Relief Valve link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidReliefValve.o))

PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (New base) (2012-04))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

#include "GunnsFluidPressureSensitiveValve.hh"
#include "GunnsFluidValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pressure Relief Valve Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Pressure Relief Valve link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidReliefValveConfigData : public GunnsFluidPressureSensitiveValveConfigData
{
    public:
        double        mReseatPressure;   /**< (kPa)  trick_chkpnt_io(**) Valve close pressure. */
        double        mCrackPressure;    /**< (kPa)  trick_chkpnt_io(**) Valve open pressure. */
        double        mFullOpenPressure; /**< (kPa)  trick_chkpnt_io(**) Valve full open pressure. */
        double        mPopPosition;      /**< (--)   trick_chkpnt_io(**) Initial pop position. */
        double        mPopSlopeScale;    /**< (kPa)  trick_chkpnt_io(**) Initial pop slope scale factor. */
        /// @brief    Default constructs this Pressure Relief Valve configuration data.
        GunnsFluidReliefValveConfigData(const std::string& name                 = "",
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
        /// @brief    Copy constructs this Pressure Relief Valve configuration data.
        GunnsFluidReliefValveConfigData(
                                             const GunnsFluidReliefValveConfigData& that);
        /// @brief    Default destructs this Pressure Relief Valve configuration data.
        virtual ~GunnsFluidReliefValveConfigData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidReliefValveConfigData& operator =(const GunnsFluidReliefValveConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pressure Relief Valve Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Positive Valve link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidReliefValveInputData : public GunnsFluidPressureSensitiveValveInputData
{
    public:
        /// @brief    Default constructs this Pressure Relief Valve input data.
        GunnsFluidReliefValveInputData(const bool   malfBlockageFlag      = false,
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
        /// @brief    Copy constructs this Pressure Relief Valve input data.
        GunnsFluidReliefValveInputData(const GunnsFluidReliefValveInputData& that);
        /// @brief    Default destructs this Pressure Relief Valve input data.
        virtual ~GunnsFluidReliefValveInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidReliefValveInputData& operator =(const GunnsFluidReliefValveInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pressure Relief Valve Link Model
///
/// @details  The GUNNS Fluid Pressure Relief Valve link model attempts to maintain the delta
///           pressure below the reseat pressure and the crack pressure. As the delta pressure
///           increases above the crack pressure, the valve rapidly opens to the pop position, then
///           continues to open with increasing pressure at a more leisurely rate. Once the delta
///           pressure begins to decrease, there is a brief valve position hold due to hysteresis
///           and then the valve position closes at a leisurely rate until it reaches the pop
///           position, at which point it closes rapidly with increasing delta pressure. This
///           provides positive pressure relief when port 1 and port 3 (pressure port 1) are
///           mapped to the ambient node and negative pressure relief when port 0 and port 2
///           (pressure port 0) are mapped to the ambient node.
///           \verbatim
///                 valve position
///                    1 -|                                            ----
///                       |                                        ---- --
///                       |                                    ----   --
///                       |                                ----     --
///                       |                            ----       --
///                       |                        ----         --
///                       |                    ----           --
///                       |                ----             --
///                       |            ----               --
///                  pop _|        ----                 --
///                       |       /                    /
///           ____________|______/____________________/____________________|___  delta pressure
///                       |  reseat              crack                 full open
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidReliefValve : public  GunnsFluidPressureSensitiveValve
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidReliefValve);
    public:
        /// @brief    Default constructs this Pressure Relief Valve.
        GunnsFluidReliefValve();
        /// @brief    Default destructs this Pressure Relief Valve.
        virtual ~GunnsFluidReliefValve();
        /// @brief    Initializes this Pressure Relief Valve with configuration and input data.
        void initialize(const GunnsFluidReliefValveConfigData& configData,
                        const GunnsFluidReliefValveInputData&  inputData,
                        std::vector<GunnsBasicLink*>&          networkLinks,
                        const int                              port0,
                        const int                              port1,
                        const int                              pressurePort0,
                        const int                              pressurePort1);
        /// @brief    Returns the state of the fluid relief valve
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
        /// @brief    Validates the initialization of this Pressure Relief Valve.
        void validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Computes the derived attributes for this Pressure Relief Valve.
        void derive();
        /// @brief    Updates the state of this Pressure Relief Valve.
        virtual void updateState(const double dt);
        /// @brief    Updates valve state of this Pressure Relief Valve.
        void updateValveState();
        /// @brief    Updates the valve position of this Pressure Relief Valve.
        void updatePosition(const double dt);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidReliefValve(const GunnsFluidReliefValve&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidReliefValve& operator =(const GunnsFluidReliefValve&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   GunnsFluidValve::State  (--)  The state of the valve.
///
/// @details  Gets the valve state of this GUNNS Fluid Pressure Relief Valve Link Model.
////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsFluidValve::State GunnsFluidReliefValve::getState() const
{
        return mState;
}

#endif
