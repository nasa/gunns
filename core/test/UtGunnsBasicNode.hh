#ifndef UtGunnsBasicNode_EXISTS
#define UtGunnsBasicNode_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_BASIC_NODE    Gunns Basic Node Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Basic Node class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
// Must list of all required C Code Model includes.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "core/GunnsBasicNode.hh"
#include "aspects/fluid/fluid/PolyFluid.hh"

class UtGunnsBasicNode;

class GunnsBasicNodeUnitTest : public GunnsBasicNode
{
    public:
        friend class UtGunnsBasicNode;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details
/// Class containing model tests.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsBasicNode : public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsBasicNode(const UtGunnsBasicNode& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsBasicNode& operator =(const UtGunnsBasicNode& that);

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Test Suite Name.
        ////////////////////////////////////////////////////////////////////////////////////////////
        CPPUNIT_TEST_SUITE(UtGunnsBasicNode);

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// List all unit test methods here.
        ////////////////////////////////////////////////////////////////////////////////////////////
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testValidate);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST(testResetFlows);
        CPPUNIT_TEST(testIntegrateFlows);
        CPPUNIT_TEST(testPlaceholderMethods);
        CPPUNIT_TEST(testRestart);

        CPPUNIT_TEST_SUITE_END();

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Define any data structures required in setUp.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsBasicNodeUnitTest tNode;
        GunnsNodeList          tNodeList;

    public:
        UtGunnsBasicNode();
        virtual ~UtGunnsBasicNode();
        void tearDown();
        void setUp();
        void testDefaultConstruction();
        void testInitialize();
        void testValidate();
        void testAccessMethods();
        void testResetFlows();
        void testIntegrateFlows();
        void testPlaceholderMethods();
        void testRestart();
};

///@}

#endif
