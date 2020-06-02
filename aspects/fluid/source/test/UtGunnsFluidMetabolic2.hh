#ifndef UtGunnsFluidMetabolic22_EXISTS
#define UtGunnsFluidMetabolic22_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOURCE_METABOLIC_2   Improved Metabolic Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Improved Metabolic link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/source/GunnsFluidMetabolic2.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidMetabolic2 and befriend UtGunnsFluidMetabolic2.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidMetabolic2 : public GunnsFluidMetabolic2 {
    public:
        FriendlyGunnsFluidMetabolic2();
        virtual ~FriendlyGunnsFluidMetabolic2();
        friend class UtGunnsFluidMetabolic2;
};
inline FriendlyGunnsFluidMetabolic2::FriendlyGunnsFluidMetabolic2() : GunnsFluidMetabolic2() {}
inline FriendlyGunnsFluidMetabolic2::~FriendlyGunnsFluidMetabolic2() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidNode and befriend UtGunnsFluidMetabolic2.
///
/// @details  Class derived from the GUNNS fluid node. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidNodeMetabolic2 : public GunnsFluidNode {
    public:
        FriendlyGunnsFluidNodeMetabolic2();
        virtual ~FriendlyGunnsFluidNodeMetabolic2();
        friend class UtGunnsFluidMetabolic2;
};
inline FriendlyGunnsFluidNodeMetabolic2::FriendlyGunnsFluidNodeMetabolic2() : GunnsFluidNode() {}
inline FriendlyGunnsFluidNodeMetabolic2::~FriendlyGunnsFluidNodeMetabolic2() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Metabolic unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Metabolic link model within
///           the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidMetabolic2: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Metabolic unit test.
        UtGunnsFluidMetabolic2();
        /// @brief    Default destructs this Metabolic unit test.
        virtual ~UtGunnsFluidMetabolic2();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config and input data.
        void testConfigAndInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests initialize method with no trace compounds.
        void testNominalInitializationNoTc();
        /// @brief    Tests accessor methods.
        void testAccess();
        /// @brief    Tests modifier methods.
        void testModify();
        /// @brief    Tests update state method.
        void testUpdateState();
        /// @brief    Tests update fluid method.
        void testUpdateFluid();
        /// @brief    Tests update fluid method with no trace compounds.
        void testUpdateFluidNoTc();
        /// @brief    Tests specific port mapping rules.
        void testPortMapping();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
        /// @brief    Tests restart method.
        void testRestart();
   private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidMetabolic2);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testNominalInitializationNoTc);
        CPPUNIT_TEST(testAccess);
        CPPUNIT_TEST(testModify);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST(testUpdateFluid);
        CPPUNIT_TEST(testUpdateFluidNoTc);
        CPPUNIT_TEST(testPortMapping);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST_SUITE_END();
        enum { N_CONSTITUENTS = 4, N_TC = 14 };
        DefinedFluidProperties*             tFluidProperties;    /**< (--) Defined fluid properties. */
        FluidProperties::FluidType          tTypes[UtGunnsFluidMetabolic2::N_CONSTITUENTS]; /**< (--) Array of fluid types. */
        DefinedChemicalCompounds*           tCompoundProperties; /**< (--) Defined chemical compounds. */
        ChemicalCompound::Type              tTcTypes[UtGunnsFluidMetabolic2::N_TC]; /**< (--) Array of trace compound types. */
        GunnsFluidTraceCompoundsConfigData* tFluidTcConfig;      /**< (--) Fluid trace compounds config. */
        double                              tFractions[UtGunnsFluidMetabolic2::N_CONSTITUENTS]; /**< (--) Array of fluid mass fractions. */
        PolyFluidConfigData*                tFluidConfig;        /**< (--) Fluid config data. */
        PolyFluidInputData*                 tFluidInput;         /**< (--) Fluid input data. */
        std::vector<GunnsBasicLink*>        tLinks;              /**< (--) Link vector. */
        std::string                         tName;               /**< (--) Nominal name. */
        FriendlyGunnsFluidNodeMetabolic2    tNodes[3];           /**< (--) Fluid nodes. */
        GunnsNodeList                       tNodeList;           /**< (--) Node list. */
        GunnsFluidMetabolic2ConfigData*     tConfigData;         /**< (--) Nominal configuration data. */
        double                              tNNominal;           /**< (--) Nominal number of crew members in nominal state. */
        double                              tNSleep;             /**< (--) Nominal number of crew members in sleep state. */
        double                              tNRecovery0;         /**< (--) Nominal number of crew members in recovery 0 state. */
        double                              tNRecovery1;         /**< (--) Nominal number of crew members in recovery 1 state. */
        double                              tNRecovery2;         /**< (--) Nominal number of crew members in recovery 2 state. */
        double                              tNRecovery3;         /**< (--) Nominal number of crew members in recovery 3 state. */
        double                              tNExercise0;         /**< (--) Nominal number of crew members in exercise 0 state. */
        double                              tNExercise1;         /**< (--) Nominal number of crew members in exercise 1 state. */
        GunnsFluidMetabolic2InputData*      tInputData;          /**< (--) Nominal input data. */
        FriendlyGunnsFluidMetabolic2*       tArticle;            /**< (--) Article under test. */
        static int                          TEST_ID;             /**< (--) Test identification number. */
        /// @brief  Verifies the given trace compound quantity in the node's fluid inflow.
        void verifyTraceCompoundInFlow(const double                 mdot,
                                       const ChemicalCompound::Type type);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidMetabolic2(const UtGunnsFluidMetabolic2&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidMetabolic2& operator =(const UtGunnsFluidMetabolic2&);
};

///@}

#endif
