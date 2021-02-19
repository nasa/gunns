#ifndef ChemicalReaction_EXISTS
#define ChemicalReaction_EXISTS
/**
@defgroup  TSM_UTILITIES_PROPERTIES_CHEMICAL_REACTION  Chemical Reactions
@ingroup   TSM_UTILITIES_PROPERTIES

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for modeling Chemical Reactions.)

REQUIREMENTS:
- ()

REFERENCE:
- (
  )

ASSUMPTIONS AND LIMITATIONS:
- (Only those  Chemical Reactions in the Defined Chemical Reaction class are available.)

 CLASS:
- ()

 LIBRARY DEPENDENCY:
- ((ChemicalReaction.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-05))

@{
*/

#include "properties/ChemicalCompound.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Class for a Chemical Reaction.
///
/// @details  Provides a class for a Chemical Reaction: compounds, mole ratios, reactant efficiency
///           and maximum reactant removal rate for a chemical reaction that consumes one reactant
///           and one reagent, producing one or two products.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ChemicalReaction {
    TS_MAKE_SIM_COMPATIBLE(ChemicalReaction);
    public:
        friend class DefinedChemicalReactions;
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief    Enumeration of the types of Chemical Reactions.
        ///
        /// @details  This enumeration is used to index the array of Defined Chemical Reactions.
        ////////////////////////////////////////////////////////////////////////////////////////////
        enum Type {
            CO_REMOVAL        = 0,   ///<  2CO + O2 --> 2CO2
            CH4_REMOVAL       = 1,   ///<  CH4 + 2O2 --> CO2 + 2H2O
            H2_REMOVAL        = 2,   ///<  2H2 + O2 --> 2H2O
            LIOHH2O_SYNTHESIS = 3,   ///<  H2O + LiOH --> LiOH*H2O
            LI2CO3_FORMATION  = 4,   ///<  CO2 + 2LiOH*H2O --> Li2CO3 + 3H2O
            NH3_REMOVAL       = 5,   ///<  2NH3 + H3PO4 --> (NH4)2(HPO4)
            SABATIER_RXN      = 6,   ///<  CO2 + 4H2 --> 2H2O + CH4
            NO_REACTION       = 7,   ///<  Invalid or number of reactions - Keep this last!
        };
        const ChemicalReaction::Type mType;                    /**< (--)     Reaction type. */
        const double                 mReactantMoleRatio;       /**< (--)     Reactant mole ratio. */
        const ChemicalCompound::Type mReactantType;            /**< (--)     Reactant compound type. */
        const double                 mReagentMoleRatio;        /**< (--)     Reagent mole ratio. */
        const ChemicalCompound::Type mReagentType;             /**< (--)     Reagent compound type. */
        const double                 mProduct1MoleRatio;       /**< (--)     Product 1 mole ratio. */
        const ChemicalCompound::Type mProduct1Type;            /**< (--)     Product 1 compound type. */
        const double                 mProduct2MoleRatio;       /**< (--)     Product 2 mole ratio. */
        const ChemicalCompound::Type mProduct2Type;            /**< (--)     Product 2 compound type. */
        const double                 mReactantEfficiency;      /**< (--)     Efficiency of reaction in removing the rectant. */
        const double                 mMaxReactantRate;         /**< (kg/s)   Max rate the reaction can remove the reactant. */
        const double                 mReactionHeat;            /**< (kJ/mol) Reaction heat added or removed (+ exo, - endothermic). */
        const double                 mReactionHeatScaleFactor; /**< (--)     Reaction heat scale factor. */
        /// @brief    Default constructs this Chemical Reaction with arguments.
        ChemicalReaction( const ChemicalReaction::Type type               = ChemicalReaction::NO_REACTION,
                          const double                 reactantMoleRatio  = 0.0,
                          const ChemicalCompound::Type reactantType       = ChemicalCompound::NO_COMPOUND,
                          const double                 reagentMoleRatio   = 0.0,
                          const ChemicalCompound::Type reagentType        = ChemicalCompound::NO_COMPOUND,
                          const double                 product1MoleRatio  = 0.0,
                          const ChemicalCompound::Type product1Type       = ChemicalCompound::NO_COMPOUND,
                          const double                 product2MoleRatio  = 0.0,
                          const ChemicalCompound::Type product2Type       = ChemicalCompound::NO_COMPOUND,
                          const double                 reactantEfficiency = 0.0,
                          const double                 maxReactantRate    = 0.0,
                          const double                 reactionHeat       = 0.0,
                          const double                 reactionHeatScaleFactor = 0.0);
        /// @brief    Default destructs this Chemical Reaction.
        virtual ~ChemicalReaction();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        ChemicalReaction(const ChemicalReaction&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        ChemicalReaction& operator =(const ChemicalReaction&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Class for Defined Chemical Reactions.
///
/// @details  Provides the Defined Chemical Reactions.
////////////////////////////////////////////////////////////////////////////////////////////////////
class DefinedChemicalReactions {
    TS_MAKE_SIM_COMPATIBLE(DefinedChemicalReactions);
    public:
        /// @brief    Default constructs this Defined Chemical Reactions.
        DefinedChemicalReactions();
        /// @brief    Default destructs this Defined Chemical Reactions.
        virtual ~DefinedChemicalReactions();
        /// @brief    Returns a pointer to the specified Chemical Reaction.
        const ChemicalReaction* getReaction(const ChemicalReaction::Type& type) const;
    protected:
        ChemicalReaction  mReactionCORemoval;                        /**< (--) CO removal chemical reaction. */
        ChemicalReaction  mReactionCH4Removal;                       /**< (--) CH4 removal chemical reaction. */
        ChemicalReaction  mReactionH2Removal;                        /**< (--) H2 removal chemical reaction. */
        ChemicalReaction  mReactionLiOHH2OSynthesis;                 /**< (--) LiOH*H2O synthesis reaction. */
        ChemicalReaction  mReactionLi2CO3Formation;                  /**< (--) Li2CO3 formation reaction. */
        ChemicalReaction  mReactionNH3Removal;                       /**< (--) NH3 removal chemical reaction. */
        ChemicalReaction  mReactionSabatier;                         /**< (--) Sabatier reaction. */
        ChemicalReaction* mReactions[ChemicalReaction::NO_REACTION]; /**< (--) Reactions array. */
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        DefinedChemicalReactions(const DefinedChemicalReactions&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        DefinedChemicalReactions& operator =(const DefinedChemicalReactions&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    type   Type of Chemical Reaction
///
/// @return   A pointer to the specified Chemical Reaction or null if undefined.
///
/// @details  Returns a pointer to the Chemical Reaction specified by Chemical Reaction type.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const ChemicalReaction* DefinedChemicalReactions::getReaction(const ChemicalReaction::Type& type) const
{
    ChemicalReaction* result = 0;

    if (0 <= type && type < ChemicalReaction::NO_REACTION) {
        result =  mReactions[type];
    }

    return result;
}

#endif
