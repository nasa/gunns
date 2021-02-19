#ifndef GunnsFluidAdsorptionCompound_EXISTS
#define GunnsFluidAdsorptionCompound_EXISTS

/**
@file
@brief    GUNNS Fluid Adsorbed Compound Model declarations

@defgroup  TSM_GUNNS_FLUID_SOURCE_ADSORBED_COMPOUND  Fluid Adsorbed Compound Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for the GUNNS Fluid Adsorbed Compound model.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ((Adsorption & desorption are assumed to be the same reversible chemical reaction.))

 LIBRARY DEPENDENCY:
- ((GunnsFluidAdsorptionCompound.o))

 PROGRAMMERS:
- ((Jason Harvey) (CACI) (Initial) (2018-01))

@{
*/

#include "properties/ChemicalCompound.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <string>

// Forward-declare pointer types.
class PolyFluid;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This models a compound and its adsorption & desorption between a fluid and an adsorber
///           device.  Features:
///
///           - Switches automatically between adsorption & desorption based on fluid pressure
///             and/or temperature.
///           - Adsorbs a fraction of the compound from the fluid based on a temperature-dependent
///             efficiency.
///           - Desorbs to the fluid based on a temperature-dependent efficiency and partial-
///             pressure of the compound in the fluid.
///           - Has a malfunction to override the temperature-dependent efficiency.
///           - This compound can be either a network fluid constituent or a trace compound.
///           - Can optionally adjust sorption based on the loading of one other compound in the
///             adsorber.  For instance, CO2 adsorbers using Metox only become efficient with CO2
///             after they've adsorbed H2O.  The dependent compound can either be a network fluid
///             constituent or a trace compound.
///           - Sorption rates can optionally diminish as the adsorbed mass approaches maximum or
///             zero limits.
///           - Models an optional heat of the sorbing reactions.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidAdsorptionCompound
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidAdsorptionCompound);
    public:
        ChemicalCompound::Type mType;                  /**< (--)       trick_chkpnt_io(**) The compound that is sorbed. */
        double                 mEfficiencyCoeff0;      /**< (--)       trick_chkpnt_io(**) Sorption efficiency. */
        double                 mEfficiencyCoeff1;      /**< (1/K)      trick_chkpnt_io(**) Sorption efficiency temperature coefficient. */
        double                 mMaxAdsorbedMass;       /**< (kg)       trick_chkpnt_io(**) Maximum mass that can be adsorbed. */
        double                 mDesorbPartialPressure; /**< (kPa)      trick_chkpnt_io(**) Partial pressure at switch between adsorption and desorption. */
        double                 mDesorbRateFactor;      /**< (kg/s/kPa) trick_chkpnt_io(**) Desorption rate dependence on pressure. */
        double                 mHeatOfAdsorption;      /**< (kJ/mol)   trick_chkpnt_io(**) Enthalpy change of reaction in adsorption direction, per mol of absorbed compound (negative is exothermic). */
        bool                   mTaperOffFlag;          /**< (--)       trick_chkpnt_io(**) Whether sorption rate diminishes as adsorbed mass approaches limits. */
        ChemicalCompound::Type mDependentType;         /**< (--)       trick_chkpnt_io(**) Optional other compound on which this compound's sorption depends. */
        bool                   mMalfEfficiencyFlag;    /**< (--)                           Initial efficiency malfunction flag. */
        double                 mMalfEfficiencyValue;   /**< (--)                           Initial efficiency malfunction value. */
        double                 mAdsorbedMass;          /**< (kg)                           Initial adsorbed mass of this compound in the adsorber. */
        double                 mFillFraction;          /**< (--)       trick_chkpnt_io(**) Current fraction (0-1) of the maximum adsorbed mass. */
        double                 mBreakthroughExp;       /**< (--)       trick_chkpnt_io(**) Exponent value for breakthrough adsorption. */
        double                 mAdsorptionRate;        /**< (kg/s)     trick_chkpnt_io(**) Current mass rate of adsorption from the fluid stream. */
        double                 mSorptionHeat;          /**< (W)        trick_chkpnt_io(**) Current heat output from sorb reaction to sorbant (positive is exothermic). */
        /// @brief  Default constructor for this Fluid Adsorbed Compound Model.
        GunnsFluidAdsorptionCompound();
        /// @brief  Default destructor for this Fluid Adsorbed Compound Model.
        virtual ~GunnsFluidAdsorptionCompound();
        /// @brief  Copy constructor for this Fluid Adsorbed Compound Model.
        GunnsFluidAdsorptionCompound(const GunnsFluidAdsorptionCompound& that);
        /// @brief  Assignment operator for this Fluid Adsorbed Compound Model.
        GunnsFluidAdsorptionCompound& operator=(const GunnsFluidAdsorptionCompound& that);
        /// @brief  Computes sorption and related outputs.
        void sorb(const double dt, const double tAvg, const double pAvg, const double mdot);
        /// @brief  Returns whether this compound is a trace compound or fluid constituent in the network.
        bool isTraceCompound() const;
        /// @brief  Returns this Fluid Adsorbed Compound Model index in the network fluid or TC array.
        int  getIndex() const;
        /// @brief  Sets and resets this Fluid Adsorbed Compound Model efficiency malfunction.
        void setMalfEfficiency(const bool flag = false, const double value = 0.0);
        /// @brief  Initializes this Fluid Adsorbed Compound Model.
        void initialize(const std::string&                  name,
                        const GunnsFluidAdsorptionCompound& config,
                        const PolyFluid*                    fluid,
                        const GunnsFluidAdsorptionCompound* dependentCompound);

    protected:
        std::string                         mName;              /**< *o (--)    trick_chkpnt_io(**) Name of this instance for H&S messages. */
        bool                                mTraceCompound;     /**< *o (--)    trick_chkpnt_io(**) True if this is a trace compound. */
        int                                 mIndex;             /**< *o (--)    trick_chkpnt_io(**) Index of this compound in the network fluid or TC array. */
        double                              mMWeight;           /**<    (1/mol) trick_chkpnt_io(**) Molecular weight of the compound. */
        const PolyFluid*                    mFluid;             /**< *o (--)    trick_chkpnt_io(**) Pointer to the fluid object this compound interacts with. */
        const GunnsFluidAdsorptionCompound* mDependentCompound; /**< *o (--)    trick_chkpnt_io(**) Optional pointer to another compound on which this compound's sorption depends. */
        static const double                 m100EpsilonLimit;   /**< ** (--)    trick_chkpnt_io(**) Limit 100 * DBL_EPSILON for some applications. */
        /// @brief  Computes the new fill fraction.
        void updateFillFraction();
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
/// @param[in] value (--) (0-1) Malfunction value to override the efficiency.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidAdsorptionCompound::setMalfEfficiency(const bool flag, const double value)
{
    mMalfEfficiencyFlag  = flag;
    mMalfEfficiencyValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) True if this is a trace compound, false if it is a fluid constituent.
///
/// @details  Returns the value of mTraceCompound.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsFluidAdsorptionCompound::isTraceCompound() const
{
    return mTraceCompound;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int (--) Index of this compound in the network fluid or TC array.
///
/// @details  Returns the value of mIndex.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsFluidAdsorptionCompound::getIndex() const
{
    return mIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the new fill fraction as the ratio of current to maximum adsorbed mass.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidAdsorptionCompound::updateFillFraction()
{
    mFillFraction = mAdsorbedMass / std::max(mMaxAdsorbedMass, DBL_EPSILON);
}

#endif
