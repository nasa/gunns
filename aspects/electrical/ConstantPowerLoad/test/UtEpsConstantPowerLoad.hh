#ifndef UtEpsConstantPowerLoad_EXISTS
#define UtEpsConstantPowerLoad_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TS_EPS_CONSTANT_POWER_LOAD    Constant Power Load Unit Test
/// @ingroup  UT_TS_EPS
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details  Unit Tests for the EPS Constant Power Load
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>
#include <string>
#include <vector>

#include "aspects/electrical/ConstantPowerLoad/EpsConstantPowerLoad.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from EpsConstantPowerLoad and befriend UtEpsConstantPowerLoad.
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyEpsConstantPowerLoad : public EpsConstantPowerLoad {
    public:
        FriendlyEpsConstantPowerLoad();
        virtual ~FriendlyEpsConstantPowerLoad();
        friend class UtEpsConstantPowerLoad;
};
inline FriendlyEpsConstantPowerLoad::FriendlyEpsConstantPowerLoad() : EpsConstantPowerLoad() {}
inline FriendlyEpsConstantPowerLoad::~FriendlyEpsConstantPowerLoad() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basic Constant Load  unit tests.
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtEpsConstantPowerLoad: public CppUnit::TestFixture {
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtEpsConstantPowerLoad(const UtEpsConstantPowerLoad& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtEpsConstantPowerLoad& operator =(const UtEpsConstantPowerLoad& that);

        CPPUNIT_TEST_SUITE(UtEpsConstantPowerLoad);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        EpsConstantPowerLoadConfigData* mConfigData;

        /// --     Pointer to nominal input data
        EpsConstantPowerLoadInputData*  mInputData;

        /// --     Test Article
        FriendlyEpsConstantPowerLoad*   mArticle;

        /// --     Conductor Name
        std::string                       mLinkName;

        /// --     The default conductivity of the link.
        double                            mInitialConductivity;

        /// --     The default power load of the link.
        double                            mDefaultPower;

        /// --     The trigger potential error for re-solving the conductivity.
        double                            mMinimumVoltageLimit;

        /// --     Network Nodes
        GunnsBasicNode                    mNodes[2];

        /// --     Node List
        GunnsNodeList                     mNodeList;

        /// --     Network Links
        std::vector<GunnsBasicLink*>      mLinks;

        /// --     Nominal inlet port index
        int                               mPort0;

        /// --     Nominal outlet port index
        int                               mPort1;

        /// (s)    Nominal time step
        double                            mTimeStep;

        /// --     Nominal tolerance for comparison of expected and returned values
        double                            mTolerance;


    public:
        UtEpsConstantPowerLoad();
        virtual ~UtEpsConstantPowerLoad();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testAccessors();
        void testModifiers();
        void testStep();
        void testComputeFlows();
};

/// @}

#endif
