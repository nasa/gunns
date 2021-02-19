#ifndef UtMaterialProperties_EXISTS
#define UtMaterialProperties_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_PROPERTIES_MATERIAL    Material Properties Unit Tests
/// @ingroup  UT_TSM_PROPERTIES
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Material Properties model classes.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "properties/MaterialProperties.hh"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief       Material Properties unit tests.
///
/// @details     This class provides the unit tests for the Material Properties within the CPPUnit
///              framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtMaterialProperties: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Material Properties unit test.
        UtMaterialProperties();
        /// @brief    Default destructs this Material Properties unit test.
        ~UtMaterialProperties();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        void checkMinimums(const MaterialProperties &props);
        void Teststeel304();
        void Testaluminum6061();
        void TestDefaultConstructor();
        void GetKnownProperty();
        void GetUnknownProperty();
        void AllProertiesTested();
    private:
        CPPUNIT_TEST_SUITE(UtMaterialProperties);
        CPPUNIT_TEST(Teststeel304);
        CPPUNIT_TEST(Testaluminum6061);
        CPPUNIT_TEST(TestDefaultConstructor);
        CPPUNIT_TEST(GetKnownProperty);
        CPPUNIT_TEST(GetUnknownProperty);
        CPPUNIT_TEST(AllProertiesTested);
        CPPUNIT_TEST_SUITE_END();
        double minValue; // no obvious min for the properties
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtMaterialProperties(const UtMaterialProperties&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtMaterialProperties& operator =(const UtMaterialProperties&);
};

/// @}

#endif
