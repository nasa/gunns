#ifndef UtGunnsElectPvRegShunt_EXISTS
#define UtGunnsElectPvRegShunt_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_PHOTOVOLTAIC_SHUNTING_REGULATOR    Photovoltaic Array Shunting Regulator Link Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Photovoltaic Array Shunting Regulator Link
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/SolarArray/GunnsElectPvRegShunt.hh"
#include "core/GunnsSensorAnalogWrapper.hh"
#include "UtGunnsElectPvArray.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectPvRegShunt and befriend UtGunnsElectPvRegShunt.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectPvRegShunt : public GunnsElectPvRegShunt
{
    public:
        FriendlyGunnsElectPvRegShunt() {;}
        virtual ~FriendlyGunnsElectPvRegShunt() {;}
        friend class UtGunnsElectPvRegShunt;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electric Photovoltaic Array Shunting Regulator Link unit tests.
////
/// @details  This class provides the unit tests for the GunnsElectPvRegShunt within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectPvRegShunt: public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtGunnsElectPvRegShunt();
        /// @brief  Nominal destructs
        virtual ~UtGunnsElectPvRegShunt();
        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Executes after each test.
        void tearDown();
        /// @brief  Tests config data.
        void testConfig();
        /// @brief  Tests input data.
        void testInput();
        /// @brief  Tests construction methods.
        void testConstruction();
        /// @brief  Tests the initialization method with nominal values and custom strings load order list.
        void testNominalInitialization();
        /// @brief  Tests the initialization method with nominal values and default strings load order list.
        void testDefaultLoadOrderInitialization();
        /// @brief  Tests the initialization method with errors.
        void testInitializationErrors();
        /// @brief  Tests the restart method.
        void testRestart();
        /// @brief  Tests the step method.
        void testStep();
        /// @brief  Tests the minorStep method.
        void testMinorStep();
        /// @brief  Tests the getter and setter methods.
        void testAccessors();
        /// @brief  Tests the confirmSolutionAcceptable method.
        void testConfirmSolutionAcceptable();
        /// @brief  Tests the computeFlows method.
        void testComputeFlows();

    private:
        /// @brief  Sets up the suite of tests for the GunnsElectPvRegShunt unit testing.
        CPPUNIT_TEST_SUITE(UtGunnsElectPvRegShunt);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testDefaultLoadOrderInitialization);
        CPPUNIT_TEST(testInitializationErrors);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testMinorStep);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testConfirmSolutionAcceptable);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST_SUITE_END();
        /// @brief  Enumeration for the number of nodes.
        enum {N_NODES = 2};
        std::vector<GunnsBasicLink*>    tLinks;               /**< (--)    Network links vector. */
        GunnsBasicNode                  tNodes[N_NODES];      /**< (--)    Network nodes. */
        GunnsNodeList                   tNodeList;            /**< (--)    Network nodes list. */
        int                             tPort0;               /**< (--)    Port 0 node mapping. */
        int                             tPort1;               /**< (--)    Port 1 node mapping. */
        std::string                     tName;                /**< (--)    Test article name. */
        GunnsElectPvRegShuntConfigData* tConfigData;          /**< (--)    Pointer to config data. */
        GunnsElectPvRegShuntInputData*  tInputData;           /**< (--)    Pointer to input data. */
        FriendlyGunnsElectPvRegShunt*   tArticle;             /**< (--)    Pointer to article under test. */
        double                          tOutputConductance;   /**< (1/ohm) Nominal config data. */
        double                          tShuntConductance;    /**< (1/ohm) Nominal config data. */
        GunnsSensorAnalogWrapper        tSensorIin;           /**< (--)    Sensor object. */
        GunnsSensorAnalogWrapper        tSensorVin;           /**< (--)    Sensor object. */
        GunnsSensorAnalogWrapper        tSensorIout;          /**< (--)    Sensor object. */
        GunnsSensorAnalogWrapper        tSensorVout;          /**< (--)    Sensor object. */
        double                          tInOverCurrentTrip;   /**< (amp)   Nominal config data. */
        double                          tInOverVoltageTrip;   /**< (v)     Nominal config data. */
        double                          tOutOverCurrentTrip;  /**< (amp)   Nominal config data. */
        double                          tOutOverVoltageTrip;  /**< (v)     Nominal config data. */
        double                          tOutUnderVoltageTrip; /**< (v)     Nominal config data. */
        unsigned int                    tTripPriority;        /**< (v)     Nominal config data. */
        FriendlyGunnsElectPvArray*      tArray;               /**< (--)    Test PV array. */
        GunnsElectPvArrayConfigData*    tArrayConfig;         /**< (--)    Test PV array config data. */
        GunnsElectPvArrayInputData*     tArrayInput;          /**< (--)    Test PV array input data. */
        double                          tVoltageSetpoint;     /**< (v)     Nominal input data. */
        bool                            tPowered;             /**< (--)    Nominal input data. */
        bool                            tEnabled;             /**< (--)    Nominal input data. */
        double                          tMinOperatePower;     /**< (W)     Nominal input data. */
        static int                      TEST_ID;              /**< (--)    Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectPvRegShunt(const UtGunnsElectPvRegShunt& that);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectPvRegShunt& operator =(const UtGunnsElectPvRegShunt& that);
};

///@}

#endif
