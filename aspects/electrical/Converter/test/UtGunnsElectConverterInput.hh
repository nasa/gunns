#ifndef UtGunnsElectConverterInput_EXISTS
#define UtGunnsElectConverterInput_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_ELECTRICAL_CONVERTER_INPUT_LINK    Electrical Converter Input Link Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2022 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Electrical Converter Input Link
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/Converter/GunnsElectConverterInput.hh"
#include "aspects/electrical/Converter/GunnsElectConverterOutput.hh"
#include "core/GunnsSensorAnalogWrapper.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectConverterInput and befriend UtGunnsElectConverterInput.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectConverterInput : public GunnsElectConverterInput
{
    public:
        FriendlyGunnsElectConverterInput() {;}
        virtual ~FriendlyGunnsElectConverterInput() {;}
        friend class UtGunnsElectConverterInput;
        friend class UtGunnsElectConverterOutput;
};

class FriendlyGunnsElectConverterOutput : public GunnsElectConverterOutput
{
    public:
        FriendlyGunnsElectConverterOutput() {;}
        virtual ~FriendlyGunnsElectConverterOutput() {;}
        friend class UtGunnsElectConverterInput;
        friend class UtGunnsElectConverterOutput;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electric Electrical Converter Input Link unit tests.
////
/// @details  This class provides the unit tests for the GunnsElectConverterInput within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectConverterInput: public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtGunnsElectConverterInput();
        /// @brief  Nominal destructs
        virtual ~UtGunnsElectConverterInput();
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
        /// @brief  Tests the computeInputVoltage method.
        void testComputeInputVoltage();
        /// @brief  Tests the getter and setter methods.
        void testAccessors();
        /// @brief  Tests the confirmSolutionAcceptable method.
        void testConfirmSolutionAcceptable();
        /// @brief  Tests the resetLastMinorStep method.
        void testResetLastMinorStep();
        /// @brief  Tests the computeFlows method.
        void testComputeFlows();

    private:
        /// @brief  Sets up the suite of tests for the GunnsElectConverterInput unit testing.
        CPPUNIT_TEST_SUITE(UtGunnsElectConverterInput);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationErrors);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testMinorStep);
        CPPUNIT_TEST(testComputeInputVoltage);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testConfirmSolutionAcceptable);
        CPPUNIT_TEST(testResetLastMinorStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST_SUITE_END();
        /// @brief  Enumeration for the number of nodes.
        enum {N_NODES = 3};
        std::vector<GunnsBasicLink*>         tLinks;              /**< (--) Network links vector. */
        GunnsBasicNode                       tNodes[N_NODES];     /**< (--) Network nodes. */
        GunnsNodeList                        tNodeList;           /**< (--) Network nodes list. */
        int                                  tPort0;              /**< (--) Port 0 node mapping. */
        std::string                          tName;               /**< (--) Test article name. */
        GunnsElectConverterInputConfigData*  tConfigData;         /**< (--) Pointer to config data. */
        GunnsElectConverterInputInputData*   tInputData;          /**< (--) Pointer to input data. */
        FriendlyGunnsElectConverterInput*    tArticle;            /**< (--) Pointer to article under test. */
        GunnsSensorAnalogWrapper             tSensorVin;          /**< (--) Sensor object. */
        GunnsSensorAnalogWrapper             tSensorIin;          /**< (--) Sensor object. */
        unsigned int                         tTripPriority;       /**< (--) Nominal config data. */
        double                               tInOverVoltageTrip;  /**< (V)  Nominal config data. */
        double                               tInUnderVoltageTrip; /**< (V)  Nominal config data. */
        TsLinearInterpolator*                tEfficiencyTable;    /**< (--) Nominal config data. */
        bool                                 tMalfBlockageFlag;   /**< (--) Nominal input data. */
        double                               tMalfBlockageValue;  /**< (--) Nominal input data. */
        bool                                 tEnabled;            /**< (--) Nominal input data. */
        double                               tInputVoltage;       /**< (V)  Nominal input data. */
        double                               tInputPower;         /**< (W)  Nominal input data. */
        double                               tReferencePower;     /**< (W)  Nominal input data. */
        GunnsElectConverterOutputConfigData* tOutputConfigData;   /**< (--) Pointer to config data. */
        GunnsElectConverterOutputInputData*  tOutputInputData;    /**< (--) Pointer to input data. */
        FriendlyGunnsElectConverterOutput    tOutputLink;         /**< (--) Test converter output link. */
        static int                           TEST_ID;             /**< (--) Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectConverterInput(const UtGunnsElectConverterInput& that);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectConverterInput& operator =(const UtGunnsElectConverterInput& that);
};

///@}

#endif
