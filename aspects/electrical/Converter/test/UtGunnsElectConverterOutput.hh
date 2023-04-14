#ifndef UtGunnsElectConverterOutput_EXISTS
#define UtGunnsElectConverterOutput_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_ELECTRICAL_CONVERTER_OUTPUT_LINK    Electrical Converter Output Link Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2023 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Electrical Converter Output Link
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "UtGunnsElectConverterInput.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electric Electrical Converter Output Link unit tests.
////
/// @details  This class provides the unit tests for the GunnsElectConverterInput within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectConverterOutput: public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtGunnsElectConverterOutput();
        /// @brief  Nominal destructs
        virtual ~UtGunnsElectConverterOutput();
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
        /// @brief  Tests the computeInputPower method.
        void testComputeInputPower();
        /// @brief  Tests the getter and setter methods.
        void testAccessors();
        /// @brief  Tests the confirmSolutionAcceptable method.
        void testConfirmSolutionAcceptable();
        /// @brief  Tests the confirmSolutionAcceptable method for current sources.
        void testConfirmSolutionCurrentSource();
        /// @brief  Tests the computeFlows method.
        void testComputeFlows();
        /// @brief  Tests the getControlVoltage method.
        void testControlVoltage();

    private:
        /// @brief  Sets up the suite of tests for the GunnsElectConverterInput unit testing.
        CPPUNIT_TEST_SUITE(UtGunnsElectConverterOutput);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationErrors);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testMinorStep);
        CPPUNIT_TEST(testComputeInputPower);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testConfirmSolutionAcceptable);
        CPPUNIT_TEST(testConfirmSolutionCurrentSource);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testControlVoltage);
        CPPUNIT_TEST_SUITE_END();
        /// @brief  Enumeration for the number of nodes.
        enum {N_NODES = 3};
        std::vector<GunnsBasicLink*>             tLinks;               /**< (--)    Network links vector. */
        GunnsBasicNode                           tNodes[N_NODES];      /**< (--)    Network nodes. */
        GunnsNodeList                            tNodeList;            /**< (--)    Network nodes list. */
        int                                      tPort0;               /**< (--)    Port 0 node mapping. */
        std::string                              tName;                /**< (--)    Test article name. */
        GunnsElectConverterOutputConfigData*     tConfigData;          /**< (--)    Pointer to config data. */
        GunnsElectConverterOutputInputData*      tInputData;           /**< (--)    Pointer to input data. */
        FriendlyGunnsElectConverterOutput*       tArticle;             /**< (--)    Pointer to article under test. */
        GunnsSensorAnalogWrapper                 tSensorVout;          /**< (--)    Sensor object. */
        GunnsSensorAnalogWrapper                 tSensorIout;          /**< (--)    Sensor object. */
        GunnsElectConverterOutput::RegulatorType tRegulatorType;       /**< (--)    Nominal config data. */
        double                                   tOutputConductance;   /**< (1/ohm) Nominal config data. */
        double                                   tConverterEfficiency; /**< (--)    Nominal config data. */
        unsigned int                             tTripPriority;        /**< (--)    Nominal config data. */
        double                                   tOutOverVoltageTrip;  /**< (V)     Nominal config data. */
        double                                   tOutUnderVoltageTrip; /**< (V)     Nominal config data. */
        double                                   tOutOverCurrentTrip;  /**< (amp)   Nominal config data. */
        bool                                     tEnableLimit;         /**< (--)    Nominal config data. */
        bool                                     tMalfBlockageFlag;    /**< (--)    Nominal input data. */
        double                                   tMalfBlockageValue;   /**< (--)    Nominal input data. */
        bool                                     tEnabled;             /**< (--)    Nominal input data. */
        double                                   tInputVoltage;        /**< (V)     Nominal input data. */
        double                                   tInputPower;          /**< (W)     Nominal input data. */
        double                                   tSetpoint;            /**< (--)    Nominal input data. */
        GunnsElectConverterInputConfigData*      tInputConfigData;     /**< (--)    Pointer to config data. */
        GunnsElectConverterInputInputData*       tInputInputData;      /**< (--)    Pointer to input data. */
        FriendlyGunnsElectConverterInput         tInputLink;           /**< (--)    Test converter input link. */
        static int                               TEST_ID;              /**< (--)    Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectConverterOutput(const UtGunnsElectConverterOutput& that);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectConverterOutput& operator =(const UtGunnsElectConverterOutput& that);
};

///@}

#endif
