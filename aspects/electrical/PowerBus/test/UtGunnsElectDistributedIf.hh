#ifndef UtGunnsElectDistributedIf_EXISTS
#define UtGunnsElectDistributedIf_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_ELECTRICAL_DISTRIBUTED_INTERFACE    Electrical Distributed Interface Link Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2023 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Electrical Distributed Interface Link
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/PowerBus/GunnsElectDistributedIf.hh"
#include "UtGunnsElectDistributed2WayBus.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectDistributedIf and befriend UtGunnsElectDistributedIf.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectDistributedIf : public GunnsElectDistributedIf
{
    public:
        FriendlyGunnsElectDistributedIf() {;}
        virtual ~FriendlyGunnsElectDistributedIf() {;}
        friend class UtGunnsElectDistributedIf;
};

class FriendlyGunnsElectConverterOutput : public GunnsElectConverterOutput
{
    public:
        FriendlyGunnsElectConverterOutput() {;}
        virtual ~FriendlyGunnsElectConverterOutput() {;}
        friend class UtGunnsElectDistributedIf;
};

class FriendlyGunnsElectConverterInput : public GunnsElectConverterInput
{
    public:
        FriendlyGunnsElectConverterInput() {;}
        virtual ~FriendlyGunnsElectConverterInput() {;}
        friend class UtGunnsElectDistributedIf;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electrical Distributed Interface Link unit tests.
////
/// @details  This class provides the unit tests for the GunnsElectDistributedIf within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectDistributedIf: public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtGunnsElectDistributedIf();
        /// @brief  Nominal destructs
        virtual ~UtGunnsElectDistributedIf();
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
        /// @brief  Tests the initialization method with nominal values.
        void testNominalInitialization();
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
        /// @brief  Sets up the suite of tests for the GunnsElectDistributedIf unit testing.
        CPPUNIT_TEST_SUITE(UtGunnsElectDistributedIf);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationErrors);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testMinorStep);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testConfirmSolutionAcceptable);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST_SUITE_END();
        /// @brief  Enumeration for the number of nodes.
        enum {N_NODES = 3};
        std::vector<GunnsBasicLink*>          tLinks;           /**< (--)    Network links vector. */
        GunnsBasicNode                        tNodes[N_NODES];  /**< (--)    Network nodes. */
        GunnsNodeList                         tNodeList;        /**< (--)    Network nodes list. */
        double*                               tNodeNetCapDp;    /**< (--)    Network node network capacitance delta-potential array. */
        int                                   tPort0;           /**< (--)    Port 0 node mapping. */
        std::string                           tName;            /**< (--)    Test article name. */
        GunnsElectDistributedIfConfigData*    tConfigData;      /**< (--)    Pointer to config data. */
        GunnsElectDistributedIfInputData*     tInputData;       /**< (--)    Pointer to input data. */
        FriendlyGunnsElectDistributedIf*      tArticle;         /**< (--)    Pointer to article under test. */
        FriendlyGunnsElectDistributed2WayBus* tInterface;       /**< (--)    Pointer the interface model within the test article. */
        FriendlyGunnsElectConverterOutput*    tVoltageSource;   /**< (--)    Pointer to the voltage source link within the test article. */
        FriendlyGunnsElectConverterInput*     tPowerLoad;       /**< (--)    Pointer to the power load link within the test article. */
        bool                                  tIsPairPrimary;   /**< (--)    Nominal config data. */
        double                                tConductance;     /**< (1/ohm) Nominal config data. */
        double                                tNetCapDvThresh;  /**< (V)     Nominal config data. */
        bool                                  tForceDemandMode; /**< (--)    Nominal input data. */
        bool                                  tForceSupplyMode; /**< (--)    Nominal input data. */
        FriendlyGunnsElectConverterOutput     tSupply;          /**< (--)    Supply link. */
        static int                            TEST_ID;          /**< (--)    Test identification number. */
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        UtGunnsElectDistributedIf(const UtGunnsElectDistributedIf& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        UtGunnsElectDistributedIf& operator =(const UtGunnsElectDistributedIf& that);
};

///@}

#endif
