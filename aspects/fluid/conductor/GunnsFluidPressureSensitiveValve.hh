#ifndef GunnsFluidPressureSensitiveValve_EXISTS
#define GunnsFluidPressureSensitiveValve_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_PRESSURE_SENSITIVE_VALVE    Pressure Sensitive Valve Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Pressure Sensitive Valve link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidPressureSensitiveValve.o))

PROGRAMMERS:
- ((Kenneth McMurtrie)  (TSI) (2012-04) (New base class))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

#include "core/GunnsFluidLink.hh"
#include "core/GunnsFluidUtils.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pressure Sensitive Valve Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Pressure Sensitive Valve link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPressureSensitiveValveConfigData : public GunnsFluidLinkConfigData
{
    public:
        double      mMaxConductivity;       /**< (m2)   trick_chkpnt_io(**) Link Max Conductivity. */
        double      mExpansionScaleFactor;  /**< (--)   trick_chkpnt_io(**) Scaling for isentropic gas cooling (0-1). */
        double      mRateLimit;             /**< (1/s)  trick_chkpnt_io(**) Fractional position rate limit. */
        double      mThermalLength;         /**< (m)    trick_chkpnt_io(**) Tube length for thermal convection. */
        double      mThermalDiameter;       /**< (m)    trick_chkpnt_io(**) Tube inner diameter for thermal convection. */
        double      mSurfaceRoughness;      /**< (m)    trick_chkpnt_io(**) Tube wall surface roughness for thermal convection. */
        /// @brief  Default constructs this Pressure Sensitive Valve configuration data.
        GunnsFluidPressureSensitiveValveConfigData(const std::string& name                 = "",
                                                   GunnsNodeList*     nodes                = 0,
                                                   const double       maxConductivity      = 0.0,
                                                   const double       expansionScaleFactor = 0.0,
                                                   const double       rateLimit            = 0.0,
                                                   const double       thermalLength        = 0.0,
                                                   const double       thermalDiameter      = 0.0,
                                                   const double       surfaceRoughness     = 0.0);
        /// @brief  Copy constructs this Pressure Sensitive Valve configuration data.
        GunnsFluidPressureSensitiveValveConfigData(const GunnsFluidPressureSensitiveValveConfigData& that);
        /// @brief  Default destructs this Pressure Sensitive Valve configuration data.
        virtual ~GunnsFluidPressureSensitiveValveConfigData();
    protected:
    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidPressureSensitiveValveConfigData& operator =(const GunnsFluidPressureSensitiveValveConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pressure Sensitive Valve Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Pressure Sensitive Valve link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPressureSensitiveValveInputData : public GunnsFluidLinkInputData
{
    public:
        double      mPosition;                   /**< (--)   Fractional position of this valve. */
        bool        mMalfLeakThruFlag;           /**< (--)   Leak through rate malfunction flag. */
        double      mMalfLeakThruValue;          /**< (kg/s) Leak through rate malfunction value. */
        bool        mMalfPressureBiasFlag;       /**< (--)   Control pressure bias malfunction flag. */
        double      mMalfPressureBiasValue;      /**< (kPa)  Control pressure bias malfunction value. */
        double      mSetPointPressureBias;       /**< (kPa)  Set point pressure bias value. */
        double      mWallTemperature;            /**< (K)    Tube wall temperature for thermal convection */
        bool        mMalfStuckFlag;              /**< (--)   Stuck at current position malfunction flag. */
        bool        mMalfFailToFlag;             /**< (--)   Fail to position position malfunction flag. */
        double      mMalfFailToValue;            /**< (--)   Fail to position position malfunction value. */
        /// @brief  Default constructs this Pressure Sensitive Valve input data.
        GunnsFluidPressureSensitiveValveInputData(const bool   malfBlockageFlag      = false,
                                                  const double malfBlockageValue     = 0.0,
                                                  const double position              = 0.0,
                                                  const bool   malfLeakThruFlag      = false,
                                                  const double malfLeakThruValue     = 0.0,
                                                  const bool   malfPressureBiasFlag  = false,
                                                  const double malfPressureBiasValue = 0.0,
                                                  const double setPointPressureBias  = 0.0,
                                                  const double wallTemperature       = 0.0,
                                                  const bool   malfStuckFlag         = false,
                                                  const bool   malfFailToFlag        = false,
                                                  const double malfFailToValue       = 0.0);
        /// @brief  Copy constructs this Fluid Pressure Sensitive Valve input data.
        GunnsFluidPressureSensitiveValveInputData(const GunnsFluidPressureSensitiveValveInputData& that);
        /// @brief  Default destructs this Pressure Sensitive Valve input data.
        virtual ~GunnsFluidPressureSensitiveValveInputData();
    protected:
    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidPressureSensitiveValveInputData& operator =(const GunnsFluidPressureSensitiveValveInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pressure Sensitive Valve Link Model
///
/// @details  This class replaces GunnsFluidConductor and GunnsFluidValve as a base class for
///           pressure sensitive valves by adding two ports for the pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPressureSensitiveValve : public GunnsFluidLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidPressureSensitiveValve);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool          mMalfLeakThruFlag;         /**< (--)            Malfunction initial leak thru rate flag. */
        double        mMalfLeakThruValue;        /**< (kg/s)          Malfunction initial leak thru rate value. */
        bool          mMalfPressureBiasFlag;     /**< (--)            Control pressure bias malfunction flag. */
        double        mMalfPressureBiasValue;    /**< (kPa)           Control pressure bias malfunction value. */
        bool          mMalfStuckFlag;            /**< (--)            Stuck at current position malfunction flag. */
        bool          mMalfFailToFlag;           /**< (--)            Fail to position position malfunction flag. */
        double        mMalfFailToValue;          /**< (--)            Fail to position position malfunction value. */
        /// @}

        /// @brief  Default constructs this Pressure Sensitive Valve.
        GunnsFluidPressureSensitiveValve();
        /// @brief  Default destructs this Pressure Sensitive Valve.
        virtual ~GunnsFluidPressureSensitiveValve();
        /// @brief  Initializes this Pressure Sensitive Valve model.
        void initialize(const GunnsFluidPressureSensitiveValveConfigData& configData,
                        const GunnsFluidPressureSensitiveValveInputData&  inputData,
                        std::vector<GunnsBasicLink*>&                     networkLinks,
                        const int                                         port0,
                        const int                                         port1,
                        const int                                         pressurePort0,
                        const int                                         pressurePort1);
        /// @brief  Updates this Pressure Sensitive Valve.
        virtual void step(const double dt);
        /// @brief  Computes the flows in this Pressure Sensitive Valve.
        virtual void computeFlows(const double dt);
        /// @brief  Transports the flows in this Pressure Sensitive Valve.
        virtual void transportFlows(const double dt);
       /// @brief   Gets the position of this Pressure Sensitive Valve.
        double getPosition() const;
        /// @brief  Sets the position on this Pressure Sensitive Valve.
        void setPosition(const double position);
        /// @brief  Sets and resets the initial leak through rate malfunction.
        void setMalfLeakThru(const bool flag = false, const double value = 0.0);
        /// @brief  Sets and resets the control pressure bias malfunction.
        void setMalfPressureBias(const bool flag = false, const double value = 0.0);
        /// @brief  Sets and resets the stuck at current position malfunction.
        void setMalfStuck(const bool flag = false);
        /// @brief  Sets and resets the fail to position malfunction.
        void setMalfFailTo(const bool flag = false, const double value = 0.0);
        /// @brief    Sets the thermal surface area of this Pressure Sensitive Valve.
        void   setThermalSurfaceArea(const double value);
        /// @brief    Sets the wall temperature of this Pressure Sensitive Valve.
        void   setWallTemperature(const double value);
    protected:
        double        mMaxConductivity;          /**< (m2)            trick_chkpnt_io(**) Link Maximum Conductivity. */
        double        mExpansionScaleFactor;     /**< (--)            trick_chkpnt_io(**) Scaling for isentropic gas cooling (0-1). */
        double        mRateLimit;                /**< (1/s)           trick_chkpnt_io(**) Fractional position rate limit. */
        double        mThermalDiameter;          /**< (m)             trick_chkpnt_io(**) Tube inner diameter for thermal convection           */
        double        mThermalSurfaceArea;       /**< (m2)            trick_chkpnt_io(**) Tube inner surface area for thermal convection       */
        double        mThermalROverD;            /**< (--)            trick_chkpnt_io(**) Tube surface roughness over diameter for convection  */
        double        mPosition;                 /**< (--)                                Fractional position of this Valve. */
        double        mSetPointPressureBias;     /**< (kPa)                               Set point pressure bias value. */
        double        mWallTemperature;          /**< (K)                                 Tube wall temperature for thermal convection. */
        double        mPreviousLeakRate;         /**< (kg/s)                              Previous leak thru rate value. */
        double        mLeakConductivity;         /**< (m2)                                Conductivity equivalent to the leak. */
        GunnsFluidUtils::TuningMode  mTuneMode;  /**< (--)            trick_chkpnt_io(**) Auto-tunes the link to desired flow type. */
        double        mTuneMassFlow;             /**< (kg/s)          trick_chkpnt_io(**) The desired mass flow for link tuning. */
        double        mTuneVolFlow;              /**< (m3/s)          trick_chkpnt_io(**) The desired volumetric flow for link tuning. */
        double        mTuneDeltaT;               /**< (K)             trick_chkpnt_io(**) The desired delta-temperature for link tuning. */
        double        mEffectiveConductivity;    /**< (m2)            trick_chkpnt_io(**) Effective conductivity of the link. */
        double        mSystemConductance;        /**< (kg*mol/kPa/s)  trick_chkpnt_io(**) Limited molar conductance. */
        double        mControlPressure;          /**< (kPa)           trick_chkpnt_io(**) Valve control pressure. */
        double        mWallHeatFlux;             /**< (W)                                 Convection heat flux from the fluid to the tube wall */
        /// @brief    Updates the state of this Pressure Sensitive Valve.
        virtual void updateState(const double dt);
        /// @brief    Updates the internal fluid of this Pressure Sensitive Valve.
        virtual void updateFluid(const double dt, const double flowRate);
       /// @brief     Validates the initialization of this Pressure Sensitive Valve.
        void validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Checks for unique port node assignment.
        virtual bool checkPortDuplication(const int port, const int node) const;
        /// @brief    Updates the admittance and source terms of this Pressure Sensitive Valve.
        virtual void buildConductance();
        /// @brief    Computes molar flux rate through this Pressure Sensitive Valve.
        virtual void computeFlux();
        /// @brief    Computes mPower for this Pressure Sensitive Valve.
        virtual void computePower();
        /// @brief    Tunes this Pressure Sensitive Valve conductivity to create the desired flow rate.
        void tuneFlow(const double flowRate);
    private:
        /// @details  Define the number of ports this link class has.  All objects of the same link
        ///           class always have the same number of ports.  We use an enum rather than a
        ///           static const int so that we can reuse the NPORTS name and allow each class to
        ///           define its own value.
        enum {NPORTS = 4};
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidPressureSensitiveValve(const GunnsFluidPressureSensitiveValve&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidPressureSensitiveValve& operator =(const GunnsFluidPressureSensitiveValve&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return    double  (--)   Fractional valve position (0-1).
///
/// @details   Gets the fractional valve position of this GUNNS Fluid Pressure Sensitive Valve Link Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidPressureSensitiveValve::getPosition() const
{
    return mPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] position  (--)  Commanded fractional valve position (0-1).
///
/// @return    void
///
/// @details   Sets the fractional valve position of this GUNNS Fluid Pressure Sensitive Valve Link Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidPressureSensitiveValve::setPosition(const double position)
{
    mPosition = position;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return    void
///
/// @details  Computes the molar flux rate through this GUNNS Fluid Pressure Sensitive Valve Link Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidPressureSensitiveValve::computeFlux()
{
    /// - Units: Flux (kgmol/s) = Admittance (kgmol/kPa/s) * dP (kPa)
    mFlux = mPotentialDrop * mAdmittanceMatrix[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return    void
///
/// @details  Computes the hydraulic power in this GUNNS Fluid Pressure Sensitive Valve Link Model.  .
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidPressureSensitiveValve::computePower()
{
    /// - Hydraulic power in the link is -volumetric flow * potential drop.  Note that this is
    ///   defined as power gain, such as from a pump. Pressure-producing links such as pumps and
    ///   fans will have positive power gain, and the power in dissipating links such as pipes, etc.
    ///   will be negative. Since potential is in units of kPa, it must be convertedt to Pa to
    ///   express power in Watts.
    mPower = -UnitConversion::PA_PER_KPA * mVolFlowRate * getDeltaPotential();
}

#endif
