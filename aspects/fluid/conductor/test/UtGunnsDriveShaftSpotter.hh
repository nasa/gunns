#ifndef UtGunnsDriveShaftSpotter_EXISTS
#define UtGunnsDriveShaftSpotter_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_DRIVE_SHAFT_SPOTTER    Gunns Drive Shaft Unit Test
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Drive Shaft class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsDriveShaftSpotter.hh"
#include "aspects/fluid/conductor/GunnsGasTurbine.hh"
#include "aspects/fluid/potential/GunnsGasFan.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsDriveShaftSpotter and befriend UtGunnsDriveShaftSpotter.
///
/// @details  Class derived from the unit under test.  It has a constructor with the same arguments
///           as the parent and a default destructor, but it befriends the unit test case driver
///           class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsDriveShaftSpotter : public GunnsDriveShaftSpotter
{
    public:
        FriendlyGunnsDriveShaftSpotter() : GunnsDriveShaftSpotter() {};
        virtual ~FriendlyGunnsDriveShaftSpotter() {;}
        friend class UtGunnsDriveShaftSpotter;
};

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
        BadSpotterInputData() : GunnsNetworkSpotterInputData(){}
        virtual ~BadSpotterInputData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsGasFan and befriend UtGunnsDriveShaftSpotter.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsGasFan2 : public GunnsGasFan
{
    public:
        FriendlyGunnsGasFan2();
        virtual ~FriendlyGunnsGasFan2();
        friend class UtGunnsDriveShaftSpotter;
};
inline FriendlyGunnsGasFan2::FriendlyGunnsGasFan2() : GunnsGasFan() {};
inline FriendlyGunnsGasFan2::~FriendlyGunnsGasFan2() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsGasTurbine and befriend UtGunnsDriveShaftSpotter.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsGasTurbine2 : public GunnsGasTurbine
{
    public:
        FriendlyGunnsGasTurbine2();
        virtual ~FriendlyGunnsGasTurbine2();
        friend class UtGunnsDriveShaftSpotter;
};
inline FriendlyGunnsGasTurbine2::FriendlyGunnsGasTurbine2() : GunnsGasTurbine() {};
inline FriendlyGunnsGasTurbine2::~FriendlyGunnsGasTurbine2() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Drive Shaft Unit Tests.
///
/// @details  This class provides the unit tests for the GunnsDriveShaftSpotter class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsDriveShaftSpotter : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GunnsDriveShaftSpotter unit test.
        UtGunnsDriveShaftSpotter();
        /// @brief    Default destructs this GunnsDriveShaftSpotter unit test.
        virtual ~UtGunnsDriveShaftSpotter();
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
        /// @brief    Tests the stepPreSolver method.
        void testPreSolver();
        /// @brief    Tests the stepPostSolver method.
        void testPostSolver();
        /// @brief    Tests the addReference method.
        void testAddReference();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsDriveShaftSpotter);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testAddReference);
        CPPUNIT_TEST(testPreSolver);
        CPPUNIT_TEST(testPostSolver);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsDriveShaftSpotter   tArticle;           /**< (--)          Test article */
        std::string                      tName;              /**< (--)          Instance name */
        GunnsDriveShaftSpotterConfigData tConfig;            /**< (--)          Nominal config data */
        GunnsDriveShaftSpotterInputData  tInput;             /**< (--)          Nominal input data */
        std::vector<GunnsGasTurbine*>    tTurbRef;           /**< (--)          vector of references to turbines      */
        std::vector<GunnsGasFan*>        tFanRef;            /**< (--)          vector of references to fans          */
        FriendlyGunnsGasTurbine2         tTurbine;           /**< (--)          Turbine attached to drive shaft       */
        FriendlyGunnsGasFan2             tFan;               /**< (--)          Fan attached to drive shaft           */
        double                           tFrictionConstant;  /**< (N*m*min/rev) Dynamic friction torque constant      */
        double                           tFrictionMinSpeed;  /**< (rev/min)     Minimum speed for dynamic friction    */
        double                           tInertia;           /**< (kg*m2)       Inertia of the motor+load mass        */
        double                           tMotorSpeed;        /**< (rev/min)     Motor speed                           */
        double                           tFrictionTorque;    /**< (N*m)         Dynamic friction torque               */
        double                           tTotalExternalLoad; /**< (N*m)         Total external torque load on shaft   */
        static int                       TEST_ID;            /**< (--)          Test identification number. */

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsDriveShaftSpotter(const UtGunnsDriveShaftSpotter& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsDriveShaftSpotter& operator =(const UtGunnsDriveShaftSpotter& that);
};

///@}

#endif
