#ifndef UtGunnsBasicSource_EXISTS
#define UtGunnsBasicSource_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_BASIC_SOURCE    Gunns Fluid Conductor Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Conductor
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsBasicSource.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBasicSource and befriend UtGunnsBasicSource.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsBasicSource : public GunnsBasicSource
{
    public:
        FriendlyGunnsBasicSource();
        virtual ~FriendlyGunnsBasicSource();
        friend class UtGunnsBasicSource;
};
inline FriendlyGunnsBasicSource::FriendlyGunnsBasicSource() : GunnsBasicSource() {};
inline FriendlyGunnsBasicSource::~FriendlyGunnsBasicSource() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basic Source unit tests.
////
/// @details  This class provides the unit tests for the GunnsBasicSource class within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsBasicSource: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsBasicSource(const UtGunnsBasicSource& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsBasicSource& operator =(const UtGunnsBasicSource& that);

        CPPUNIT_TEST_SUITE(UtGunnsBasicSource);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testSetters);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsBasicSourceConfigData*  mConfigData;

        /// --     Pointer to nominal input data
        GunnsBasicSourceInputData*   mInputData;

        /// --     Test Article
        FriendlyGunnsBasicSource*    mArticle;

        /// --     Link Name
        std::string                  mLinkName;

        /// --     Link flux demand
        double                       mInitialDemand;

        /// --     Network Nodes
        GunnsBasicNode               mNodes[2];

        /// --     Node List
        GunnsNodeList                mNodeList;

        /// --     Network Links
        std::vector<GunnsBasicLink*> mLinks;

        /// --     Nominal inlet port index
        int                          mPort0;

        /// --     Nominal outlet port index
        int                          mPort1;

        /// (s)    Nominal time step
        double                       mTimeStep;

        /// --     Nominal tolerance for comparison of expected and returned values
        double                       mTolerance;

    public:
        UtGunnsBasicSource();
        virtual ~UtGunnsBasicSource();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testStep();
        void testComputeFlows();
        void testSetters();
        void testAccessMethods();
};

///@}

#endif
