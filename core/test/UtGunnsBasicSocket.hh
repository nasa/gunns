#ifndef UtGunnsBasicSocket_EXISTS
#define UtGunnsBasicSocket_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_BASIC_SOCKET    Gunns Basic Socket Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Basic Socket link.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsBasicSocket.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBasicSocket and befriend UtGunnsBasicSocket.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsBasicSocket : public GunnsBasicSocket
{
    public:
        FriendlyGunnsBasicSocket() : GunnsBasicSocket() {}
        virtual ~FriendlyGunnsBasicSocket() {}
        friend class UtGunnsBasicSocket;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basic Socket unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Basic Socket link within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsBasicSocket: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsBasicSocket(const UtGunnsBasicSocket& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsBasicSocket& operator =(const UtGunnsBasicSocket& that);

        CPPUNIT_TEST_SUITE(UtGunnsBasicSocket);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testRequestConnection);
        CPPUNIT_TEST(testRequestDisconnection);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsBasicSocket*    tArticle;             /**< (--) Nominal config data */
        std::string                  tLinkName;            /**< (--) Nominal config data */
        double                       tDefaultConductivity; /**< (--) Nominal config data */
        bool                         tSelfSealing;         /**< (--) Nominal config data */
        GunnsBasicSocketConfigData*  tConfigData;          /**< (--) Nominal config data */
        double                       tBlockage;            /**< (--) Blockage malfunction */
        GunnsBasicSocketInputData*   tInputData;           /**< (--) Nominal input data */
        GunnsBasicNode               tNodes[2];            /**< (--) Network nodes */
        GunnsNodeList                tNodeList;            /**< (--) Network nodes list */
        std::vector<GunnsBasicLink*> tLinks;               /**< (--) Network links vector */
        int                          tPort0;               /**< (--) Nominal inlet port index */
        int                          tPort1;               /**< (--) Nominal exit port index */

    public:
        UtGunnsBasicSocket();
        virtual ~UtGunnsBasicSocket();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testRequestConnection();
        void testRequestDisconnection();
        void testAccessors();
        void testStep();
};

///@}

#endif
