#ifndef UtGunnsLosslessSource_EXISTS
#define UtGunnsLosslessSource_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_LOSSLESS_SOURCE    Gunns Lossless Source Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Lossless Source
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>
#include <cmath>

#include "strings/UtResult.hh"
#include "aspects/electrical/LosslessSource/GunnsLosslessSource.hh"
#include "math/approximation/TsLinearInterpolator.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsLosslessSource and befriend UtGunnsLosslessSource.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsLosslessSource : public GunnsLosslessSource
{
    public:
        FriendlyGunnsLosslessSource();
        virtual ~FriendlyGunnsLosslessSource();
        friend class UtGunnsLosslessSource;
};
inline FriendlyGunnsLosslessSource::FriendlyGunnsLosslessSource() : GunnsLosslessSource() {};
inline FriendlyGunnsLosslessSource::~FriendlyGunnsLosslessSource() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Lossless Source unit tests.
////
/// @details  This class provides the unit tests for the GunnsLosslessSource class within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsLosslessSource: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsLosslessSource(const UtGunnsLosslessSource& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsLosslessSource& operator =(const UtGunnsLosslessSource& that);

        CPPUNIT_TEST_SUITE(UtGunnsLosslessSource);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testLosslessComputeFlows);
        CPPUNIT_TEST(testSetters);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST(testInefficiencyAndWaste);
        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsLosslessSourceConfigData*  mConfigData;

        /// --     Pointer to nominal input data
        GunnsLosslessSourceInputData*   mInputData;

        /// --     Test Article
        FriendlyGunnsLosslessSource*    mArticle;

        /// --     Link Name
        std::string                  mLinkName;

        /// --     Link flux demand
        double                       mInitialDemand;

        /// -- Link Default efficiency
        double mEfficiency;

        double xArr[2] = {0};
        double yArr[2] = {0};
        const int nArr = 2;
        TsLinearInterpolator* mEffTable;

        double mDefReferencePower;
        bool mDefStaticEff;

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
        /// -- 
        static int TEST_ID;
        UtGunnsLosslessSource();
        virtual ~UtGunnsLosslessSource();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testStep();
        void testLosslessComputeFlows();
        void testSetters();
        void testAccessMethods();
        void testInefficiencyAndWaste();
};

///@}

#endif
