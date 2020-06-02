#ifndef UtGunnsFluidHiFiValve_EXISTS
#define UtGunnsFluidHiFiValve_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_FLUID_HIFI_VALVE    Gunns Fluid High-Fidelity Valve Unit Test
/// @ingroup  UT_GUNNS_FLUID
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Hi-Fi Valve
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/fluid/hi-fi/GunnsFluidHiFiValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidHiFiValve and befriend UtGunnsFluidHiFiValve.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidHiFiValve : public GunnsFluidHiFiValve
{
    public:
        FriendlyGunnsFluidHiFiValve();
        virtual ~FriendlyGunnsFluidHiFiValve();
        friend class UtGunnsFluidHiFiValve;
};
inline FriendlyGunnsFluidHiFiValve::FriendlyGunnsFluidHiFiValve() : GunnsFluidHiFiValve() {};
inline FriendlyGunnsFluidHiFiValve::~FriendlyGunnsFluidHiFiValve() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basis Conductor  unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidHiFiValve: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidHiFiValve(const UtGunnsFluidHiFiValve& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidHiFiValve& operator =(const UtGunnsFluidHiFiValve& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidHiFiValve);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStepGas);
        CPPUNIT_TEST(testStepLiquid);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST_SUITE_END();

        std::string                               tLinkName;               /**< (--) Nominal config data */
        GunnsFluidHiFiValveConfigData::CoeffTypes tCoefficientType;        /**< (--) Nominal config data */
        double                                    tCoefficientValue;       /**< (--) Nominal config data */
        double                                    tThroatDiameter;         /**< (--) Nominal config data */
        double                                    tCriticalReynolds;       /**< (--) Nominal config data */
        double                                    tExpansionScaleFactor;   /**< (--) Nominal config data */
        double                                    tFlowTuningFactor;       /**< (--) Nominal config data */
        GunnsFluidHiFiValveConfigData*            tConfigData;             /**< (--) Nominal config data */
        bool                                      tMalfBlockageFlag;       /**< (--) Nominal input data */
        double                                    tMalfBlockageValue;      /**< (--) Nominal input data */
        double                                    tPosition;               /**< (--) Nominal input data. */
        bool                                      tMalfLeakThruFlag;       /**< (--) Nominal input data. */
        double                                    tMalfLeakThruValue;      /**< (--) Nominal input data. */
        GunnsFluidHiFiValveInputData*             tInputData;              /**< (--) Nominal input data */
        FriendlyGunnsFluidHiFiValve*              tArticle;                /**< (--) Article under test */
        GunnsFluidNode                            tNodes[5];               /**< (--) Network nodes */
        GunnsNodeList                             tNodeList;               /**< (--) Network node list */
        std::vector<GunnsBasicLink*>              tLinks;                  /**< (--) Network links vector */
        int                                       tPort0;                  /**< (--) Nominal init data */
        int                                       tPort1;                  /**< (--) Nominal init data */
        double                                    tTimeStep;               /**< (s)  Nominal time step */
        DefinedFluidProperties*                   tFluidProperties;        /**< (--) Nominal config data */
        PolyFluidConfigData*                      tFluidConfig;            /**< (--) Nominal config data */
        PolyFluidInputData*                       tFluidInput1;            /**< (--) Nominal input data */
        PolyFluidInputData*                       tFluidInput2;            /**< (--) Nominal input data */
        PolyFluidInputData*                       tFluidInput3;            /**< (--) Nominal input data */
        double*                                   fractions;               /**< (--) Nominal input data */

    public:
        UtGunnsFluidHiFiValve();
        virtual ~UtGunnsFluidHiFiValve();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testRestart();
        void testStepGas();
        void testStepLiquid();
        void testAccessMethods();
};

///@}

#endif
