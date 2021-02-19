#ifndef UtGunnsFluidEqConductor_EXISTS
#define UtGunnsFluidEqConductor_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_EQUIVALENT    Equivalent-Circuit Conductor Link Unit Test
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Equivalent-Circuit Conductor Link
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/fluid/conductor/GunnsFluidEqConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidEqConductor and befriend UtGunnsFluidEqConductor.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidEqConductor : public GunnsFluidEqConductor
{
    public:
        FriendlyGunnsFluidEqConductor();
        virtual ~FriendlyGunnsFluidEqConductor();
        friend class UtGunnsFluidEqConductor;
};
inline FriendlyGunnsFluidEqConductor::FriendlyGunnsFluidEqConductor() : GunnsFluidEqConductor() {};
inline FriendlyGunnsFluidEqConductor::~FriendlyGunnsFluidEqConductor() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Equivalent-Circuit Conductor unit tests.
////
/// @details  This class provides the unit tests for the GunnsFluidEqConductor class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidEqConductor: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidEqConductor(const UtGunnsFluidEqConductor& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidEqConductor& operator =(const UtGunnsFluidEqConductor& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidEqConductor);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testIoMethods);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsFluidEqConductor*   tArticle;               /**< (--) Test article */
        GunnsFluidNode                   tNodes[3];              /**< (--) Test nodes */
        DefinedFluidProperties*          tFluidProperties;       /**< (--) Node setup data */
        PolyFluidConfigData*             tLocalConfig;           /**< (--) Node setup data */
        PolyFluidInputData*              tFluidInput1;           /**< (--) Node setup data */
        PolyFluidInputData*              tFluidInput2;           /**< (--) Node setup data */
        std::string                      tLinkName;              /**< (--) Nominal config data */
        bool                             tUseNetworkCapacitance; /**< (--) Nominal config data */
        GunnsFluidEqConductorConfigData* tConfigData;            /**< (--) Nominal config data */
        GunnsNodeList                    tNodeList;              /**< (--) Nominal config data */
        std::vector<GunnsBasicLink*>     tLinks;                 /**< (--) Nominal config data */
        GunnsFluidEqConductorInputData*  tInputData;             /**< (--) Nominal input data */
        int                              tPort0;                 /**< (--) Nominal initialization data */
        int                              tPort1;                 /**< (--) Nominal initialization data */
        double                           tTimeStep;              /**< (--) Test timestep */
        static int                       TEST_ID;                /**< (--) Test identification number. */
    public:
        UtGunnsFluidEqConductor();
        virtual ~UtGunnsFluidEqConductor();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testIoMethods();
        void testStep();
        void testComputeFlows();

    protected:
        void verifyStepOutputs(const double equivC,
                               const double leakC0,
                               const double leakC1,
                               const bool   flag);
};

///@}

#endif
