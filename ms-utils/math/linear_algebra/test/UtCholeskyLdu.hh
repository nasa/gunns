#ifndef UtCholeskyLdu_EXISTS
#define UtCholeskyLdu_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_UTILITIES_MATH_LINEAR_ALGEBRA_CHOLESKY_LDU Cholesky LDU Decomposition Unit Tests
/// @ingroup  UT_UTILITIES_MATH_LINEAR_ALGEBRA
///
/// @details  Unit Tests for the CholeskyLdu class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "math/linear_algebra/CholeskyLdu.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Utilities unit tests.
////
/// @details  This class provides the unit tests for the CholeskyLdu class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtCholeskyLdu : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Utilities unit test.
        UtCholeskyLdu();
        /// @brief    Default destructs this Utilities unit test.
        virtual ~UtCholeskyLdu();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests [A]{x} = {b} using decomposition.
        void testNormalSolution();
        /// @brief    Tests an unconditioned matrix.
        void testUnConditioned();
        /// @brief    Tests a singular matrix.
        void testSingular();
        /// @brief    Tests for handled underflow during decomposition.
        void testDecompUnderflow();
        /// @brief    Tests [A] * inverse[A] = [I].
        void testInvert();
        /// @brief    Tests [A]{x} = {b} using decomposition by island vectors.
        void testDecomposeVector();
        /// @brief    Tests [A]{x} = {b} using decomposition for [A] having positive off-diagonals.
        void testPosOffDiagSolution();
    private:
        CholeskyLdu    tArticle;                /**< (--) Unit under test. */
        CPPUNIT_TEST_SUITE(UtCholeskyLdu);
        CPPUNIT_TEST(testNormalSolution);
        CPPUNIT_TEST(testUnConditioned);
        CPPUNIT_TEST(testSingular);
        CPPUNIT_TEST(testDecompUnderflow);
        CPPUNIT_TEST(testInvert);
        CPPUNIT_TEST(testDecomposeVector);
        CPPUNIT_TEST(testPosOffDiagSolution);
        CPPUNIT_TEST_SUITE_END();

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtCholeskyLdu(const UtCholeskyLdu& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtCholeskyLdu& operator =(const UtCholeskyLdu& that);
};

///@}

#endif
