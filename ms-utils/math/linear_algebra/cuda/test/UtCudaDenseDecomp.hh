#ifndef UtCudaDenseDecomp_EXISTS
#define UtCudaDenseDecomp_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_UTILITIES_MATH_LINEAR_ALGEBRA_CUDA_DENSE CUDA Dense Matrix Decomposition Unit Tests
/// @ingroup  UT_UTILITIES_MATH_LINEAR_ALGEBRA
///
/// @details  Unit Tests for the CudaDenseDecomp class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "math/linear_algebra/cuda/CudaDenseDecomp.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    CUDA Dense Matrix Decomposition unit tests.
////
/// @details  This class provides the unit tests for the CudaDenseDecomp class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtCudaDenseDecomp : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Utilities unit test.
        UtCudaDenseDecomp();
        /// @brief    Default destructs this Utilities unit test.
        virtual ~UtCudaDenseDecomp();
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
        CudaDenseDecomp    tArticle;                /**< (--) Unit under test. */
        CPPUNIT_TEST_SUITE(UtCudaDenseDecomp);
        CPPUNIT_TEST(testNormalSolution);
        CPPUNIT_TEST(testUnConditioned);
        CPPUNIT_TEST(testSingular);
        CPPUNIT_TEST(testDecompUnderflow);
        CPPUNIT_TEST(testPosOffDiagSolution);
        CPPUNIT_TEST(testResizing);
        CPPUNIT_TEST_SUITE_END();
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtCudaDenseDecomp(const UtCudaDenseDecomp& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtCudaDenseDecomp& operator =(const UtCudaDenseDecomp& that);
};

///@}

#endif
