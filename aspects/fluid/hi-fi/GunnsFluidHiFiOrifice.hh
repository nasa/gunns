#ifndef GunnsFluidHiFiOrifice_EXISTS
#define GunnsFluidHiFiOrifice_EXISTS

/**
@file
@brief    GUNNS Fluid High-Fidelity Orifice Model declarations

@defgroup  TSM_GUNNS_FLUID_HIFI_ORIFICE    High-Fidelity Orifice Model
@ingroup   TSM_GUNNS_FLUID_HIFI

@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Hi-Fi Orifice link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidHiFiOrifice.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Prototype) (2015-09))

@{
*/

#include "core/GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hi-Fi Orifice Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Hi-Fi Orifice link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHiFiOrificeConfigData : public GunnsFluidLinkConfigData
{
    public:
        enum CoeffTypes {
            DISCHARGE_COEFF = 0, ///< Coefficient of Discharge, Cd, (--)
            FLOW_FACTOR     = 1, ///< Flow Factor, Kv, (m3/h)
            FLOW_COEFF      = 2  ///< Flow Coefficient, Cv, (US gallon/minute)
        };
        CoeffTypes mCoefficientType;        /**<    (--) trick_chkpnt_io(**) Type of coefficient to be specified. */
        double     mCoefficientValue;       /**<    (--) trick_chkpnt_io(**) Coefficient value of the specified type. */
        double     mThroatDiameter;         /**<    (m)  trick_chkpnt_io(**) Throat diameter. */
        double     mCriticalReynolds;       /**<    (--) trick_chkpnt_io(**) Optional Reynolds number at laminar/turbulent transition. */
        double     mExpansionScaleFactor;   /**<    (--) trick_chkpnt_io(**) (0-1) Optional scaling for expansion gas cooling. */
        double     mFlowTuningFactor;       /**<    (--) trick_chkpnt_io(**) Optional factor for flow tuning. */
        static const double mConvertKvToCd; /**< ** (--) trick_chkpnt_io(**) Convert Kv to Cd constant. */
        static const double mConvertCvToCd; /**< ** (--) trick_chkpnt_io(**) Convert Cv to Cd constant. */
        /// @brief  Default constructs this Hi-Fi Orifice configuration data.
        GunnsFluidHiFiOrificeConfigData(const std::string& name                 = "",
                                        GunnsNodeList*     nodes                = 0,
                                        const CoeffTypes   coefficientType      = DISCHARGE_COEFF,
                                        const double       coefficientValue     = 0.0,
                                        const double       throatDiameter       = 0.0,
                                        const double       criticalReynolds     = 0.0,
                                        const double       expansionScaleFactor = 0.0,
                                        const double       flowTuningFactor     = 1.0);
        /// @brief  Copy constructs this Hi-Fi Orifice configuration data.
        GunnsFluidHiFiOrificeConfigData(const GunnsFluidHiFiOrificeConfigData& that);
        /// @brief  Default destructs this Hi-Fi Orifice configuration data.
        virtual ~GunnsFluidHiFiOrificeConfigData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidHiFiOrificeConfigData& operator =(const GunnsFluidHiFiOrificeConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hi-Fi Orifice Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Hi-Fi Orifice link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHiFiOrificeInputData : public GunnsFluidLinkInputData
{
    public:
         /// @brief  Default constructs this Hi-Fi Orifice input data.
        GunnsFluidHiFiOrificeInputData(const bool   malfBlockageFlag  = false,
                                       const double malfBlockageValue = 0.0);
        /// @brief  Copy constructs this Hi-Fi Orifice input data.
        GunnsFluidHiFiOrificeInputData(const GunnsFluidHiFiOrificeInputData& that);
        /// @brief  Default destructs this Hi-Fi Orifice input data.
        virtual ~GunnsFluidHiFiOrificeInputData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidHiFiOrificeInputData& operator =(const GunnsFluidHiFiOrificeInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hi-Fi Orifice Model
///
/// @details  The GUNNS Fluid Hi-Fi Orifice link model is the base class from which other hi-fi
///           orifices (valves, etc) derive.  This is similar to GunnsFluidConductor except that it
///           applies higher-fidelity flow equations to gas flows and it is configured with real h/w
///           parameters instead of a raw conductance.  Choked, non-choked, and laminar flow regimes
///           are modeled.  This assumes a thin orifice and neglects to model forced convection with
///           the walls, although derived classes my model it.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHiFiOrifice : public GunnsFluidLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidHiFiOrifice);
    public:
        /// @brief  Default constructs this Hi-Fi Orifice.
        GunnsFluidHiFiOrifice();
        /// @brief  Default destructs this Hi-Fi Orifice.
        virtual ~GunnsFluidHiFiOrifice();
        /// @brief  Initializes this Hi-Fi Orifice with configuration and input data.
        void           initialize(const GunnsFluidHiFiOrificeConfigData& configData,
                                  const GunnsFluidHiFiOrificeInputData&  inputData,
                                  std::vector<GunnsBasicLink*>&          networkLinks,
                                  const int                              port0,
                                  const int                              port1);
        /// @brief  Computes the link contributions to the network system of equations.
        virtual void   step(const double dt);
        /// @brief  Computes flows through the link based on the network solution.
        virtual void   computeFlows(const double dt);
        /// @brief  Transports flows through the link based on the computeFlows solution.
        virtual void   transportFlows(const double dt);

    protected:
        double mCdDefault;             /**< (--)           trick_chkpnt_io(**) Normal (turbulent) coefficient of discharge, Cd (0-1). */
        double mThroatDiameter;        /**< (m)            trick_chkpnt_io(**) Throat diameter. */
        double mReCritical;            /**< (--)           trick_chkpnt_io(**) Reynolds number at laminar/turbulent transition. */
        double mExpansionScaleFactor;  /**< (--)           trick_chkpnt_io(**) Scaling for expansion gas cooling. */
        double mFlowTuningFactor;      /**< (--)           trick_chkpnt_io(**) Factor for flow tuning. */
        double mThroatArea;            /**< (m2)           trick_chkpnt_io(**) Throat area at full-open. */
        double mReActual;              /**< (--)           trick_chkpnt_io(**) Actual Reynolds number at throat. */
        double mCdActual;              /**< (--)           trick_chkpnt_io(**) Actual (laminar/turbulent) coefficient of discharge. */
        double mSystemConductance;     /**< (kg*mol/kPa/s) trick_chkpnt_io(**) Limited conductance for the system of equations. */
        double mPressureRatio;         /**< (--)           trick_chkpnt_io(**) Critical pressure ratio (p1/p*). */
        /// @brief  Validates the initialization of this Hi-Fi Orifice.
        void           validate() const;
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void   restartModel();
        /// @brief  Updates the state of derived links.
        virtual void   updateState(const double dt);
        /// @brief  Updates the internal fluid of this Hi-Fi Orifice.
        virtual void   updateFluid(const double dt, const double flowRate);
        /// @brief  Updates the conductance effect for the system of equations.
        void           computeConductance(const PolyFluid* fluid0, const PolyFluid* fluid1);
        /// @brief  Returns the linearized mass flow conductivity of a gas flow.
        double         computeGasConductivity(const PolyFluid* fluid0, const PolyFluid* fluid1);
        /// @brief  Returns the ideal mass flux for critical (choked) gas flow.
        double         computeCriticalGasFlux(const double gamma, const double p0,
                                              const double rho0) const;
        /// @brief  Returns the ideal mass flux for sub-critical (non-choked) gas flow.
        double         computeSubCriticalGasFlux(const double gamma, const double p0,
                                                 const double rho0,  const double p1) const;
        /// @brief  Returns the linearized mass flow conductivity of a liquid flow.
        double         computeLiquidConductivity(const PolyFluid* fluid0,
                                                 const PolyFluid* fluid1) const;
        /// @brief  Returns the ideal mass flux for an incompressible flow.
        double         computeBernoulliFlux(const double rho, const double dp) const;
        /// @brief  Builds the link contributions to the system admittance matrix.
        void           buildAdmittanceMatrix();
        /// @brief  Computes molar flux rate through the link.
        virtual void   computeFlux();
        /// @brief  Computes hydraulic power gain/loss through the link.
        virtual void   computePower();
        /// @brief  Returns the effective throat area of the device.
        virtual double getEffectiveArea() const;

    private:
        /// @brief  Define the number of ports this link class has.  All objects of the same link
        ///         class always have the same number of ports.  We use an enum rather than a static
        ///         const int so that we can reuse the NPORTS name and allow each class to define
        ///         its own value.
        enum {NPORTS = 2};
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidHiFiOrifice(const GunnsFluidHiFiOrifice&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidHiFiOrifice& operator =(const GunnsFluidHiFiOrifice&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s)  Time step (not used).
///
/// @return   void
///
/// @details  This Hi-Fi Orifice implementation does nothing.  This is meant for derived classes to
///           override and implement functions like valve position & malfs, fancier flow
///           coefficients, etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidHiFiOrifice::updateState(const double dt __attribute__((unused)))
{
    // nothing to do.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt       (s)    Not used.
/// @param[in] flowRate (kg/s) Not used.
///
/// @details  This Hi-Fi Orifice implementation does nothing.  This is meant for derived classes to
///           override and implement functions that affect the properties of the fluid flowing thru
///           the orifice (forced convection, etc.)
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidHiFiOrifice::updateFluid(const double dt __attribute__((unused)),
                                               const double flowRate __attribute__((unused)))
{
    // nothing to do.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Hydraulic power in the link is -volumetric flow * potential drop.  Note that this is
///           defined as power gain, such as from a pump.  Pressure-producing links such as pumps
///           and fans will have positive power gain, and the power in dissipating links such as
///           pipes, etc. will be negative.  Since potential is in units of kPa, we must convert it
///           to Pa to express power in Watts.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidHiFiOrifice::computePower()
{
    mPower = -UnitConversion::PA_PER_KPA * mVolFlowRate * getDeltaPotential();
}

#endif

