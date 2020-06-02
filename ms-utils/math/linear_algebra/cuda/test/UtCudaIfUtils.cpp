/************************** TRICK HEADER ***********************************************************
 LIBRARY DEPENDENCY:
    ((math/linear_algebra/cuda/CudaIfUtils.o))
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsNumericalException.hh"
#include "UtCudaIfUtils.hh"
#include <cuda_runtime.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Cholesky Ldu Decomposition unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtCudaIfUtils::UtCudaIfUtils()
    :
    tArticle()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Cholesky Ldu Decomposition unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtCudaIfUtils::~UtCudaIfUtils()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCudaIfUtils::setUp()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCudaIfUtils::tearDown()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests handling return values from the CUDA runtime library.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCudaIfUtils::testCudaRuntime()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtCudaIfUtils ......... 01: testCudaRuntime ........................";

    /// - Test success value.
    CPPUNIT_ASSERT_NO_THROW(tArticle.checkReturn(cudaSuccess, " ", 0));

    /// - Test a known error value.
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(cudaErrorInvalidValue, " ", 0), TsInitializationException);

    /// - Test dynamic allocation for code coverage.
    CudaIfUtils* article = new CudaIfUtils();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests handling return values from the CUDA cusolver library.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCudaIfUtils::testCusolver()
{
    std::cout << "\n UtCudaIfUtils ......... 02: testCusolver ...........................";

    /// - Test success value.
    CPPUNIT_ASSERT_NO_THROW(tArticle.checkReturn(CUSOLVER_STATUS_SUCCESS, " ", 0));

    /// - Test known error values.
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSOLVER_STATUS_NOT_INITIALIZED,           " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSOLVER_STATUS_ALLOC_FAILED,              " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSOLVER_STATUS_INVALID_VALUE,             " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSOLVER_STATUS_ARCH_MISMATCH,             " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSOLVER_STATUS_MAPPING_ERROR,             " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSOLVER_STATUS_EXECUTION_FAILED,          " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSOLVER_STATUS_INTERNAL_ERROR,            " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSOLVER_STATUS_MATRIX_TYPE_NOT_SUPPORTED, " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSOLVER_STATUS_NOT_SUPPORTED,             " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSOLVER_STATUS_ZERO_PIVOT,                " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSOLVER_STATUS_INVALID_LICENSE,           " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSOLVER_STATUS_NOT_INITIALIZED,           " ", 0), TsNumericalException);

    /// - Test invalid enumeration.
    cusolverStatus_t err = static_cast<cusolverStatus_t>(12345);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(err, " ", 0), TsNumericalException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests handling return values from the CUDA cusparse library.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCudaIfUtils::testCusparse()
{
    std::cout << "\n UtCudaIfUtils ......... 03: testCusparse ...........................";

    /// - Test success value.
    CPPUNIT_ASSERT_NO_THROW(tArticle.checkReturn(CUSPARSE_STATUS_SUCCESS, " ", 0));

    /// - Test known error values.
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSPARSE_STATUS_NOT_INITIALIZED,           " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSPARSE_STATUS_ALLOC_FAILED,              " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSPARSE_STATUS_INVALID_VALUE,             " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSPARSE_STATUS_ARCH_MISMATCH,             " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSPARSE_STATUS_MAPPING_ERROR,             " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSPARSE_STATUS_EXECUTION_FAILED,          " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSPARSE_STATUS_INTERNAL_ERROR,            " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSPARSE_STATUS_MATRIX_TYPE_NOT_SUPPORTED, " ", 0), TsNumericalException);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(CUSPARSE_STATUS_ZERO_PIVOT,                " ", 0), TsNumericalException);

    /// - Test invalid enumeration.
    cusparseStatus_t err = static_cast<cusparseStatus_t>(12345);
    CPPUNIT_ASSERT_THROW(tArticle.checkReturn(err, " ", 0), TsNumericalException);

    std::cout << "... Pass";
}
