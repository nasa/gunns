#ifndef UtGunnsBasicExternalSupply_EXISTS
#define UtGunnsBasicExternalSupply_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_BASIC_EXTERNAL_SUPPLY    Gunns Basic External Supply Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GunnsBasicExternalSupply class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsBasicExternalSupply.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBasicExternalSupply and befriend UtGunnsBasicExternalSupply.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsBasicExternalSupply : public GunnsBasicExternalSupply
{
    public:
        FriendlyGunnsBasicExternalSupply();
        virtual ~FriendlyGunnsBasicExternalSupply();
        friend class UtGunnsBasicExternalSupply;
};
inline FriendlyGunnsBasicExternalSupply::FriendlyGunnsBasicExternalSupply()
    : GunnsBasicExternalSupply() {};
inline FriendlyGunnsBasicExternalSupply::~FriendlyGunnsBasicExternalSupply() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basic External Supply unit tests.
////
/// @details  This class provides the unit tests for the GunnsBasicExternalSupply class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsBasicExternalSupply: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsBasicExternalSupply(const UtGunnsBasicExternalSupply& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsBasicExternalSupply& operator =(const UtGunnsBasicExternalSupply& that);

        CPPUNIT_TEST_SUITE(UtGunnsBasicExternalSupply);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testIoMethods);
        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsBasicExternalSupplyConfigData* tConfigData;

        /// --     Pointer to nominal input data
        GunnsBasicExternalSupplyInputData*  tInputData;

        /// --     Test Article
        FriendlyGunnsBasicExternalSupply*   tArticle;

        /// --     Conductor Name
        std::string                         tLinkName;

        /// --     Link demand
        double                              tInitialDemand;

        /// --     Network Nodes
        GunnsBasicNode                      tNodes[3];

        /// --     Node List
        GunnsNodeList                       tNodeList;

        /// --     Network Links
        std::vector<GunnsBasicLink*>        tLinks;

        /// --     Nominal inlet port index
        int                                 tPort0;

        /// --     Nominal outlet port index
        int                                 tPort1;

        /// (s)    Nominal time step
        double                              tTimeStep;

    public:
        UtGunnsBasicExternalSupply();
        virtual ~UtGunnsBasicExternalSupply();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testIoMethods();
};

///@}

#endif
