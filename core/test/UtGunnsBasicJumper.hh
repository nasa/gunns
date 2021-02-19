#ifndef UtGunnsBasicJumper_EXISTS
#define UtGunnsBasicJumper_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_BASIC_JUMPER    Gunns Basic Jumper Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Basic Jumper link.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsBasicJumper.hh"
#include "core/GunnsBasicSocket.hh"
#include "UtGunnsBasicJumperPlug.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBasicJumper and befriend UtGunnsBasicJumper.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsBasicJumper : public GunnsBasicJumper
{
    public:
        FriendlyGunnsBasicJumper() : GunnsBasicJumper() {}
        virtual ~FriendlyGunnsBasicJumper() {}
        friend class UtGunnsBasicJumper;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basic Jumper unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Basic Jumper link within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsBasicJumper: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsBasicJumper(const UtGunnsBasicJumper& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsBasicJumper& operator =(const UtGunnsBasicJumper& that);

        CPPUNIT_TEST_SUITE(UtGunnsBasicJumper);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsBasicJumper*     tArticle;             /**< (--) Nominal config data */
        std::string                   tLinkName;            /**< (--) Nominal config data */
        double                        tDefaultConductivity; /**< (--) Nominal config data */
        FriendlyGunnsBasicJumperPlug* tJumperPlug0;         /**< (--) Nominal config data */
        FriendlyGunnsBasicJumperPlug* tJumperPlug1;         /**< (--) Nominal config data */
        bool                          tSelfSealing0;        /**< (--) Nominal config data */
        bool                          tSelfSealing1;        /**< (--) Nominal config data */
        GunnsBasicJumperConfigData*   tConfigData;          /**< (--) Nominal config data */
        double                        tBlockage;            /**< (--) Nominal input data */
        int                           tConnection0;         /**< (--) Nominal input data */
        int                           tConnection1;         /**< (--) Nominal input data */
        GunnsBasicJumperInputData*    tInputData;           /**< (--) Nominal input data */
        GunnsBasicNode                tNodes[3];            /**< (--) Network nodes */
        GunnsNodeList                 tNodeList;            /**< (--) Network nodes list */
        std::vector<GunnsBasicLink*>  tLinks;               /**< (--) Network links vector */
        int                           tPort0;               /**< (--) Nominal inlet port index */
        int                           tPort1;               /**< (--) Nominal exit port index */
        GunnsBasicSocket              tSocket0A;            /**< (--) Test socket */
        GunnsBasicSocket              tSocket0B;            /**< (--) Test socket */
        GunnsBasicSocket              tSocket0C;            /**< (--) Test socket */
        GunnsBasicSocket              tSocket1A;            /**< (--) Test socket */
        GunnsBasicSocket              tSocket1B;            /**< (--) Test socket */
        GunnsBasicSocket              tSocket1C;            /**< (--) Test socket */

    public:
        UtGunnsBasicJumper();
        virtual ~UtGunnsBasicJumper();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testStep();
};

///@}

#endif
