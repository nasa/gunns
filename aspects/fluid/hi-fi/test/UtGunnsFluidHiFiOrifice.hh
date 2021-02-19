#ifndef UtGunnsFluidHiFiOrifice_EXISTS
#define UtGunnsFluidHiFiOrifice_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_FLUID_HIFI_ORIFICE    Gunns Fluid High-Fidelity Orifice Unit Test
/// @ingroup  UT_GUNNS_FLUID
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Hi-Fi Orifice
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/fluid/hi-fi/GunnsFluidHiFiOrifice.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidHiFiOrifice and befriend UtGunnsFluidHiFiOrifice.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidHiFiOrifice : public GunnsFluidHiFiOrifice
{
    public:
        FriendlyGunnsFluidHiFiOrifice();
        virtual ~FriendlyGunnsFluidHiFiOrifice();
        friend class UtGunnsFluidHiFiOrifice;
};
inline FriendlyGunnsFluidHiFiOrifice::FriendlyGunnsFluidHiFiOrifice() : GunnsFluidHiFiOrifice() {};
inline FriendlyGunnsFluidHiFiOrifice::~FriendlyGunnsFluidHiFiOrifice() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basis Conductor  unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidHiFiOrifice: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidHiFiOrifice(const UtGunnsFluidHiFiOrifice& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidHiFiOrifice& operator =(const UtGunnsFluidHiFiOrifice& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidHiFiOrifice);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationOptions);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStepGasNonChoked);
        CPPUNIT_TEST(testStepGasChoked);
        CPPUNIT_TEST(testStepLiquid);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST(testInitializationRealValves);
        CPPUNIT_TEST_SUITE_END();

        std::string                                 tLinkName;               /**< (--) Nominal config data */
        GunnsFluidHiFiOrificeConfigData::CoeffTypes tCoefficientType;        /**< (--) Nominal config data */
        double                                      tCoefficientValue;       /**< (--) Nominal config data */
        double                                      tThroatDiameter;         /**< (--) Nominal config data */
        double                                      tCriticalReynolds;       /**< (--) Nominal config data */
        double                                      tExpansionScaleFactor;   /**< (--) Nominal config data */
        double                                      tFlowTuningFactor;       /**< (--) Nominal config data */
        GunnsFluidHiFiOrificeConfigData*            tConfigData;             /**< (--) Nominal config data */
        bool                                        tMalfBlockageFlag;       /**< (--) Nominal input data */
        double                                      tMalfBlockageValue;      /**< (--) Nominal input data */
        GunnsFluidHiFiOrificeInputData*             tInputData;              /**< (--) Nominal input data */
        FriendlyGunnsFluidHiFiOrifice*              tArticle;                /**< (--) Article under test */
        GunnsFluidNode                              tNodes[5];               /**< (--) Network nodes */
        GunnsNodeList                               tNodeList;               /**< (--) Network node list */
        std::vector<GunnsBasicLink*>                tLinks;                  /**< (--) Network links vector */
        int                                         tPort0;                  /**< (--) Nominal init data */
        int                                         tPort1;                  /**< (--) Nominal init data */
        double                                      tTimeStep;               /**< (s)  Nominal time step */
        DefinedFluidProperties*                     tFluidProperties;        /**< (--) Nominal config data */
        PolyFluidConfigData*                        tFluidConfig;            /**< (--) Nominal config data */
        PolyFluidInputData*                         tFluidInput1;            /**< (--) Nominal input data */
        PolyFluidInputData*                         tFluidInput2;            /**< (--) Nominal input data */
        PolyFluidInputData*                         tFluidInput3;            /**< (--) Nominal input data */
        double*                                     fractions;               /**< (--) Nominal input data */

    public:
        UtGunnsFluidHiFiOrifice();
        virtual ~UtGunnsFluidHiFiOrifice();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationOptions();
        void testInitializationExceptions();
        void testRestart();
        void testStepGasNonChoked();
        void testStepGasChoked();
        void testStepLiquid();
        void testComputeFlows();
        void testAccessMethods();
        void testInitializationRealValves();
};

///@}

#endif
