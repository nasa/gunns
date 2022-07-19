#ifndef UtSorbantProperties_EXISTS
#define UtSorbantProperties_EXISTS

/*
@file   UtSorbantProperties.hh
@brief  Unit tests for SorbantProperties

@defgroup UT_TSM_PROPERTIES_SORBANT  Sorbant Properties Unit Tests
@ingroup  UT_TSM_PROPERTIES

@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details  Unit Tests for the Sorbant Properties model classes.
@{
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Chemical Compound Properties unit tests.
////
/// @details  This class provides the unit tests for the Sorbant Properties within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtSorbantProperties: public CppUnit::TestFixture
{
    public:
        /// @brief  Default constructs this Sorbant Properties unit test.
        UtSorbantProperties();
        /// @brief  Default destructs this Sorbant Properties unit test.
        ~UtSorbantProperties();
        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Executes after each test.
        void tearDown();
        void testDefinedSorbantsConstruction();
        void testDefinedSorbantsProperties();
        void testDefinedSorbatesProperties();
        void testCustomSorbant();
        void testConstructionExceptions();
        /// @brief  Test function for Toth equilibrium loading.
        double computeExpectedLoadingEquil(const double tothA, const double tothB,
                const double tothE, const double tothT, const double tothC, const double pp,
                const double T) const;

    private:
        CPPUNIT_TEST_SUITE(UtSorbantProperties);
        CPPUNIT_TEST(testDefinedSorbantsConstruction);
        CPPUNIT_TEST(testDefinedSorbantsProperties);
        CPPUNIT_TEST(testDefinedSorbatesProperties);
        CPPUNIT_TEST(testCustomSorbant);
        CPPUNIT_TEST(testConstructionExceptions);
        CPPUNIT_TEST_SUITE_END();
        static int  TEST_ID;     /**< (--)  Test identification number. */
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        UtSorbantProperties(const UtSorbantProperties&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        UtSorbantProperties& operator =(const UtSorbantProperties&);
};

/// @}

#endif
