#ifndef SorbantProperties_EXISTS
#define SorbantProperties_EXISTS

/**
@file     SorbantProperties.hh
@brief    Chemical Sorbant & Sorbate Properties declarations

@defgroup  TSM_UTILITIES_PROPERTIES_SORBANT  Chemical Sorbant & Sorbate Properties
@ingroup   TSM_UTILITIES_PROPERTIES

@copyright Copyright 2022 United States Government as represented by the Administrator of the
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

//TODO
// - this is used for both blocking properties and offgas properties:
//   - for blocking, mInteraction is the relationship between sorbate and blocking compound loading fractions
//   - for offgassing, mInteraction is the mole ratio of offgassed compound to desorbed sorbate
struct SorbateInteractingCompounds
{
    ChemicalCompound::Type mCompound;    /**< (1) trick_chkpnt_io(**) The chemical compound that interacts. */
    double                 mInteraction; /**< (1) trick_chkpnt_io(**) Interaction amount. */
};

//TODO
//struct SorbateOffgasProperties
//{
//    ChemicalCompound::Type mCompound;  /**< (1) trick_chkpnt_io(**) The chemical compound that is offgassed. */
//    double                 mMoleRatio; /**< (1) trick_chkpnt_io(**) Moles of offgas per moles desorbed. */
//};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sorbate Properties Model
///
/// @details  This models the properties of a single sorbate with respect to a sorbant.  It holds
///           Toth isotherm equation constants, heat of adsorption, and adsorption time constant.
///           It has a function for computing loading equilibrium.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SorbateProperties
{
    //TODO if this only ever exists in a vector, then we don't need sim compatible or chkpnt_io defs.
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
        //TODO
        double computeLoadingRate(const double loadingEquil, const double loading) const;
        //TODO
        double computeHeatFlux(const double adsorptionRate) const;
        /// @brief  Returns the chemical compound properties of this Sorbate.
        const ChemicalCompound* getCompound() const;
        /// @brief  Returns the blocking chemical compound types of this Sorbate Properties.
        const std::vector<SorbateInteractingCompounds>* getBlockingCompounds() const;
        /// @brief  Returns the offgassing chemical compound types of this Sorbate Properties.
        const std::vector<SorbateInteractingCompounds>* getOffgasCompounds() const;

    protected:
        const ChemicalCompound* mCompound; /**< (1)             trick_chkpnt_io(**) Chemical compound properties of this sorbate. */
        const double            mTothA0;   /**< (kg*mol/kg/kPa) trick_chkpnt_io(**) Toth isotherm parameter a0. */
        const double            mTothB0;   /**< (1/kPa)         trick_chkpnt_io(**) Toth isotherm parameter b0. */
        const double            mTothE;    /**< (K)             trick_chkpnt_io(**) Toth isotherm parameter E. */
        const double            mTothT0;   /**< (1)             trick_chkpnt_io(**) Toth isotherm parameter t0. */
        const double            mTothC0;   /**< (K)             trick_chkpnt_io(**) Toth isotherm parameter c0. */
        const double            mDh;       /**< (kJ/mol)        trick_chkpnt_io(**) Heat of adsorption of this sorbate in the sorbant. */
        const double            mKm;       /**< (1/s)           trick_chkpnt_io(**) Adsorption time constant. */
        std::vector<SorbateInteractingCompounds> mBlockingCompounds; /**< (1) trick_chkpnt_io(**) List of chemical compounds that inhibit sorbtion of this sorbate. */
        std::vector<SorbateInteractingCompounds> mOffgasCompounds;   /**< (1) trick_chkpnt_io(**) List of chemical compounds that are offgassed during desorption of this sorbate. */
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sorbant Properties Model
///
/// @details  This models the properties of a sorbant material and its interactions with sorbates.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SorbantProperties {
    TS_MAKE_SIM_COMPATIBLE(SorbantProperties);
    public:
        friend class DefinedSorbantPropertiess;
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief    Enumeration of the Sorbant types.
        ///
        /// @details  This enumeration is used to index the array of Defined Sorbant Properties.
        ////////////////////////////////////////////////////////////////////////////////////////////
        enum Type {
            SILICA_GEL_B125 = 0, ///< Sylobead B125 brand silica gel
            SILICA_GEL_40   = 1, ///< Grace Grade 40 brand silica gel
            ZEO_5A_RK38     = 2, ///< 5A zeolite type RK38
            ZEO_5A_522      = 3, ///< Grace Grade 522 brand 5A zeolite
            ZEO_13X_544     = 4, ///< Grace Grade 544 brand 13X zeolite
            SA9T            = 5, ///< SA9T solid amine
            GLASS_BEADS     = 6, ///< Inert glass beads
            CUSTOM          = 7  ///< Custom type and number of sorbant types - keep this last!
            //TODO add "Sorbead WS Zeolite"
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
        void addSorbate(const ChemicalCompound::Type                    compound,
                        const std::vector<SorbateInteractingCompounds>* blockingCompounds,
                        const std::vector<SorbateInteractingCompounds>* offgasCompunds,
                        const double                                    tothA0,
                        const double                                    tothB0,
                        const double                                    tothE,
                        const double                                    tothT0,
                        const double                                    tothC0,
                        const double                                    dh,
                        const double                                    km);
        //TODO
        const std::vector<SorbateProperties>* getSorbates() const;
        //TODO
        double computeVolume(const double enclosureVolume) const;
        //TODO
        double computeThermalCapacity(const double volume) const;

    protected:
        //TODO some mapping between compound type and sorbate vector index, or get compound from sorbate
        //TODO loading interactions: blocking compounds
        //  - it scales the loading equilibrium of a sorbate as a function of the other sorbate's loading
        //  - equil. loading --> 0 as other --> 1, no configuration needed.
        //  - so, this is just an optional other compound name for each sorbate
        //  - there may be more than 1 blocking compound
        //TODO offgas compounds:
        //  - can optionally offgas a different compound in proportion to sorption
        //  - sorbates degrade over time, offgassing the products of degradation.  For example,
        //    amines offgas ammonia as they degrade...
        //  - separate for each sorbate?
        //  - is similar to loading interactions
        const Type                  mType;        /**< (--)     trick_chkpnt_io(**) Defined type of this sorbant. */
        const double                mDensity;     /**< (kg/m3)  trick_chkpnt_io(**) Density of the sorbant material. */
        const double                mPorosity;    /**< (1)      trick_chkpnt_io(**) Fraction of the packed sorbant enclosure volume that is voids. */
        const double                mCp;          /**< (J/kg/K) trick_chkpnt_io(**) Specific heat of the sorbant material. */
        std::vector<SorbateProperties> mSorbates; //TODO
        const DefinedChemicalCompounds mDefinedCompounds; //TODO
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
        SorbantProperties  mSilicaGelB125;                        /**< (--) Silica Gel B125 sorbant. */
        SorbantProperties  mSilicaGel40;                          /**< (--) Silica Gel 40 sorbant. */
        SorbantProperties  mZeo5aRk38;                            /**< (--) Zeolite 5A RK38 sorbant. */
        SorbantProperties  mZeo5a522;                             /**< (--) Zeolite 5A 522 sorbant. */
        SorbantProperties  mZeo13x544;                            /**< (--) Zeolite 13X 544 sorbant. */
        SorbantProperties  mSa9t;                                 /**< (--) SA9T sorbant. */
        SorbantProperties  mGlassBeads;                           /**< (--) Glass beads sorbant. */
        SorbantProperties* mSorbants[SorbantProperties::CUSTOM];  /**< (--) Sorbants array. */

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        DefinedSorbantProperties(const DefinedSorbantProperties&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        DefinedSorbantProperties& operator =(const DefinedSorbantProperties&);
};

/// @}

//TODO
inline const ChemicalCompound* SorbateProperties::getCompound() const
{
    return mCompound;
}

//TODO
inline const std::vector<SorbateInteractingCompounds>* SorbateProperties::getBlockingCompounds() const
{
    return &mBlockingCompounds;
}

//TODO
inline const std::vector<SorbateInteractingCompounds>* SorbateProperties::getOffgasCompounds() const
{
    return &mOffgasCompounds;
}

//TODO
inline const std::vector<SorbateProperties>* SorbantProperties::getSorbates() const
{
    return &mSorbates;
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
