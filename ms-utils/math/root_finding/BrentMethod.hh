#ifndef BrentMethod_EXISTS
#define BrentMethod_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_ROOT_FINDING_BRENT_METHOD Brent's Function Root Finding Method
@ingroup   TSM_UTILITIES_MATH_ROOT_FINDING

@details
PURPOSE:
- (Implements Brent's method for finding zeroes of a function.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((BrentMethod.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2012-08) (Initial Delivery))

@{
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Function root finding utility methods
///
/// @details  Various methods for finding roots of functions.
///
/// @note     This is an abstract base class and has a pure virtual method that must be implemented
///           in a derived class.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BrentMethod
{
    public:
        /// @brief Default constructs this Root Finding object.
        BrentMethod();

        /// @brief Default destructs this Root Finding object.
        virtual ~BrentMethod();

        /// @details  This method is pure virtual and requires an implementation of the desired
        ///           function in a derived class.
        ///
        /// @brief Virtual implementation of a univariate function to evaluate.
        virtual double evaluate(const double x) = 0;

        /// @brief Uses Brent's method to find a root of a function bounded between x1 and x2.
        int solve(double&      x,
                  const double x1,
                  const double x2,
                  const double tol);

    private:
        /// @brief    Copy constructor unavailable since declared private and not implemented.
        BrentMethod(const BrentMethod&);

        /// @brief    Assignment operator unavailable since declared private and not implemented.
        BrentMethod& operator =(const BrentMethod&);
};

/// @}

#endif
