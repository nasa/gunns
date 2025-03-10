#ifndef UtGunnsFluidMetabolic3_EXISTS
#define UtGunnsFluidMetabolic3_EXISTS

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

#include "aspects/fluid/source/GunnsFluidMetabolic3.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidMetabolic3 and befriend UtGunnsFluidMetabolic3.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidMetabolic3 : public GunnsFluidMetabolic3 {
    public:
        FriendlyGunnsFluidMetabolic3();
        virtual ~FriendlyGunnsFluidMetabolic3();
        friend class UtGunnsFluidMetabolic3;
};
inline FriendlyGunnsFluidMetabolic3::FriendlyGunnsFluidMetabolic3() : GunnsFluidMetabolic3() {}
inline FriendlyGunnsFluidMetabolic3::~FriendlyGunnsFluidMetabolic3() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidNode and befriend UtGunnsFluidMetabolic3.
///
/// @details  Class derived from the GUNNS fluid node. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidNodeMetabolic3 : public GunnsFluidNode {
    public:
        FriendlyGunnsFluidNodeMetabolic3();
        virtual ~FriendlyGunnsFluidNodeMetabolic3();
        friend class UtGunnsFluidMetabolic3;
};
inline FriendlyGunnsFluidNodeMetabolic3::FriendlyGunnsFluidNodeMetabolic3() : GunnsFluidNode() {}
inline FriendlyGunnsFluidNodeMetabolic3::~FriendlyGunnsFluidNodeMetabolic3() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Metabolic unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Metabolic link model within
///           the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidMetabolic3: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Metabolic unit test.
        UtGunnsFluidMetabolic3();
        /// @brief    Default destructs this Metabolic unit test.
        virtual ~UtGunnsFluidMetabolic3();
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
        void testStep();
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
        CPPUNIT_TEST_SUITE(UtGunnsFluidMetabolic3);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testNominalInitializationNoTc);
        CPPUNIT_TEST(testAccess);
        CPPUNIT_TEST(testModify);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testUpdateFluid);
        CPPUNIT_TEST(testUpdateFluidNoTc);
        CPPUNIT_TEST(testPortMapping);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST_SUITE_END();
        enum { N_CONSTITUENTS = 4, N_TC = 14 };
        DefinedFluidProperties*             tFluidProperties;    /**< (--) Defined fluid properties. */
        FluidProperties::FluidType          tTypes[UtGunnsFluidMetabolic3::N_CONSTITUENTS]; /**< (--) Array of fluid types. */
        DefinedChemicalCompounds*           tCompoundProperties; /**< (--) Defined chemical compounds. */
        ChemicalCompound::Type              tTcTypes[UtGunnsFluidMetabolic3::N_TC]; /**< (--) Array of trace compound types. */
        GunnsFluidTraceCompoundsConfigData* tFluidTcConfig;      /**< (--) Fluid trace compounds config. */
        double                              tFractions[UtGunnsFluidMetabolic3::N_CONSTITUENTS]; /**< (--) Array of fluid mass fractions. */
        PolyFluidConfigData*                tFluidConfig;        /**< (--) Fluid config data. */
        PolyFluidInputData*                 tFluidInput;         /**< (--) Fluid input data. */
        std::vector<GunnsBasicLink*>        tLinks;              /**< (--) Link vector. */
        std::string                         tName;               /**< (--) Nominal name. */
        FriendlyGunnsFluidNodeMetabolic3    tNodes[3];           /**< (--) Fluid nodes. */
        GunnsNodeList                       tNodeList;           /**< (--) Node list. */
        GunnsFluidMetabolic3ConfigData*     tConfigData;         /**< (--) Nominal configuration data. */
        double                              tNNominal;           /**< (--) Nominal number of crew members in nominal state. */
        double                              tNSleep;             /**< (--) Nominal number of crew members in sleep state. */
        double                              tNRecovery0;         /**< (--) Nominal number of crew members in recovery 0 state. */
        double                              tNRecovery1;         /**< (--) Nominal number of crew members in recovery 1 state. */
        double                              tNRecovery2;         /**< (--) Nominal number of crew members in recovery 2 state. */
        double                              tNRecovery3;         /**< (--) Nominal number of crew members in recovery 3 state. */
        double                              tNAerobic0;          /**< (--) Nominal number of crew members in aerobic 0 state. */
        double                              tNAerobic1;          /**< (--) Nominal number of crew members in aerobic 1 state. */
        double                              tNResistive0;        /**< (--) Nominal number of crew members in resistive 0 state. */
        double                              tNResistive1;        /**< (--) Nominal number of crew members in resistive 1 state. */
        double                              tNResistive2;        /**< (--) Nominal number of crew members in resistive 2 state. */
        double                              tNResistive3;        /**< (--) Nominal number of crew members in resistive 3 state. */
        GunnsFluidMetabolic3InputData*      tInputData;          /**< (--) Nominal input data. */
        FriendlyGunnsFluidMetabolic3*       tArticle;            /**< (--) Article under test. */
        static int                          TEST_ID;             /**< (--) Test identification number. */
        /// @brief  Verifies the given trace compound quantity in the node's fluid inflow.
        void verifyTraceCompoundInFlow(const double                 mdot,
                                       const ChemicalCompound::Type type);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidMetabolic3(const UtGunnsFluidMetabolic3&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidMetabolic3& operator =(const UtGunnsFluidMetabolic3&);
};

///@}

#endif
