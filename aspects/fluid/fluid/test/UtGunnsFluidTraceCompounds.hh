#ifndef UtGunnsFluidTraceCompounds_EXISTS
#define UtGunnsFluidTraceCompounds_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_FLUID_TRACE_COMPOUNDS  GunnsFluidTraceCompounds Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_FLUID
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GunnsFluidTraceCompounds class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/fluid/GunnsFluidTraceCompounds.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidTraceCompounds and befriend UtGunnsFluidTraceCompounds.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidTraceCompounds : public GunnsFluidTraceCompounds {
    public:
        FriendlyGunnsFluidTraceCompounds();
        FriendlyGunnsFluidTraceCompounds(const double& fluidMoles);
        FriendlyGunnsFluidTraceCompounds(const GunnsFluidTraceCompounds& that,
                                         const double&                   fluidMoles,
                                         const std::string&              name);
        virtual ~FriendlyGunnsFluidTraceCompounds();
        friend class UtGunnsFluidTraceCompounds;
    private:
        FriendlyGunnsFluidTraceCompounds& operator =(const FriendlyGunnsFluidTraceCompounds&);
        FriendlyGunnsFluidTraceCompounds(const FriendlyGunnsFluidTraceCompounds&);
};
inline FriendlyGunnsFluidTraceCompounds::FriendlyGunnsFluidTraceCompounds()
    : GunnsFluidTraceCompounds() {};
inline FriendlyGunnsFluidTraceCompounds::FriendlyGunnsFluidTraceCompounds(const double& fluidMoles)
    : GunnsFluidTraceCompounds(fluidMoles) {};
inline FriendlyGunnsFluidTraceCompounds::FriendlyGunnsFluidTraceCompounds(const GunnsFluidTraceCompounds& that,
                                                                          const double&                   fluidMoles,
                                                                          const std::string&              name)
    : GunnsFluidTraceCompounds(that, fluidMoles, name) {}
inline FriendlyGunnsFluidTraceCompounds::~FriendlyGunnsFluidTraceCompounds() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsFluidTraceCompounds unit tests.
////
/// @details  This class provides the unit tests for the GunnsFluidTraceCompounds within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidTraceCompounds : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this MonoFluid unit test.
        UtGunnsFluidTraceCompounds();
        /// @brief    Default destructs this MonoFluid unit test.
        virtual ~UtGunnsFluidTraceCompounds();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        void testConfigAndInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testCopyConstruction();
        void testInitializationExceptions();
        void testRestart();
        void testGetType();
        void testFind();
        void testGetMass();
        void testGetMoleFraction();
        void testSetMasses();
        void testSetMass();
        void testSetMoleFraction();
        void testFlowIn();
        void testFlowOut();
        void testLimitPositive();
        void testAccessors();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidTraceCompounds);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testCopyConstruction);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testGetType);
        CPPUNIT_TEST(testFind);
        CPPUNIT_TEST(testGetMass);
        CPPUNIT_TEST(testGetMoleFraction);
        CPPUNIT_TEST(testSetMasses);
        CPPUNIT_TEST(testSetMass);
        CPPUNIT_TEST(testSetMoleFraction);
        CPPUNIT_TEST(testFlowIn);
        CPPUNIT_TEST(testFlowOut);
        CPPUNIT_TEST(testLimitPositive);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();
        enum {NSINGLE = 1, NDUAL = 2, NMULTI = 7};       /**< (--)     Typedef for number of chemical compounds in trace compounds. */
        std::string                         tName;       /**< (--)     Test article name string. */
        double                              tMole;       /**< (kg*mol) Parent fluid moles. */
        ChemicalCompound::Type              tType[UtGunnsFluidTraceCompounds::NMULTI - 1]; /**< (--) Array of chemical compounds types. */
        double                              tMoleFraction[UtGunnsFluidTraceCompounds::NMULTI]; /**< (--) Array of mole fractions. */
        DefinedChemicalCompounds*           tProperties; /**< (--)     Pointer to defined chemical compounds properties. */
        GunnsFluidTraceCompoundsConfigData* tConfigData; /**< (--)     Pointer to nominal config data. */
        GunnsFluidTraceCompoundsInputData*  tInputData;  /**< (--)     Pointer to nominal input data. */
        FriendlyGunnsFluidTraceCompounds*   tArticle;    /**< (--)     Pointer to test article. */
        static int                          TEST_ID;     /**< (--)     Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidTraceCompounds(const UtGunnsFluidTraceCompounds&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidTraceCompounds& operator =(const UtGunnsFluidTraceCompounds&);
};

/// @}

#endif
