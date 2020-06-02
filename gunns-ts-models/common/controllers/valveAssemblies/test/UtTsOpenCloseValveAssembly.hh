#ifndef UtTsOpenCloseValveAssembly_EXISTS
#define UtTsOpenCloseValveAssembly_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_CONTROLLERS_TsOpenCloseValveAssembly  Open/Close Valve Assembly Unit Tests
/// @ingroup   UT_TSM_CONTROLLERS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the Open/Close Valve Assembly model.
///
///@{
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "../TsOpenCloseValveAssembly.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from TsOpenCloseValveAssembly and befriend UtTsOpenCloseValveAssembly.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyTsOpenCloseValveAssembly : public TsOpenCloseValveAssembly
{
    public:
        FriendlyTsOpenCloseValveAssembly();
        virtual ~FriendlyTsOpenCloseValveAssembly();
        friend class UtTsOpenCloseValveAssembly;
};
inline FriendlyTsOpenCloseValveAssembly::FriendlyTsOpenCloseValveAssembly() : TsOpenCloseValveAssembly() {};
inline FriendlyTsOpenCloseValveAssembly::~FriendlyTsOpenCloseValveAssembly() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Valve Assembly unit tests.
///
/// @details  This class provides the unit tests for the Open/Close Valve Assembly model within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsOpenCloseValveAssembly : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this OpenClose Valve Assembly unit test.
        UtTsOpenCloseValveAssembly();
        /// @brief    Default destructs this OpenClose Valve Assembly unit test.
        virtual ~UtTsOpenCloseValveAssembly();
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
        CPPUNIT_TEST_SUITE(UtTsOpenCloseValveAssembly);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationFailure);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateNominal);
        CPPUNIT_TEST_SUITE_END();
        TsPoweredValveControllerConfigData  cController;   /**< (--) Nominal config data */
        SensorBooleanAiConfigData           cSensorOpen;   /**< (--) Nominal config data */
        SensorBooleanAiConfigData           cSensorClosed; /**< (--) Nominal config data */
        TsOpenCloseValveAssemblyConfigData* tConfig;       /**< (--) Nominal config data */
        TsPoweredValveControllerInputData   iController;   /**< (--) Nominal input data */
        SensorBooleanAiInputData            iSensorOpen;   /**< (--) Nominal input data */
        SensorBooleanAiInputData            iSensorClosed; /**< (--) Nominal input data */
        TsOpenCloseValveAssemblyInputData*  tInput;        /**< (--) Nominal input data */
        std::string                         tName;         /**< (--) Object name */
        FriendlyTsOpenCloseValveAssembly*   tArticle;      /**< (--) Pointer to article under test */
        double                              tTimeStep;     /**< (s)  Nominal time step */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsOpenCloseValveAssembly(const UtTsOpenCloseValveAssembly&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsOpenCloseValveAssembly& operator =(const UtTsOpenCloseValveAssembly&);
};

/// @}

#endif
