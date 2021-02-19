#ifndef UtGunnsFluidSocket_EXISTS
#define UtGunnsFluidSocket_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_Fluid_SOCKET    Gunns Fluid Socket Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Socket link.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsFluidSocket.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidSocket and befriend UtGunnsFluidSocket.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidSocket : public GunnsFluidSocket
{
    public:
        FriendlyGunnsFluidSocket() : GunnsFluidSocket() {}
        virtual ~FriendlyGunnsFluidSocket() {}
        friend class UtGunnsFluidSocket;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid Socket unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Socket link within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidSocket: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidSocket(const UtGunnsFluidSocket& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidSocket& operator =(const UtGunnsFluidSocket& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidSocket);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRequestConnection);
        CPPUNIT_TEST(testRequestDisconnection);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsFluidSocket*    tArticle;              /**< (--)   Nominal config data */
        std::string                  tLinkName;             /**< (--)   Nominal config data */
        double                       tMaxConductivity;      /**< (--)   Nominal config data */
        double                       tExpansionScaleFactor; /**< (--)   Nominal config data */
        bool                         tSelfSealing;          /**< (--)   Nominal config data */
        GunnsFluidSocketConfigData*  tConfigData;           /**< (--)   Nominal config data */
        double                       tBlockage;             /**< (--)   Blockage malfunction */
        bool                         tMalfLeakRateFlag;     /**< (--)   Nominal input data */
        double                       tMalfLeakRateValue;    /**< (kg/s) Nominal input data */
        GunnsFluidSocketInputData*   tInputData;            /**< (--)   Nominal input data */
        GunnsFluidNode               tNodes[2];             /**< (--)   Network nodes */
        GunnsNodeList                tNodeList;             /**< (--)   Network nodes list */
        std::vector<GunnsBasicLink*> tLinks;                /**< (--)   Network links vector */
        int                          tPort0;                /**< (--)   Nominal inlet port index */
        int                          tPort1;                /**< (--)   Nominal exit port index */
        DefinedFluidProperties*      tFluidProperties;      /**< (--)   Predefined fluid properties */
        PolyFluidConfigData*         tFluidConfig;          /**< (--)   Fluid config data */
        PolyFluidInputData*          tFluidInput1;          /**< (--)   Fluid 1 input data */
        PolyFluidInputData*          tFluidInput2;          /**< (--)   Fluid 1 input data */
        double*                      tFractions;            /**< (--)   Fluid mass fractions */

    public:
        UtGunnsFluidSocket();
        virtual ~UtGunnsFluidSocket();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testRequestConnection();
        void testRequestDisconnection();
        void testAccessors();
        void testStep();
};

///@}

#endif
