#ifndef UtGunnsFluidLink_EXISTS
#define UtGunnsFluidLink_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_FLUID_LINK    Gunns Fluid Link Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Link
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsFluidLink.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidLink and befriend UtGunnsFluidLink.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidLink : public GunnsFluidLink
{
    public:
        FriendlyGunnsFluidLink(const int numPorts);
        virtual ~FriendlyGunnsFluidLink();
        friend class UtGunnsFluidLink;
};
inline FriendlyGunnsFluidLink::FriendlyGunnsFluidLink(const int numPorts):
        GunnsFluidLink(numPorts) {};
inline FriendlyGunnsFluidLink::~FriendlyGunnsFluidLink() {}

class FriendlyGunnsFluidNode : public GunnsFluidNode
{
    public:
        friend class UtGunnsFluidLink;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basis Link  unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidLink: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidLink(const UtGunnsFluidLink& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidLink& operator =(const UtGunnsFluidLink& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidLink);

        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testNominalConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testNodeRegistration);
        CPPUNIT_TEST(testGetNodeContent);
        CPPUNIT_TEST(testCreateInternalFluidFromDefault);
        CPPUNIT_TEST(testCreateInternalFluidFromInput);
        CPPUNIT_TEST(testCreateInternalFluidException);
        CPPUNIT_TEST(testTransportFluid);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST(testSetPort);

        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsFluidLinkConfigData*    tConfigData;

        /// --     Pointer to nominal input data
        GunnsFluidLinkInputData*     tInputData;

        /// --     Pointer to Test Article
        FriendlyGunnsFluidLink*      tArticle;

        /// --     Link Name
        std::string                  tLinkName;

        /// --     Network Fluid Nodes
        FriendlyGunnsFluidNode*      tNetworkNodes;

        /// --     Network Node List
        GunnsNodeList                tNodeList;

        /// --     Network Links
        std::vector<GunnsBasicLink*> tNetworkLinks;

        /// --     Nominal inlet port index
        int*                         tPortMap;

        PolyFluidConfigData*         tFluidConfig;
        DefinedFluidProperties       tFluidProperties;

    public:
        UtGunnsFluidLink();
        virtual ~UtGunnsFluidLink();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testNominalConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testNodeRegistration();
        void testGetNodeContent();
        void testCreateInternalFluidFromDefault();
        void testCreateInternalFluidFromInput();
        void testCreateInternalFluidException();
        void testTransportFluid();
        void testAccessMethods();
        void testSetPort();
};

///@}

#endif
