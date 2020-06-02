#ifndef UtGunnsNetworkSpotter_EXISTS
#define UtGunnsNetworkSpotter_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_NETWORK_SPOTTER    Gunns Network Spotter Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Network Spotter class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/GunnsNetworkSpotter.hh"
#include "core/GunnsBasicConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test Example Derived Network Spotter Configuration Data
////////////////////////////////////////////////////////////////////////////////////////////////////
class ExampleDerivedSpotterConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        int mPreStepCounter;                      /**< (--) initial counter to support unit tests */
        ExampleDerivedSpotterConfigData(const std::string& name,
                                        const int          preStepCounter = 0);
        virtual ~ExampleDerivedSpotterConfigData() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test Example Derived Network Spotter Input Data
////////////////////////////////////////////////////////////////////////////////////////////////////
class ExampleDerivedSpotterInputData : public GunnsNetworkSpotterInputData
{
    public:
        int mPostStepCounter;                     /**< (--) initial counter to support unit tests */
        ExampleDerivedSpotterInputData(const int postStepCounter = 0);
        virtual ~ExampleDerivedSpotterInputData() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test Example Derived Network Spotter Class.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ExampleDerivedSpotter : public GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(ExampleDerivedSpotter);
    public:
        friend class UtGunnsNetworkSpotter;
        ExampleDerivedSpotter(GunnsBasicConductor& linkRef);
        virtual     ~ExampleDerivedSpotter() {;}
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        virtual void stepPreSolver(const double dt);
        virtual void stepPostSolver(const double dt);

    protected:
        GunnsBasicConductor& mLinkRef;              /**< (--) example reference to a network link */
        int                  mPreStepCounter;       /**< (--) counter to support unit tests */
        int                  mPostStepCounter;      /**< (--) counter to support unit tests */
        const ExampleDerivedSpotterConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        const ExampleDerivedSpotterInputData*  validateInput (const GunnsNetworkSpotterInputData* input);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from ExampleDerivedSpotter and befriend UtGunnsNetworkSpotter.
///
/// @details  Class derived from the unit under test.  It has a constructor with the same arguments
///           as the parent and a default destructor, but it befriends the unit test case driver
///           class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
//class FriendlyExampleDerivedSpotter : public ExampleDerivedSpotter
//{
//    public:
//        FriendlyExampleDerivedSpotter(GunnsBasicConductor& linkRef) : ExampleDerivedSpotter(linkRef) {};
//        virtual ~FriendlyExampleDerivedSpotter() {;}
//        friend class UtGunnsNetworkSpotter;
//};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterConfigData.
///
/// @details  Derives from GunnsNetworkSpotterConfigData and is used to test that a dynamic_cast of
///           this type to the ExampleDerivedSpotterConfigData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadSpotterConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        BadSpotterConfigData(const std::string& name) : GunnsNetworkSpotterConfigData(name) {}
        virtual ~BadSpotterConfigData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterInputData.
///
/// @details  Derives from GunnsNetworkSpotterInputData and is used to test that a dynamic_cast of
///           this type to the ExampleDerivedSpotterInputData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadSpotterInputData : public GunnsNetworkSpotterInputData
{
    public:
        BadSpotterInputData() {}
        virtual ~BadSpotterInputData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Network Spotter Unit Tests.
///
/// @details  This class provides the unit tests for the GunnsNetworkSpotter class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsNetworkSpotter : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GunnsNetworkSpotter unit test.
        UtGunnsNetworkSpotter();
        /// @brief    Default destructs this GunnsNetworkSpotter unit test.
        virtual ~UtGunnsNetworkSpotter();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests the config data class.
        void testConfig();
        /// @brief    Tests the input data class.
        void testInput();
        /// @brief    Tests default constructors.
        void testDefaultConstruction();
        /// @brief    Tests initialization.
        void testInitialize();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests the stepPreSolver method.
        void testPreSolver();
        /// @brief    Tests the stepPostSolver method.
        void testPostSolver();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsNetworkSpotter);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testPreSolver);
        CPPUNIT_TEST(testPostSolver);
        CPPUNIT_TEST_SUITE_END();

        ExampleDerivedSpotter           tArticle;         /**< (--) Test article */
        GunnsBasicConductor             tConductor;       /**< (--) Test referenced network link */
        std::string                     tName;            /**< (--) Instance name */
        ExampleDerivedSpotterConfigData tConfig;          /**< (--) Nominal config data */
        ExampleDerivedSpotterInputData  tInput;           /**< (--) Nominal input data */

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsNetworkSpotter(const UtGunnsNetworkSpotter& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsNetworkSpotter& operator =(const UtGunnsNetworkSpotter& that);
};

///@}

#endif
