#ifndef UtTsToggleValveAssembly_EXISTS
#define UtTsToggleValveAssembly_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_CONTROLLERS_TsToggleValveAssembly  Toggle Valve Assembly Unit Tests
/// @ingroup   UT_TSM_CONTROLLERS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the Toggle Valve Assembly model.
///
///@{
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "../TsToggleValveAssembly.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from TsToggleValveAssembly and befriend UtTsToggleValveAssembly.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyTsToggleValveAssembly : public TsToggleValveAssembly
{
    public:
        FriendlyTsToggleValveAssembly();
        virtual ~FriendlyTsToggleValveAssembly();
        friend class UtTsToggleValveAssembly;
};
inline FriendlyTsToggleValveAssembly::FriendlyTsToggleValveAssembly() : TsToggleValveAssembly() {};
inline FriendlyTsToggleValveAssembly::~FriendlyTsToggleValveAssembly() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Valve Assembly unit tests.
///
/// @details  This class provides the unit tests for the Toggle Valve Assembly model within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsToggleValveAssembly : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Toggle Valve Assembly unit test.
        UtTsToggleValveAssembly();
        /// @brief    Default destructs this Toggle Valve Assembly unit test.
        virtual ~UtTsToggleValveAssembly();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config and input data.
        void testConfigAndInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method (nominal).
        void testNominalInitialization();
        /// @brief    Tests initialize method (objects fail to init).
        void testInitializationFailure();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update method (nominal).
        void testUpdateNominal();
    private:
        CPPUNIT_TEST_SUITE(UtTsToggleValveAssembly);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationFailure);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateNominal);
        CPPUNIT_TEST_SUITE_END();
        TsPoweredValveControllerConfigData cController;   /**< (--) Nominal config data */
        SensorBooleanAiConfigData          cSensorOpen;   /**< (--) Nominal config data */
        SensorBooleanAiConfigData          cSensorClosed; /**< (--) Nominal config data */
        TsToggleValveAssemblyConfigData*   tConfig;       /**< (--) Nominal config data */
        TsPoweredValveControllerInputData  iController;   /**< (--) Nominal input data */
        SensorBooleanAiInputData           iSensorOpen;   /**< (--) Nominal input data */
        SensorBooleanAiInputData           iSensorClosed; /**< (--) Nominal input data */
        TsToggleValveAssemblyInputData*    tInput;        /**< (--) Nominal input data */
        std::string                        tName;         /**< (--) Object name */
        FriendlyTsToggleValveAssembly*     tArticle;      /**< (--) Pointer to article under test */
        double                             tTimeStep;     /**< (s)  Nominal time step */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsToggleValveAssembly(const UtTsToggleValveAssembly&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsToggleValveAssembly& operator =(const UtTsToggleValveAssembly&);
};

/// @}

#endif
