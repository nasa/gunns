/**
@file
@brief    GUNNS Fluid Adsorbed Compound Model implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((aspects/fluid/fluid/PolyFluid.o))
*/

#include "GunnsFluidAdsorptionCompound.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "aspects/fluid/fluid/PolyFluid.hh"
#include "core/GunnsBasicNode.hh"
#include "math/MsMath.hh"

/// @details  Limit 100 * DBL_EPSILON for some applications.
const double GunnsFluidAdsorptionCompound::m100EpsilonLimit = 100.0 * DBL_EPSILON;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Fluid Adsorbed Compound Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAdsorptionCompound::GunnsFluidAdsorptionCompound()
    :
    mType(ChemicalCompound::NO_COMPOUND),
    mEfficiencyCoeff0(0.0),
    mEfficiencyCoeff1(0.0),
    mMaxAdsorbedMass(0.0),
    mDesorbPartialPressure(0.0),
    mDesorbRateFactor(0.0),
    mHeatOfAdsorption(0.0),
    mTaperOffFlag(false),
    mDependentType(ChemicalCompound::NO_COMPOUND),
    mMalfEfficiencyFlag(false),
    mMalfEfficiencyValue(0.0),
    mAdsorbedMass(0.0),
    mFillFraction(0.0),
    mBreakthroughExp(1.0),
    mAdsorptionRate(0.0),
    mSorptionHeat(0.0),
    mName(""),
    mTraceCompound(false),
    mIndex(-1),
    mMWeight(0.0),
    mFluid(0),
    mDependentCompound(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Adsorbed Compound Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAdsorptionCompound::~GunnsFluidAdsorptionCompound()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  The object to be copied from.
///
/// @details  Copy constructs this Fluid Adsorbed Compound Model.
///
/// @note  This is a shallow copy, and the copied object needs to be initialized.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAdsorptionCompound::GunnsFluidAdsorptionCompound(const GunnsFluidAdsorptionCompound& that)
     :
     mType(that.mType),
     mEfficiencyCoeff0(that.mEfficiencyCoeff0),
     mEfficiencyCoeff1(that.mEfficiencyCoeff1),
     mMaxAdsorbedMass(that.mMaxAdsorbedMass),
     mDesorbPartialPressure(that.mDesorbPartialPressure),
     mDesorbRateFactor(that.mDesorbRateFactor),
     mHeatOfAdsorption(that.mHeatOfAdsorption),
     mTaperOffFlag(that.mTaperOffFlag),
     mDependentType(that.mDependentType),
     mMalfEfficiencyFlag(that.mMalfEfficiencyFlag),
     mMalfEfficiencyValue(that.mMalfEfficiencyValue),
     mAdsorbedMass(that.mAdsorbedMass),
     mFillFraction(that.mFillFraction),
     mBreakthroughExp(that.mBreakthroughExp),
     mAdsorptionRate(0.0),
     mSorptionHeat(0.0),
     mName(""),
     mTraceCompound(false),
     mIndex(-1),
     mMWeight(0.0),
     mFluid(0),
     mDependentCompound(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  The object to be set equal to.
///
/// @returns  GunnsFluidAdsorptionCompound&  (--)  Reference to this object.
///
/// @details  Assigns this Fluid Adsorbed Compound Model equal to the given object.
///
/// @note  This is a shallow assignment, and this object needs to be initialized.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAdsorptionCompound& GunnsFluidAdsorptionCompound::operator=(const GunnsFluidAdsorptionCompound& that)
{
    if (&that != this) {
        mType                  = that.mType;
        mEfficiencyCoeff0      = that.mEfficiencyCoeff0;
        mEfficiencyCoeff1      = that.mEfficiencyCoeff1;
        mMaxAdsorbedMass       = that.mMaxAdsorbedMass;
        mDesorbPartialPressure = that.mDesorbPartialPressure;
        mDesorbRateFactor      = that.mDesorbRateFactor;
        mHeatOfAdsorption      = that.mHeatOfAdsorption;
        mTaperOffFlag          = that.mTaperOffFlag;
        mDependentType         = that.mDependentType;
        mMalfEfficiencyFlag    = that.mMalfEfficiencyFlag;
        mMalfEfficiencyValue   = that.mMalfEfficiencyValue;
        mAdsorbedMass          = that.mAdsorbedMass;
        mFillFraction          = that.mFillFraction;
        mBreakthroughExp       = that.mBreakthroughExp;
        mAdsorptionRate        = 0.0;
        mSorptionHeat          = 0.0;
        mName                  = "";
        mTraceCompound         = false;
        mIndex                 = -1;
        mMWeight               = 0.0;
        mFluid                 = 0;
        mDependentCompound     = 0;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name              (--) Name of this instance for H&S messages.
/// @param[in] config            (--) Reference to another oject containing configuration values to copy.
/// @param[in] fluid             (--) Pointer to the fluid object that this compound sorbs to/from.
/// @param[in] dependentCompound (--) Pointer to another object representing the dependent compound, can be NULL.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Fluid Adsorbed Compound Model with its name & configuration data, and
///           validates the initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAdsorptionCompound::initialize(const std::string&                  name,
                                              const GunnsFluidAdsorptionCompound& config,
                                              const PolyFluid*                    fluid,
                                              const GunnsFluidAdsorptionCompound* dependentCompound)
{
    /// - Initialize with given config data.
    *this = config;

    mName = name;
    /// - Throw an exception on an empty name.
    if (mName.empty()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "instance is missing a name.");
    }

    updateFillFraction();
    mFluid             = fluid;
    mDependentCompound = dependentCompound;

    DefinedChemicalCompounds definedCompounds;
    const ChemicalCompound* compound = definedCompounds.getCompound(config.mType);
    if (!compound) {
        /// - Throw an exception if the compound type is not valid.
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "invalid compound type.");
    }

    mIndex         = -1;
    mTraceCompound = false;
    try {
        mIndex = mFluid->find(compound->mFluidType);
    } catch (TsOutOfBoundsException& e) {
        ; // nothing to do
    }

    GunnsFluidTraceCompounds* tc = mFluid->getTraceCompounds();
    if (tc) { // some TC's exist in network
        try {
            const int tcIndex = tc->find(config.mType);

            /// - Throw an exception if the compound was found as a TC but it's also a fluid type.
            if (mIndex > -1) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "compound is both a fluid and a trace compound in the network.");
            } else {
                mIndex         = tcIndex;
                mTraceCompound = true;
            }
        } catch (TsOutOfBoundsException& e) {
            ; // nothing to do
        }
    }
    mMWeight = compound->mMWeight;

    if (mIndex < 0) {
        /// - Throw an exception if the compound doesn't exist in the network.
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "compound is not in network as either a fluid or trace compound.");
    }

    /// - Throw an exception on maximum adsorbed mass < DBL_EPSILON.
    if (config.mMaxAdsorbedMass < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "maximum adsorbed mass < DBL_EPSILON.");
    }

    /// - Throw an exception on desorb rate factor < 0.
    if (config.mDesorbRateFactor < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "desorb rate factor < 0.");
    }

    /// - Throw an exception on NULL dependent compound pointer but a dependent compound was
    ///   specified.  This happens when the adsorber's compounds array is missing the dependent
    ///   compound.
    if (ChemicalCompound::NO_COMPOUND != mDependentType and not mDependentCompound) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "dependent type isn't in the adsorber's compounds list.");
    }

    /// - Throw an exception on initial mass in adsorber not between zero and max.
    if (!MsMath::isInRange(0.0, config.mAdsorbedMass, config.mMaxAdsorbedMass)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "initial mass in adsorber not between 0 and max.");
    }

    /// - Throw an exception on initial efficiency malf outside valid range.
    if (!MsMath::isInRange(0.0, config.mMalfEfficiencyValue, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Initial efficiency malfunction value outside (0-1).");
    }
    /// - Throw an exception on Breakthrough exponent not in between valid range (1-100).
    if (!MsMath::isInRange(1.0, mBreakthroughExp, 100.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Breakthrough exponent value not between 1.0 and 100.0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt   (s)    Integration time step.
/// @param[in] tAvg (K)    Average of the adsorber inlet and outlet temperatures.
/// @param[in] pAvg (kPa)  Average of the adsorber inlet and outlet total pressures.
/// @param[in] mdot (kg/s) Total fluid mass flow rate through the adsorber.
///
/// @details  Computes adsorption or desorption rate as a function of: fluid stream pressure &
///           temperature, fill fraction of this compound and the optional dependent compound, and
///           efficiency override malfunction.  Updates the adsorbed mass and outputs the
///           sorbing mass rate and reaction heat.
///
/// @note  The caller must ensure dt > 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAdsorptionCompound::sorb(const double dt, const double tAvg, const double pAvg,
                                        const double mdot)
{
    /// - Compute sorption efficiency as a function of fluid temperature.
    const double efficiency = MsMath::limitRange(-1.0, mEfficiencyCoeff0
                                                   + mEfficiencyCoeff1 * tAvg, 1.0);

    /// - Compute partial pressure of this compound by its mole fraction in the fluid stream.
    double partialPressure = 0.0;
    if (mTraceCompound) {
        partialPressure = pAvg * mFluid->getTraceCompounds()->getMoleFractions()[mIndex];
    } else {
        partialPressure = pAvg * mFluid->getMoleFraction(mIndex);
    }

    double adsorbRate = 0.0;
    if ( (partialPressure < mDesorbPartialPressure) or (efficiency < 0.0) ) {

        /// - Desorption rate is inversely proportional to compound partial pressure.
        adsorbRate = std::min(0.0, partialPressure - mDesorbPartialPressure) * mDesorbRateFactor
                   * fabs(efficiency);

    } else {

        /// - Adsorption rate is the efficiency times the mass flow thru.
        double mdotThru = 0.0;
        if (mTraceCompound and fabs(mdot) > m100EpsilonLimit) {
            mdotThru = mFluid->getTraceCompounds()->getMasses()[mIndex];
        } else {
            mdotThru = mFluid->getMassFraction(mIndex) * fabs(mdot);
        }
        adsorbRate = efficiency * mdotThru;
    }

    /// - Scale sorption rate for the optional taper-off.
    /// - Adsorption diminishes as we approach max absorbed mass.
    /// - Desorption diminishes as we approach zero absorbed mass.
    if (mTaperOffFlag) {
        if (adsorbRate > 0.0) {
            adsorbRate *= (1 - pow(std::max(0.1, mFillFraction),
                                   MsMath::limitRange(1.0, mBreakthroughExp, 100.0)));
        } else {
            adsorbRate *= mFillFraction;
        }
    }

    /// - Scale sorption rate based on the amount absorbed of the dependent type.
    /// - Adsorption increases as we approach max dependent compound mass.
    /// - Desorption increases as we approach zero dependent compound mass.
    if (mDependentCompound) {
        if (adsorbRate > 0.0) {
            adsorbRate *= mDependentCompound->mFillFraction;
        } else {
            adsorbRate *= (1.0 - mDependentCompound->mFillFraction);
        }
    }

    /// - Scale sorption rate by the efficiency malfunction.
    if (mMalfEfficiencyFlag) {
        adsorbRate *= mMalfEfficiencyValue;
    }

    /// - Limit sorption rate to not overshoot the max and zero mass limits during integration.
    if (adsorbRate > 0.0) {
        adsorbRate = std::min(adsorbRate, (mMaxAdsorbedMass - mAdsorbedMass) / dt);
    } else {
        adsorbRate = std::max(adsorbRate, -mAdsorbedMass / dt);
    }

    /// - Update and limit the adsorbed mass.
    mAdsorbedMass += adsorbRate * dt;
    if (mAdsorbedMass < DBL_EPSILON) {
        mAdsorbedMass = 0.0;
    } else if (mMaxAdsorbedMass - mAdsorbedMass < DBL_EPSILON) {
        mAdsorbedMass = mMaxAdsorbedMass;
    }
    updateFillFraction();

    /// - Final output rates.
    mAdsorptionRate = adsorbRate;

    /// - Output sorption heat, positive for exothermic.  The mHeatOfAbsorption term is negative for
    ///   exothermic, so we must reverse the sign for output.  Also convert units from kJ/s to W
    ///   and mol to kg:
    ///     (kJ/mol) * (kg/s) * (1000 J/kJ) * (1000 g/kg) * (1/MW) (mol/g) = (1e6/MW)(J/s)
    mSorptionHeat = -mHeatOfAdsorption * adsorbRate * UnitConversion::UNIT_PER_MEGA / mMWeight;
}
