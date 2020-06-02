/***************************************** TRICK HEADER ********************************************
 PURPOSE:
     (Implements Brent's method for finding zeroes of a function.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     ()

LIBRARY DEPENDENCY:
     ((software/exceptions/TsOutOfBoundsException.o)
      (software/exceptions/TsNumericalException.o))

 PROGRAMMERS:
     ((Jason Harvey) (L-3 Communications) (2012-08) (Initial Delivery))
 **************************************************************************************************/

#include <cmath>
#include <cfloat>
#include "BrentMethod.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "software/exceptions/TsNumericalException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Brent's Method object.
////////////////////////////////////////////////////////////////////////////////////////////////////
BrentMethod::BrentMethod()
{
    //nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Brent's Method object.
////////////////////////////////////////////////////////////////////////////////////////////////////
BrentMethod::~BrentMethod()
{
    //nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] x    (--) The root solution
/// @param[in]  x1   (--) Lower bound for the root
/// @param[in]  x2   (--) Upper bound for the root
/// @param[in]  tol  (--) The tolerance for convergence
///
/// @returns  int (--) The number of iterations taken.
///
/// @throws   TsOutOfBoundsException, if the root is not bracketed by x1 & x2.
/// @throws   TsNumericalException,   if method fails to converge on a root.
///
/// @details  Uses Brent's method to return a zero of a function known to lie between x1 and x2,
///           refined until its accuracy is within the given tolerance.  The function used must be
///           implemented in an override of the virtual evaluate method in this class.
///           Taken from Numerical Recipes, http://wwww.nr.com
////////////////////////////////////////////////////////////////////////////////////////////////////
int BrentMethod::solve(double&      x,
                       const double x1,
                       const double x2,
                       const double tol)
{
    const int MAXIT = 100;     // Maximum allowed number of iterations.

    double fa = evaluate(x1);
    double fb = evaluate(x2);

    if ((fa > 0.0 and fb > 0.0) or (fa < 0.0 and fb < 0.0)) {
        /// - Throw an exception if the root is not bracketed.
        throw TsOutOfBoundsException("Input argument out of bounds", "BrentMethod::solve",
                                     "Zero of function not bracketed by x1 & x2.");
    }

    double a  = x1;
    double b  = x2;
    double c  = x2;
    double d  = 0.0;
    double e  = 0.0;
    double p  = 0.0;
    double q  = 0.0;
    double r  = 0.0;
    double s  = 0.0;
    double fc = fb;
    double tol1 = 0.0;
    double xm   = 0.0;
    double min1 = 0.0;
    double min2 = 0.0;

    // Main iteration loop.
    for (int iter=1; iter <= MAXIT; ++iter) {
        if ((fb > 0.0 and fc > 0.0) or (fb < 0.0 and fc < 0.0)) {
            c  = a;            // Rename a, b, c and adjust bounding interval d.
            fc = fa;
            d  = b - a;
            e  = d;
        }
        if (fabs(fc) < fabs(fb)) {
            a  = b;
            b  = c;
            c  = a;
            fa = fb;
            fb = fc;
            fc = fa;
        }

        // Convergence check.
        tol1 = 2.0 * DBL_EPSILON * fabs(b) + 0.5 * tol;
        xm = 0.5 * (c - b);
        if (fabs(xm) <= tol1 or 0.0 == fb) {
            x = b;
            return iter;
        }

        // Attempt inverse quadratic interpolation.
        if (fabs(e) >= tol1 and fabs(fa) > fabs(fb)) {
            s = fb / fa;
            if (a == c) {
                p = 2.0 * xm * s;
                q = 1.0 - s;
            } else {
                q = fa / fc;
                r = fb / fc;
                p = s * (2.0 * xm * q * (q-r) - (b-a)*(r-1.0));
                q = (q-1.0) * (r-1.0) * (s-1.0);
            }

            // Check whether in bounds.
            if (p > 0.0) {
                q = -q;
            }
            p = fabs(p);
            min1 = 3.0 * xm * q - fabs(tol1 * q);
            min2 = fabs(e * q);

            if (2.0 * p < (min1 < min2 ? min1 : min2)) {
                e = d;         // Accept interpolation.
                d = p / q;
            } else {
                d = xm;        // Interpolation failed, use bisection.
                e = d;
            }
        } else {               // Bounds decreasing too slowly, use bisection.
            d = xm;
            e = d;
        }
        a  = b;                // Move last best guess to a.
        fa = fb;
        if (fabs(d) > tol1) {  // Evaluate new trial root.
            b += d;
        } else {
            if (xm >= 0.0) {   // b += SIGN(tol1, xm)
                b += tol1;
            } else {
                b -= tol1;
            }
        }
        fb = evaluate(b);
    }

    /// - Throw an exception if maximum iterations were exceeded and no zero found.
    throw TsNumericalException("Maximum iterations exceeded", "BrentMethod::solve",
                               "Could not find a zero of the function.");
}
