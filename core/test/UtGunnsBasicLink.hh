#ifndef UtGunnsBasicLink_EXISTS
#define UtGunnsBasicLink_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_BASIC_LINK    Gunns Basic Link Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Basic Link
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsBasicLink.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBasicLink and befriend UtGunnsBasicLink.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsBasicLink : public GunnsBasicLink
{
    public:
        FriendlyGunnsBasicLink(const int numPorts);
        virtual ~FriendlyGunnsBasicLink();
        friend class UtGunnsBasicLink;
};
inline FriendlyGunnsBasicLink::FriendlyGunnsBasicLink(const int numPorts)
       :
        GunnsBasicLink(numPorts) {};
inline FriendlyGunnsBasicLink::~FriendlyGunnsBasicLink() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basis Link  unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsBasicLink: public CppUnit::TestFixture
{
    public:
        UtGunnsBasicLink();
        virtual ~UtGunnsBasicLink();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testAccessors();
        void testModifiers();
        void testCheckPortRules();
        void testSetPort();
        void testTransportFlux();
        void testUserPortCommand();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsBasicLink(const UtGunnsBasicLink& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsBasicLink& operator =(const UtGunnsBasicLink& that);

        CPPUNIT_TEST_SUITE(UtGunnsBasicLink);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testCheckPortRules);
        CPPUNIT_TEST(testSetPort);
        CPPUNIT_TEST(testTransportFlux);
        CPPUNIT_TEST(testUserPortCommand);
        CPPUNIT_TEST_SUITE_END();

        GunnsBasicLinkConfigData*    mConfigData; /**< (--) Pointer to nominal configuration data. */
        GunnsBasicLinkInputData*     mInputData;  /**< (--) Pointer to nominal input data. */
        FriendlyGunnsBasicLink*      mArticle;    /**< (--) Test Article. */
        std::string                  mLinkName;   /**< (--) Link Name. */
        GunnsBasicNode               mNodes[2];   /**< (--) Network Nodes. */
        GunnsNodeList                mNodeList;   /**< (--) Node List. */
        std::vector<GunnsBasicLink*> mLinks;      /**< (--) Network Links. */
        int*                         mPortMap;    /**< (--) Nominal inlet port index. */
        int                          mNumPorts;   /**< (--) Number of Link Ports. */
        double                       mTimeStep;   /**< (s)  Nominal time step. */
        double                       mTolerance;  /**< (--) Nominal tolerance for comparison of expected and returned values. */
};

///@}

#endif
