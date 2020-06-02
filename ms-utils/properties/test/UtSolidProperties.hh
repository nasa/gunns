#ifndef UtSolidProperties_EXISTS
#define UtSolidProperties_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_PROPERTIES_SOLID     Solid Properties Unit Tests
/// @ingroup  UT_TSM_PROPERTIES
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Solid Properties model classes.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "properties/SolidProperties.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from DefinedSolidProperties and befriend UtSolidProperties.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlySolidProperties : public SolidProperties
{
    public:
        FriendlySolidProperties(
                  const SolidProperties::SolidType  type                = SolidProperties::NO_SOLID,
                  const double                      density             = 0.0,
                  const double                      specificHeat        = 0.0,
                  const double                      thermalConductivity = 0.0,
                  const double                      roughness           = 0.0);
        virtual ~FriendlySolidProperties();
        friend class UtSolidProperties;
};
inline FriendlySolidProperties::FriendlySolidProperties(
                                               const SolidProperties::SolidType type,
                                               const double                     density,
                                               const double                     specificHeat,
                                               const double                     thermalConductivity,
                                               const double                     roughness)
    :
    SolidProperties(type, density, specificHeat, thermalConductivity, roughness) {};
inline FriendlySolidProperties::~FriendlySolidProperties() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Predefined Solid Properties unit tests.
///
/// @details  This class provides the unit tests for the Solid Properties and Defined Solid
///           Properties models within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtSolidProperties: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Solid Properties unit test.
        UtSolidProperties();
        /// @brief    Default destructs this Solid Properties unit test.
        ~UtSolidProperties();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Test properties default construction.
        void testPropertyDefaultConstruction();
        /// @brief    Test properties construction with arguments.
        void testPropertyConstruction();
        /// @brief    Test properties accessors.
        void testPropertyAccessors();
        /// @brief    Test defined properties default construction.
        void testDefined();
    private:
        CPPUNIT_TEST_SUITE(UtSolidProperties);
        CPPUNIT_TEST(testPropertyDefaultConstruction);
        CPPUNIT_TEST(testPropertyConstruction);
        CPPUNIT_TEST(testPropertyAccessors);
        CPPUNIT_TEST(testDefined);
        CPPUNIT_TEST_SUITE_END();
        /// --       Type of this Solid.
        SolidProperties::SolidType  mType;
        /// (kg/m3)  Density of this Solid.
        double                      mDensity;
        /// (J/kg/K) Specific heat of this Solid.
        double                      mSpecificHeat;
        /// (W/m/K)  Thermal conductivity of this Solid.
        double                      mThermalConductivity;
        /// (m)      Roughness of this Solid.
        double                      mRoughness;
        /// --       Pointer to the friendly properties test article.
        FriendlySolidProperties*    mArticle;
        /// --       Pointer to the defined properties test article.
        DefinedSolidProperties*     mDefined;
        /// --       Tolerance for comparison of expected and returned values.
        double                      mTolerance;
        static int                  TEST_ID;     /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtSolidProperties(const UtSolidProperties&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtSolidProperties& operator =(const UtSolidProperties&);
};

/// @}

#endif
