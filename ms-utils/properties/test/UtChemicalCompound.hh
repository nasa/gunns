#ifndef UtChemicalCompound_EXISTS
#define UtChemicalCompound_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_PROPERTIES_CHEMICAL_COMPOUND  Chemical Compound Properties Unit Tests
/// @ingroup  UT_TSM_PROPERTIES
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Chemical Compound Properties model classes.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Chemical Compound Properties unit tests.
////
/// @details  This class provides the unit tests for the Chemical Compound Properties within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtChemicalCompound: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Chemical Compound Properties unit test.
        UtChemicalCompound();
        /// @brief    Default destructs this Chemical Compound Properties unit test.
        ~UtChemicalCompound();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        void testConstruction();
        void testConsistency();
    private:
        CPPUNIT_TEST_SUITE(UtChemicalCompound);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testConsistency);
        CPPUNIT_TEST_SUITE_END();
        double      mTolerance;  /**< (--)  Nominal tolerance for comparison of expected and returned values. */
        static int  TEST_ID;     /**< (--)  Test identification number. */
       ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtChemicalCompound(const UtChemicalCompound&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtChemicalCompound& operator =(const UtChemicalCompound&);
};

/// @}

#endif
