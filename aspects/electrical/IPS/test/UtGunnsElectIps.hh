#ifndef UtGunnsElectIps_EXISTS
#define UtGunnsElectIps_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_ELECT_IPS    Internal Power Supply Link Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2022 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Internal Power Supply link.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "aspects/electrical/IPS/GunnsElectIps.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectIps and befriend UtGunnsElectIps.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectIps : public GunnsElectIps
{
    public:
        FriendlyGunnsElectIps() : GunnsElectIps() {;}
        virtual ~FriendlyGunnsElectIps() {;}
        friend class UtGunnsElectIps;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electric Internal Power Supply Link unit tests.
////
/// @details  This class provides the unit tests for the GunnsElectIps within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectIps : public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtGunnsElectIps();
        /// @brief  Nominal destructs
        virtual ~UtGunnsElectIps();
        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Executes after each test.
        void tearDown();
        /// @brief  Test the configuration data.
        void testConfig();
        /// @brief  Test the input data.
        void testInput();
        /// @brief  Tests construction methods.
        void testConstruction();
        /// @brief  Test the initialize method.
        void testInitialize();
        /// @brief  Test the initialize method exceptions.
        void testInitializationErrors();
        /// @brief  Test the step method.
        void testStep();
        /// @brief  Test the SetterGetter method.
        void testSetterGetter();
        /// @brief  Test the confirmSolutionAcceptable method.
        void testConfirmSolutionAcceptable();
        /// @brief  Test the computeFlows method.
        void testComputeFlows();
        /// @brief  Testing the Validate method.
        void testValidate();
        /// @brief  Test the getVoltagePrimarySourceGreaterThanBackupThreshold method.
        void testGetVoltagePrimarySourceGreaterThanBackupThreshold();
        /// @brief  Test the getVoltagePrimarySourceGreaterUnderVoltageLimit method.
        void testGetVoltagePrimarySourceGreaterUnderVoltageLimit();
        /// @brief  Test the getVoltageSourceToUseWithBackup method.
        void testGetVoltageSourceToUseWithBackup();
        /// @brief  Test the getVoltageSourceToUseWithBackup method.
        void testGetVoltageSourceToUseWithoutBackup();
        /// @brief  Test the restart method.
        void testRestart();

  private:
        /// @brief  Sets up the suite of tests for the GunnsElectIps unit testing.
        CPPUNIT_TEST_SUITE(UtGunnsElectIps);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testInitializationErrors);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testSetterGetter);
        CPPUNIT_TEST(testConfirmSolutionAcceptable);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testValidate);
        CPPUNIT_TEST(testGetVoltagePrimarySourceGreaterThanBackupThreshold);
        CPPUNIT_TEST(testGetVoltagePrimarySourceGreaterUnderVoltageLimit);
        CPPUNIT_TEST(testGetVoltageSourceToUseWithBackup);
        CPPUNIT_TEST(testGetVoltageSourceToUseWithoutBackup);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST_SUITE_END();

        /// @brief  Enumeration for the number of nodes.
        enum {N_NODES = 6};
        std::vector<GunnsBasicLink*> tLinks;                              /**< (--)    Network links vector. */
        GunnsBasicNode               tNodes[N_NODES];                     /**< (--)    Network nodes. */
        GunnsNodeList                tNodeList;                           /**< (--)    Network nodes list. */
        std::vector<int>             tPorts;                              /**< (--)    Vector of port numbers for the single-port source. */
        std::string                  tName;                               /**< (--)    Test article name. */
        GunnsElectIpsConfigData*     tConfigData;                         /**< (--)    Pointer to config data. */
        GunnsElectIpsInputData*      tInputData;                          /**< (--)    Pointer to input data. */
        FriendlyGunnsElectIps*       tArticle;                            /**< (--)    Pointer to article under test. */
        bool                         tBackUpPowerImplemented;             /**< (--)    Nominal config data. */
        double                       tDefaultPowerConsumedOn;             /**< (W)     Nominal config data. */
        double                       tAuxOnePowerConsumedOn;              /**< (W)     Nominal config data. */
        double                       tAuxTwoPowerConsumedOn;              /**< (W)     Nominal config data. */
        double                       tUnderVoltageLimit;                  /**< (V)     Nominal config data. */
        double                       tBackUpVoltageThreshold;             /**< (V)     Nominal config data. */
        double                       tPotentialOnTolerance;               /**< (V)     Nominal config data. */
        double                       tThermalFraction;                    /**< (--)    Nominal config data. */
        double                       tBackupVoltageMin;                   /**< (V)     Nominal config data. */
        double                       tBackupVoltageMax;                   /**< (V)     Nominal config data. */
        double                       tConductanceTolerance;               /**< (--)    Nominal config data. */
        int                          tConvergedFrameToCheckVoltage;       /**< (--)    Nominal config data. */
        int                          tNumberOfVoltageSwitchesInASolution; /**< (--)    Nominal config data. */
        bool                         tCommandOnUsed;                      /**< (--)    Nominal config data. */
        double                       tUnselectedInputConductance;         /**< (1/ohm) Nominal config data. */
        bool                         tMalfBlockageFlag;                   /**< (--)    Nominal input data. */
        double                       tMalfBlockageValue;                  /**< (--)    Nominal input data. */

        static int                   TEST_ID;                      /**< (--)   Test identification number. */

        /// @brief  Private un-implemented copy constructor prevents invocation.
        UtGunnsElectIps(const UtGunnsElectIps&);
        /// @brief  Private un-implemented assignment operator prevents invocation.
        UtGunnsElectIps & operator =(const UtGunnsElectIps& n);
};

///@}

#endif
