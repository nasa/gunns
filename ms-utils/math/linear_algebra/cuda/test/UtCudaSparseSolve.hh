#ifndef UtCudaSparseSolve_EXISTS
#define UtCudaSparseSolve_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_UTILITIES_MATH_LINEAR_ALGEBRA_CUDA_DENSE CUDA Sparse System Solver Unit Tests
/// @ingroup  UT_UTILITIES_MATH_LINEAR_ALGEBRA
///
/// @details  Unit Tests for the CudaSparseSolve class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "math/linear_algebra/cuda/CudaSparseSolve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    CUDA Sparse System Solver unit tests.
////
/// @details  This class provides the unit tests for the CudaSparseSolve class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtCudaSparseSolve : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Utilities unit test.
        UtCudaSparseSolve();
        /// @brief    Default destructs this Utilities unit test.
        virtual ~UtCudaSparseSolve();
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
        /// @brief    Tests [A]{x} = {b} using decomposition for [A] having positive off-diagonals.
        void testPosOffDiagSolution();
        /// @brief    Tests dynamic resizing of the matrix in run-time.
        void testResizing();

    private:
        CudaSparseSolve    tArticle;                /**< (--) Unit under test. */
        CPPUNIT_TEST_SUITE(UtCudaSparseSolve);
        CPPUNIT_TEST(testNormalSolution);
        CPPUNIT_TEST(testUnConditioned);
        CPPUNIT_TEST(testSingular);
        CPPUNIT_TEST(testDecompUnderflow);
        CPPUNIT_TEST(testPosOffDiagSolution);
        CPPUNIT_TEST(testResizing);
        CPPUNIT_TEST_SUITE_END();
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtCudaSparseSolve(const UtCudaSparseSolve& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtCudaSparseSolve& operator =(const UtCudaSparseSolve& that);
};

///@}

#endif
