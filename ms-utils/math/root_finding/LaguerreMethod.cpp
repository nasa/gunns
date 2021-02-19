/***************************************** TRICK HEADER ********************************************
 PURPOSE:
     (Implements Laguerre's method for finding zeroes of polynomial functions.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     ()

LIBRARY DEPENDENCY:
     ((software/exceptions/TsNumericalException.o))

 PROGRAMMERS:
     ((Jason Harvey) (L-3 Communications) (2012-08) (Initial Delivery))
 **************************************************************************************************/

#include <cfloat>
#include <complex>
#include "LaguerreMethod.hh"
#include "software/exceptions/TsNumericalException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] x      (--) Given root to improve
/// @param[in]     coeffs (--) Array of complex coefficients of the polynomial
/// @param[in]     size   (--) Number of coefficients, equal to polynomial order + 1
///
/// @returns  int (--) The number of iterations taken
///
/// @throws   TsNumericalException, when no zero can be found.
///
/// @details  This method uses Laguerre's method to improve a given estimate of a complex root of
///           the given complex coefficients of a polynomial of given order.
///           Taken from Numerical Recipes, http://wwww.nr.com
////////////////////////////////////////////////////////////////////////////////////////////////////
int LaguerreMethod::solve(std::complex<double>& x, std::complex<double>* coeffs, int size)
{
    const int MR = 8;           // Maximum limit cycles
    const int MT = 10;          // Maximum iterations per limit cycle
    const int MAXIT = MR * MT;  // Total maximum allowed iterations

    static const double frac[MR+1] = {0.0, 0.5, 0.25, 0.75, 0.13, 0.38, 0.62, 0.88, 1.0};

    const int m = size - 1;

    // Loop over iterations up to allowed maximum.
    for (int iter=1; iter<=MAXIT; ++iter) {
        std::complex<double> f = 0.0;
        std::complex<double> d = 0.0;
        std::complex<double> b = coeffs[m];
        double err = std::abs(b);
        double abx = std::abs(x);

        // Efficient computation of the polynomial and its first two derivatives.  f stores P''/2.
        for (int j=m-1; j>=0; --j) {
            f   = x * f + d;
            d   = x * d + b;
            b   = x * b + coeffs[j];
            err = std::abs(b) + abx * err;
        }
        err *= DBL_EPSILON;

        // Estimate of roundoff error in evaluating polynomial.
        if (std::abs(b) <= err) {
            return(iter);    // we are on the root
        }

        // Use Laguerre's formula to improve the root.
        std::complex<double> g  = d / b;
        std::complex<double> g2 = g*g;
        std::complex<double> h  = g2 - 2.0 * f / b;
        std::complex<double> sq = sqrt(double(m-1) * (double(m)*h - g2));
        std::complex<double> gp = g + sq;
        std::complex<double> gm = g - sq;
        double abp = std::abs(gp);
        double abm = std::abs(gm);
        if (abp < abm) {
            gp = gm;
        }
        std::complex<double> dx;
        if (std::max(abp, abm) > 0.0) {
            dx =  double(m) / gp;
        } else {
            dx = std::polar(1.0 + abx, double(iter));
        }
        std::complex<double> x1 = x - dx;
        if (x == x1) {
            return (iter);  // converged
        }

        // Every so often we take a fractional step to break any limit cycle (a rare occurence).
        if (iter % MT != 0) {
            x = x1;
        } else {
            x -= frac[iter/MT] * dx;
        }
    }

    /// - Throw an exception if maximum iterations were exceeded and no zero found.
    throw TsNumericalException("Maximum iterations exceeded", "LaguerreMethod::solve",
                               "Could not find a zero of the polynomial.");
}
