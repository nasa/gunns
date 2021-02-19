#ifndef UtMonoFluid_EXISTS
#define UtMonoFluid_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_FLUID_MONO_FLUID     MonoFluid Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_FLUID
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the MonoFluid class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/fluid/MonoFluid.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from MonoFluid and befriend UtMonoFluid.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyMonoFluid : public MonoFluid {
    public:
        FriendlyMonoFluid();
        FriendlyMonoFluid(const FluidProperties*     properties,
                          const MonoFluidInputData&  initData);
        FriendlyMonoFluid(const FriendlyMonoFluid& that);
        virtual ~FriendlyMonoFluid();
        friend class UtMonoFluid;
};
inline FriendlyMonoFluid::FriendlyMonoFluid()
    : MonoFluid() {}
inline FriendlyMonoFluid::FriendlyMonoFluid(const FluidProperties*    properties,
                                            const MonoFluidInputData& initData)
    : MonoFluid(properties, initData) {}
inline FriendlyMonoFluid::FriendlyMonoFluid(const FriendlyMonoFluid& that)
    : MonoFluid(that) {}
inline FriendlyMonoFluid::~FriendlyMonoFluid() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    MonoFluid unit tests.
////
/// @details  This class provides the unit tests for the MonoFluid within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtMonoFluid : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this MonoFluid unit test.
        UtMonoFluid();
        /// @brief    Default destructs this MonoFluid unit test.
        virtual ~UtMonoFluid();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        void testConfigAndInput();
        void testNominalConstruction();
        void testDefaultConstruction();
        void testCopyConstruction();
        void testNominalInitialization();
        void testMultipleInit();
        void testStateAccessors();
        void testPropertyAccessors();
        void testStateModifiers();
        void testStateUpdaters();
        void testComputes();
        void testConstructionExceptions();
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtMonoFluid);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testNominalConstruction);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testCopyConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testMultipleInit);
        CPPUNIT_TEST(testStateAccessors);
        CPPUNIT_TEST(testPropertyAccessors);
        CPPUNIT_TEST(testStateModifiers);
        CPPUNIT_TEST(testStateUpdaters);
        CPPUNIT_TEST(testComputes);
        CPPUNIT_TEST(testConstructionExceptions);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        /// --     Nominal fluid type
        FluidProperties::FluidType       mType;
        /// (K)    Nominal temperature
        double                           mTemperature;
        /// (Pa)   Nominal pressure
        double                           mPressure;
        /// (kg/s) Nominal flow rate
        double                           mFlowRate;
        /// (kg)   Nominal mass
        double                           mMass;
        /// (mo)   Nominal moles (mass / molecular weight)
        double                           mMole;
        /// --     Pointer to defined fluid properties
        const DefinedFluidProperties*    mProperties;
        /// --     Pointer to nominal initialization data
        MonoFluidInputData*              mInputData;
        /// --     Pointer to the friendly Check Valve test article
        FriendlyMonoFluid*               mArticle;
        /// (s)    Nominal time step
        double                           mTimeStep;
        /// --     Nominal tolerance for comparison of expected and returned values
        double                           mTolerance;
        static int                       TEST_ID; /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtMonoFluid(const UtMonoFluid&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtMonoFluid& operator =(const UtMonoFluid&);
};

///@}

#endif
