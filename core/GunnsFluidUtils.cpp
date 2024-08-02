/**
@file
@brief    GUNNS Fluid Utilities implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Utility methods commonly used by fluid links.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     ()

LIBRARY DEPENDENCY:
    ((aspects/fluid/fluid/PolyFluid.o))

 PROGRAMMERS:
    ((Jason Harvey) (L3) (2011-06) (Initial Delivery))
*/

#include "math/UnitConversion.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "GunnsFluidUtils.hh"

/// @details Reference: TBD
const double GunnsFluidUtils::RE_LAMINAR_LIMIT = 2300.0;
/// @details Reference: TBD
const double GunnsFluidUtils::RE_TRANSITION_LIMIT = 4000.0;
/// @details Reference: TBD. If the Reynolds number gets too large we will get a divide by zero in
///          calculation of the Darcy Friction Factor.
const double GunnsFluidUtils::RE_TURBULENT_LIMIT = 1E8;
/// @details See derivations in the comments of the computeGasDiffusion method.
const double GunnsFluidUtils::SIGMA            = 3.0E-19;
/// @details See derivations in the comments of the computeGasDiffusion method.
const double GunnsFluidUtils::LAMBDA_BASE      = UnitConversion::BOLTZMANN_CONST /
                                                 (UnitConversion::PA_PER_KPA *
                                                  GunnsFluidUtils::SIGMA);
/// @details Reference: NIST (http://webbook.nist.gov/)
const double GunnsFluidUtils::ANTOINE_H2O_A[] = {  3.55959,   5.08354,   5.07680,   5.20389,   5.40221,  4.65430};
/// @details Reference: NIST (http://webbook.nist.gov/)
const double GunnsFluidUtils::ANTOINE_H2O_B[] = {  643.748,  1663.125,  1659.793,  1733.926,  1838.675, 1435.264};
/// @details Reference: NIST (http://webbook.nist.gov/)
const double GunnsFluidUtils::ANTOINE_H2O_C[] = { -198.043,   -45.622,   -45.854,   -39.485,   -31.737,  -64.848};
/// @details Reference: NIST (http://webbook.nist.gov/)
const double GunnsFluidUtils::ANTOINE_H2O_T[] = {    379.0,     364.0,     334.0,     304.0,     273.0};
/// @details Reference: NIST (http://webbook.nist.gov/)
const double GunnsFluidUtils::ANTOINE_H2O_P[] = {124.40375, 72.409663, 20.727646, 4.4542637, 0.6041849};
/// @details Natural log of 10
const double GunnsFluidUtils::LN10 = log(10.0);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] conductivity      (m2)           Link effective conductivity.
/// @param[in] minLinearizationP (kPa)          Minimum link delta-pressure for linearization.
/// @param[in] fluid0            (--)           Pointer to node content fluid at a link port
///                                                 (order doesn't matter).
/// @param[in] fluid1            (--)           Pointer to node content fluid at a link port
///                                                 (order doesn't matter).
/// @param[in] exponent          (--)           Exponent on the (rho/dP) term, default = 1/2.
///
/// @return                      (kgmol/kPa/s)  Linearized molar admittance.
///
/// @note     The minLinearizationP argument should be the calling link's mMinLinearizationPotential
///           attribute.
///
/// @details  Calculates and returns the link admittance as a function of conductivity and fluid
///           properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidUtils::computeAdmittance(const double     conductivity,
                                          const double     minLinearizationP,
                                          const PolyFluid* fluid0,
                                          const PolyFluid* fluid1,
                                          const double     exponent)
{
    /// \verbatim
    /// Linearized fluid molar admittance, which relates the link molar flow rate to the pressure
    /// drop across the link.  The mass flow rate is:
    ///                                                   X
    ///                          mdot = G * (avg_rho * dP)
    ///
    /// G       = Conductivity                                 (m2)
    /// avg_rho = average density across the link              (kg/m3)
    /// dP      = delta-Pressure across the link               (Pa)
    /// X       = exponent on the (rho/dP) term (normally 1/2) (--)
    ///
    /// This is the momentum equation for steady one-dimensional fluid flow, ignoring body forces,
    /// viscous shear forces, and momentum exchange with the outside.  Reference:
    /// John D. Anderson, Jr., Modern Compressible Flow With Historical Perspective, 2nd Ed., 1990.
    /// (Equation 3.5).  Note that only exponent X = 1/2 is valid, from a units perspective.  But we
    /// allow other exponents because they can be useful.  In particular, a value of 1 is useful for
    /// modeling laminar flow.  We limit to allowed range 0.5 <= X <= 1.
    ///
    /// Then this is converted to molar flow rate and linearized by the delta-pressure:
    ///
    ///                          A = mdot / dP / avg_MW,
    ///
    /// A       = Admittance                                   (kgmol/kPa/s)
    /// dP      = delta-Pressure across the link               (kPa)
    /// avg_MW  = average molecular weight across the link     (kg/kgmol)
    ///
    /// The dP term is limited to a minimum value (the minLinearizationP argument) for two reasons:
    /// - It keeps A from jumping to zero when link delta-pressure is zero.  This improves onset
    ///   of flow when delta-pressure changes away from zero.
    /// - It reduces noise in A at low link delta-pressure.  This reduces noise in the network
    ///   solution.
    /// \endverbatim
    const double avgDensity = 0.5 * (fluid0->getDensity() + fluid1->getDensity());

    /// - The network Ground node always has zero molecular weight, which we can't use when
    ///   converting from mass to molar flow rate.  We use the average molecular weight of the 2
    ///   nodes, except when one is the Ground node, in which case we use the other node's value.
    ///   When both nodes are the Ground node, then the average weight is zero, handled below.
    const double MW0 = fluid0->getMWeight();
    const double MW1 = fluid1->getMWeight();
    double useMW = 0.0;
    if (MW0 < DBL_EPSILON) {
        useMW = MW1;
    } else if (MW1 < DBL_EPSILON) {
        useMW = MW0;
    } else {
        useMW = 0.5 * (MW0 + MW1);
    }

    /// - Limited delta-pressure.
    const double dP_lin = std::max(minLinearizationP,
                                   fabs(fluid0->getPressure() - fluid1->getPressure()));
    const double dP_Pa  = UnitConversion::PA_PER_KPA * dP_lin;

    /// - Calculate admittance.
    if (dP_lin > DBL_EPSILON && useMW > DBL_EPSILON) {
        double mdot = conductivity; // mass flow rate (kg/s), will complete below
        const double limitExp = MsMath::limitRange(0.5, exponent, 1.0);
        if (0.5 == limitExp) {
            mdot *= std::sqrt(avgDensity * dP_Pa);
        } else if (1.0 == limitExp) {
            mdot *= avgDensity * dP_Pa;
        } else {
            mdot *= std::pow(avgDensity * dP_Pa, limitExp);
        }
        /// - Convert mdot (kg/s) to mole rate (kgmol/s) and linearize to admittance (kgmol/s/kPa).
        return mdot / dP_lin / useMW;
    }
    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fluid  (--) The fluid to compute the capacitance for
/// @param[in] volume (m3) The volume to compute the capacitance for
///
/// @return   (kg*mol/kPa) Node capacitance limited to >= 0
///
/// @details  Computes the fluid capacitance of the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidUtils::computeCapacitance(const PolyFluid* fluid,
                                           const double     volume)
{
    /// \verbatim
    /// The system of equations expects capacitance in units of kgmol/kPa, it represents the moles
    /// (we use kilogram-moles) of fluid added to the node to raise the node pressure by 1 kPa:
    ///
    ///                             c = (V/MW) * (del-rho/del-P),
    ///
    /// c   = fluid capacitance (kgmol/kPa)
    /// V   = volume            (m3)
    /// MW  = molecular weight  (kg/kgmol)
    /// rho = mass density      (kg/m3)
    /// P   = pressure          (kPa)
    ///
    /// Units:  c = (m3*kgmol/kg) * (kg/m3/kPa) = kgmol/kPa
    ///
    /// del-rho is gotten by calculating densities at small increments of pressure, holding
    /// temperature constant.  Divide by zero is protected for by limiting pressure.  The molecular
    /// weight is limited in the PolyFluid class.  Gunns uses the return value from this method
    /// rather than mCapacitance; mCapacitance is kept for insight.
    /// \endverbatim
    double p = fluid->getPressure();
    if (p < static_cast<double>(FLT_EPSILON)) {
        p = static_cast<double>(FLT_EPSILON);
    }
    const double p1 = p * 0.999;
    const double p2 = p * 1.001;

    double capacitance = (fluid->computeDensity(fluid->getTemperature(), p2) -
                          fluid->computeDensity(fluid->getTemperature(), p1))
                       * volume / (fluid->getMWeight() * (p2 - p1));

    /// - Negative capacitance tends to blow up the network solution, so if somehow the fluid
    ///   properties are so screwed up as to give a negative capacitance from the above equation,
    ///   constrain it to be positive before returning.
    return (std::max(0.0, capacitance));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] expansionScaleFactor (--)  Scale factor for the isentropic cooling effect.
/// @param[in] p1                   (kPa) Initial fluid pressure.
/// @param[in] p2                   (kPa) Final fluid pressure.
/// @param[in] fluid                (--)  Pointer to fluid undergoing expansion.
///
/// @return                         (K)   Final temperature of the fluid after isentropic expansion.
///
/// @note     The expansionScaleFactor argument should be the calling link's mExpansionScaleFactor
///           attribute.
///
/// @details  Calculates and returns the final temperature of a fluid after isentropic expansion.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidUtils::computeIsentropicTemperature(const double     expansionScaleFactor,
                                                     const double     p1,
                                                     const double     p2,
                                                     const PolyFluid* fluid)
{
    /// - The default output temperature is unchanged from the input (which happens for liquids).
    double finalT = fluid->getTemperature();

    /// - For gases, proceed with the new temperature calculation.
    if ( (fluid->getPhase() == FluidProperties::GAS) && (p1 > 0.0) ) {

        /// - The pressure ratio is relative to the initial pressure (p1) and the final pressure
        ///   (p2).
        const double pressureRatio = p2/p1;

        /// - Use the isentropic flow relationship between pressure and temperature to calculate the
        ///   exit temperature of the gas:
        ///
        /// -                      (T1/T0) = (P1/P0) ^ ((gamma-1)/gamma)
        ///
        /// - Reference: John D. Anderson, Jr., Modern Compressible Flow With Historical
        ///              Perspective, 2nd Ed., 1990. (Equation 3.29)
        const double gamma   = fluid->getAdiabaticIndex();
        const double supplyT = fluid->getTemperature();
        if (pressureRatio > DBL_EPSILON && gamma > DBL_EPSILON) {
            finalT = supplyT * pow(pressureRatio,((gamma-1.0)/gamma));
            finalT = supplyT + expansionScaleFactor * (finalT - supplyT);
        }
    }

    return finalT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  mdot               (kg/s)  Desired mass flow rate across the link.
/// @param[in]  minLinearizationP  (--)    Minimum link delta-pressure for linearization.
/// @param[in]  fluid0             (--)    Pointer to node content fluid at a link port
///                                            (order doesn't matter).
/// @param[in]  fluid1             (--)    Pointer to node content fluid at a link port
///                                            (order doesn't matter).
/// @param[in]  exponent           (--)    Exponent on the (rho/dP) term, default = 1/2.
///
/// @return                        (--)    Predicted effective conductivity.
///
/// @note     This method returns zero if the link delta-pressure is < DBL_EPSILON.
///
/// @note     The minLinearizationP argument should be the calling link's mMinLinearizationPotential
///           attribute.
///
/// @details  Predicts the effective conductivity needed to produce the desired mass flow rate
///           across the link at this instant in time using current network conditions.  This uses
///           the inverse of the equation in the computeAdmittance method.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidUtils::predictConductivity(const double     mdot,
                                            const double     minLinearizationP,
                                            const PolyFluid* fluid0,
                                            const PolyFluid* fluid1,
                                            const double     exponent)
{
    /// - Only compute conductivity if the current link delta-pressure is > DBL_EPSILON.  Otherwise,
    ///   return zero.
    double conductivity = 0.0;
    double dP = fabs(fluid0->getPressure() - fluid1->getPressure());
    if (dP > DBL_EPSILON) {
        const double avgDensity = 0.5 * (fluid0->getDensity() +
                                         fluid1->getDensity());

        if (dP < minLinearizationP) {
            dP = minLinearizationP;
        }

        /// - Only continue if density is > DBL_EPSILON, else return zero conductivity.
        if (avgDensity > DBL_EPSILON) {
            conductivity = fabs(mdot);
            const double limitExp = MsMath::limitRange(0.5, exponent, 1.0);
            if (0.5 == limitExp) {
                conductivity /= std::sqrt(UnitConversion::PA_PER_KPA * dP * avgDensity);
            } else if (1.0 == limitExp) {
                conductivity /= (UnitConversion::PA_PER_KPA * dP * avgDensity);
            } else {
                conductivity /= std::pow(UnitConversion::PA_PER_KPA * dP * avgDensity, limitExp);
            }
        }
    }
    return conductivity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  deltaTemperature  (K)   Desired temperature drop across the link.
/// @param[in]  fluid0            (--)  Pointer to node content fluid at a link port
///                                         (order doesn't matter).
/// @param[in]  fluid1            (--)  Pointer to node content fluid at a link port
///                                         (order doesn't matter).
///
/// @return                       (--)  Predicted expansion scale factor (0-1).
///
/// @note     The expansion scale factor is only used when the link has an internal fluid.
///
/// @note     This method returns zero for liquids.
///
/// @details  Predicts the expansion scale factor needed to produce the desired temperature drop
///           across the link at this instant in time using current network conditions.  The
///           resulting scale factor will be limited between 0 and 1.  This uses the inverse of the
///           equation in the computeIsentropicTemperature method.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidUtils::predictExpansionScaleFactor(const double     deltaTemperature,
                                                    const PolyFluid* fluid0,
                                                    const PolyFluid* fluid1)
{
    double tunedFactor = 0.0;

    /// - Protect for divide by zero in pressure
    double p0 = std::max(DBL_EPSILON, fluid0->getPressure());
    double p1 = std::max(DBL_EPSILON, fluid1->getPressure());

    /// - Determine inlet fluid conditions and pressure ratio based on flow direction (pressure
    ///   gradient).  Divide by zero is protected by fluids never return zero adiabatic index
    ///   for a gas, and we check for pressures to be unequal before proceeding.
    if (p0 >= p1 && fluid0->getPhase() == FluidProperties::GAS) {
        const double supplyT = fluid0->getTemperature();
        const double gamma   = fluid0->getAdiabaticIndex();
        tunedFactor = -fabs(deltaTemperature) /
                      (supplyT * (pow((p1/p0), (gamma - 1.0) / gamma) - 1.0));
    }
    else if (p1 >= p0 && fluid1->getPhase() == FluidProperties::GAS) {
        const double supplyT = fluid1->getTemperature();
        const double gamma   = fluid1->getAdiabaticIndex();
        tunedFactor = -fabs(deltaTemperature) /
                      (supplyT * (pow((p0/p1), (gamma - 1.0) / gamma) - 1.0));
    }

    return MsMath::limitRange( 0.0, tunedFactor, 1.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out]   transformMap   (--)  The output constituent transformation index map.
/// @param[in]    inConfig       (--)  Pointer to configuration data for the input fluid.
/// @param[in]    outConfig      (--)  Pointer to configuration data for the output fluid.
/// @param[in]    convertToType  (--)  The fluid type to convert missing constituents into.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  This method builds an index to map the constituent configuration of an input fluid to
///           the config of the output fluid.  This is used to set up the interface between two
///           networks when they have a different fluid config.  The inConfig argument is the fluid
///           config of the external network, and outConfig is the config of our local network.
///           Think in terms of a fluid coming into our network from the external network.  inFluid
///           describes the config of the incoming external fluid, and outFluid describes the config
///           of the output of this process, which goes out to our local network.
///
///           The convertToType argument describes how to map constituents in the external network
///           (inConfig) that aren't present in our local network (outConfig).  The options are any
///           of the fluid constituents in outConfig (FluidProperties::GUNNS_N2, etc) or none of
///           them (FluidProperties::NO_FLUID).  An exception is thrown if none of these options are
///           specified.
///
///           If a local fluid type is specified, then any types in the external fluid that are not
///           present in our local network are converted into the specified type.  For instance, if
///           inConfig consists of (GUNNS_O2, GUNNS_N2, GUNNS_CO2) and outConfig consists of
///           (GUNNS_N2, GUNNS_O2) and convertToType is GUNNS_N2, then the CO2 from the external
///           network is converted into an equal number of moles of N2 when it enters our local
///           network.  Note that because of the difference in molecular weights, this means that
///           mass is NOT conserved during this transformation.  Moles are conserved instead, to
///           improve pressure stability between the networks.
///
///           If NO_FLUID is specified, then any types in the external fluid not present in our
///           local network are converted to ALL of the remaining types that are present, in the
///           same proportions that those types have in the mixture.  For instance, if in the above
///           example the external fluid's mixture was (0.2, 0.79, 0.01) and convertToType were
///           NO_FLUID, then the CO2 is converted into 0.2/0.99 parts O2 and 0.79/0.99 parts N2.
///           Again, moles are conserved, not mass.
///
///           This method just builds the transformation map.  The transformState method does the
///           actual fluid conversion in run-time.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidUtils::buildTransformMap(int*                             transformMap,
                                        const PolyFluidConfigData*       inConfig,
                                        const PolyFluidConfigData*       outConfig,
                                        const FluidProperties::FluidType convertToType)
{
    /// - This is the location in the local network's fluid config (outConfig) of the type to be
    ///   converted to, or -1 to specify NO_FLUID.
    int convertToIndex = -1;

    if (convertToType != FluidProperties::NO_FLUID) {

        /// - Find the convertToType's location in outConfig.
        bool found = false;
        for (int i = 0; i < outConfig->mNTypes; ++i) {
            if (convertToType == outConfig->mTypes[i]) {
                found = true;
                convertToIndex = i;
                break;
            }
        }

        /// - Throw an exception if convertToType is not present in outConfig or NO_FLUID.
        if (!found) {
            TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Illegal convertToType argument.",
                            TsInitializationException, "Invalid Configuration Data", "Unknown");
        }
    }

    /// - The first index in transformMap is the size of the input fluid.
    transformMap[0] = inConfig->mNTypes;
    bool emptyMap = true;

    /// - Load the transform map array.  Loop through the input configuration constituents and map
    ///   their locations to the output config.
    for (int i = 0; i < inConfig->mNTypes; ++i) {

        // The -99 is a temporary value to indicate that this constituent has not yet been mapped.
        transformMap[i+1] = -99;
        for (int j = 0; j < outConfig->mNTypes; ++j) {
            if (inConfig->mTypes[i] == outConfig->mTypes[j]) {
                transformMap[i+1] = j;
                break;
            }
        }

        /// - Any types that have not been found in the output config are converted to the specified
        ///   type (again, -1 denotes NO_FLUID).
        if (-99 == transformMap[i+1]) {
            transformMap[i+1] = convertToIndex;
        }

        if (transformMap[i+1] >= 0.0) {
            emptyMap = false;
        }
    }

    /// - Throw an exception if the transform map is empty, meaning that none of the inConfig's
    ///   fluids have been mapped to outConfig.  In this case, the interface should have been set up
    ///   as a pressure-only interface.
    if (emptyMap) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Empty transform map.",
                        TsInitializationException, "Invalid Configuration Data", "Unknown");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] outFluid          (--)  Pointer to converted outgoing fluid.
/// @param[in]  inPressure        (kPa) Incoming fluid pressure.
/// @param[in]  inTemperature     (K)   Incoming fluid temprature.
/// @param[in]  inMassFractions   (--)  Incoming fluid mass fractions to be converted.
/// @param[in]  transformMap      (--)  Constituent transformation index map.
/// @param[in]  inTcMoleFractions (--)  Optional pointer to incoming trace compounds mole fractions.
///
/// @return     void
///
/// @throws   TsOutOfBoundsException
///
/// @details  This method converts the data describing a fluid in an external network into the
///           outFluid.  outFluid takes the incoming external fluid's pressure and temperature, but
///           the mass fractions are re-mapped into outFluid's constituents, using the transformMap.
///           This is used in external network interfaces between two networks that can have the
///           same or different fluid configurations.  See the comments for the buildTransformMap
///           method above.
///
///           We use mass fractions here, but we could have used either mass or mole fractions for
///           the purpose of this method.  The actual conservation of moles of fluid is done in the
///           GunnsFluidExternalSupply & Demand links, which transfer molar flux between them, not
///           mass rate.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidUtils::transformState(PolyFluid*    outFluid,
                                     const double  inPressure,
                                     const double  inTemperature,
                                     const double* inMassFractions,
                                     const int*    transformMap,
                                     const double* inTcMoleFractions)
{
    /// - Define a large enough array to hold all possible constituents in the output fluid.
    double massFraction[FluidProperties::NO_FLUID];
    double massFractionSum = 0.0;

    /// - Zero out the mass fractions.
    for (int i = 0; i < FluidProperties::NO_FLUID; ++i) {
        massFraction[i] = 0.0;
    }

    /// - Map the incoming mass fractions to the outFluid's mass fractions using the transform map.
    ///   If the inFluid type is mapped to -1, then it is left out.
    for (int i = 1; i <= transformMap[0]; ++i) {
        if (transformMap[i] >= 0) {
            massFraction[transformMap[i]] += inMassFractions[i-1];
            massFractionSum               += inMassFractions[i-1];
        }
    }

    /// - Since some inFluid constituents may not have a home in outFluid, the mass fractions array
    ///   may not sum to 1 at this time, so normalize it.
    if (massFractionSum > 0.0) {
        for (int i = 0; i < FluidProperties::NO_FLUID; ++i) {
            massFraction[i] /= massFractionSum;
        }
        outFluid->setMassAndMassFractions(0.0, massFraction);
    }

    /// - Set the fluid properties that Gunns needs.
    outFluid->setPressure(inPressure);
    outFluid->setTemperature(inTemperature);

    /// - Copy trace compounds mole fractions, if there is an incoming mole fractions array, and if
    ///   the output fluid has a trace compounds object.
    if (inTcMoleFractions) {
        GunnsFluidTraceCompounds* tc = outFluid->getTraceCompounds();
        if (tc) {
            tc->setMoleFractions(inTcMoleFractions);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flowRate  (kg/s)    Mass flow rate.
/// @param[in]  fluid     (--)      Pointer to internal fluid.
/// @param[in]  rOverD    (--)      Ratio of pipe inner surface roughness to inner diameter.
/// @param[in]  diameter  (m)       Pipe inner diameter.
///
/// @returns    double    (W/m2/K)  Convective heat transfer coefficient.
///
/// @details  Computes the fluid convective heat transfer coefficient for forced convection in a
///           pipe with circular cross-section.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidUtils::computeConvectiveHeatTransferCoefficient(const double     flowRate,
                                                                 const PolyFluid* fluid,
                                                                 const double     rOverD,
                                                                 const double     diameter)
{
    double result     = 0.0;
    const double mdot = fabs(flowRate);

    if (mdot > DBL_EPSILON and diameter > DBL_EPSILON) {
        /// - Determine mean velocity through a circular pipe section.
        const double vm = mdot / fluid->getDensity() / UnitConversion::PI_OVER_4 / diameter / diameter;

        /// - Determine Reynolds number, limited to prevent divide by zero and overflows in
        ///   subsequent ts-utils.
        const double re = MsMath::limitRange(DBL_EPSILON,
                                           GunnsFluidUtils::computeReynoldsNumber(fluid, vm, diameter),
                                           RE_TURBULENT_LIMIT);

        /// - Calculate the fluid conductivity over characteristic length.
        const double kOverL = fluid->getThermalConductivity() / diameter;

        /// - Calculate Nusselt number.
        result = kOverL * GunnsFluidUtils::computeNusseltNumber(fluid, rOverD, re);
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  re  (--)  Reynolds number of the flow.
///
/// @returns  double  (--)  Regime interpolating factor (0-1).
///
/// @details  Computes a factor for use in interpolating between laminar and turbulent endpoints for
///           the transition flow regime.  Returns zero for all Reynolds numbers in the laminar flow
///           regime, 1 for all points in turbulent flow, and between 0-1 for transition.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidUtils::computeFlowRegimeFactor(const double re)
{
    return MsMath::limitRange(0.0, (re - RE_LAMINAR_LIMIT) / (RE_TRANSITION_LIMIT - RE_LAMINAR_LIMIT), 1.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  fluid     (--)   Pointer to the fluid.
/// @param[in]  velocity  (m/s)  Mean fluid flow velocity.
/// @param[in]  length    (m)    Characteristic length.
///
/// @returns  double  (--)  Reynolds number
///
/// @details  Calculates Reynolds number given fluid density and dynamic viscosity (properties of
///           the given fluid), flow velocity and characteristic length.  Protection for divide by
///           zero is given since viscosity is never allowed to be zero in the fluid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidUtils::computeReynoldsNumber(const PolyFluid* fluid,
                                              const double     velocity,
                                              const double     length)
{
    return fluid->getDensity() * velocity * length / fluid->getViscosity();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  fluid   (--)  Pointer to the fluid.
/// @param[in]  rOverD  (--)  The ratio of pipe surface roughness to inner diameter.
/// @param[in]  re      (--)  Reynolds number of the flow.
///
/// @returns  double  (--)  Nusselt number.
///
/// @details  Calculates the Nusselt number for forced convection in a pipe with circular cross-
///           section, for 0 < Re < 1E8.  Laminar flow is assumed to be fully developed with uniform
///           surface temperature, and returns 3.66. Turbulent flow uses the Gnielinski correlation.
///           For transition flow, the result is linearly interpolated between the max-Re laminar
///           and min-Re turbulent points.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidUtils::computeNusseltNumber(const PolyFluid* fluid,
                                             const double     rOverD,
                                             const double     re)
{
    /// - Determine from Reynolds number if the flow regime is laminar, turbulent, or in
    ///   transition.
    const double regimeFactor = GunnsFluidUtils::computeFlowRegimeFactor(re);

    /// - First initialize the laminar flow result, for circular tubes with uniform surface
    ///   temperature.
    double nusselt = 3.66;

    if (regimeFactor > 0.0) {
        /// - For turbulent/transition flow, get a turbulent result using the Gnielinksi Nusselt
        ///   number correlation.
        const double ret = std::max(re, RE_TRANSITION_LIMIT);
        const double darcy8 = GunnsFluidUtils::computeDarcyFrictionFactor(rOverD, ret) / 8.0;
        const double pr = MsMath::limitRange(0.0, fluid->getPrandtlNumber(), 1.0/DBL_EPSILON);
        const double turbulent = darcy8 * (ret - 1000.0) * pr /
                (1.0 + 12.7 * sqrt(darcy8) * (pow(pr, (2.0 / 3.0)) - 1.0));

        /// - For transition regime, linearly interpolate between the maximum Re laminar result
        ///   and the minimum Re turbulent value.
        nusselt = nusselt * (1.0 - regimeFactor) + turbulent * regimeFactor;
    }
    return nusselt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  rOverD  (--)  The ratio of pipe surface roughness to inner diameter.
/// @param[in]  re      (--)  Reynolds number of the flow.
///
/// @returns  double  (--)  Darcy Friction Factor.
///
/// @details  Calculates the Darcy Friction Factor for a pipe with circular cross-section, for
///           0 < Re < 1E8.  Laminar flow returns 64/Re.  Turbulent flow uses an approximation to
///           the Colebrook-White Equation, including surface roughness.  For transition flow, the
///           result is linearly interpolated between the max-Re laminar and min-Re turbulent
///           points.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidUtils::computeDarcyFrictionFactor(const double rOverD,
                                                   const double re)
{
    /// - Determine from Reynolds number if the flow regime is laminar, turbulent, or in
    ///   transition.
    const double regimeFactor = GunnsFluidUtils::computeFlowRegimeFactor(re);

    /// - First initialize the laminar flow result.
    double darcy = 64.0 / MsMath::limitRange(DBL_EPSILON, re, RE_LAMINAR_LIMIT);

    if (regimeFactor > 0.0) {
        /// - For turbulent/transition flow, get a turbulent result using Serghide's
        ///   approximation to the Colebrook-White Equation for turbulent flow.  Accurate to
        ///   0.14% from 4000 < re < 1E8, and Reynolds number is limited in that range for this
        ///   equation.
        const double rod = MsMath::limitRange(0.0, rOverD, 0.5) / 3.7;
        const double ret = std::max(re, RE_TRANSITION_LIMIT);
        const double a   = log10(rod + 12.0 / ret);
        const double b   = log10(rod - 5.02 * a / ret);
        const double c   = log10(rod + 10.04 * a * b / ret);
        const double turbulent = 0.25 / pow(a - pow(b - a, 2.0) / (c - 2.0 * b + a), 2.0);

        /// - For transition regime, linearly interpolate between the maximum Re laminar result
        ///   and the minimum Re turbulent value.
        darcy = darcy * (1.0 - regimeFactor) + turbulent * regimeFactor;
    }
    return darcy;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out]  fluid            (--)    Pointer to the fluid moving through the pipe.
/// @param[in]      flowRate         (kg/s)  The mass flow rate of the fluid through the pipe.
/// @param[in]      UA               (W/K)   Product of heat transfer coefficient & surface area.
/// @param[in]      wallTemperature  (K)     Pipe wall temperature, assumed constant at all points.
///
/// @returns  double  (W)  Heat flux from fluid to pipe wall.
///
/// @details  Computes the convective heat flux from a fluid moving through a pipe and updates the
///           fluid temperature resulting from the heat flux.
///
/// @note     This is an overloaded method.  This version receives a known heat transfer coefficient
///           times surface area product (commonly called UA).
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidUtils::computeConvectiveHeatFlux(PolyFluid*   fluid,
                                                  const double flowRate,
                                                  const double UA,
                                                  const double wallTemperature)
{
    const double mDot = fabs(flowRate);
    double heatFlux   = 0.0;

    /// - Skip if mass flow rate or coefficient is too small.
    if (mDot > DBL_EPSILON and UA > DBL_EPSILON) {

        /// - Inlet fluid enthalpy.
        const double inFluidH  = fluid->getSpecificEnthalpy();

        /// - Compute the delta temperature coefficient, limited to a range that will not overflow
        ///   in the exp function below.
        const double Cp        = fluid->getSpecificHeat();
        const double limitUA   = std::min(UA, 100.0 * mDot * Cp);
        const double UAmDotCp  = limitUA / mDot / Cp;

        /// - Get the inlet fluid temp.
        const double inFluidT  = fluid->getTemperature();

        /// - Compute the outlet fluid temperature.
        const double outFluidT = wallTemperature + exp(-UAmDotCp) * (inFluidT - wallTemperature);

        /// - Update the internal fluid temperature.
        fluid->setTemperature(outFluidT);

        /// - Compute the heat transfer from the fluid to the wall.
        const double outFluidH = fluid->getSpecificEnthalpy();
        heatFlux               = mDot * (inFluidH - outFluidH);
    }
    return heatFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out]  fluid            (--)    Pointer to the fluid moving through the pipe.
/// @param[in]      flowRate         (kg/s)  The mass flow rate of the fluid through the pipe.
/// @param[in]      rOverD           (--)    The ratio of pipe surface roughness to inner diameter.
/// @param[in]      diameter         (m)     Hydraulic (inner) diameter of the pipe.
/// @param[in]      surfaceArea      (m2)    Total inner surface area of the pipe length.
/// @param[in]      wallTemperature  (K)     Pipe wall temperature, assumed constant at all points.
///
/// @returns  double  (W)  Heat flux from fluid to pipe wall.
///
/// @details  Computes the convective heat flux from a fluid moving through a pipe and updates the
///           fluid temperature resulting from the heat flux.
///
/// @note     This is an overloaded method.  This version calculates its own heat transfer
///           coefficient from the given pipe geometry.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidUtils::computeConvectiveHeatFlux(PolyFluid*   fluid,
                                                  const double flowRate,
                                                  const double rOverD,
                                                  const double diameter,
                                                  const double surfaceArea,
                                                  const double wallTemperature)
{
    /// - Find UA (W/K), the product of heat transfer coefficient (W/m2/K) and surface area (m2).
    const double UA = surfaceArea * GunnsFluidUtils::computeConvectiveHeatTransferCoefficient(
                                                      flowRate, fluid, rOverD, diameter);

    /// - Perform the actual heat flux using the calculated UA.
    return computeConvectiveHeatFlux(fluid, flowRate, UA, wallTemperature);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  area     (m2)  Open cross-sectional area of the interface.
/// @param[in]  fluid0   (--)  Pointer to port 0 node content fluid.
/// @param[in]  fluid1   (--)  Pointer to port 1 node content fluid.
/// @param[in]  length0  (m)   Distance from the interface to center of node 0 volume.
/// @param[in]  length1  (m)   Distance from the interface to center of node 1 volume.
///
/// @return              (W)   Conductive heat flux from port 0 to port 1.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Computes the conductive heat flux between the fluid contents of the linked nodes,
///           given the geometry of the interface.  Flux is positive from port 0 to port 1, but can
///           go in either direction.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidUtils::computeConductiveHeatFlux(const double     area,
                                                  const PolyFluid* fluid0,
                                                  const PolyFluid* fluid1,
                                                  const double     length0,
                                                  const double     length1)
{
    const double totalLength = length0 + length1;

    /// - Throw an exception if the total conduction length is not greater than zero.
    if (totalLength <= DBL_EPSILON) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Total conduction length is <= 0.",
                TsOutOfBoundsException, "Invalid Calling Arguments", "Unknown");
    }
    const double invLength = 1.0 / totalLength;

    /// - Conductive heat transfer coefficient is the average of the node fluid's thermal
    ///   conductivities weighted by the distance the flux has to travel in through each fluid.
    const double conductivity = (fluid0->getThermalConductivity() * length0 +
                                 fluid1->getThermalConductivity() * length1) * invLength;

    return area * conductivity * (fluid0->getTemperature() - fluid1->getTemperature()) * invLength;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out]  fluid          (--)   Pointer to the link internal fluid.
/// @param[in]      area           (m2)   Open cross-sectional area of the interface.
/// @param[in]      fluid0         (--)   Pointer to port 0 node fluid.
/// @param[in]      fluid1         (--)   Pointer to port 1 node fluid.
/// @param[in]      bulkFlowRate   (kg/s) Bulk flow rate through the link.
/// @param[in]      length0        (m)    Distance from the interface to center of node 0 volume.
/// @param[in]      length1        (m)    Distance from the interface to center of node 1 volume.
///
/// @return                   (kg/s) Net mass flow rate from port 0 to port 1.
///
/// @throws   TsOutOfBoundsException
///
/// @note     A mixture of ideal gases is assumed.
///
/// @note     As a simplification, we assume molecular diameter of all gases is that of diatomic
///           molecules (N2, O2, etc), since they constitute ~99% of air.
///
/// @note     As a simplification, we assume that diffusion rate is not affected by pressure flow
///           through the same interface.
///
/// @details  Computes the free gas diffusion between the fluid contents of the linked nodes, given
///           the geometry of the interface.  Net mass flow rate is positive from port 0 to port 1,
///           but can go in either direction.  This method returns the net mass flow rate, and
///           modifies the link's internal fluid such that, when combined with the net flow rate,
///           will exchange gas constituents in either or both directions, as appropriate.  Net
///           molar flow across the interface is not returned - although temperature differences
///           across the interface normally cause a net molar flow, the discrepancies caused by not
///           incorporating it in the Gunns system of equations are small, and are corrected for in
///           the node's pressure correction logic.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidUtils::computeGasDiffusion(PolyFluid*       fluid,
                                            const double     area,
                                            const PolyFluid* fluid0,
                                            const PolyFluid* fluid1,
                                            const double     bulkFlowRate,
                                            const double     length0,
                                            const double     length1)
{
    /* We use Fick's First Law for diffusive flux, J (mol/m2/s), as a function of the partial molar
     * density of the constituents in each node, phi (mol/m3), the diffusion coefficient, D (m2/s),
     * and the length of the diffusion path, del-x (m):
     *
     *                                   J = -D del-phi/del-x
     *
     * del-x will be the distance between the center of the node volumes, given as config data to
     * the caller (hatch model, etc.)
     *
     * Partial density, phi, of each constituent in each node is calculated from the node total
     * density molecular weight, and mole fraction of the constituent:
     *
     *            phi (kg*mol/m3) = density (kg/m3) * (1/MW) (kg*mol/kg) * mole fraction
     *
     * Diffusion coefficient D is a function of mean free path, Lambda, and the average velocity of
     * the gas, c, which is a function of temperature:
     *
     *                            D (m2/s) = Lambda (m) * c (m/s) / 3
     *
     *                Lambda (m) = k (J/K) * T (K) / P (Pa) / sigma (m2), Pa = J/m3
     *
     *     c = sqrt(3*k*T/m),     sqrt( (J/K) * (K) * (mol/kg) ) = sqrt( (kg*m2/s2) / kg ) = m/s
     *
     * K = boltzmann's, T = temperature, m = mass of molecule, P = pressure, sigma = molecular
     * cross-sectional area.
     *
     * For calculating sigma, we use the established van der Waals radius of 1.55 angstroms for
     * nitrogen, since that is the primary constituent in gas mixtures in Gunns networks.  This
     * gives an area of 3.0E-19 m2.
     *
     * This is a utility function rather than a dedicated GUNNS link, for the following reasons:
     * - It can be included by a hatch class or any other link needing to do diffusion.
     * - Since this doesn't affect the system of equations, this does not need to be a gunns link
     * - Diffusion must be scaled by position of the conductor through which it really flows.  We
     *   don't want to have to create a reference to a separate conductor link thru the manager.
     */

    /// - Calculate diffusion distance.
    const double totalLength = length0 + length1;

    /// - Throw an exception if the total diffusion length is not greater than zero.
    if (totalLength <= DBL_EPSILON) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Total diffusion length is <= 0.",
                TsOutOfBoundsException, "Invalid Calling Arguments", "Unknown");
    }
    const double invLength = 1.0 / totalLength;

    /// - Calculate mean free path of the gas mixtures.
    const double lambda0 = fluid0->getTemperature() / fmax(fluid0->getPressure(), FLT_EPSILON);
    const double lambda1 = fluid1->getTemperature() / fmax(fluid1->getPressure(), FLT_EPSILON);
    const double lambda  = LAMBDA_BASE * (lambda0 * length0 + lambda1 * length1) * invLength;

    /// - Calculate mean temperature along the diffusion length.
    const double meanTemperature = fmax(DBL_EPSILON, (fluid0->getTemperature() * length0 +
                                                      fluid1->getTemperature() * length1) * invLength);


    /// - Calculate mean velocity of the molecules.
    /*
     *                                  c = sqrt(3*k*T/m),
     *
     * m = mass of a molecule (kg) = MW (g/mol) / Avogadro's (molecule/mol) / 1000 (g/kg)
     * for N2: MW = ~28: m = 4.65E-26 kg
     * for N2 @ 300K, c = ~517 m/s
     *
     * Avogadro's number (NA) & Boltzmann's constant reduce to universal gas constant R:
     *
     *                   c = sqrt( 3*k*T * 1000*NA/MW ) = sqrt( 3000*R*T/MW )
     *
     * Speed of sound = sqrt( gamma*R*T ), so we're ballpark
     */
    const double meanMW = fmax(DBL_EPSILON, (fluid0->getMWeight() * length0 +
                                             fluid1->getMWeight() * length1) * invLength);
    const double meanVelocity  = sqrt(3000.0 * UnitConversion::UNIV_GAS_CONST_SI *
                                      meanTemperature / meanMW);

    /// - Diffusivity of the mean gas.
    const double diffusivity   = lambda * meanVelocity / 3.0;

    double positiveFlux  = 0.0;
    double negativeFlux  = 0.0;
    double fluid0MolarDensity = fluid0->getDensity() / fmax(DBL_EPSILON, fluid0->getMWeight());
    double fluid1MolarDensity = fluid1->getDensity() / fmax(DBL_EPSILON, fluid1->getMWeight());
    double meanMolarDensity = fmax(DBL_EPSILON, (fluid0MolarDensity * length0 + fluid1MolarDensity * length1) * invLength);

    /// - Loop over the constituents.
    for (int i = 0; i < fluid0->getNConstituents(); ++i) {
        FluidProperties::FluidType type = fluid->getType(i);
        const double mW    = fluid0->getProperties(type)->getMWeight();

        /// - Calculate molar density of the constituent.
        const double molarDensity0 = fluid0->getMoleFraction(type) * meanMolarDensity;
        const double molarDensity1 = fluid1->getMoleFraction(type) * meanMolarDensity;

        /// - Diffusive flux of the constituent, positive for flux from port 0 to port 1:
        const double diffusiveFlux = diffusivity * (molarDensity0 - molarDensity1) * invLength;

        /// - Update mass flux in opposing directions
        if (diffusiveFlux >= 0.0) {
            positiveFlux += diffusiveFlux;
        } else {
            negativeFlux += diffusiveFlux;
        }

        /// - Set mass of the constituent in the internal fluid.  Note this is not actually mass,
        ///   we're just using this to set up the relative mixture of the constituents.
        fluid->setMass(i, diffusiveFlux * mW);
    }

    /// - Limit flux such that bulk flow will overtake the effect. Opposing diffusive flow is
    ///   compared to the bulk flow to derive a scale value. The scale value is applied to
    ///   to both opposing and complimentary diffusive flows, so the entire diffusion
    ///   effect is diminished by bulk flow in either direction.
    double limitNetFluxRatio      = 1.0;
    // Protect from divide by 0
    if (area > 0.0) {
        const double fluid0Mw = fluid0->getMWeight();
        const double fluid1Mw = fluid1->getMWeight();
        if (bulkFlowRate > 0.0 && negativeFlux < 0.0 && fluid0Mw != 0.0) {
            const double bulkFlux = bulkFlowRate / fluid0Mw / area;
            const double opposingFlux = fmin(0.0, negativeFlux + bulkFlux);
            limitNetFluxRatio = MsMath::limitRange(0.0, opposingFlux / negativeFlux, 1.0);
        } else if (bulkFlowRate < 0.0 && positiveFlux > 0.0 && fluid1Mw != 0.0) {
            const double bulkFlux = bulkFlowRate / fluid1Mw / area;
            const double opposingFlux = fmax(0.0, positiveFlux + bulkFlux);
            limitNetFluxRatio = MsMath::limitRange(0.0, opposingFlux / positiveFlux, 1.0);
        }
    }

    double netMassFlux  = 0.0;
    for (int i = 0; i < fluid0->getNConstituents(); ++i) {
        FluidProperties::FluidType type = fluid->getType(i);
        double newMass = fluid->getConstituent(type)->getMass() * limitNetFluxRatio;

        /// - Set mass of the constituent in the internal fluid.  Note this is not actually mass,
        ///   we're just using this to set up the relative mixture of the constituents.
        fluid->setMass(i, newMass);

        /// - Update mass flux per unit area.
        netMassFlux += newMass;
    }

    fluid->updateMass();

    /// - Calculate net mass flux.
    netMassFlux *= area;

    /// - Set internal fluid temperature equal to that of the receiving node so that node
    ///   temperatures are not affected.
    if (netMassFlux > 0.0) {
        fluid->setTemperature(fluid1->getTemperature());
    } else {
        fluid->setTemperature(fluid0->getTemperature());
    }

    return netMassFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  fluid  (--)  Pointer to the fluid to calculate for
///
/// @returns  (--)  Fractional relative humidity
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returned value of 1.0 represents 100% relative humidity, but the returned value can be
///           greater than 1.0.  If gaseous H2O is not present in the fluid config, an exception is
///           thrown by the PolyFluid object.  If H2O saturation vapor pressure is zero (as with a
///           super-cold fluid), return zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidUtils::computeRelativeHumidityH2O(const PolyFluid* fluid)
{
    double result = 0.0;
    const double ppH2o    = fluid->getPartialPressure(FluidProperties::GUNNS_H2O);
    const double ppH2oSat = fluid->getProperties(FluidProperties::GUNNS_H2O)->getSaturationPressure(fluid->getTemperature());

    if (ppH2oSat > DBL_EPSILON) {
        result = ppH2o / ppH2oSat;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] outMixture  (--) The resulting mass fractions
/// @param[in]  inMixture   (--) The input mole fractions
/// @param[in]  fluidConfig (--) The input fluid configuration
///
/// @details  This method converts an array of mole fractions of a fluid mixture into mass
///           fractions.  If the sum of the mole fractions in the input array is zero, this method
///           will return without modifying the output array.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidUtils::convertMoleFractionToMassFraction(double*                    outMixture,
                                                        const double*              inMixture,
                                                        const PolyFluidConfigData* fluidConfig)
{
    double* temp = new double[fluidConfig->mNTypes];
    double sum = 0.0;

    // Find the relative mass fractions per mole
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        temp[i] = inMixture[i]
                * fluidConfig->mProperties->getProperties(fluidConfig->mTypes[i])->getMWeight();
        sum += temp[i];
    }

    // Normalize the mass fractions array
    if (sum >= DBL_EPSILON) {
        for (int i = 0; i < fluidConfig->mNTypes; ++i) {
            outMixture[i] = temp[i] / sum;
        }
    }

    delete [] temp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] outMixture  (--) The resulting mole fractions
/// @param[in]  inMixture   (--) The input mass fractions
/// @param[in]  fluidConfig (--) The input fluid configuration
///
/// @details  This method converts an array of mass fractions of a fluid mixture into mole
///           fractions.  If the sum of the mass fractions in the input array is zero, this method
///           will return without modifying the output array.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidUtils::convertMassFractionToMoleFraction(double*          outMixture,
                                                        const double*    inMixture,
                                                        const PolyFluidConfigData* fluidConfig)
{
    double* temp = new double[fluidConfig->mNTypes];
    double sum = 0.0;

    // Find the relative mole fractions per unit mass
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        temp[i] = inMixture[i]
                / fluidConfig->mProperties->getProperties(fluidConfig->mTypes[i])->getMWeight();
        sum += temp[i];
    }

    // Normalize the mole fractions array
    if (sum >= DBL_EPSILON) {
        for (int i = 0; i < fluidConfig->mNTypes; ++i) {
            outMixture[i] = temp[i] / sum;
        }
    }

    delete [] temp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] outMixture  (--) The resulting mole fractions
/// @param[in]  inMixture   (--) The input partial pressures
/// @param[in]  fluidConfig (--) The input fluid configuration
///
/// @details  This method converts an array of partial pressures of a fluid mixture into mole
///           fractions.  If the sum of the partial pressures in the input array is zero, this
///           method will return without modifying the output array.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidUtils::convertPartialPressureToMoleFraction(double*                    outMixture,
                                                           const double*              inMixture,
                                                           const PolyFluidConfigData* fluidConfig)
{
    double totalPressure = 0.0;
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        totalPressure += inMixture[i];
    }

    if (totalPressure >= DBL_EPSILON) {
        for (int i = 0; i < fluidConfig->mNTypes; ++i) {
            outMixture[i] = inMixture[i] / totalPressure;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] outMixture    (--) The resulting partial pressures
/// @param[in]  inMixture     (--) The input mole fractions
/// @param[in]  fluidConfig   (--) The input fluid configuration
/// @param[in]  totalPressure (--) The total pressure of the resulting fluid
///
/// @details  This method converts an array of mole fractions of a fluid mixture into partial
///           pressures.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidUtils::convertMoleFractionToPartialPressure(double*                    outMixture,
                                                           const double*              inMixture,
                                                           const PolyFluidConfigData* fluidConfig,
                                                           const double               totalPressure)
{
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        outMixture[i] = inMixture[i] * totalPressure;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] fractions (--) The fractions array to normalize.
/// @param[in]     size      (--) The number of elements in the array.
///
/// @returns       bool      (--) True if normalization was necessary.
///
/// @details  Any elements with values less than DBL_EPSILON * DBL_EPSILON are zeroed, to prevent
///           arithmetic underflows from developing in the array.  If the resulting sum of the
///           fractions is not within FLT_EPSILON of 1.0, then the the array is normalized such that
///           it sums to 1.0 and the method returns a True.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidUtils::normalizeArray(double* fractions, const int size)
{
    bool   result        = false;
    double fractionsSum  = 0.0;
    const double minimum = DBL_EPSILON*DBL_EPSILON;

    for (int i = 0; i < size; ++i) {
        if (fractions[i] < minimum) {
            fractions[i] = 0.0;
        }
        fractionsSum += fractions[i];
    }

    if (std::fabs(fractionsSum - 1.0) > static_cast<double>(FLT_EPSILON)) {
        if (fractionsSum > 0.0) {
            for (int i = 0; i < size; ++i) {
                fractions[i] /= fractionsSum;
            }
        }
        result = true;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] outFluid      (--) Pointer to the fluid to be added to.
/// @param[in]     outMass       (kg) The original mass of outFluid prior to mixing.
/// @param[in]     inFluid       (--) Pointer to the fluid to be added to outFluid.
/// @param[in]     inMass        (kg) The mass of inFluid to be added to outFluid.
/// @param[in]     numFluidTypes (--) The number of fluid constituents in the fluid mixtures.
///
/// @throws        TsOutOfBoundsException
///
/// @returns       bool (--) True if normalization of outFluid mass fractions was necessary.
///
/// @details  Mixes a given mass of the inFluid mixture into a given mass of the outFluid mixture.
///           Only the outFluid is changed.  The resulting mixture of outFluid is normalized such
///           that each of its constituent's mass fractions cannot be less than zero and all the
///           fractions sum to 1.  The outFluid's total mass and mass fractions are set.  A returned
///           flag indicates whether normalization of the mass fractions was necessary.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidUtils::mixFluidMasses(PolyFluid* outFluid, const double outMass,
                                     PolyFluid* inFluid,  const double inMass,
                                     const int  numFluidTypes)
{
    const double newMass = inMass + outMass;
    if (newMass < DBL_EPSILON) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Total mass is <= 0.",
                        TsOutOfBoundsException, "Invalid Calling Arguments", "Unknown");
    }
    double newMassFractions[numFluidTypes];
    for (int i = 0; i < numFluidTypes; ++i) {
        const double partialMass = inMass  * inFluid ->getMassFraction(i)
                                 + outMass * outFluid->getMassFraction(i);
        newMassFractions[i] = partialMass / newMass;
    }
    const bool result = GunnsFluidUtils::normalizeArray(newMassFractions, numFluidTypes);

    /// - Update the new fluid mass fractions, mole fractions, and molecular weight.
    outFluid->setMassAndMassFractions(newMass, newMassFractions);

    /// - Update the new trace compounds.
    GunnsFluidTraceCompounds* traceCompounds = outFluid->getTraceCompounds();
    if (traceCompounds) {
        traceCompounds->flowIn(*inFluid->getTraceCompounds(), inMass / inFluid->getMWeight());
    }

    return result;
}
