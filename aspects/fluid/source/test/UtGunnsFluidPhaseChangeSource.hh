#ifndef UtGunnsFluidPhaseChangeSource_EXISTS
#define UtGunnsFluidPhaseChangeSource_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_FLUID_PHASE_CHANGE_SOURCE    Gunns Fluid Phase Chabge Source Unit Test
/// @ingroup  UT_GUNNS_FLUID
///
/// @copyright Copyright 2020 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Phase Change Source
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/fluid/source/GunnsFluidPhaseChangeSource.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidPhaseChangeSource and befriend UtGunnsFluidPhaseChangeSource.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidPhaseChangeSource : public GunnsFluidPhaseChangeSource
{
    public:
        FriendlyGunnsFluidPhaseChangeSource();
        virtual ~FriendlyGunnsFluidPhaseChangeSource();
        friend class UtGunnsFluidPhaseChangeSource;
};
inline FriendlyGunnsFluidPhaseChangeSource::FriendlyGunnsFluidPhaseChangeSource() : GunnsFluidPhaseChangeSource() {};
inline FriendlyGunnsFluidPhaseChangeSource::~FriendlyGunnsFluidPhaseChangeSource() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns fluid membrane evaporator link unit tests.
///
/// @details  This class provides unit tests within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidPhaseChangeSource: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidPhaseChangeSource(const UtGunnsFluidPhaseChangeSource& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidPhaseChangeSource& operator =(const UtGunnsFluidPhaseChangeSource& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidPhaseChangeSource);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testTransportFlows);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST(testPortMapping);
        CPPUNIT_TEST_SUITE_END();

        std::string                            tLinkName;          /**< (--) Nominal config data. */
        FluidProperties::FluidType             tLiquidType;        /**< (--) Nominal config data. */
        FluidProperties::FluidType             tGasType;           /**< (--) Nominal config data. */
        double                                 tEfficiency;        /**< (--) Nominal config data. */
        GunnsFluidPhaseChangeSourceConfigData* tConfigData;        /**< (--) Nominal config data. */
        bool                                   tMalfBlockageFlag;  /**< (--) Nominal input data. */
        double                                 tMalfBlockageValue; /**< (--) Nominal input data. */
        double                                 tPowerInput;        /**< (W)  Nominal input data. */
        GunnsFluidPhaseChangeSourceInputData*  tInputData;         /**< (--) Nominal input data. */
        FriendlyGunnsFluidPhaseChangeSource*   tArticle;           /**< (--) Article under test. */
        GunnsFluidNode                         tNodes[5];          /**< (--) Network nodes. */
        GunnsNodeList                          tNodeList;          /**< (--) Network node list. */
        std::vector<GunnsBasicLink*>           tLinks;             /**< (--) Network links vector. */
        int                                    tPort0;             /**< (--) Nominal init data. */
        int                                    tPort1;             /**< (--) Nominal init data. */
        double                                 tTimeStep;          /**< (s)  Nominal time step. */
        DefinedFluidProperties*                tFluidProperties;   /**< (--) Nominal config data. */
        PolyFluidConfigData*                   tFluidConfig;       /**< (--) Nominal config data. */
        PolyFluidInputData*                    tFluidInput1;       /**< (--) Nominal input data. */
        PolyFluidInputData*                    tFluidInput2;       /**< (--) Nominal input data. */
        double*                                tFractions;         /**< (--) Nominal input data. */
        static int                             TEST_ID;            /**< (--) Test identification number. */

    public:
        UtGunnsFluidPhaseChangeSource();
        virtual ~UtGunnsFluidPhaseChangeSource();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testRestart();
        void testStep();
        void testTransportFlows();
        void testComputeFlows();
        void testAccessMethods();
        void testPortMapping();
};

///@}

#endif
