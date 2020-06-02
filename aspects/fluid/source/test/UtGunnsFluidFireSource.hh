#ifndef UtGunnsFluidFireSource_EXISTS
#define UtGunnsFluidFireSource_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOURCE_BOUNDARY    Gunns Fluid Source Boundary Unit Test
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Source Boundary
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/fluid/source/GunnsFluidFireSource.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidFireSource and befriend UtGunnsFluidFireSource.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidFireSource : public GunnsFluidFireSource
{
    public:
        FriendlyGunnsFluidFireSource();
        virtual ~FriendlyGunnsFluidFireSource();
        friend class UtGunnsFluidFireSource;
};
inline FriendlyGunnsFluidFireSource::FriendlyGunnsFluidFireSource() : GunnsFluidFireSource() {};
inline FriendlyGunnsFluidFireSource::~FriendlyGunnsFluidFireSource() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basis Source Boundary  unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidFireSource: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidFireSource(const UtGunnsFluidFireSource& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidFireSource& operator =(const UtGunnsFluidFireSource& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidFireSource);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testAccessorsAndGetters);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlowsFromNode);
        CPPUNIT_TEST_SUITE_END();

        GunnsFluidFireSourceConfigData*     tConfigData;         /**< (--)   Nominal config data. */
        GunnsFluidFireSourceInputData*      tInputData;          /**< (--)   Nominal input data. */
        FriendlyGunnsFluidFireSource*       tArticle;            /**< (--)   Article under test. */
        double                              tO2ConsumpRate;      /**< (g/J)  Nominal config data. */
        double                              tCO2ProductRate;     /**< (g/J)  Nominal config data. */
        double                              tH2OProductRate;     /**< (g/J)  Nominal config data. */
        double                              tMinReqO2;           /**< (kPa)  Nominal config data. */
        double*                             tTcRatesState;       /**< (--)   Nominal config data. */
        std::string                         tLinkName;           /**< (--)   Nominal initialization data. */
        bool                                tFireFlag;           /**< (--)   Nominal input data. */
        double                              tHeatOutput;         /**< (W)    Nominal input data. */
        GunnsFluidTraceCompoundsInputData*  tTcInput;            /**< (--)   Trace compounds input data for nodes. */
        GunnsFluidNode                      tNodes[3];           /**< (--)   Network nodes. */
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
        double                              tFractions1[4];      /**< (--)   Array of fluid mass fractions for node 0. */
        double                              tFractions2[4];      /**< (--)   Array of fluid mass fractions for the test article. */
        static int                          TEST_ID;             /**< (--)   Test identification number. */

    public:
        UtGunnsFluidFireSource();
        virtual ~UtGunnsFluidFireSource();
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
        void testComputeFlowsFromNode();
};

///@}

#endif
