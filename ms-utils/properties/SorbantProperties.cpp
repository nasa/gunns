/*
@file     SorbantProperties.cpp
@brief    Chemical Sorbant & Sorbate Properties implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
- ((software/exceptions/TsInitializationException.o))
*/

#include "SorbantProperties.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] compound          (--)            The chemical compound properties of this sorbate.
/// @param[in] blockingCompounds (--)            Optional list of chemical compound types that block the sorption of this sorbate.
/// @param[in] offgasCompounds   (--)            Optional list of chemical compound types produced when this sorbate is desorbed.
/// @param[in] tothA0            (kg*mol/kg/kPa) Toth isotherm parameter a0.
/// @param[in] tothB0            (1/kPa)         Toth isotherm parameter b0.
/// @param[in] tothE             (K)             Toth isotherm parameter E.
/// @param[in] tothT0            (--)            Toth isotherm parameter t0.
/// @param[in] tothC0            (K)             Toth isotherm parameter c0.
/// @param[in] dh                (kJ/mol)        Heat of adsorption of this sorbate in the sorbant, see note on sign convention below.
/// @param[in] km                (1/s)           Adsorption time constant.
///
/// @throws   TsInitializationException
///
/// @details  Constructs this Sorbate Properties with arguments and validates properties values.
///           Validation checks throw exception on failures.
///
/// @note     Sign convention for dh: adsorption is usually exotheric (adsorbing sorbates produces
///           waste heat) and desorption usually endothermic.  For this argument, a negative sign
///           represents exothermic, and positive is endothermic.  This matches the convention for
///           delta-enthalpy typically given for heats of reaction in literature.
////////////////////////////////////////////////////////////////////////////////////////////////////
SorbateProperties::SorbateProperties(const ChemicalCompound*                         compound,
                                     const std::vector<SorbateInteractingCompounds>* blockingCompounds,
                                     const std::vector<SorbateInteractingCompounds>* offgasCompounds,
                                     const double                                    tothA0,
                                     const double                                    tothB0,
                                     const double                                    tothE,
                                     const double                                    tothT0,
                                     const double                                    tothC0,
                                     const double                                    dh,
                                     const double                                    km)
    :
    mCompound(compound),
    mTothA0(tothA0),
    mTothB0(tothB0),
    mTothE(tothE),
    mTothT0(tothT0),
    mTothC0(tothC0),
    mDh(dh),
    mKm(km),
    mBlockingCompounds(),
    mOffgasCompounds()
{
    mBlockingCompounds.clear();
    if (blockingCompounds) {
        mBlockingCompounds = *blockingCompounds;
    }
    mOffgasCompounds.clear();
    if (offgasCompounds) {
        mOffgasCompounds = *offgasCompounds;
    }

    /// - Validate constructed values.
    if (km < DBL_EPSILON) {
        throw TsInitializationException();
    }
    for (unsigned int i=0; i<mBlockingCompounds.size(); ++i) {
        if (mBlockingCompounds[i].mCompound == compound->mType) {
            throw TsInitializationException();
        }
    }
    for (unsigned int i=0; i<mOffgasCompounds.size(); ++i) {
        if (mOffgasCompounds[i].mCompound == compound->mType) {
            throw TsInitializationException();
        }
        if (mOffgasCompounds[i].mInteraction < 0.0) {
            throw TsInitializationException();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Sorbate Properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
SorbateProperties::~SorbateProperties()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to be copied from.
///
/// @details  Copy constructs this Sorbate Properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
SorbateProperties::SorbateProperties(const SorbateProperties& that)
    :
    mCompound(that.mCompound),
    mTothA0(that.mTothA0),
    mTothB0(that.mTothB0),
    mTothE(that.mTothE),
    mTothT0(that.mTothT0),
    mTothC0(that.mTothC0),
    mDh(that.mDh),
    mKm(that.mKm),
    mBlockingCompounds(that.mBlockingCompounds),
    mOffgasCompounds(that.mOffgasCompounds)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to be assigned equal to.
///
/// @details  Assignment operator for this Sorbate Properties.  This only assigns the non-const
///           members.
////////////////////////////////////////////////////////////////////////////////////////////////////
SorbateProperties& SorbateProperties::operator =(const SorbateProperties& that)
{
    if (this != &that) {
        mBlockingCompounds = that.mBlockingCompounds;
        mOffgasCompounds   = that.mOffgasCompounds;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  pp          (kPa)  Partial pressure of the sorbate in the freestream.
/// @param[in]  temperature (K)    Temperature of the freestream.
///
/// @returns  double (kg*mol/m3) Equilibrium loading of the sorbate.
///
/// @details  Computes & returns the equilibrium loading of the sorbate under current conditions,
///           using the Toth isotherm equation for this sorbate in the sorbant.
///
/// @note  The caller must ensure temperature > 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
double SorbateProperties::computeLoadingEquil(const double pp, const double temperature) const
{
    double result = 0.0;
    if (0.0 != mTothE and pp >= FLT_EPSILON) {
        const double EoverT = MsMath::limitRange(0.0, mTothE / temperature, 100.0);
        const double expT   = exp(EoverT);
        const double a      = mTothA0 * expT;
        const double b      = mTothB0 * expT;
              double tT     = MsMath::limitRange(-100.0, mTothT0 + mTothC0 / temperature, 100.0);
                     tT     = MsMath::innerLimit(-0.1, tT, 0.1);
        const double denom  = powf(1.0 + powf(b * pp, tT), 1.0 / tT);
        result = a * pp / std::max(denom, DBL_EPSILON);
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] compound    (--) The chemical compound that interacts with this Sorbate.
/// @param[in] interaction (--) The amount of interaction with this Sorbate.
///
/// @details  Pushes a new SorbateInteractingCompounds onto mBlockingCompounds with the given
///           values.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SorbateProperties::addBlockingCompound(const ChemicalCompound::Type compound, const double interaction)
{
    mBlockingCompounds.push_back(SorbateInteractingCompounds());
    mBlockingCompounds.back().mCompound    = compound;
    mBlockingCompounds.back().mInteraction = interaction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] compound    (--) The chemical compound that interacts with this Sorbate.
/// @param[in] interaction (--) The amount of interaction with this Sorbate.
///
/// @details  Pushes a new SorbateInteractingCompounds onto mOffgasCompounds with the given values.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SorbateProperties::addOffgasCompound(const ChemicalCompound::Type compound, const double interaction)
{
    mOffgasCompounds.push_back(SorbateInteractingCompounds());
    mOffgasCompounds.back().mCompound    = compound;
    mOffgasCompounds.back().mInteraction = interaction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] type     (--)     Defined type of this sorbant.
/// @param[in] density  (kg/m3)  Density of this sorbant material.
/// @param[in] porosity (--)     Fraction of the packed sorbant enclosure volume that is voids.
/// @param[in] cp       (J/kg/K) Specific heat of this sorbant material.
///
/// @throws   TsInitializationException
///
/// @details  Constructs this Sorbant Properties with arguments and validates properties values.
///           Validation checks throw exception on failures.
////////////////////////////////////////////////////////////////////////////////////////////////////
SorbantProperties::SorbantProperties(const SorbantProperties::Type type,
                                     const double                  density,
                                     const double                  porosity,
                                     const double                  cp)
    :
    mType(type),
    mDensity(density),
    mPorosity(porosity),
    mCp(cp),
    mSorbates(),
    mDefinedCompounds()
{
    /// - Validate constructud values.
    if (density < DBL_EPSILON) {
        throw TsInitializationException();
    }
    if (not MsMath::isInRange(0.0, porosity, 1.0)) {
        throw TsInitializationException();
    }
    if (cp < DBL_EPSILON) {
        throw TsInitializationException();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Sorbant Properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
SorbantProperties::~SorbantProperties()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to be copied from.
///
/// @details  Copy constructs this Sorbant Properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
SorbantProperties::SorbantProperties(const SorbantProperties& that)
    :
    mType(that.mType),
    mDensity(that.mDensity),
    mPorosity(that.mPorosity),
    mCp(that.mCp),
    mSorbates(that.mSorbates),
    mDefinedCompounds()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to be assigned equal to.
///
/// @details  Assignment operator for this Sorbant Properties.  This only asssings the non-const
///           members.
////////////////////////////////////////////////////////////////////////////////////////////////////
SorbantProperties& SorbantProperties::operator =(const SorbantProperties& that)
{
    if (this != &that) {
        mSorbates = that.mSorbates;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] compound          (--)            Sorbate chemical compound type.
/// @param[in] blockingCompounds (--)            Optional list of chemical compound types that block the sorption of this sorbate.
/// @param[in] offgasCompounds   (--)            Optional list of chemical compound types produced when this sorbate is desorbed.
/// @param[in] tothA0            (kg*mol/kg/kPa) Sorbate Toth isotherm parameter a0 in this sorbant.
/// @param[in] tothB0            (1/kPa)         Sorbate Toth isotherm parameter b0 in this sorbant.
/// @param[in] tothE             (K)             Sorbate Toth isotherm parameter E in this sorbant.
/// @param[in] tothT0            (--)            Sorbate Toth isotherm parameter t0 in this sorbant.
/// @param[in] tothC0            (K)             Sorbate Toth isotherm parameter c0 in this sorbant.
/// @param[in] dh                (kJ/mol)        Heat of adsorption of the sorbate in this sorbant.
/// @param[in] km                (1/s)           Adsorption time constant.
///
/// @returns  Pointer to the newly created sorbate.
///
/// @details  Adds a sorbate with the given properties to the list of sorbates that this sorbant
///           will interact with, and returns a pointer to the new sorbate.
////////////////////////////////////////////////////////////////////////////////////////////////////
SorbateProperties* SorbantProperties::addSorbate(const ChemicalCompound::Type                    compound,
                                                 const std::vector<SorbateInteractingCompounds>* blockingCompounds,
                                                 const std::vector<SorbateInteractingCompounds>* offgasCompounds,
                                                 const double                                    tothA0,
                                                 const double                                    tothB0,
                                                 const double                                    tothE,
                                                 const double                                    tothT0,
                                                 const double                                    tothC0,
                                                 const double                                    dh,
                                                 const double                                    km)
{
    const ChemicalCompound* properties = mDefinedCompounds.getCompound(compound);
    SorbateProperties newSorbate(properties, blockingCompounds, offgasCompounds,
                                 tothA0, tothB0, tothE, tothT0, tothC0, dh, km);
    mSorbates.push_back(newSorbate);
    return &mSorbates.back();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Defined Sorbant Properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
DefinedSorbantProperties::DefinedSorbantProperties()
    :
    mSilicaGelB125    (SorbantProperties::SILICA_GEL_B125,     1240.0, 0.348, 870.0),
    mSilicaGel40      (SorbantProperties::SILICA_GEL_40,       1240.0, 0.415, 870.0),
    mZeo5aRk38        (SorbantProperties::ZEO_5A_RK38,         1370.0, 0.445, 650.0),
    mZeo5a522         (SorbantProperties::ZEO_5A_522,          1190.0, 0.331, 750.0),
    mZeo13x544        (SorbantProperties::ZEO_13X_544,         1260.0, 0.457, 800.0),
    //TODO see gunns_misc/sorbant_research
    // it looks like this:
    // Eldridge, C., and Papale, W., “SA9T Sorbent CO2 and H2O Vapor Isotherm Testing”, 2007, Cooperative
    //    Agreement NNJ04HF73A. Hamilton Sundstrand
    // has what we need, but i can't find it.
    // GunnsFluidAdsorberRca models SA9T so maybe we can back out the isotherms from it
    mSa9t             (SorbantProperties::SA9T,                   1.0, 0.0,     1.0), //TODO data still needed
    /// - Porosity values for different sphere packing is from
    ///     https://en.wikipedia.org/wiki/Sphere_packing.
    mGlassBeadsLattice(SorbantProperties::GLASS_BEADS_LATTICE, 2500.0, 0.26,  840.0),
    mGlassBeadsRandom (SorbantProperties::GLASS_BEADS_RANDOM,  2500.0, 0.365, 840.0),
    mSorbants()
{
    /// - Set up blocking compounds lists.  Note that different sorbants might have different lists,
    ///   with different interactions.
    /// - We don't define the loading interaction values.  Rather, we simply identify the
    ///   interacting compounds.
    /// - Offgassing interaction value are TBD.  TODO SA9T offgasses ammonia, for example
    SorbateInteractingCompounds h2oBlockingCo2;
    h2oBlockingCo2.mCompound    = ChemicalCompound::H2O;
    h2oBlockingCo2.mInteraction = 1.0;
    std::vector<SorbateInteractingCompounds> blockingCompounds;
    blockingCompounds.push_back(h2oBlockingCo2);

    /// - Load the sorbants with their sorbates.
    mSilicaGelB125.addSorbate(ChemicalCompound::H2O, 0,                  0, 1.767e+2, 2.787e-5,  1.093e+3, -1.190e-3,  2.213e+1, -50.2, 0.002);
    mSilicaGelB125.addSorbate(ChemicalCompound::CO2, &blockingCompounds, 0, 7.678e-6, 5.164e-7,  2.330e+3, -3.053e-1,  2.386e+2, -40.0, 0.011375);
    mSilicaGel40  .addSorbate(ChemicalCompound::H2O, 0,                  0, 1.767e+2, 2.787e-5,  1.093e+3, -1.190e-3,  2.213e+1, -50.2, 0.00125);
    mSilicaGel40  .addSorbate(ChemicalCompound::CO2, &blockingCompounds, 0, 7.678e-6, 5.164e-7,  2.330e+3, -3.053e-1,  2.386e+2, -40.0, 0.011375);
    mZeo5aRk38    .addSorbate(ChemicalCompound::H2O, 0,                  0, 1.106e-8, 4.714e-10, 9.955e+3,  3.548e-1, -5.114e+1, -45.0, 0.007);
    mZeo5aRk38    .addSorbate(ChemicalCompound::CO2, &blockingCompounds, 0, 9.875e-7, 6.761e-8,  5.625e+3,  2.700e-1, -2.002e+1, -38.0, 0.003);
    mZeo5a522     .addSorbate(ChemicalCompound::H2O, 0,                  0, 1.106e-8, 4.714e-10, 9.955e+3,  3.548e-1, -5.114e+1, -45.0, 0.007);
    mZeo5a522     .addSorbate(ChemicalCompound::CO2, &blockingCompounds, 0, 9.875e-7, 6.761e-8,  5.625e+3,  2.700e-1, -2.002e+1, -38.0, 0.003);
    mZeo13x544    .addSorbate(ChemicalCompound::H2O, 0,                  0, 3.634e-6, 2.408e-7,  6.852e+3,  3.974e-1, -4.199e+0, -55.0, 0.007);
    mZeo13x544    .addSorbate(ChemicalCompound::CO2, &blockingCompounds, 0, 6.509e-3, 4.884e-4,  2.991e+3,  7.487e-2,  3.810e+1, -40.0, 0.00325);
//    mSa9t         .addSorbate(ChemicalCompound::H2O, 0,                  0, 0.0,      0.0,       0.0,       0.0,       0.0,       0.0,  1.0);
//    mSa9t         .addSorbate(ChemicalCompound::CO2, 0,                  0, 0.0,      0.0,       0.0,       0.0,       0.0,       0.0,  1.0);
    /// - Glass beads are inert and have no sorbates.

    /// - Load the sorbants array with the pointers to the sorbants.
    mSorbants[SorbantProperties::SILICA_GEL_B125]     = &mSilicaGelB125;
    mSorbants[SorbantProperties::SILICA_GEL_40]       = &mSilicaGel40;
    mSorbants[SorbantProperties::ZEO_5A_RK38]         = &mZeo5aRk38;
    mSorbants[SorbantProperties::ZEO_5A_522]          = &mZeo5a522;
    mSorbants[SorbantProperties::ZEO_13X_544]         = &mZeo13x544;
    mSorbants[SorbantProperties::SA9T]                = &mSa9t;
    mSorbants[SorbantProperties::GLASS_BEADS_LATTICE] = &mGlassBeadsLattice;
    mSorbants[SorbantProperties::GLASS_BEADS_RANDOM]  = &mGlassBeadsRandom;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Defined Chemical Reactions.
////////////////////////////////////////////////////////////////////////////////////////////////////
DefinedSorbantProperties::~DefinedSorbantProperties()
{
    // nothing to do
}
