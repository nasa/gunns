#ifndef UtGunnsElectDistributed2WayBus_EXISTS
#define UtGunnsElectDistributed2WayBus_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_ELECTRICAL_2WAY_BUS    Electrical Distributed 2-Way Bus Interface Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2023 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Electrical Distributed 2-Way Bus Interface
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/PowerBus/GunnsElectDistributed2WayBus.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectDistributed2WayBus and befriend UtGunnsElectDistributed2WayBus.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectDistributed2WayBus : public GunnsElectDistributed2WayBus
{
    public:
        FriendlyGunnsElectDistributed2WayBus() {;}
        virtual ~FriendlyGunnsElectDistributed2WayBus() {;}
        friend class UtGunnsElectDistributed2WayBus;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electric Electrical Distributed 2-Way Bus Interface unit tests.
////
/// @details  This class provides the unit tests for the GunnsElectDistributed2WayBus within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectDistributed2WayBus : public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtGunnsElectDistributed2WayBus();
        /// @brief  Nominal destructs
        virtual ~UtGunnsElectDistributed2WayBus();
        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Executes after each test.
        void tearDown();
        /// @brief  Tests construction of the GunnsElectDistributed2WayBusNotification class.
        void testNotificationConstruction();
        /// @brief  Tests default construction.
        void testConstruction();
        /// @brief  Tests the initialization method with nominal values.
        void testNominalInitialization();
        /// @brief  Tests the initialization method with errors.
        void testInitializationErrors();
        /// @brief  Tests the updateFrameCounts method.
        void testUpdateFrameCounts();
        /// @brief  Tests the update method.
        void testUpdate();
        /// @brief  Tests the update method with forced roles.
        void testUpdateForcedRole();
        /// @brief  Tests the accessor methods.
        void testAccessors();

    private:
        /// @brief  Sets up the suite of tests for the GunnsElectDistributed2WayBus unit testing.
        CPPUNIT_TEST_SUITE(UtGunnsElectDistributed2WayBus);
        CPPUNIT_TEST(testNotificationConstruction);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationErrors);
        CPPUNIT_TEST(testUpdateFrameCounts);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST(testUpdateForcedRole);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();
        /// @brief  Enumeration for the number of nodes.
//        enum {N_NODES = 3};
//        std::vector<GunnsBasicLink*>             tLinks;               /**< (--)    Network links vector. */
//        GunnsBasicNode                           tNodes[N_NODES];      /**< (--)    Network nodes. */
//        GunnsNodeList                            tNodeList;            /**< (--)    Network nodes list. */
//        int                                      tPort0;               /**< (--)    Port 0 node mapping. */
        std::string                              tName;                /**< (--)    Test article name. */
//        GunnsElectDistributed2WayBusConfigData*     tConfigData;          /**< (--)    Pointer to config data. */
//        GunnsElectDistributed2WayBusInputData*      tInputData;           /**< (--)    Pointer to input data. */
        FriendlyGunnsElectDistributed2WayBus*       tArticle;             /**< (--)    Pointer to article under test. */
//        GunnsSensorAnalogWrapper                 tSensorVout;          /**< (--)    Sensor object. */
//        GunnsSensorAnalogWrapper                 tSensorIout;          /**< (--)    Sensor object. */
//        GunnsElectDistributed2WayBus::RegulatorType tRegulatorType;       /**< (--)    Nominal config data. */
//        double                                   tOutputConductance;   /**< (1/ohm) Nominal config data. */
//        double                                   tConverterEfficiency; /**< (--)    Nominal config data. */
//        unsigned int                             tTripPriority;        /**< (--)    Nominal config data. */
//        double                                   tOutOverVoltageTrip;  /**< (V)     Nominal config data. */
//        double                                   tOutUnderVoltageTrip; /**< (V)     Nominal config data. */
//        double                                   tOutOverCurrentTrip;  /**< (amp)   Nominal config data. */
//        bool                                     tEnableLimit;         /**< (--)    Nominal config data. */
//        bool                                     tMalfBlockageFlag;    /**< (--)    Nominal input data. */
//        double                                   tMalfBlockageValue;   /**< (--)    Nominal input data. */
//        bool                                     tEnabled;             /**< (--)    Nominal input data. */
//        double                                   tInputVoltage;        /**< (V)     Nominal input data. */
//        double                                   tInputPower;          /**< (W)     Nominal input data. */
//        double                                   tSetpoint;            /**< (--)    Nominal input data. */
//        GunnsElectDistributed2WayBusConfigData*      tInputConfigData;     /**< (--)    Pointer to config data. */
//        GunnsElectDistributed2WayBusInputData*       tInputInputData;      /**< (--)    Pointer to input data. */
//        FriendlyGunnsElectDistributed2WayBus         tInputLink;           /**< (--)    Test converter input link. */
        static int                               TEST_ID;              /**< (--)    Test identification number. */
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        UtGunnsElectDistributed2WayBus(const UtGunnsElectDistributed2WayBus& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        UtGunnsElectDistributed2WayBus& operator =(const UtGunnsElectDistributed2WayBus& that);
};

///@}

#endif
