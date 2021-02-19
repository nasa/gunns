#ifndef UtGunnsBasicPotential_EXISTS
#define UtGunnsBasicPotential_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_BASIC_POTENTIAL    Gunns Basic Potential Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Basic Potential
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsBasicPotential.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBasicPotential and befriend UtGunnsBasicPotential.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsBasicPotential : public GunnsBasicPotential
{
    public:
        FriendlyGunnsBasicPotential();
        virtual ~FriendlyGunnsBasicPotential();
        friend class UtGunnsBasicPotential;
};
inline FriendlyGunnsBasicPotential::FriendlyGunnsBasicPotential() : GunnsBasicPotential() {};
inline FriendlyGunnsBasicPotential::~FriendlyGunnsBasicPotential() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basis Conductor  unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsBasicPotential: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsBasicPotential(const UtGunnsBasicPotential& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsBasicPotential& operator =(const UtGunnsBasicPotential& that);

        CPPUNIT_TEST_SUITE(UtGunnsBasicPotential);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsBasicPotentialConfigData* mConfigData;

        /// --     Pointer to nominal input data
        GunnsBasicPotentialInputData*  mInputData;

        /// --     Test Article
        FriendlyGunnsBasicPotential*   mArticle;

        /// --     Conductor Name
        std::string                    mLinkName;

        /// --     Link Default Conductance
        double                         mInitialConductivity;

        /// --     The default potential rise of the link.
        double                         mInitialPotential;

        /// --     Network Nodes
        GunnsBasicNode                 mNodes[2];

        /// --     Node List
        GunnsNodeList                  mNodeList;

        /// --     Network Links
        std::vector<GunnsBasicLink*>   mLinks;

        /// --     Nominal inlet port index
        int                            mPort0;

        /// --     Nominal outlet port index
        int                            mPort1;

        /// (s)    Nominal time step
        double                         mTimeStep;

        /// --     Nominal tolerance for comparison of expected and returned values
        double                         mTolerance;

    public:
        UtGunnsBasicPotential();
        virtual ~UtGunnsBasicPotential();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testStep();
        void testComputeFlows();
        void testAccessMethods();
};

///@}

#endif
