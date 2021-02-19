#ifndef UtCombust_EXISTS
#define UtCombust_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_PROPERTIES_COMBUST  Combust Unit Test
/// @ingroup  UT_TSM_PROPERTIES
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Combust model base and derived classes.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "properties/CombustCH4.hh"
#include "properties/CombustFactory.hh"
#include "properties/FluidProperties.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from CombustCH4 and befriend UtCombust.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyCombustCH4 : public CombustCH4
{
    public:
        FriendlyCombustCH4();
        virtual ~FriendlyCombustCH4();
        friend class UtCombust;
};
inline FriendlyCombustCH4::FriendlyCombustCH4() : CombustCH4() {};
inline FriendlyCombustCH4::~FriendlyCombustCH4() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   Combust and CombustCH4 unit tests.
////
/// @details  This class provides the unit tests for the Combustion model within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtCombust: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this combustion model unit test.
        UtCombust();
        /// @brief    Default destructs this combustion model unit test.
        ~UtCombust();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Test that the default values are set correctly.
        void testDefault();
        /// @brief    Test the entire simulation inlet conditions envelope.
        void testFull();
        /// @brief    Test all of the functions that return data to user.
        void testGet();
        /// @brief    Test all of the functions that allow users to set quantities.
        void testSet();
        /// @brief    Test all possible initialization errors due to non-physical inputs.
        void testBadInit();
        /// @brief    Test initialization errors due to unacceptable fluid config data.
        void testBadFluid();
        /// @brief    Test combustion factory.
        void testFactory();
        /// @brief    Generate solutions for a variety of inlet condition, to be compared against validation model
        void validate();
        /// @brief    Tests update method with no inlet mole fractions (uses last pass mole fractions)
        void testRecombine();

    private:
        CPPUNIT_TEST_SUITE(UtCombust);
        CPPUNIT_TEST(testDefault);
        CPPUNIT_TEST(testFull);
        CPPUNIT_TEST(testGet);
        CPPUNIT_TEST(testSet);
        CPPUNIT_TEST(testBadInit);
        CPPUNIT_TEST(testBadFluid);
        CPPUNIT_TEST(testFactory);
        CPPUNIT_TEST(testRecombine);
        //CPPUNIT_TEST(validate);
        CPPUNIT_TEST_SUITE_END();
        double       tNStep;                /**< (--)  Number of points used in testFull method. */
        const double tTempMin;              /**< (K)   Minimum possible inlet temperature. */
        const double tTempMax;              /**< (K)   Maximum possible inlet temperature. */
        const double tPressMin;             /**< (kPa) Minimum expected chamber pressure. */
        const double tPressMax;             /**< (kPa) Maximum expected chamber pressure. */
        const double tHeMin;                /**< (--)  Minimum possible inlet concentration of Helium. */
        const double tHeMax;                /**< (--)  Maximum expected inlet concentration of Helium. */
        double       tCH4Min;               /**< (--)  Minimum possible inlet concentration of Methane. */
        double       tCH4Max;               /**< (--)  Maximum possible inlet concentration of Methane. */
        double       tTolerance;            /**< (--)  Nominal tolerance for comparison of expected and returned values. */
        static int   TEST_ID;               /**< (--)  Test identification number. */
        static const int tNFluidTypes = 3;  /**< (--)  Number of fluid types in model. */
        FriendlyCombustCH4* tArticle;       /**< (--)  Pointer to test article. */
        double       tTempIn;               /**< (K)   Inlet/combustion temperature. */
        double       tTempGuess;            /**< (K)   Guess for recombination exit temperature. */
        double       tPress;                /**< (kPa) Chamber pressure. */
        double       tGamma;                /**< (--)  Combustion ratio of specific heats. */
        double       tMW;                   /**< (1/mol) Combustion molecular weight. */
        double       tMoles[tNFluidTypes];  /**< (--)  Array of inlet mole fractions. */
        FluidProperties::FluidType tTypes[tNFluidTypes]; /**< (--) Array of model fluid types. */

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtCombust(const UtCombust&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtCombust& operator =(const UtCombust&);
};

/// @}

#endif
