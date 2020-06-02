#ifndef GunnsFluidSimpleRocket_EXISTS
#define GunnsFluidSimpleRocket_EXISTS

/**
@file
@brief    GUNNS Fluid Simple Rocket Model declarations

@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_ROCKET_THRUSTER    Simple Rocket Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Simple Rocket link model.)

REFERENCE:
- (Hill & Peterson, "Mechanics and Thermodynamics of Propulsion", 2nd ed., 1992)

ASSUMPTIONS AND LIMITATIONS:
- ((Ideal Gas)
   (All flows are isentropic)
   (If a combustion model isn't used, inlet conditions are always at the reference configuration)
   (Combustion is complete before entering the nozzle)
   (Specific heat and specific heat ratio is constant through the nozzle)
   (Adiabatic, no heat transfer with the rocket walls)
   (Backflow results in no combustion and thrust-related calculation are incorrect)
   (Flow is assumed attached to the nozzle wall even under grossly over-expanded flow))

LIBRARY DEPENDENCY:
- ((GunnsFluidSimpleRocket.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (Initial) (2016-06))

@{
*/

#include "core/GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "properties/CombustFactory.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Simple Rocket Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Simple Rocket link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSimpleRocketConfigData : public GunnsFluidLinkConfigData
{
    public:
        double                        mThroatArea;             /**< (m2)  trick_chkpnt_io(**) Area of nozzle throat. */
        double                        mAreaRatio;              /**< (--)  trick_chkpnt_io(**) Ratio of nozzle exit to throat areas. */
        double                        mCharacteristicVelocity; /**< (m/s) trick_chkpnt_io(**) Combustion characteristic velocity at design point. */
        double                        mCombustionTemperature;  /**< (K)   trick_chkpnt_io(**) Combustion adiabatic flame temperature at design point. */
        double                        mCombustionGamma;        /**< (--)  trick_chkpnt_io(**) Combustion ratio of specific heats at design point. */
        double                        mThrustTuning;           /**< (--)  trick_chkpnt_io(**) Thrust tuning scale factor. */
        double                        mRecombinationTuning;    /**< (--)  trick_chkpnt_io(**) Tuning scale factor (0-1) for effect of chemical recombination in nozzle on thrust. */
        bool                          mNeedsIgnition;          /**< (--)  trick_chkpnt_io(**) True if ignitor is required to start combustion. */
        CombustFactory::CombustorType mCombustType ;           /**< (--)  trick_chkpnt_io(**) Optional combustion model type. */
        /// @brief  Default constructs this Simple Rocket configuration data.
        GunnsFluidSimpleRocketConfigData(
                const std::string&                  name                   = "",
                GunnsNodeList*                      nodes                  = 0,
                const double                        throatArea             = 0.0,
                const double                        areaRatio              = 0.0,
                const double                        characteristicVelocity = 0.0,
                const double                        combustionTemperature  = 0.0,
                const double                        combustionGamma        = 0.0,
                const double                        thrustTuning           = 0.0,
                const double                        recombinationTuning    = 0.0,
                const bool                          needsIgnition          = false,
                const CombustFactory::CombustorType combustType            = CombustFactory::NONE);
        /// @brief  Copy constructs this Simple Rocket configuration data.
        GunnsFluidSimpleRocketConfigData(const GunnsFluidSimpleRocketConfigData& that);
        /// @brief  Default destructs this Simple Rocket configuration data.
        virtual ~GunnsFluidSimpleRocketConfigData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSimpleRocketConfigData& operator =(const GunnsFluidSimpleRocketConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Simple Rocket Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Simple Rocket link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSimpleRocketInputData : public GunnsFluidLinkInputData
{
    public:
        bool mIgnition;   /**< (--)  trick_chkpnt_io(**) Ignition is occurring. */
        bool mCombustion; /**< (--)  trick_chkpnt_io(**) Combustion is occurring. */
        /// @brief  Default constructs this Simple Rocket input data.
        GunnsFluidSimpleRocketInputData(const bool   malfBlockageFlag  = false,
                                        const double malfBlockageValue = 0.0,
                                        const bool   ignition          = false,
                                        const bool   combustion        = false);
        /// @brief  Copy constructs this Simple Rocket input data.
        GunnsFluidSimpleRocketInputData(const GunnsFluidSimpleRocketInputData& that);
        /// @brief  Default destructs this Simple Rocket input data.
        virtual ~GunnsFluidSimpleRocketInputData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSimpleRocketInputData& operator =(const GunnsFluidSimpleRocketInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Simple Rocket Model
///
/// @details  This is a simple model of a rocket engine, including a combustion chamber and
///           de Laval nozzle.  The purpose is to model total thrust, chamber pressure and
///           combustion temperature, and flow rate.  See above for a list of assumptions &
///           limitations.
///
///           Even though a rocket engine performs a chemical reaction and we can model simple
///           reactions in links like the GunnsFluidReactor, this link doesn't actually change the
///           the fluid properties (mixture & temperature) passing through it.  This is because:
///           - real combustion is more complicated: because of the high temperatures, products
///             dissociate into other compounds, not all of which we have fluid properties for.
///           - we don't actually need a combustion products fluid object for calculation of the
///             thrust-related outputs, and
///           - we assume the output fluid is discarded to the ambient node and accurate exhaust
///             properties are not needed elsewhere in the network.
///
///           This link can use an optional combustion model for calculating outputs of combustion
///           that dynamically responds to changing inlet conditions, including the addition of a
///           purge gas such as helium.  Dissociated compounds are calculated and affect the bulk
///           fluid properties.  Shifting chemical equilibrium in the nozzle expansion is modeled,
///           as the dissociated compounds tend to re-combine as the fluid pressure & temperature
///           drop across the nozzle.  This effect is tunable between the fully 'frozen' and full
///           equilibrium limits.  Use of the combustion model is more accurate, but slower.
///
///           If the combustion model is not used, then this link doesn't care what fluids flow
///           through it -- it assumes that the fluid is the constant fuel/oxidizer mix
///           corresponding to the config data.  Flowing purge Helium gas thru the link while
///           combustion is occuring would continue combustion on the Helium mass flow as if it were
///           the same propellant mixture.
///
///           Port 0 of the link should be attached to a fluid gas node representing the combustion
///           chamber volume, and port 1 should be attached to the ambient node.
///
///           Base class attribute mVolFlowRate is not calculated or used in this link.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSimpleRocket : public GunnsFluidLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSimpleRocket);
    public:
        /// @brief  Default constructs this Simple Rocket.
        GunnsFluidSimpleRocket();
        /// @brief  Default destructs this Simple Rocket.
        virtual ~GunnsFluidSimpleRocket();
        /// @brief  Initializes this Simple Rocket with configuration and input data.
        void           initialize(const GunnsFluidSimpleRocketConfigData& configData,
                                  const GunnsFluidSimpleRocketInputData&  inputData,
                                  std::vector<GunnsBasicLink*>&           networkLinks,
                                  const int                               port0,
                                  const int                               port1);
        /// @brief  Computes the link contributions to the network system of equations.
        virtual void   step(const double dt);
        /// @brief  Computes flows through the link based on the network solution.
        virtual void   computeFlows(const double dt);
        /// @brief  Transports flows through the link based on the computeFlows solution.
        virtual void   transportFlows(const double dt);
        /// @brief  Sets the engine ignition flag for initiating combustion.
        virtual void   setIgnition(const bool ignition);
        /// @brief  Returns the net thrust produced by the engine.
        virtual double getThrust() const;
        /// @brief  Returns the engine combustion chamber pressure.
        virtual double getChamberPressure() const;
        /// @brief  Returns the engine combustion fluid temperature.
        virtual double getChamberTemperature() const;

    protected:
        double         mThroatArea;             /**<    (m2)           trick_chkpnt_io(**) Area of nozzle throat. */
        double         mAreaRatio;              /**<    (--)           trick_chkpnt_io(**) Ratio of nozzle exit to throat areas. */
        double         mCombustionTemperature;  /**<    (K)            trick_chkpnt_io(**) Combustion adiabatic flame temperature at design point. */
        double         mCombustionGamma;        /**<    (--)           trick_chkpnt_io(**) Combustion ratio of specific heats at design point. */
        double         mCombustionMWeight;      /**<    (1/mol)        trick_chkpnt_io(**) Molecular weight of combustion gas. */
        double         mThrustTuning;           /**<    (--)           trick_chkpnt_io(**) Thrust tuning scale factor. */
        double         mRecombinationTuning;    /**<    (--)           trick_chkpnt_io(**) Tuning scale factor (0-1) for effect of chemical recombination in nozzle on thrust. */
        bool           mNeedsIgnition;          /**<    (--)           trick_chkpnt_io(**) True if ignitor is required to start combustion. */
        Combust*       mCombustModel;           /**<    (--)           trick_chkpnt_io(**) Optional combustion model. */
        bool           mIgnition;               /**<    (--)                               Ignition is occurring. */
        bool           mCombustion;             /**<    (--)                               Combustion is occurring. */
        double         mDefaultConductance;     /**<    (kg/s/kPa)     trick_chkpnt_io(**) Steady-state conductance corresponding to default combustion. */
        double         mChamberPressure;        /**<    (kPa)                              Output chamber pressure. */
        double         mChamberTemperature;     /**<    (K)                                Output chamber temperature. */
        double         mChamberGamma;           /**<    (--)           trick_chkpnt_io(**) Chamber ratio of specific heats. */
        double         mChamberMWeight;         /**<    (1/mol)        trick_chkpnt_io(**) Output chamber molecular weight. */
        double         mExitMach;               /**<    (--)           trick_chkpnt_io(**) Constant Mach number at nozzle exit. */
        double         mExitPressure;           /**<    (kPa)          trick_chkpnt_io(**) Output pressure at the nozzle exit plane. */
        double         mExitTemperature;        /**<    (K)            trick_chkpnt_io(**) Exhaust temperature at the nozzle exit plane. */
        double         mExitGamma;              /**<    (--)           trick_chkpnt_io(**) Exhaust ratio of specific heats. */
        double         mExitMWeight;            /**<    (1/mol)        trick_chkpnt_io(**) Exhaust molecular weight at the nozzle exit plane. */
        double         mExitVelocity;           /**<    (m/s)          trick_chkpnt_io(**) Exhaust velocity at the nozzle exit plane. */
        double         mSpecificImpulse;        /**<    (s)            trick_chkpnt_io(**) Output specific impulse, by weight. */
        double         mThrust;                 /**<    (N)                                Output thrust. */
        double         mCharacteristicVelocity; /**<    (m/s)          trick_chkpnt_io(**) Combustion characteristic velocity. */
        double         mSystemConductance;      /**<    (kg*mol/kPa/s) trick_chkpnt_io(**) Limited conductance for the system of equations. */
        int            mCombustionCount;        /**<    (--)                               Frame count of active combustion run. */
        CombustFactory mCombustFactory;         /**< ** (--)           trick_chkpnt_io(**) Factory to create combustion model. */
        static const double mMachTolerance;     /**< ** (--)           trick_chkpnt_io(**) Mach number convergence tolerance. */
        /// @brief  Validates the initialization data of this Simple Rocket.
        void           validate(const GunnsFluidSimpleRocketConfigData& configData) const;
        /// @brief  Computes and sets the nozzle exit Mach number.
        void           updateExitMach(const double gamma);
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void   restartModel();
        /// @brief  Updates the conductance effect for the system of equations.
        void           updateConductance(const double mdot);
        /// @brief  Updates the combustion chamber properties depending on presence of combustion.
        void           updateChamber();
        /// @brief  Updates the nozzle properties and performance.
        double         updateNozzle();
        /// @brief  Builds the link contributions to the system admittance matrix.
        void           buildAdmittanceMatrix();
        /// @brief  Computes molar flux rate through the link.
        virtual void   updateFlux(const double dt, const double flux);
        /// @brief  Updates the combustion state and products.
        void           updateCombustion();
        /// @brief  Updates the rocket performance and returns the modeled mass flow rate.
        double         updateRocketPerformance();

    private:
        /// @brief  Define the number of ports this link class has.  All objects of the same link
        ///         class always have the same number of ports.  We use an enum rather than a static
        ///         const int so that we can reuse the NPORTS name and allow each class to define
        ///         its own value.
        enum {NPORTS = 2};
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSimpleRocket(const GunnsFluidSimpleRocket&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSimpleRocket& operator =(const GunnsFluidSimpleRocket&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  ignition  (--)  Flag to initiate combustion when True.
///
/// @details  Sets model ignition flag.  When set true and there is forward flow through the link,
///           combustion will initiate.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSimpleRocket::setIgnition(const bool ignition)
{
    mIgnition = ignition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (N)  The engine net thrust.
///
/// @details  Returns the engine net thrust.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSimpleRocket::getThrust() const
{
    return mThrust;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (kPa)  The combustion chamber pressure.
///
/// @details  Returns the combustion chamber pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSimpleRocket::getChamberPressure() const
{
    return mChamberPressure;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (K)  The combustion chamber fluid temperature.
///
/// @details  Returns the combustion chamber fluid (not wall) temperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSimpleRocket::getChamberTemperature() const
{
    return mChamberTemperature;
}

#endif
