#ifndef UtGunnsFluidSourceBoundary_EXISTS
#define UtGunnsFluidSourceBoundary_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOURCE_BOUNDARY    Gunns Fluid Source Boundary Unit Test
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2021 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Source Boundary
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/fluid/source/GunnsFluidSourceBoundary.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidSourceBoundary and befriend UtGunnsFluidSourceBoundary.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidSourceBoundary : public GunnsFluidSourceBoundary
{
    public:
        FriendlyGunnsFluidSourceBoundary();
        virtual ~FriendlyGunnsFluidSourceBoundary();
        friend class UtGunnsFluidSourceBoundary;
};
inline FriendlyGunnsFluidSourceBoundary::FriendlyGunnsFluidSourceBoundary() : GunnsFluidSourceBoundary() {};
inline FriendlyGunnsFluidSourceBoundary::~FriendlyGunnsFluidSourceBoundary() {}

class FriendlyGunnsFluidSourceBoundaryNode : public GunnsFluidNode
{
    public:
        FriendlyGunnsFluidSourceBoundaryNode() {;}
        virtual ~FriendlyGunnsFluidSourceBoundaryNode() {;}
        friend class UtGunnsFluidSourceBoundary;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basis Source Boundary  unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidSourceBoundary: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidSourceBoundary(const UtGunnsFluidSourceBoundary& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidSourceBoundary& operator =(const UtGunnsFluidSourceBoundary& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidSourceBoundary);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testAccessorsAndGetters);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlowsToNode);
        CPPUNIT_TEST(testComputeFlowsFromNode);
        CPPUNIT_TEST(testComputeFlowsZeroFlow);
        CPPUNIT_TEST(testComputeFlowsZeroDensity);
        CPPUNIT_TEST(testComputeFlowsTcOnly);
        CPPUNIT_TEST_SUITE_END();

        GunnsFluidSourceBoundaryConfigData* tConfigData;         /**< (--)   Nominal config data. */
        GunnsFluidSourceBoundaryInputData*  tInputData;          /**< (--)   Nominal input data. */
        FriendlyGunnsFluidSourceBoundary*   tArticle;            /**< (--)   Article under test. */
        bool                                tFlipFlowSign;       /**< (--)   Nominal config data. */
        bool                                tTraceCompoundsOnly; /**< (--)   Nominal config data. */
        std::string                         tLinkName;           /**< (--)   Nominal initialization data. */
        double                              tInitialFlowDemand;  /**< (kg/s) Nominal input data. */
        double*                             tTcRatesState;       /**< (--)   Nominal input data. */
        GunnsFluidTraceCompoundsInputData*  tTcInput;            /**< (--)   Trace compounds input data for nodes. */
        GunnsFluidNode                      tNodes[2];           /**< (--)   Network nodes. */
        GunnsNodeList                       tNodeList;           /**< (--)   Network nodes list. */
        std::vector<GunnsBasicLink*>        tLinks;              /**< (--)   Network links. */
        int                                 tPort0;              /**< (--)   Nominal initialization data. */
        double                              tTimeStep;           /**< (s)    Time step. */
        double                              tTolerance;          /**< (--)   Test tolerance. */
        DefinedFluidProperties*             tFluidProperties;    /**< (--)   Predefined fluid properties. */
        DefinedChemicalCompounds*           tCompoundProperties; /**< (--)   Predefined chemical compound properties. */
        ChemicalCompound::Type              tTcTypes[2];         /**< (--)   Trace compounds config data for nodes. */
        GunnsFluidTraceCompoundsConfigData* tFluidTcConfig;      /**< (--)   Trace compounds config data for nodes. */
        PolyFluidConfigData*                tFluidConfig;        /**< (--)   Fluid config data. */
        GunnsFluidTraceCompoundsInputData*  tFluidTcInput;       /**< (--)   Trace compounds input data for nodes. */
        PolyFluidInputData*                 tFluidInput1;        /**< (--)   Fluid 1 input data. */
        PolyFluidInputData*                 tFluidInput2;        /**< (--)   Fluid 2 input data. */
        double                              tFractions1[2];      /**< (--)   Array of fluid mass fractions for node 0. */
        double                              tFractions2[2];      /**< (--)   Array of fluid mass fractions for the test article. */
        static int                          TEST_ID;             /**< (--)   Test identification number. */

    public:
        UtGunnsFluidSourceBoundary();
        virtual ~UtGunnsFluidSourceBoundary();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testRestart();
        void testAccessorsAndGetters();
        void testStep();
        void testComputeFlowsToNode();
        void testComputeFlowsFromNode();
        void testComputeFlowsZeroFlow();
        void testComputeFlowsZeroDensity();
        void testComputeFlowsTcOnly();
};

///@}

#endif
