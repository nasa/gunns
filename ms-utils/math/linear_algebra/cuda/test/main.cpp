/**
@defgroup  UT_UTILITIES_MATH_LINEAR_ALGEBRA_CUDA CUDA Linear Algebra Utilities Unit Tests
@ingroup   UT_UTILITIES_MATH_LINEAR_ALGEBRA
@details   This is the unit test main for the ts-models CUDA linear algebra utilities classes.

@{

LIBRARY DEPENDENCY:
(
)

*/

#include <cppunit/ui/text/TestRunner.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for CUDA Linear Algebra Utilities Unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "UtCudaIfUtils.hh"
#include "UtCudaDenseDecomp.hh"
#include "UtCudaSparseSolve.hh"

int main(int argc, char** argv) {
    CppUnit::TextTestRunner runner;

    runner.addTest( UtCudaIfUtils::suite() );
    runner.addTest( UtCudaDenseDecomp::suite() );
    runner.addTest( UtCudaSparseSolve::suite() );

    runner.run();

    return 0;
}

/// @}

