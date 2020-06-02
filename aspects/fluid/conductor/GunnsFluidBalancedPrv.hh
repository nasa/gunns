#ifndef GunnsFluidBalancedPrv_EXISTS
#define GunnsFluidBalancedPrv_EXISTS

/**
@file
@brief    GUNNS Fluid Balanced Pressure Reducing Valve Link declarations

@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_BALANCED_PRV  GUNNS Fluid Balanced Pressure Reducing Valve Link
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Balanced Pressure Reducing Valve.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidBalancedPrv.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2016-08) (Initial Prototype))

@{
*/

#include "core/GunnsFluidLink.hh"
#include "core/GunnsFluidUtils.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Balanced Pressure Reducing Valve Configuration Data.
///
/// @details  This provides a data structure for the Fluid Balanced Pressure Reducing Valve
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidBalancedPrvConfigData : public GunnsFluidLinkConfigData
{
    public:
        double mExitPressureDroop; /**< (kPa*s/kg) trick_chkpnt_io(**) Exit pressure droop per unit exit mass flow. */
        /// @brief Default constructs this Balanced Pressure Reducing Valve configuration data.
        GunnsFluidBalancedPrvConfigData(const std::string& name              = "",
                                        GunnsNodeList*     nodes             = 0,
                                        const double       exitPressureDroop = 0.0);
        /// @brief Copy constructs this Balanced Pressure Reducing Valve configuration data.
        GunnsFluidBalancedPrvConfigData(const GunnsFluidBalancedPrvConfigData& that);
        /// @brief Default destructs this Balanced Pressure Reducing Valve configuration data.
        virtual ~GunnsFluidBalancedPrvConfigData();

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidBalancedPrvConfigData& operator =(const GunnsFluidBalancedPrvConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Balanced Pressure Reducing Valve Input Data.
///
/// @details  This provides a data structure for the Fluid Balanced Pressure Reducing Valve input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidBalancedPrvInputData : public GunnsFluidLinkInputData
{
    public:
        bool   mMalfPressureBiasFlag;  /**< (--)  trick_chkpnt_io(**) Initial activation flag for pressure setpoint bias malfunction. */
        double mMalfPressureBiasValue; /**< (kPa) trick_chkpnt_io(**) Initial value for pressure setpoint bias malfunction. */
        double mPressureSetpoint;      /**< (kPa) trick_chkpnt_io(**) Initial regulated pressure setpoint. */
        /// @brief Default constructs this Balanced Pressure Reducing Valve input data.
        GunnsFluidBalancedPrvInputData(const bool   malfBlockageFlag      = false,
                                       const double malfBlockageValue     = 0.0,
                                       const bool   malfPressureBiasFlag  = false,
                                       const double malfPressureBiasValue = 0.0,
                                       const double pressureSetpoint      = 0.0);
        /// @brief Copy constructs this Balanced Pressure Reducing Valve input data.
        GunnsFluidBalancedPrvInputData(const GunnsFluidBalancedPrvInputData& that);
        /// @brief Default destructs this Balanced Pressure Reducing Valve input data.
        virtual ~GunnsFluidBalancedPrvInputData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidBalancedPrvInputData& operator =(const GunnsFluidBalancedPrvInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Balanced Pressure Reducing Valve
///
/// @details  This is a simpler, more ideal and easier to tune model of a pressure regulator valve
///           than the GunnsFluidRegulatorValve.  Whereas that link dynamically moves an actual
///           valve position to try and control the downstream pressure, this link simply uses a
///           potential source effect from Ground to the downstream node.  This constrains the
///           downstream node pressure to be more stable and avoids most of the chatter & overshoot
///           problems associated with a badly-tuned GunnsFluidRegulatorValve.
///
///           The actual flow into the downstream node is tracked in a mass deficit buffer that is
///           owed from the inlet side.  The link creates a conductance effect from the inlet node
///           to Ground, and the resulting flow is removed from the mass deficit buffer -- the two
///           effects working together create the appearance of flow from inlet to exit ports.
///
///           The outlet potential source effect uses an associated conductance to model droop in
///           the outlet pressure under load.  The droop is proportional to outlet flow rate.  This
///           link models a "balanced" regulator that compensates for falling inlet pressure.
///           Outlet pressure isn't affected by inlet pressure until the inlet drops below the
///           setpoint pressure.  This link does not allow backflow.
///
///           This link does not perfectly conserve mass or energy.  Because flow is supplied to the
///           outlet node before it is pulled from the inlet node, this link temporarily creates a
///           small amount of mass in the system.  Total mass is conserved when flow stops and the
///           remaining mass deficit buffer is drained from the inlet.  However, if the inlet fluid
///           mixture or temperature is changing, then constituent masses and energy are not
///           conserved, respectively.
///
///           This is a 3-port link:
///             Port 0: inlet node,
///             Port 1: outlet node,
///             Port 2: reference pressure port.
///           Port 2 is only used as the reference pressure for the control setpoint -- thus the
///           regulator can be used to regulated absolute or gauge pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidBalancedPrv : public GunnsFluidLink
{
        TS_MAKE_SIM_COMPATIBLE(GunnsFluidBalancedPrv);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfPressureBiasFlag;  /**< (--)                               Activation flag for pressure setpoint bias malfunction. */
        double mMalfPressureBiasValue; /**< (kPa)                              Value for pressure setpoint bias malfunction. */
        /// @}
        /// @brief Default Constructor.
        GunnsFluidBalancedPrv();
        /// @brief Default constructor.
        virtual ~GunnsFluidBalancedPrv();
        /// @brief Initialize method.
        void initialize(const GunnsFluidBalancedPrvConfigData& configData,
                        const GunnsFluidBalancedPrvInputData&  inputData,
                        std::vector<GunnsBasicLink*>&          networkLinks,
                        const int                              port0,
                        const int                              port1,
                        const int                              port2);
        /// @brief Default constructor.
        virtual void   step(const double dt);
        /// @brief Compute Flows.
        virtual void   computeFlows(const double dt);
        /// @brief Transport Flows.
        virtual void   transportFlows(const double dt);
        /// @brief Sets the regulator pressure setpoint.
        void           setPressureSetpoint(const double value);
        /// @brief Sets and resets the pressure setpoint bias malfunction.
        void           setMalfPressureBias(const bool flag = false, const double value = 0.0);
        /// @brief Returns the regulator valve's inlet gauge pressure.
        double         getInletGaugePressure() const;
        /// @brief Returns the regulator valve's exit gauge pressure.
        double         getExitGaugePressure() const;

    protected:
        double mExitPressureDroop;     /**< (kPa*s/kg)     trick_chkpnt_io(**) Exit pressure drop per unit exit mass flow. */
        double mPressureSetpoint;      /**< (kPa)                              Regulated pressure setpoint. */
        double mInletConductance;      /**< (kg*mol/kPa/s) trick_chkpnt_io(**) Limited molar conductance on inlet port. */
        double mExitConductance;       /**< (kg*mol/kPa/s) trick_chkpnt_io(**) Limited molar conductance on exit port. */
        double mInletMassBuffer;       /**< (kg)                               Stored deficit of mass demand owed by inlet. */
        double mRegulatedPressure;     /**< (kPa)          trick_chkpnt_io(**) Actual pressure being regulated to. */
        double mInletFlux;             /**< (kg*mol/s)     trick_chkpnt_io(**) Molar flow rate from inlet. */
        double mInletFlowRate;         /**< (kg/s)         trick_chkpnt_io(**) Mass flow rate from inlet. */
        bool   mBackPressureCutoff;    /**< (--)                               Regulator shut due to back-pressure. */
        /// @brief Validates the initialization of this Balanced Pressure Reducing Valve.
        void           validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void   restartModel();
        /// @brief Updates the admittance matrix terms of the link.
        virtual void   buildAdmittanceMatrix();
        /// @brief Updates the source vector terms of the link.
        virtual void   buildSourceVector();
        //TODO check specific port rules, can't connect port 0 to ground
        /// @brief Checks for valid implementation-specific port node assignment.
        virtual bool   checkSpecificPortRules(const int port, const int node) const;

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 3};
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidBalancedPrv(const GunnsFluidBalancedPrv& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidBalancedPrv& operator =(const GunnsFluidBalancedPrv& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value (kPa) New value for the pressure setpoint.
///
/// @details  Sets the valve's regulated pressure setpoint to the given value.  The value is limited
///           to be >= 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidBalancedPrv::setPressureSetpoint(const double value)
{
    mPressureSetpoint = std::max(value, 0.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (kPa)  The gauge pressure between the inlet port and the ambient port.
///
/// @details  Returns the gauge pressure between the inlet port and the ambient port.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidBalancedPrv::getInletGaugePressure() const
{
    return mPotentialVector[0] - mPotentialVector[2];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (kPa)  The gauge pressure between the exit port and the ambient port.
///
/// @details  Returns the gauge pressure between the exit port and the ambient port.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidBalancedPrv::getExitGaugePressure() const
{
    return mPotentialVector[1] - mPotentialVector[2];
}

#endif
