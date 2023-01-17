#ifndef UtGunnsFluidSorptionBed_EXISTS
#define UtGunnsFluidSorptionBed_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SORPTION_BED    GUNNS Fluid Sorption Bed Unit Test
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2023 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Sorption Bed
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/fluid/source/GunnsFluidSorptionBed.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidSorptionBed and befriend UtGunnsFluidSorptionBed.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidSorptionBed : public GunnsFluidSorptionBed
{
    public:
        FriendlyGunnsFluidSorptionBed() {;}
        virtual ~FriendlyGunnsFluidSorptionBed() {;}
        friend class UtGunnsFluidSorptionBed;
};

class FriendlyGunnsFluidSorptionBedConfigData : public GunnsFluidSorptionBedConfigData
{
    public:
        FriendlyGunnsFluidSorptionBedConfigData(const std::string& name            = "",
                                                GunnsNodeList*     nodes           = 0,
                                                const double       maxConductivity = 0.0)
            : GunnsFluidSorptionBedConfigData(name, nodes, maxConductivity) {;}
        virtual ~FriendlyGunnsFluidSorptionBedConfigData() {;}
        friend class UtGunnsFluidSorptionBed;
};

class FriendlyGunnsFluidSorptionBedSorbate : public GunnsFluidSorptionBedSorbate
{
    public:
        FriendlyGunnsFluidSorptionBedSorbate() {;}
        virtual ~FriendlyGunnsFluidSorptionBedSorbate() {;}
        friend class UtGunnsFluidSorptionBed;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Sorption Bed unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidSorptionBed: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidSorptionBed(const UtGunnsFluidSorptionBed& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidSorptionBed& operator =(const UtGunnsFluidSorptionBed& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidSorptionBed);
        CPPUNIT_TEST(testBedConfig);
        CPPUNIT_TEST(testBedInput);
        CPPUNIT_TEST(testSegmentConfig);
        CPPUNIT_TEST(testSegmentInput);
        CPPUNIT_TEST(testBedSorbateInit);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testBedSorbateLoadingEquil);
        CPPUNIT_TEST(testBedSorbateUpdateLoading);
        CPPUNIT_TEST(testBedSegmentUpdate);
        CPPUNIT_TEST(testTransportFlows);
        CPPUNIT_TEST(testPortRules);
        CPPUNIT_TEST(testRunExceptions);
        CPPUNIT_TEST_SUITE_END();

        double                              tMaxConductivity;    /**< (m2)   Nominal config data. */
        GunnsFluidSorptionBedConfigData*    tConfigData;         /**< (--)   Nominal config data. */
        double                              tWallTemperature;    /**< (K)    Nominal input data. */
        GunnsFluidSorptionBedInputData*     tInputData;          /**< (--)   Nominal input data. */
        FriendlyGunnsFluidSorptionBed*      tArticle;            /**< (--)   Article under test. */
        std::string                         tLinkName;           /**< (--)   Nominal initialization data. */
        GunnsFluidTraceCompoundsInputData*  tTcInput;            /**< (--)   Trace compounds input data for nodes. */
        GunnsFluidNode                      tNodes[4];           /**< (--)   Network nodes. */
        GunnsNodeList                       tNodeList;           /**< (--)   Network nodes list. */
        std::vector<GunnsBasicLink*>        tLinks;              /**< (--)   Network links. */
        int                                 tPort0;              /**< (--)   Nominal initialization data. */
        int                                 tPort1;              /**< (--)   Nominal initialization data. */
        double                              tTimeStep;           /**< (s)    Time step. */
        DefinedFluidProperties*             tFluidProperties;    /**< (--)   Predefined fluid properties. */
        DefinedChemicalCompounds*           tCompoundProperties; /**< (--)   Predefined chemical compound properties. */
        ChemicalCompound::Type              tTcTypes[2];         /**< (--)   Trace compounds config data for nodes. */
        GunnsFluidTraceCompoundsConfigData* tFluidTcConfig;      /**< (--)   Trace compounds config data for nodes. */
        PolyFluidConfigData*                tFluidConfig;        /**< (--)   Fluid config data. */
        GunnsFluidTraceCompoundsInputData*  tFluidTcInput;       /**< (--)   Trace compounds input data for nodes. */
        PolyFluidInputData*                 tFluidInput1;        /**< (--)   Fluid 1 input data. */
        PolyFluidInputData*                 tFluidInput2;        /**< (--)   Fluid 2 input data. */
        double                              tFractions1[4];      /**< (--)   Array of fluid mass fractions for node 0. */
        double                              tFractions2[4];      /**< (--)   Array of fluid mass fractions for node 2. */
        SorbantProperties*                  tCustomSorbant;      /**< (--)   A custom sorbant properties. */
        static int                          TEST_ID;             /**< (--)   Test identification number. */

    public:
        UtGunnsFluidSorptionBed();
        virtual ~UtGunnsFluidSorptionBed();
        void tearDown();
        void setUp();
        void testBedConfig();
        void testBedInput();
        void testBedSorbateInit();
        void testSegmentConfig();
        void testSegmentInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testRestart();
        void testBedSorbateLoadingEquil();
        void testBedSorbateUpdateLoading();
        void testBedSegmentUpdate();
        void testTransportFlows();
        void testPortRules();
        void testRunExceptions();
};

///@}

#endif
