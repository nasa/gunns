#ifndef GunnsFluidUtils_EXISTS
#define GunnsFluidUtils_EXISTS

/**
@file
@brief    GUNNS Fluid Utilities declarations

@defgroup  TSM_GUNNS_CORE_FLUID_UTILS    GUNNS Fluid Utilities
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (This class contains utility methods that are commonly used by Gunns fluid links.  Removing these
  methods from those link classes cleans up their code considerably.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsFluidUtils.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2011-06) (Initial Delivery))

@{
*/

#include "aspects/fluid/fluid/PolyFluid.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Utilities
///
/// @details  A collection of methods commonly used by fluid links.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidUtils
{
    public:
        /// @brief Enumeration of fluid mixture array types.
        enum MixtureTypes {MASS_FRACTION    = 0,
                           MOLE_FRACTION    = 1,
                           PARTIAL_PRESSURE = 2};

        /// @brief Enumeration of link automatic tuning control modes.
        enum TuningMode {OFF      = 0,
                         MASS     = 1,
                         TRUE_VOL = 2,
                         STD_VOL  = 3,
                         DELTA_T  = 4};

        /// @brief    Calculates a fluid link admittance at the current network state.
        static double computeAdmittance(const double     conductivity,
                                        const double     minLinearizationP,
                                        const PolyFluid* fluid1,
                                        const PolyFluid* fluid2,
                                        const double     exponent = 0.5);

        /// @brief    Calculates the fluid capacitance of a volume.
        static double computeCapacitance(const PolyFluid* fluid,
                                         const double     volume);

        /// @brief    Calculates a new temperature of a fluid in response to pressure change.
        static double computeIsentropicTemperature(const double     expansionScaleFactor,
                                                   const double     p1,
                                                   const double     p2,
                                                   const PolyFluid* fluid);

        /// @brief    Predicts conductivity that will produce the desired mass flow rate at the
        ///           current network state.
        static double predictConductivity(const double     mdot,
                                          const double     minLinearizationP,
                                          const PolyFluid* fluid0,
                                          const PolyFluid* fluid1,
                                          const double     exponent = 0.5);

        /// @brief    Predicts an isentropic expansion scale factor that will produce the desired
        ///           temperature drop at the current network state.
        static double predictExpansionScaleFactor(const double     dT,
                                                  const PolyFluid* fluid0,
                                                  const PolyFluid* fluid1);

        /// @brief    Assembles an index to transform between two fluids with different constituent
        ///           configuration data.
        static void   buildTransformMap(int*                             transformMap,
                                        const PolyFluidConfigData*       inConfig,
                                        const PolyFluidConfigData*       outConfig,
                                        const FluidProperties::FluidType convertToType);

        /// @brief    Uses a transform map to set a local network fluid's properties equal to an
        ///           external network's fluid.
        static void   transformState(PolyFluid*    outFluid,
                                     const double  inPressure,
                                     const double  inTemperature,
                                     const double* inMassFractions,
                                     const int*    transformMap,
                                     const double* inTcMoleFractions = 0);

        /// @brief    Computes the fluid convective heat transfer coefficient.
        static double computeConvectiveHeatTransferCoefficient(const double     flowRate,
                                                               const PolyFluid* fluid,
                                                               const double     rOverD,
                                                               const double     diameter);

        /// @brief    Computes a factor describing the laminar/transition/turbulent flow regime.
        static double computeFlowRegimeFactor(const double re);

        /// @brief    Computes the Reynolds number for the given conditions.
        static double computeReynoldsNumber(const PolyFluid* fluid,
                                            const double     velocity,
                                            const double     length);

        /// @brief    Computes the Nusselt number for forced convection in a circular pipe.
        static double computeNusseltNumber(const PolyFluid* fluid,
                                           const double     rOverD,
                                           const double     re);

        /// @brief    Computes the Darcy Friction Factor for a circular pipe.
        static double computeDarcyFrictionFactor(const double rOverD,
                                                 const double re);

        /// @brief    Performs forced convection for fluid flow through a circular pipe.
        static double computeConvectiveHeatFlux(PolyFluid*   fluid,
                                                const double flowRate,
                                                const double UA,
                                                const double wallTemperature);

        /// @brief    Performs forced convection for fluid flow through a circular pipe.
        static double computeConvectiveHeatFlux(PolyFluid*   fluid,
                                                const double flowRate,
                                                const double rOverD,
                                                const double diameter,
                                                const double surfaceArea,
                                                const double wallTemperature);

        /// @brief    Computes the conductive heat flux between two fluids.
        static double computeConductiveHeatFlux(const double     area,
                                                const PolyFluid* fluid0,
                                                const PolyFluid* fluid1,
                                                const double     length0,
                                                const double     length1);

        /// @brief    Computes the gas diffusion between two fluids.
        static double computeGasDiffusion(PolyFluid*       fluid,
                                          const double     area,
                                          const PolyFluid* fluid0,
                                          const PolyFluid* fluid1,
                                          const double     bulkFlowRate,
                                          const double     length0,
                                          const double     length1);

        /// @brief Computes the relative humidity of water in a given fluid.
        static double computeRelativeHumidityH2O(const PolyFluid* fluid);

        /// @brief Converts an array describing mole fractions of a mixture to mass fractions.
        static void convertMoleFractionToMassFraction(double*                    outMixture,
                                                      const double*              inMixture,
                                                      const PolyFluidConfigData* fluidConfig);

        /// @brief Converts an array describing mass fractions of a mixture to mole fractions.
        static void convertMassFractionToMoleFraction(double*                    outMixture,
                                                      const double*              inMixture,
                                                      const PolyFluidConfigData* fluidConfig);

        /// @brief Converts an array describing partial pressures of a mixture to mole fractions.
        static void convertPartialPressureToMoleFraction(double*                    outMixture,
                                                         const double*              inMixture,
                                                         const PolyFluidConfigData* fluidConfig);

        /// @brief Converts an array describing mole fractions of a mixture to partial pressures.
        static void convertMoleFractionToPartialPressure(double*                    outMixture,
                                                         const double*              inMixture,
                                                         const PolyFluidConfigData* fluidConfig,
                                                         const double               totalPressure);

        /// @brief Normalizes an array of fractions to sum to one.
        static bool normalizeArray(double* array, const int size);

        /// @brief Adds a fluid mixture to another fluid mixture.
        static bool mixFluidMasses(PolyFluid*   outFluid,
                                   const double outMass,
                                   PolyFluid*   inFluid,
                                   const double inMass,
                                   const int    numFluidTypes);

        /// @brief    Although never instantiated, Trick 10 requires a public destructor.
        virtual ~GunnsFluidUtils();

    protected:
        static const double RE_LAMINAR_LIMIT;    /**< (--)    Laminar flow upper limit to Reynolds number */
        static const double RE_TRANSITION_LIMIT; /**< (--)    Transition flow upper limit to Reynolds number */
        static const double RE_TURBULENT_LIMIT;  /**< (--)    Turbulent flow upper limit to Reynolds number */
        static const double SIGMA;               /**< (m2)    Molecular cross-sectional area for diatomic molecules */
        static const double LAMBDA_BASE;         /**< (kPa*m) A pre-calculated constant for gas diffusion */
        static const double ANTOINE_H2O_A[6];    /**< (--)    Antoine equation A coefficients for H2O */
        static const double ANTOINE_H2O_B[6];    /**< (--)    Antoine equation B coefficients for H2O */
        static const double ANTOINE_H2O_C[6];    /**< (--)    Antoine equation C coefficients for H2O */
        static const double ANTOINE_H2O_T[5];    /**< (K)     Antoine equation temperature bands for H2O */
        static const double ANTOINE_H2O_P[5];    /**< (kPa)   Antoine equation pressure bands for H2O */
        static const double LN10;                /**< (--)    Natural log of 10 */

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Default constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidUtils();

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidUtils(const GunnsFluidUtils& that);

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidUtils& operator =(const GunnsFluidUtils& that);
};

/// @}

#endif
