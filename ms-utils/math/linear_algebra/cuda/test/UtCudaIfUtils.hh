#ifndef UtCudaIfUtils_EXISTS
#define UtCudaIfUtils_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_UTILITIES_MATH_LINEAR_ALGEBRA_CUDA_UTILS CUDA Interface Utilities Unit Tests
/// @ingroup  UT_UTILITIES_MATH_LINEAR_ALGEBRA
///
/// @details  Unit Tests for the CudaIfUtils class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "math/linear_algebra/cuda/CudaIfUtils.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    CUDA Interface Utilities unit tests.
///
/// @details  This class provides the unit tests for the CudaIfUtils class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtCudaIfUtils : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Utilities unit test.
        UtCudaIfUtils();
        /// @brief    Default destructs this Utilities unit test.
        virtual ~UtCudaIfUtils();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests handling return values from the CUDA runtime library.
        void testCudaRuntime();
        /// @brief    Tests handling return values from the CUDA cusolver library.
        void testCusolver();
        /// @brief    Tests handling return values from the CUDA cusparse library.
        void testCusparse();

    private:
        CudaIfUtils    tArticle;                /**< (--) Unit under test. */
        CPPUNIT_TEST_SUITE(UtCudaIfUtils);
        CPPUNIT_TEST(testCudaRuntime);
        CPPUNIT_TEST(testCusolver);
        CPPUNIT_TEST(testCusparse);
        CPPUNIT_TEST_SUITE_END();
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtCudaIfUtils(const UtCudaIfUtils& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtCudaIfUtils& operator =(const UtCudaIfUtils& that);
};

///@}

#endif
