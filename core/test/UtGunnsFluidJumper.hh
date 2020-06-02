#ifndef UtGunnsFluidJumper_EXISTS
#define UtGunnsFluidJumper_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_FLUID_JUMPER    Gunns Fluid Jumper Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Jumper link.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsFluidJumper.hh"
#include "UtGunnsFluidJumperPlug.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidJumper and befriend UtGunnsFluidJumper.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidJumper : public GunnsFluidJumper
{
    public:
        FriendlyGunnsFluidJumper() : GunnsFluidJumper() {}
        virtual ~FriendlyGunnsFluidJumper() {}
        friend class UtGunnsFluidJumper;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid Jumper unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Jumper link within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidJumper: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidJumper(const UtGunnsFluidJumper& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidJumper& operator =(const UtGunnsFluidJumper& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidJumper);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsFluidJumper*     tArticle;              /**< (--)   Nominal config data */
        std::string                   tLinkName;             /**< (--)   Nominal config data */
        double                        tMaxConductivity;      /**< (--)   Nominal config data */
        double                        tExpansionScaleFactor; /**< (--)   Nominal config data */
        FriendlyGunnsFluidJumperPlug* tJumperPlug0;          /**< (--)   Nominal config data */
        FriendlyGunnsFluidJumperPlug* tJumperPlug1;          /**< (--)   Nominal config data */
        bool                          tSelfSealing0;         /**< (--)   Nominal config data */
        bool                          tSelfSealing1;         /**< (--)   Nominal config data */
        GunnsFluidJumperConfigData*   tConfigData;           /**< (--)   Nominal config data */
        double                        tBlockage;             /**< (--)   Blockage malfunction */
        bool                          tMalfLeakRateFlag;     /**< (--)   Nominal input data */
        double                        tMalfLeakRateValue;    /**< (kg/s) Nominal input data */
        int                           tConnection0;          /**< (--)   Nominal input data */
        int                           tConnection1;          /**< (--)   Nominal input data */
        GunnsFluidJumperInputData*    tInputData;            /**< (--)   Nominal input data */
        GunnsFluidNode                tNodes[3];             /**< (--)   Network nodes */
        GunnsNodeList                 tNodeList;             /**< (--)   Network nodes list */
        std::vector<GunnsBasicLink*>  tLinks;                /**< (--)   Network links vector */
        int                           tPort0;                /**< (--)   Nominal inlet port index */
        int                           tPort1;                /**< (--)   Nominal exit port index */
        DefinedFluidProperties*       tFluidProperties;      /**< (--)   Predefined fluid properties */
        PolyFluidConfigData*          tFluidConfig;          /**< (--)   Fluid config data */
        PolyFluidInputData*           tFluidInput1;          /**< (--)   Fluid 1 input data */
        PolyFluidInputData*           tFluidInput2;          /**< (--)   Fluid 2 input data */
        PolyFluidInputData*           tFluidInput3;          /**< (--)   Fluid 3 input data */
        double*                       tFractions;            /**< (--)   Fluid mass fractions */
        GunnsFluidSocket              tSocket0A;             /**< (--)   Test socket */
        GunnsFluidSocket              tSocket0B;             /**< (--)   Test socket */
        GunnsFluidSocket              tSocket0C;             /**< (--)   Test socket */
        GunnsFluidSocket              tSocket1A;             /**< (--)   Test socket */
        GunnsFluidSocket              tSocket1B;             /**< (--)   Test socket */
        GunnsFluidSocket              tSocket1C;             /**< (--)   Test socket */

    public:
        UtGunnsFluidJumper();
        virtual ~UtGunnsFluidJumper();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testModifiers();
        void testStep();
};

///@}

#endif
