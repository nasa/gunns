/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Provides the classes for modeling Chemical Reactions.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     (Only those  Chemical Reactions in the Defined Chemical Reaction class are available.)

 LIBRARY DEPENDENCY:
     ((properties/ChemicalCompound.o))

 PROGRAMMERS:
     ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-05))
 **************************************************************************************************/

#include "ChemicalReaction.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type               --      Type of Chemical Reaction
/// @param[in]  reactantMoleRatio  --      Mole ratio of reactant in Chemical Reaction
/// @param[in]  reactantType       --      Chemical compound type of reactant in Chemical Reaction
/// @param[in]  reagentMoleRatio   --      Mole ratio of reagent in Chemical Reaction
/// @param[in]  reagentType        --      Chemical compound type of reagent in Chemical Reaction
/// @param[in]  product1MoleRatio  --      Mole ratio of product 1 in Chemical Reaction
/// @param[in]  product1Type       --      Chemical compound type of product 1 in Chemical Reaction
/// @param[in]  product2MoleRatio  --      Mole ratio of product 2 in Chemical Reaction
/// @param[in]  product2Type       --      Chemical compound type of product 2 in Chemical Reaction
/// @param[in]  reactantEfficiency --      Efficiency of the reaction in removing the reactant
/// @param[in]  maxReactantRate    (kg/s)  Maximum rate the reaction can remove the reactant
///
/// @details  Default constructs this Chemical Reaction with the specified arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
ChemicalReaction::ChemicalReaction( const ChemicalReaction::Type type,
                                    const double                 reactantMoleRatio,
                                    const ChemicalCompound::Type reactantType,
                                    const double                 reagentMoleRatio,
                                    const ChemicalCompound::Type reagentType,
                                    const double                 product1MoleRatio,
                                    const ChemicalCompound::Type product1Type,
                                    const double                 product2MoleRatio,
                                    const ChemicalCompound::Type product2Type,
                                    const double                 reactantEfficiency,
                                    const double                 maxReactantRate,
                                    const double                 reactionHeat,
                                    const double                 reactionScaleFactor)
    :
    mType(type),
    mReactantMoleRatio(reactantMoleRatio),
    mReactantType(reactantType),
    mReagentMoleRatio(reagentMoleRatio),
    mReagentType(reagentType),
    mProduct1MoleRatio(product1MoleRatio),
    mProduct1Type(product1Type),
    mProduct2MoleRatio(product2MoleRatio),
    mProduct2Type(product2Type),
    mReactantEfficiency(reactantEfficiency),
    mMaxReactantRate(maxReactantRate),
    mReactionHeat(reactionHeat),
    mReactionHeatScaleFactor(reactionScaleFactor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Chemical Reaction.
////////////////////////////////////////////////////////////////////////////////////////////////////
ChemicalReaction::~ChemicalReaction()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Defined Chemical Reactions.
///
/// @note  The LiOH reactions (LIOHH2O_SYNTHESIS & LI2CO3_FORMATION) are configured work as parallel
///        reactions in a GunnsFluidHotReactor link to model a LiOH CO2 scrubber cannister.  H2O is
///        required in the input stream to form enough of the LiOH*H2O monohydrate to then react
///        with CO2 in the input stream.  This models a realistic dependency on H2O input and lag
///        between when flow starts thru the cannister and when it ramps up its CO2 absorption.  Use
///        lower efficiencies < 0.5 in the HotReactor to increase the lag.
////////////////////////////////////////////////////////////////////////////////////////////////////
DefinedChemicalReactions::DefinedChemicalReactions()
    :
    mReactionCORemoval( ChemicalReaction::CO_REMOVAL,
                        2.0,  ChemicalCompound::CO,      1.0, ChemicalCompound::O2,
                        2.0,  ChemicalCompound::CO2,     0.0, ChemicalCompound::NO_COMPOUND,
                        0.80, 0.001, 0.0, 0.0),
    mReactionCH4Removal(ChemicalReaction::CH4_REMOVAL,
                        1.0,  ChemicalCompound::CH4,     2.0, ChemicalCompound::O2,
                        1.0,  ChemicalCompound::CO2,     2.0, ChemicalCompound::H2O,
                        0.64, 0.001, 0.0, 0.0),
    mReactionH2Removal( ChemicalReaction::H2_REMOVAL,
                        2.0,  ChemicalCompound::H2,      1.0, ChemicalCompound::O2,
                        2.0,  ChemicalCompound::H2O,     0.0, ChemicalCompound::NO_COMPOUND,
                        1.00, 0.001, 0.0, 0.0),
    mReactionLiOHH2OSynthesis(ChemicalReaction::LIOHH2O_SYNTHESIS,
                        1.0, ChemicalCompound::H2O,      1.0, ChemicalCompound::LIOH,
                        1.0, ChemicalCompound::LIOHH2O,  0.0, ChemicalCompound::NO_COMPOUND,
                        0.5, 0.001, 60.668, 1.0),
    mReactionLi2CO3Formation(ChemicalReaction::LI2CO3_FORMATION,
                        1.0, ChemicalCompound::CO2,      2.0, ChemicalCompound::LIOHH2O,
                        1.0, ChemicalCompound::LI2CO3,   3.0, ChemicalCompound::H2O,
                        1.0, 0.001, -15.9, 1.0),
    mReactionNH3Removal(ChemicalReaction::NH3_REMOVAL,
                        2.0, ChemicalCompound::NH3,      1.0, ChemicalCompound::H3PO4,
                        1.0, ChemicalCompound::NH42HPO4, 0.0, ChemicalCompound::NO_COMPOUND,
                        0.90, 0.001, 0.0, 0.0),
    mReactionSabatier(ChemicalReaction::SABATIER_RXN,
                        1.0, ChemicalCompound::CO2,      4.0, ChemicalCompound::H2,
                        2.0, ChemicalCompound::H2O,      1.0, ChemicalCompound::CH4,
                        0.90, 0.001, 165.0, 1.0),
    mReactions()
{
    /// - Load the chemical reactions array with the pointers to the reactions
    mReactions[ChemicalReaction::CO_REMOVAL]        = &mReactionCORemoval;
    mReactions[ChemicalReaction::CH4_REMOVAL]       = &mReactionCH4Removal;
    mReactions[ChemicalReaction::H2_REMOVAL]        = &mReactionH2Removal;
    mReactions[ChemicalReaction::LIOHH2O_SYNTHESIS] = &mReactionLiOHH2OSynthesis;
    mReactions[ChemicalReaction::LI2CO3_FORMATION]  = &mReactionLi2CO3Formation;
    mReactions[ChemicalReaction::NH3_REMOVAL]       = &mReactionNH3Removal;
    mReactions[ChemicalReaction::SABATIER_RXN]      = &mReactionSabatier;
 }

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Defined Chemical Reactions.
////////////////////////////////////////////////////////////////////////////////////////////////////
DefinedChemicalReactions::~DefinedChemicalReactions()
{
    // nothing to do
}
