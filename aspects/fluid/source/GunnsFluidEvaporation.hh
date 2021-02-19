#ifndef GunnsFluidEvaporation_EXISTS
#define GunnsFluidEvaporation_EXISTS

/**
@file
@brief    GUNNS Fluid Evaporation Link declarations

@defgroup  TSM_GUNNS_FLUID_SOURCE_EVAPORATION    Evaporation Model
@ingroup   TSM_GUNNS_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Evaporation link model.)

REFERENCE:
- (https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/20110011190.pdf, "Venting of a
   Water/Inhibited Propylene Glycol Mixture in a Vacuum Environment - Characterization and
   Representative Test Results", Ungar & Erickson, NASA/JSC)
- (https://www.engineeringtoolbox.com/evaporation-water-surface-d_690.html "Evaporation from Water
   Surfaces")

ASSUMPTIONS AND LIMITATIONS:
- (The liquid pool has equal temperature as the air.)
- (Heat of phase change is small enough that it can be neglected.)
- (From Reference: Ungar & Erickson, mixture changes to the liquid pool due to evaporation of a
   single species from its solution has no effect on evaporation rate.)
- (Effect of bulk gas velocity around the liquid pool, which normally increases evaporation rate,
   is ignored.  The evaporation coefficient can be tuned to account for a particular velocity.)
- (Does not model boiling.)
- (Liquid pool displacement of the gas volume is neglected.)

LIBRARY DEPENDENCY:
- ((GunnsFluidEvaporation.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (Initial) (December 2017))

@{
*/

#include "core/GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

// Forward declarations of pointer types.
class GunnsFluidAccum;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Evaporation Model Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Liquid
///           Leak link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidEvaporationConfigData : public GunnsFluidLinkConfigData
{
    public:
        FluidProperties::FluidType mGasType;          /**< (--)      trick_chkpnt_io(**) Gas type that the liquid phase changes into. */
        double                     mEvaporationCoeff; /**< (1/s/kPa) trick_chkpnt_io(**) Evaporation rate coefficient. */
        double                     mPoolMassExponent; /**< (--)      trick_chkpnt_io(**) Exponent on pool mass contribution to evaporation rate. */
        GunnsFluidAccum*           mLiquidPoolAccum;  /**< (--)      trick_chkpnt_io(**) Pointer to the liquid pool accumulator. */
        /// @brief    Default constructs this Leak configuration data.
        GunnsFluidEvaporationConfigData(
                const std::string&               name             = "",
                GunnsNodeList*                   nodes            = 0,
                const FluidProperties::FluidType gasType          = FluidProperties::NO_FLUID,
                const double                     evaporationCoeff = 0.0,
                const double                     poolMassExponent = 0.0,
                GunnsFluidAccum*                 liquidPoolAccum  = 0);
        /// @brief    Copy constructs this Leak configuration data.
        GunnsFluidEvaporationConfigData(const GunnsFluidEvaporationConfigData& that);
        /// @brief    Default destructs this Leak configuration data.
        virtual ~GunnsFluidEvaporationConfigData();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidEvaporationConfigData& operator =(const GunnsFluidEvaporationConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Evaporation Model Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Liquid
///           Leak link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidEvaporationInputData : public GunnsFluidLinkInputData
{
    public:
        double mGasTotalPressure; /**< (kPa)  Initial gas-side total pressure. */
        double mVaporPotential;   /**< (kPa)  Initial gas-side vapor potential. */
        double mEvaporationRate;  /**< (kg/s) Initial evaporation mass rate. */
        /// @brief    Default constructs this Leak input data.
        GunnsFluidEvaporationInputData(const bool   malfBlockageFlag  = false,
                                       const double malfBlockageValue = 0.0,
                                       const double gasTotalPressure  = 0.0,
                                       const double vaporPotential    = 0.0,
                                       const double evaporationRate   = 0.0);
        /// @brief    Copy constructs this Leak input data.
        GunnsFluidEvaporationInputData(const GunnsFluidEvaporationInputData& that);
        /// @brief    Default destructs this Leak input data.
        virtual ~GunnsFluidEvaporationInputData();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidEvaporationInputData& operator =(const GunnsFluidEvaporationInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Evaporation Link
///
/// @details  This link models simple evaporation of a liquid pool into a gas node.  This link
///           interfaces with the liquid pool mass via pointer to a GunnsFluidAccum link on the
///           liquid pool node.
///
///           The liquid pool can either be in the same network or a different network than the gas
///           volume.  This allows you the option of leaking from a separate liquid network into a
///           gas network, or combining them as desired.
///
///           The liquid can be a mixture, such as Propylene Glycol & Water (GUNNS_PG40, etc.)  In
///           such cases, this models evaporation of one specific constituent out of that mixture
///           (i.e. the water).  However since GUNNS mixed liquid types have fixed mixtures that
///           can't change, we can't model removing only water from the PG mix.  We remove an
///           adjusted mass of the PG mix from the liquid node that would contain the same water
///           mass as the mass of water vapor added to the gas side, so that we conserve the total
///           water mass.  The limitation is that the mass of the other liquid constituents are not
///           conserved.  See the list of assumptions and limitations above.
///
/// @note     This link redefines the meaning of some base class terms:
///           - mPower is the heat of evaporation
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidEvaporation : public GunnsFluidLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidEvaporation);
    public:
        /// @brief Default constructs this Leak.
        GunnsFluidEvaporation();
        /// @brief Default destructs this Leak.
        virtual ~GunnsFluidEvaporation();
        /// @brief Initializes this Leak with configuration and input data.
        void initialize(const GunnsFluidEvaporationConfigData& configData,
                        const GunnsFluidEvaporationInputData&  inputData,
                        std::vector<GunnsBasicLink*>&          networkLinks,
                        const int                              port0,
                        const int                              port1);
        /// @brief Computes the contributions to the network solution.
        virtual void step(const double dt);
        /// @brief Computes the flows through the link.
        virtual void computeFlows(const double dt);
        /// @brief Transports the flows through the link.
        virtual void transportFlows(const double dt);

    protected:
        FluidProperties::FluidType mGasType;          /**< *o (--)      trick_chkpnt_io(**) Gas type that the liquid phase changes into. */
        double                     mEvaporationCoeff; /**<    (1/s/kPa) trick_chkpnt_io(**) Evaporation rate coefficient. */
        double                     mPoolMassExponent; /**<    (--)      trick_chkpnt_io(**) Exponent on pool mass contribution to evaporation rate. */
        GunnsFluidAccum*           mLiquidPoolAccum;  /**< ** (--)      trick_chkpnt_io(**) Pointer to the liquid pool accumulator. */
        double                     mGasTotalPressure; /**<    (kPa)                         Gas-side total pressure. */
        double                     mGasMWeight;       /**< ** (1/mol)   trick_chkpnt_io(**) Constant molecular weight of the gas type. */
        PolyFluid*                 mEvaporationFluid; /**< *o (--)      trick_chkpnt_io(**) Evaporated fluid input to the gas node. */
        double                     mLiquidPoolMass;   /**<    (kg)      trick_chkpnt_io(**) Current mass of the liquid pool. */
        static const double        mSpringCoeff1;     /**< ** (--)      trick_chkpnt_io(**) Constant for liquid pool accumulator spring coefficient 1. */
        /// @brief Validates the initialization of this link.
        void validate(const GunnsFluidEvaporationConfigData& configData,
                      const GunnsFluidEvaporationInputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Checks for valid implementation-specific port node assignment.
        virtual bool checkSpecificPortRules(const int port, const int node) const;
        /// @brief Updates gas-side pressures.
        void updateVaporPotential();
        /// @brief Updates liquid side parameters and evaporation rate.
        void updateVaporRate(const double dt);
        /// @brief Updates heat of evaporation.
        void updatePower();

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 2};
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidEvaporation(const GunnsFluidEvaporation&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidEvaporation& operator =(const GunnsFluidEvaporation&);
};

/// @}

#endif
