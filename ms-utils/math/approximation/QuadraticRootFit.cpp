/************************** TRICK HEADER **********************************************************
 LIBRARY DEPENDENCY:
    ((TsApproximation.o))
***************************************************************************************************/

#include <cfloat>

#include "GenericMacros.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"

#include "QuadraticRootFit.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Univariate Quadratic Root curve fit model .
////////////////////////////////////////////////////////////////////////////////////////////////////
QuadraticRootFit::QuadraticRootFit()
    :
    TsApproximation(),
    mA(0.0),
    mB(0.0),
    mC(0.0),
    mD(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    b     (--) Second coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @throws   TsInitializationException
///
/// @note     The arguments are the coefficients of the corresponding quadratic equation.
///
/// @details  Constructs this Univariate Quadratic Root curve fit model taking coefficient and
///           range arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuadraticRootFit::QuadraticRootFit(const double a,    const double b,
                                   const double minX, const double maxX,
                                   const std::string &name)
    :
    TsApproximation(minX, maxX, -FLT_EPSILON, +FLT_EPSILON),
    mA(-a),
    mB(a * a),
    mC(4.0 * b),
    mD(0.0)
{
    init(a, b, minX, maxX, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Univariate Quadratic Root curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuadraticRootFit::~QuadraticRootFit()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    b     (--) Second coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @details  Initializes this QuadraticRoot curve fit model taking coefficient, range and
///           name arguments
////////////////////////////////////////////////////////////////////////////////////////////////////
void QuadraticRootFit::init(const double a,    const double b,
                            const double minX, const double maxX,
                            const std::string &name)
{
    double flt_epsilon = static_cast<double>(FLT_EPSILON);
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, -flt_epsilon, +flt_epsilon, name);

    /// - Reset the initialization complete flag.
    mInitFlag = false;

    /// - Throw a TsInitializationException exception on a singularity (divide by 0) in the allegedly valid range.
    TS_GENERIC_IF_ERREX((fabs(b) < DBL_EPSILON), TsInitializationException,
                        "Invalid Input Argument", "Singularity (divide by 0) in the allegedly valid range.");
    TS_GENERIC_IF_ERREX((minX + flt_epsilon < -mB / mC && -mB / mC < maxX - flt_epsilon), TsInitializationException,
                        "Invalid Input Argument", "Singularity (divide by 0) in the allegedly valid range.");

    /// - Initialize the coefficients with validated values.
    mA = -a;
    mB = a * a;
    mC = 4.0 * b;
    mD = 0.5 / b;

    /// - Set the initialization complete flag.
    mInitFlag = true;
}
