#ifndef SorbantProperties_EXISTS
#define SorbantProperties_EXISTS

/**
@file     SorbantProperties.hh
@brief    Chemical Sorbant & Sorbate Properties declarations

@defgroup  TSM_UTILITIES_PROPERTIES_SORBANT  Chemical Sorbant & Sorbate Properties
@ingroup   TSM_UTILITIES_PROPERTIES

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for modeling sorbant & sorbate properties and interactions.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 CLASS:
- ()

 LIBRARY DEPENDENCY:
- ((SorbantProperties.o))

 PROGRAMMERS:
- ((Jason Harvey) (CACI) (Initial) (2022-03))

@{
*/

#include "properties/ChemicalCompound.hh"
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sorbate Interactions Data
///
/// @details  This describes the degree of interaction between the compound stored here and the
///           compound that references this.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct SorbateInteractingCompounds
{
    ChemicalCompound::Type mCompound;    /**< (1) trick_chkpnt_io(**) The chemical compound that interacts. */
    double                 mInteraction; /**< (1) trick_chkpnt_io(**) Interaction amount. */
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sorbate Properties Model
///
/// @details  This models the properties of a single sorbate with respect to a sorbant.  It holds
///           Toth isotherm equation constants, heat of adsorption, and adsorption time constant.
///           It functions for computing loading equilibrium, loading rate, and heat flux.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SorbateProperties
{
    TS_MAKE_SIM_COMPATIBLE(SorbateProperties);
    public:
        /// @brief  Default constructs this Sorbate Properties.
        SorbateProperties(const ChemicalCompound*                         compound,
                          const std::vector<SorbateInteractingCompounds>* blockingCompounds,
                          const std::vector<SorbateInteractingCompounds>* offgasCompunds,
                          const double                                    tothA0,
                          const double                                    tothB0,
                          const double                                    tothE,
                          const double                                    tothT0,
                          const double                                    tothC0,
                          const double                                    dh,
                          const double                                    km);
        /// @brief  Default destructs this Sorbate Properties.
        virtual ~SorbateProperties();
        /// @brief  Copy constructs this Sorbate Properties.
        SorbateProperties(const SorbateProperties& that);
        /// @brief  Assigns this Sorbate Properties.
        SorbateProperties& operator =(const SorbateProperties& that);
        /// @brief  Computes and returns the equilibrium loading at current conditions.
        double computeLoadingEquil(const double pp, const double temperature) const;
        /// @brief  Computes and returns the sorption loading rate.
        double computeLoadingRate(const double loadingEquil, const double loading) const;
        /// @brief  Computes and returns the heat of sorption.
        double computeHeatFlux(const double adsorptionRate) const;
        /// @brief  Returns the chemical compound properties of this Sorbate.
        const ChemicalCompound* getCompound() const;
        /// @brief  Returns the blocking chemical compound types of this Sorbate Properties.
        const std::vector<SorbateInteractingCompounds>* getBlockingCompounds() const;
        /// @brief  Returns the offgassing chemical compound types of this Sorbate Properties.
        const std::vector<SorbateInteractingCompounds>* getOffgasCompounds() const;
        /// @brief  Adds a blocking compound with the given properties to this Sorbate Properties.
        void addBlockingCompound(const ChemicalCompound::Type compound, const double interaction);
        /// @brief  Adds an offgas compound with the given properties to this Sorbate Properties.
        void addOffgasCompound(const ChemicalCompound::Type compound, const double interaction);

    protected:
        const ChemicalCompound*                  mCompound;          /**<    (1)             trick_chkpnt_io(**) Chemical compound properties of this sorbate. */
        const double                             mTothA0;            /**<    (kg*mol/kg/kPa) trick_chkpnt_io(**) Toth isotherm parameter a0. */
        const double                             mTothB0;            /**<    (1/kPa)         trick_chkpnt_io(**) Toth isotherm parameter b0. */
        const double                             mTothE;             /**<    (K)             trick_chkpnt_io(**) Toth isotherm parameter E. */
        const double                             mTothT0;            /**<    (1)             trick_chkpnt_io(**) Toth isotherm parameter t0. */
        const double                             mTothC0;            /**<    (K)             trick_chkpnt_io(**) Toth isotherm parameter c0. */
        const double                             mDh;                /**<    (kJ/mol)        trick_chkpnt_io(**) Heat of adsorption of this sorbate in the sorbant. */
        const double                             mKm;                /**<    (1/s)           trick_chkpnt_io(**) Sorption time constant. */
        std::vector<SorbateInteractingCompounds> mBlockingCompounds; /**< ** (1)             trick_chkpnt_io(**) List of chemical compounds that inhibit sorbtion of this sorbate. */
        std::vector<SorbateInteractingCompounds> mOffgasCompounds;   /**< ** (1)             trick_chkpnt_io(**) List of chemical compounds that are offgassed during desorption of this sorbate. */
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sorbant Properties Model
///
/// @details  This models the properties of a sorbant material and its interactions with sorbates.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SorbantProperties {
    TS_MAKE_SIM_COMPATIBLE(SorbantProperties);
    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief    Enumeration of the Sorbant types.
        ///
        /// @details  This enumeration is used to index the array of Defined Sorbant Properties.
        ////////////////////////////////////////////////////////////////////////////////////////////
        enum Type {
            SILICA_GEL_B125     = 0, ///< Sylobead B125 brand silica gel
            SILICA_GEL_40       = 1, ///< Grace Grade 40 brand silica gel
            ZEO_5A_RK38         = 2, ///< 5A zeolite type RK38
            ZEO_5A_522          = 3, ///< Grace Grade 522 brand 5A zeolite
            ZEO_13X_544         = 4, ///< Grace Grade 544 brand 13X zeolite
            SA9T                = 5, ///< SA9T solid amine
            GLASS_BEADS_LATTICE = 6, ///< Inert glass beads with lattice packing
            GLASS_BEADS_RANDOM  = 7, ///< Inert glass beads with random packing
            CUSTOM              = 8  ///< Custom type and number of sorbant types - keep this last!
        };
        /// @brief  Default constructs this Sorbant Properties with arguments.
        SorbantProperties(const SorbantProperties::Type type     = SorbantProperties::CUSTOM,
                          const double                  density  = 0.0,
                          const double                  porosity = 0.0,
                          const double                  cp       = 0.0);
        /// @brief  Default destructs this Sorbant Properties.
        virtual ~SorbantProperties();
        /// @brief  Copy constructs this Sorbant Properties.
        SorbantProperties(const SorbantProperties&);
        /// @brief  Assigns this Sorbant Properties.
        SorbantProperties& operator =(const SorbantProperties&);
        /// @brief  Adds a sorbate with given properties to this Sorbant Properties.
        SorbateProperties* addSorbate(const ChemicalCompound::Type                    compound,
                                      const std::vector<SorbateInteractingCompounds>* blockingCompounds,
                                      const std::vector<SorbateInteractingCompounds>* offgasCompunds,
                                      const double                                    tothA0,
                                      const double                                    tothB0,
                                      const double                                    tothE,
                                      const double                                    tothT0,
                                      const double                                    tothC0,
                                      const double                                    dh,
                                      const double                                    km);
        /// @brief  Returns the sorbate properties for this sorbant.
        const std::vector<SorbateProperties>* getSorbates() const;
        /// @brief  Returns the volume of sorbant material in the enclosure, not including voids.
        double computeVolume(const double enclosureVolume) const;
        /// @brief  Returns the thermal capacity of the sorbant material mass.
        double computeThermalCapacity(const double volume) const;
        /// @brief  Returns this sorbant type.
        SorbantProperties::Type getType() const;

    protected:
        const Type                     mType;             /**<    (1)      trick_chkpnt_io(**) Defined type of this sorbant. */
        const double                   mDensity;          /**<    (kg/m3)  trick_chkpnt_io(**) Density of the sorbant material. */
        const double                   mPorosity;         /**<    (1)      trick_chkpnt_io(**) Fraction of the packed sorbant enclosure volume that is voids. */
        const double                   mCp;               /**<    (J/kg/K) trick_chkpnt_io(**) Specific heat of the sorbant material. */
        std::vector<SorbateProperties> mSorbates;         /**< ** (1)      trick_chkpnt_io(**) List of sorbate properties for this sorbant. */
        /// - A DefinedChemicalCompounds is defined here, rather than in DefinedSorbantProperties,
        ///   so that models may create custom sorbants independent of DefinedSorbantProperties.
        const DefinedChemicalCompounds mDefinedCompounds; /**<    (1)      trick_chkpnt_io(**) Defined chemical compounds data. */
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Class for Defined Sorbant Properties.
///
/// @details  Provides the Defined Sorbant properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
class DefinedSorbantProperties{
    TS_MAKE_SIM_COMPATIBLE(DefinedSorbantProperties);
    public:
        /// @brief  Default constructs this Defined Sorbant Properties.
    DefinedSorbantProperties();
        /// @brief  Default destructs this Defined Sorbant Properties.
        virtual ~DefinedSorbantProperties();
        /// @brief  Returns a pointer to the specified Sorbant Properties.
        const SorbantProperties* getSorbant(const SorbantProperties::Type& type) const;

    protected:
        SorbantProperties              mSilicaGelB125;                       /**< (1) Silica Gel B125 sorbant. */
        SorbantProperties              mSilicaGel40;                         /**< (1) Silica Gel 40 sorbant. */
        SorbantProperties              mZeo5aRk38;                           /**< (1) Zeolite 5A RK38 sorbant. */
        SorbantProperties              mZeo5a522;                            /**< (1) Zeolite 5A 522 sorbant. */
        SorbantProperties              mZeo13x544;                           /**< (1) Zeolite 13X 544 sorbant. */
        SorbantProperties              mSa9t;                                /**< (1) SA9T sorbant. */
        SorbantProperties              mGlassBeadsLattice;                   /**< (1) Glass beads sorbant packed in a lattice geometry. */
        SorbantProperties              mGlassBeadsRandom;                    /**< (1) Glass beads sorbant packed in a random geometry. */
        SorbantProperties*             mSorbants[SorbantProperties::CUSTOM]; /**< (1) Sorbants array. */

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        DefinedSorbantProperties(const DefinedSorbantProperties&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        DefinedSorbantProperties& operator =(const DefinedSorbantProperties&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] loadingEquil (kg*mol/m3) The equilibrium loading.
/// @param[in] loading      (kg*mol/m3) The current loading.
///
/// @returns  double (kg*mol/m3/s) Adsorption rate.
///
/// @details  Returns the sorption loading rate as the sorption time constant multiplied by the
///           difference between the given equilibrium loading and current loading.  Positive rates
///           are adsorption and negative rates are desorption.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double SorbateProperties::computeLoadingRate(const double loadingEquil, const double loading) const
{
    return mKm * (loadingEquil - loading);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] adsorptionRate (kg*mol/m3/s) Adsorption rate.
///
/// @returns  double (W) Heat of sorption, positive values are exothermic.
///
/// @details  Computes and returns the heat of sorption as the adsorption rate multiplied by the
///           heat of sorption constant for this sorbate in the sorbant.  mDh sign convention is
///           opposite our output, so we flip the sign.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double SorbateProperties::computeHeatFlux(const double adsorptionRate) const
{
    return -adsorptionRate * mDh * UnitConversion::UNIT_PER_MEGA;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  ChemicalCompound* (--) Pointer to the chemical compound of this sorbate.
///
/// @details  Returns the chemical compound of this sorbate.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const ChemicalCompound* SorbateProperties::getCompound() const
{
    return mCompound;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  std::vector<SorbateInteractingCompounds>* (--) Pointer to the vector of blocking interactions.
///
/// @details  Returns the blocking compound interactions of this sorbate.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const std::vector<SorbateInteractingCompounds>* SorbateProperties::getBlockingCompounds() const
{
    return &mBlockingCompounds;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  std::vector<SorbateInteractingCompounds>* (--) Pointer to the vector of offgassing interactions.
///
/// @details  Returns the offgassing compound interactions of this sorbate.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const std::vector<SorbateInteractingCompounds>* SorbateProperties::getOffgasCompounds() const
{
    return &mOffgasCompounds;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] enclosureVolume (m3) Volume of the sorbant layer enclosure, including voids.
///
/// @returns  double (m3) Volume of sorbant material in the enclosure, not including voids.
///
/// @details  Given the enclosure volume, returns how much of that volume is occupied by the
///           actual sorbant material, not including voids.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double SorbantProperties::computeVolume(const double enclosureVolume) const
{
    return enclosureVolume * (1.0 - mPorosity);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] volume (m3) Volume of sorbant material, not including voids in the enclosure.
///
/// @returns  double (kJ/kg)  Thermal capacity of the sorbant material mass.
///
/// @details  Returns the thermal capacity of the sorbant material mass.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double SorbantProperties::computeThermalCapacity(const double volume) const
{
    return mCp * mDensity * volume;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  std::vector<SorbateProperties>* (--) Pointer to the vector of sorbates.
///
/// @details  Returns the sorbates of this sorbant.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const std::vector<SorbateProperties>* SorbantProperties::getSorbates() const
{
    return &mSorbates;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  SorbantProperties::Type (--) Enumerated type of this sorbant.
///
/// @details  Returns the enumerated type of this sorbant.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline SorbantProperties::Type SorbantProperties::getType() const
{
    return mType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type (--) Type of Sorbant Properties
///
/// @return   A pointer to the specified Sorbant Properties or null if undefined.
///
/// @details  Returns a pointer to the Sorbant Properties specified by the Sorbant Properties type.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const SorbantProperties* DefinedSorbantProperties::getSorbant(const SorbantProperties::Type& type) const
{
    SorbantProperties* result = 0;

    if (0 <= type && type < SorbantProperties::CUSTOM) {
        result = mSorbants[type];
    }

    return result;
}

#endif
