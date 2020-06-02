/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((properties/ChemicalReaction.o))
***************************************************************************************************/

#include <iostream>

#include "properties/ChemicalReaction.hh"

#include "UtChemicalReaction.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Chemical Reaction Properties model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtChemicalReaction::UtChemicalReaction()
    :
    CppUnit::TestFixture(),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Chemical Reaction Properties model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtChemicalReaction::~UtChemicalReaction()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtChemicalReaction::setUp()
{
    mTolerance = 2.0e-04;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtChemicalReaction::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtChemicalReaction::testConstruction()
{
    std::cout << "--------------------------------------------------------------------------------";
    std::cout << "\n Chemical Reaction Properties 01: Nominal Construction Test             ";

    {
        /// @test object for creation of properties for each reaction type.
        DefinedChemicalReactions reactions;
        for (int i = 0; i < ChemicalReaction::NO_REACTION; i++) {
            const ChemicalReaction::Type expected = static_cast<ChemicalReaction::Type>(i);
            const ChemicalReaction::Type returned = reactions.getReaction(expected)->mType;
            CPPUNIT_ASSERT(expected == returned);
        }
    } {
        /// @test pointer for creation of properties for each reaction type.
        DefinedChemicalReactions* reactions;
        TS_NEW_CLASS_OBJECT(reactions, DefinedChemicalReactions, ());
        for (int i = 0; i < ChemicalReaction::NO_REACTION; i++) {
            const ChemicalReaction::Type expected = static_cast<ChemicalReaction::Type>(i);
            const ChemicalReaction::Type returned = reactions->getReaction(expected)->mType;
            CPPUNIT_ASSERT(expected == returned);
        }
        TS_DELETE_OBJECT(reactions);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for consistency.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtChemicalReaction::testConsistency()
{
    std::cout << "\n Chemical Reaction Properties 02: Consistency Test                      ";

    DefinedChemicalReactions reactions;
    for (int i = 0; i < ChemicalReaction::NO_REACTION; i++) {
        const ChemicalReaction::Type reactionType = static_cast<ChemicalReaction::Type>(i);

        /// @test reactant mole ratio >= 1.0.
        CPPUNIT_ASSERT(reactions.getReaction(reactionType)->mReactantMoleRatio >= 1.0);

        /// @test reactant fluid type defined
        CPPUNIT_ASSERT(reactions.getReaction(reactionType)->mReactantType !=
                       ChemicalCompound::NO_COMPOUND);

        /// @test reagent mole ratio >= 1.0.
        CPPUNIT_ASSERT(reactions.getReaction(reactionType)->mReagentMoleRatio >= 1.0);

        /// @test reagent fluid type defined
        CPPUNIT_ASSERT(reactions.getReaction(reactionType)->mReagentType !=
                       ChemicalCompound::NO_COMPOUND);

        /// @test product 1 mole ratio >= 1.0.
        CPPUNIT_ASSERT(reactions.getReaction(reactionType)->mProduct1MoleRatio >= 1.0);

        /// @test product 1 fluid type defined
        CPPUNIT_ASSERT(reactions.getReaction(reactionType)->mProduct1Type !=
                       ChemicalCompound::NO_COMPOUND);

        /// @test product 2 mole ratio >= 1.0 if defined
        if (reactions.getReaction(reactionType)->mProduct2Type != ChemicalCompound::NO_COMPOUND) {
            CPPUNIT_ASSERT(reactions.getReaction(reactionType)->mProduct2MoleRatio >= 1.0);
        }

        /// @test 0 <= reactant efficiency
        CPPUNIT_ASSERT(reactions.getReaction(reactionType)->mReactantEfficiency >= 0.0);

        /// @test 1 >= reactant efficiency
        CPPUNIT_ASSERT(reactions.getReaction(reactionType)->mReactantEfficiency <= 1.0);

        /// @test maximum reactant removal rate > = 0.
        CPPUNIT_ASSERT(reactions.getReaction(reactionType)->mMaxReactantRate >= 0.0);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for conservation of mass (within 0.0002 mole) in chemical reactions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtChemicalReaction::testMassConservation()
{
    std::cout << "\n Chemical Reaction Properties 03: Conservation of Mass Test             ";

    DefinedChemicalCompounds compounds;
    DefinedChemicalReactions reactions;

    /// @test for conservation of mass.
    for (int i = 0; i < ChemicalReaction::NO_REACTION; i++) {
        const ChemicalReaction::Type reactionType = static_cast<ChemicalReaction::Type>(i);
        ChemicalCompound::Type type = reactions.getReaction(reactionType)->mReactantType;
        double balance  = reactions.getReaction(reactionType)->mReactantMoleRatio *
                          compounds.getCompound(type)->mMWeight;
        type            = reactions.getReaction(reactionType)->mReagentType;
        balance        += reactions.getReaction(reactionType)->mReagentMoleRatio *
                          compounds.getCompound(type)->mMWeight;
        type            = reactions.getReaction(reactionType)->mProduct1Type;
        balance        -= reactions.getReaction(reactionType)->mProduct1MoleRatio *
                          compounds.getCompound(type)->mMWeight;
        type            = reactions.getReaction(reactionType)->mProduct2Type;
        if (type != ChemicalCompound::NO_COMPOUND) {
            balance    -= reactions.getReaction(reactionType)->mProduct2MoleRatio *
                          compounds.getCompound(type)->mMWeight;
        }

        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, balance, mTolerance);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for reaction rate between 0.0 and 1.0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtChemicalReaction::testReactionRate()
{
    std::cout << "\n Chemical Reaction Properties 03: Reaction Rate Test                    ";

    DefinedChemicalReactions reactions;

    /// @test for reaction rate.
    for (int i = 0; i < ChemicalReaction::NO_REACTION; i++) {

        const ChemicalReaction::Type reactionType = static_cast<ChemicalReaction::Type>(i);
        double efficiency = reactions.getReaction(reactionType)->mReactantEfficiency;

        CPPUNIT_ASSERT(0.0 <= efficiency && efficiency <= 1.0);
    }

    std::cout << "... Pass" << std::endl;
    std::cout << "--------------------------------------------------------------------------------";
}
