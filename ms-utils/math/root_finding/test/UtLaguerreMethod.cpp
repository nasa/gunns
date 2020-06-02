/************************** TRICK HEADER ***********************************************************
 LIBRARY DEPENDENCY:
    ((math/root_finding/LaguerreMethod.o))
***************************************************************************************************/

#include "UtLaguerreMethod.hh"
#include "software/exceptions/TsNumericalException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Laguerre's Method unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtLaguerreMethod::UtLaguerreMethod()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Laguerre's Method unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtLaguerreMethod::~UtLaguerreMethod()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtLaguerreMethod::setUp()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtLaguerreMethod::tearDown()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Uses a 5th-order polynomial with real coefficients and a known set of real and complex
///           roots to verify the Laguerre method's ability to improve roots.  The general test
///           method is to call Laguerre with the polynomial coefficients and an initial root guess,
///           then evaluate the polynomial with the returned root and verify the result is
///           sufficiently near zero, allowing for some round-off errors.  Further tests verify the
///           Laguerre's method returns specific roots.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtLaguerreMethod::testReal()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtLaguerreMethod .. 01: testReal ...................................";

    /// - This polynomial has roots (from http://xrjunque.nom.es/precis/rootfinder.aspx):
    ///     Root 1: 1.369 + 0*i
    ///     Root 2: 0.172 - 0.297*i
    ///     Root 3: 0.172 + 0.297*i
    ///     Root 4: 1.46  - 1.034*i
    ///     Root 5: 1.46  + 1.034*i
    std::complex<double> coeffs[6];
    coeffs[0] =   1.0;
    coeffs[1] =  -4.5627;
    coeffs[2] =  14.272;
    coeffs[3] = -17.043;
    coeffs[4] =   8.9822;
    coeffs[5] =  -1.9391;

    /// @test accuracy of the returned root in points throughout a complex range in +/-10 +/-10*i,
    ///       and the number of iterations taken.
    for (double x = -10.0; x <= 10.0; x+=0.1) {
        for (double i = -10.0; i <= 10.0; i+=0.1) {
            std::complex<double> root = std::complex<double>(x, i);
            int iter = LaguerreMethod::solve(root, coeffs, 6);

            std::complex<double> actual = coeffs[0]
                                        + coeffs[1] * root
                                        + coeffs[2] * root * root
                                        + coeffs[3] * root * root * root
                                        + coeffs[4] * root * root * root * root
                                        + coeffs[5] * root * root * root * root * root;

            CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, actual.real(), 1.0E-13);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, actual.imag(), 1.0E-13);
            CPPUNIT_ASSERT(11 > iter);
        }
    }

    /// @test the specific root 1.369 + 0i is returned when initially near that root.
    std::complex<double> root1 = std::complex<double>(1.37, 0.01);
    LaguerreMethod::solve(root1, coeffs, 6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.369, root1.real(), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   root1.imag(), DBL_EPSILON);

    /// @test the specific root 0.172 - 0.297i is returned when initially near that root.
    std::complex<double> root2 = std::complex<double>(0.17, -0.3);
    LaguerreMethod::solve(root2, coeffs, 6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.172, root2.real(), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.297, root2.imag(), 0.001);

    /// @test the specific root 0.172 + 0.297i is returned when initially near that root.
    std::complex<double> root3 = std::complex<double>(0.17, 0.3);
    LaguerreMethod::solve(root3, coeffs, 6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.172, root3.real(), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.297, root3.imag(), 0.001);

    /// @test the specific root 1.46 - 1.034i is returned when initially near that root.
    std::complex<double> root4 = std::complex<double>(1.47, -1.03);
    LaguerreMethod::solve(root4, coeffs, 6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.460, root4.real(), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.034, root4.imag(), 0.001);

    /// @test the specific root 1.46 + 1.034i is returned when initially near that root.
    std::complex<double> root5 = std::complex<double>(1.47, 1.03);
    LaguerreMethod::solve(root5, coeffs, 6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.460, root5.real(), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.034, root5.imag(), 0.001);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Uses a 3rd-order polynomial with complex coefficients to verify Laguerre's Method
///           returns valid roots.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtLaguerreMethod::testComplex()
{
    std::cout << "\n UtLaguerreMethod .. 02: testComplex ................................";

    std::complex<double> coeff0 = std::complex<double>( 1.0,  -0.1);
    std::complex<double> coeff1 = std::complex<double>(-0.5,   0.2);
    std::complex<double> coeff2 = std::complex<double>( 0.1,   0.3);
    std::complex<double> coeff3 = std::complex<double>(-0.05, -0.4);
    std::complex<double> coeffs[4];
    coeffs[0] = coeff0;
    coeffs[1] = coeff1;
    coeffs[2] = coeff2;
    coeffs[3] = coeff3;

    /// @test accuracy of the returned root in points throughout a complex range in +/-10 +/-10*i,
    ///       and the number of iterations taken.
    for (double x = -1000.0; x <= 1000.0; x+=10.0) {
        for (double i = -1000.0; i <= 1000.0; i+=10.0) {
            std::complex<double> root = std::complex<double>(x, i);
            int iter = LaguerreMethod::solve(root, coeffs, 4);

            std::complex<double> actual = coeffs[0]
                                        + coeffs[1] * root
                                        + coeffs[2] * root * root
                                        + coeffs[3] * root * root * root;

            CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, actual.real(), 1.0E-13);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, actual.imag(), 1.0E-13);
            CPPUNIT_ASSERT(11 > iter);
        }
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Verifies the Laguerre's method will return with an error code if it cannot converge on
///           a root within the allowed number of iterations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtLaguerreMethod::testErrors()
{
    std::cout << "\n UtLaguerreMethod .. 03: testErrors .................................";

    /// - This polynomial has no roots!
    std::complex<double> coeffs[2];
    coeffs[0] = 1.0;
    coeffs[1] = 0.0;

    /// - Verify Laguerre throws an exception when it can't find the root.
    std::complex<double> root = 0.0;
    CPPUNIT_ASSERT_THROW(LaguerreMethod::solve(root, coeffs, 2), TsNumericalException);

    std::cout << "... Pass" << std::endl;
}
