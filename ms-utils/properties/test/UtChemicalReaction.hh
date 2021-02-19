#ifndef UtChemicalReaction_EXISTS
#define UtChemicalReaction_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_PROPERTIES_CHEMICAL_REACTION  Chemical Reaction Properties Unit Tests
/// @ingroup  UT_TSM_PROPERTIES
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Chemical Reaction Properties model classes.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Chemical Reaction Properties unit tests.
////
/// @details  This class provides the unit tests for the Chemical Reaction Properties within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtChemicalReaction: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Chemical Reaction Properties unit test.
        UtChemicalReaction();
        /// @brief    Default destructs this Chemical Reaction Properties unit test.
        ~UtChemicalReaction();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        void testConstruction();
        void testConsistency();
        void testMassConservation();
        void testReactionRate();
    private:
        CPPUNIT_TEST_SUITE(UtChemicalReaction);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testConsistency);
        CPPUNIT_TEST(testMassConservation);
        CPPUNIT_TEST(testReactionRate);
        CPPUNIT_TEST_SUITE_END();
        double      mTolerance;  /**< (--)  Nominal tolerance for comparison of expected and returned values. */
        static int  TEST_ID;     /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtChemicalReaction(const UtChemicalReaction&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtChemicalReaction& operator =(const UtChemicalReaction&);
};

/// @}

#endif
