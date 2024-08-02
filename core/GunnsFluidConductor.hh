#ifndef GunnsFluidConductor_EXISTS
#define GunnsFluidConductor_EXISTS

/**
@file
@brief    GUNNS Fluid Conductor Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_FLUID_CONDUCTOR    GUNNS Fluid Conductor Link
@ingroup   TSM_GUNNS_CORE_LINK_FLUID

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Conductor.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidConductor.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2011-02) (Initial Prototype)
-  (Kevin Supak)  (L3) (2011-02) (Updated to Coding Standards))

@{
*/

#include "GunnsFluidLink.hh"
#include "GunnsFluidUtils.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Conductor Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid Conductor
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidConductorConfigData : public GunnsFluidLinkConfigData
{
    public:
        double mMaxConductivity;       /**< (m2) trick_chkpnt_io(**) Link Max Conductivity */
        double mExpansionScaleFactor;  /**< (--) trick_chkpnt_io(**) Scaling for isentropic gas cooling (0-1) */
        double mPressureExponent;      /**< (--) trick_chkpnt_io(**) Exponent on the flow equation pressure term. */
        /// @brief Default constructs this Fluid Conductor configuration data.
        GunnsFluidConductorConfigData(const std::string& name                 = "",
                                      GunnsNodeList*     nodes                = 0,
                                      const double       maxConductivity      = 0.0,
                                      const double       expansionScaleFactor = 0.0,
                                      const double       pressureExponent     = 0.5);

        /// @brief Copy constructs this GUNNS Fluid Conductor configuration data.
        GunnsFluidConductorConfigData(const GunnsFluidConductorConfigData& that);

        /// @brief Default destructs this Fluid Conductor configuration data.
        virtual ~GunnsFluidConductorConfigData();

    protected:

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidConductorConfigData& operator =(const GunnsFluidConductorConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Conductor Input Data
///
/// @details  This provides a data structure for the Fluid Conductor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidConductorInputData : public GunnsFluidLinkInputData
{
    public:
        /// @brief Default constructs this Fluid Conductor input data.
        GunnsFluidConductorInputData(const bool   malfBlockageFlag  = false,
                                     const double malfBlockageValue = 0.0);

        /// @brief Copy constructs this Fluid Conductor input data.
        GunnsFluidConductorInputData(const GunnsFluidConductorInputData& that);

        /// @brief Default destructs this Fluid Conductor input data.
        virtual ~GunnsFluidConductorInputData();

    protected:

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidConductorInputData& operator =(const GunnsFluidConductorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Conductor
///
/// @details  This class creates resistance to fluid flow, for simulating pipes, valves, etc.  This
///           is done by controlling the effective conductivity of the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidConductor : public GunnsFluidLink
{
        TS_MAKE_SIM_COMPATIBLE(GunnsFluidConductor);

    public:
        /// @brief Default Constructor
        GunnsFluidConductor();

        /// @brief Default constructor
        virtual ~GunnsFluidConductor();

        /// @brief Initialize method
        void initialize(const GunnsFluidConductorConfigData& configData,
                        const GunnsFluidConductorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&        networkLinks,
                        const int                            port0,
                        const int                            port1);

        /// @brief Update function
        virtual void step(const double dt);

        /// @brief Compute Flows
        virtual void computeFlows(const double dt);

        /// @brief Transports the flows through the link between the nodes.
        virtual void transportFlows(const double dt);

        /// @brief Returns the effective conductivity of the Fluid Conductor
        double getEffectiveConductivity() const;

    protected:
        double                      mEffectiveConductivity; /**< (m2)           trick_chkpnt_io(**) Effective conductivity of the link */
        double                      mMaxConductivity;       /**< (m2)           trick_chkpnt_io(**) Link Maximum Conductivity */
        double                      mSystemConductance;     /**< (kg*mol/kPa/s) trick_chkpnt_io(**) Limited molar conductance */
        double                      mExpansionScaleFactor;  /**< (--)           trick_chkpnt_io(**) Scaling for isentropic gas cooling (0-1) */
        double                      mPressureExponent;      /**< (--)           trick_chkpnt_io(**) Exponent on the flow equation pressure term. */
        GunnsFluidUtils::TuningMode mTuneMode;              /**< (--)           trick_chkpnt_io(**) Auto-tunes the link to desired flow type */
        double                      mTuneMassFlow;          /**< (kg/s)         trick_chkpnt_io(**) The desired mass flow for link tuning */
        double                      mTuneVolFlow;           /**< (m3/s)         trick_chkpnt_io(**) The desired volumetric flow for link tuning */
        double                      mTuneDeltaT;            /**< (K)            trick_chkpnt_io(**) The desired delta-temperature for link tuning */

        /// @brief Validates the initialization of this Gunns Fluid Conductor
        void validate() const;

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief Applies an optional linearization for the admittance matrix.
        virtual double linearizeConductance();

        /// @brief Updates the admittance and source terms of the link
        virtual void buildConductance();

        /// @brief Computes molar flux rate through the link
        virtual void computeFlux();

        /// @brief Computes mPower for the aspect-specific implementation
        virtual void computePower();

        /// @brief Tunes the link conductivity to create the desired flow rate
        void tuneFlow(const double flowRate);

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 2};

        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidConductor(const GunnsFluidConductor& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidConductor& operator =(const GunnsFluidConductor& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Hydraulic power in the link is -volumetric flow * potential drop.  Note that this is
///           defined as power gain, such as from a pump.  Pressure-producing links such as pumps
///           and fans will have positive power gain, and the power in dissipating links such as
///           pipes, etc. will be negative.  Since potential is in units of kPa, we must convert it
///           to Pa to express power in Watts.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidConductor::computePower()
{
    mPower = -UnitConversion::PA_PER_KPA * mVolFlowRate * getDeltaPotential();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return double (m2) Returns the effective conductivity.
///
/// @details  Returns the effective conductivity of the Fluid Conductor.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidConductor::getEffectiveConductivity() const
{
    return mEffectiveConductivity;
}

#endif
