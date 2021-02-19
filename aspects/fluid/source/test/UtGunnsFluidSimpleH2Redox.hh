#ifndef UtGunnsFluidSimpleH2Redox_EXISTS
#define UtGunnsFluidSimpleH2Redox_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOURCE_SIMPLE_H2_REDOX   Simple H2 Redox Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Simple H2 Redox link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/source/GunnsFluidSimpleH2Redox.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidSimpleH2Redox and befriend UtGunnsFluidSimpleH2Redox.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidSimpleH2Redox : public GunnsFluidSimpleH2Redox
{
    public:
        FriendlyGunnsFluidSimpleH2Redox();
        virtual ~FriendlyGunnsFluidSimpleH2Redox();
        friend class UtGunnsFluidSimpleH2Redox;
};
inline FriendlyGunnsFluidSimpleH2Redox::FriendlyGunnsFluidSimpleH2Redox() : GunnsFluidSimpleH2Redox() {};
inline FriendlyGunnsFluidSimpleH2Redox::~FriendlyGunnsFluidSimpleH2Redox() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Simple H2 Redox unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Simple H2 Redox link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidSimpleH2Redox: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Simple H2 Redox unit test.
        UtGunnsFluidSimpleH2Redox();
        /// @brief    Default constructs this Simple H2 Redox unit test.
        virtual ~UtGunnsFluidSimpleH2Redox();
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
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests restart method.
        void testRestart();
        /// @brief    Tests step method.
        void testStep();
        /// @brief    Tests computeFlows method.
        void testComputeFlows();
        /// @brief    Tests transportFlows method.
        void testTransportFlows();
        /// @brief    Tests specific port mapping rules.
        void testPortMapping();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidSimpleH2Redox);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testTransportFlows);
        CPPUNIT_TEST(testPortMapping);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        /// @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 5, N_FLUIDS = 5};
        DefinedFluidProperties*            tFluidProperties;   /**< (--)       Defined fluid properties. */
        FluidProperties::FluidType         tTypes[N_FLUIDS];   /**< (--)       Array of Fluid Types. */
        PolyFluidConfigData*               tFluidConfig;       /**< (--)       Fluid config data. */
        PolyFluidInputData*                tFluidInput0;       /**< (--)       Fluid 0 input data. */
        PolyFluidInputData*                tFluidInput1;       /**< (--)       Fluid 1 input data. */
        PolyFluidInputData*                tFluidInput2;       /**< (--)       Fluid 2 input data. */
        std::vector<GunnsBasicLink*>       tLinks;             /**< (--)       Link vector. */
        std::string                        tName;              /**< (--)       Nominal name. */
        GunnsFluidNode                     tNodes[N_NODES];    /**< (--)       Fluid nodes. */
        GunnsNodeList                      tNodeList;          /**< (--)       Node List. */
        int                                tPort0;             /**< (--)       Nominal H2/H2O port index. */
        int                                tPort1;             /**< (--)       Nominal O2 port index. */
        int                                tNumCells;          /**< (--)       Nominal config data. */
        double                             tCellVoltageLoaded; /**< (V)        Nominal config data. */
        double                             tCellH2ReactRate;   /**< (kg/s/amp) Nominal config data. */
        double                             tMaxEfficiency;     /**< (--)       Nominal config data. */
        GunnsFluidSimpleH2RedoxConfigData* tConfigData;        /**< (-)        Pointer to nominal configuration data. */
        bool                               tMalfBlockageFlag;  /**< (--)       Nominal input data. */
        double                             tMalfBlockageValue; /**< (--)       Nominal input data. */
        double                             tCurrent;           /**< (amp)      Nominal input data. */
        bool                               tTrippedOff;        /**< (--)       Nominal input data. */
        GunnsFluidSimpleH2RedoxInputData*  tInputData;         /**< (--)       Pointer to nominal input data. */
        FriendlyGunnsFluidSimpleH2Redox*   tArticle;           /**< (--)       Pointer to the friendly adsorber under test. */
        double                             tTimeStep;          /**< (s)        Nominal time step. */
        static int                         TEST_ID;            /**< (--)       Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSimpleH2Redox(const UtGunnsFluidSimpleH2Redox&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSimpleH2Redox& operator =(const UtGunnsFluidSimpleH2Redox&);
};

///@}

#endif
