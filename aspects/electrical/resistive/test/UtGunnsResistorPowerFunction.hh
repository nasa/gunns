#ifndef UtGunnsResistorPowerFunction_EXISTS
#define UtGunnsResistorPowerFunction_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_BASIC_BERNOULLI   Resistor With Power Function Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Resistor With Power Function link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/resistive/GunnsResistorPowerFunction.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsResistorPowerFunction and befriend UtGunnsResistorPowerFunction.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsResistorPowerFunction : public GunnsResistorPowerFunction
{
    public:
        FriendlyGunnsResistorPowerFunction();
        virtual ~FriendlyGunnsResistorPowerFunction();
        friend class UtGunnsResistorPowerFunction;
};
inline FriendlyGunnsResistorPowerFunction::FriendlyGunnsResistorPowerFunction() : GunnsResistorPowerFunction() {};
inline FriendlyGunnsResistorPowerFunction::~FriendlyGunnsResistorPowerFunction() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Valve unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Resistor With Power Function link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsResistorPowerFunction: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Check Valve unit test.
        UtGunnsResistorPowerFunction();
        /// @brief    Default constructs this Check Valve unit test.
        virtual ~UtGunnsResistorPowerFunction();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config data.
        void testConfig();
        /// @brief    Tests input data.
        void testInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests step method.
        void testStep();
        /// @brief    Tests minorStep method.
        void testMinorStep();
        /// @brief    Tests computeFlows method.
        void testComputeFlows();
        /// @brief    Tests restart method.
        void testRestart();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsResistorPowerFunction);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testMinorStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes.
        enum {N_NODES = 2};
        std::vector<GunnsBasicLink*>          tLinks;             /**< (--) Link vector. */
        std::string                           tName;              /**< (--) Nominal name. */
        GunnsBasicNode                        tNodes[N_NODES];    /**< (--) Nominal connected nodes. */
        GunnsNodeList                         tNodeList;          /**< (--) Network node structure. */
        int                                   tPort0;             /**< (--) Nominal inlet port index. */
        int                                   tPort1;             /**< (--) Nominal outlet port index. */
        double                                tResistance;        /**< (--) Nominal resistance. */
        double                                tExponent;          /**< (--) Nominal power function exponent. */
        bool                                  tUseTangentLine;    /**< (--) Nominal tangent line selection. */
        GunnsResistorPowerFunctionConfigData* tConfigData;        /**< (--) Pointer to nominal configuration data. */
        bool                                  tMalfBlockageFlag;  /**< (--) Blockage malfunction flag. */
        double                                tMalfBlockageValue; /**< (--) Blockage malfunction value. */
        GunnsResistorPowerFunctionInputData*  tInputData;         /**< (--) Pointer to nominal input data. */
        FriendlyGunnsResistorPowerFunction*   tArticle;           /**< (--) Pointer to the friendly Valve under test. */
        double                                tTimeStep;          /**< (s)  Nominal time step. */
        static int                            TEST_ID;            /**< (--) Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsResistorPowerFunction(const UtGunnsResistorPowerFunction&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsResistorPowerFunction& operator =(const UtGunnsResistorPowerFunction&);
};

///@}

#endif
