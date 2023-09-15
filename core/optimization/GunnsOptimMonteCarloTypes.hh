#ifndef GunnsOptimMonteCarloTypes_EXISTS
#define GunnsOptimMonteCarloTypes_EXISTS

/**
@file     GunnsOptimMonteCarloTypes.hh
@brief    GUNNS Optimization Monte Carlo Interface Structures declarations

@defgroup  TSM_GUNNS_CORE_OPTIM_MC_TYPES    GUNNS Optimization Monte Carlo Interface Structures
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Refer to class details below.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ()

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2023-05) (Initial))

@{
*/

#include <string>
#include <vector>

// TsApproximation variants included here to ensure that Trick will SWIG & ICG wrap them so they
// can be created in the input file.  These are used for the optional constraints on the MC input
// variables below.
#include "math/approximation/CubicFit.hh"
#include "math/approximation/ExponentialFit.hh"
#include "math/approximation/InvLinearFit.hh"
#include "math/approximation/InvQuadraticFit.hh"
#include "math/approximation/LinearFit.hh"
#include "math/approximation/PowerFit.hh"
#include "math/approximation/ProductFit.hh"
#include "math/approximation/QuadLinFit.hh"
#include "math/approximation/QuadLinInvFit.hh"
#include "math/approximation/QuadraticFit.hh"
#include "math/approximation/QuadraticRootFit.hh"
#include "math/approximation/QuarticFit.hh"
#include "math/approximation/QuinticFit.hh"
#include "math/approximation/QuotientFit.hh"
#include "math/approximation/RationalFit.hh"
#include "math/approximation/ShowmateFit.hh"
#include "math/approximation/SutherlandFit.hh"
#include "math/approximation/TsBilinearInterpolator.hh"
#include "math/approximation/TsLinearInterpolator.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Optimization Monte Carlo Constraint
///
/// @details  Describes a constraint of one MC variable on another as the address of the independent
///           variables and the function that they input to.  The output of this function is the
///           constraint on the dependent MC variable.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsOptimMonteCarloConstraint
{
    public:
        const double*    mAddressX; /**< (1) The variable that is the 'X' input to the function. */
        const double*    mAddressY; /**< (1) The optional variable that is the 'Y' input to the function. */
        TsApproximation* mFunction; /**< (1) The function describing the constraint Z = f(X, Y). */
        /// @param[in] function (--) The constraint function object.
        /// @param[in] addressX (--) The X independent variable into the constraint function.
        /// @param[in] addressY (--) The optional Y independent variable into the constraint function.
        /// @brief  Custom constructor for this constraint.
        GunnsOptimMonteCarloConstraint(TsApproximation* function, const double* addressX, const double* addressY = 0)
            : mAddressX(addressX), mAddressY(addressY), mFunction(function) {;}
        /// @returns double (--) The result of the constraint function for the current input values.
        /// @brief  Computes and returns the constraint function Z = f(X, Y);
        double evaluate() {
            if (mAddressY) {
                return mFunction->get(*mAddressX, *mAddressY);
            }
            return mFunction->get(*mAddressX);
        }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Optimization Monte Carlo Input Variable
///
/// @details  Describes a Monte Carlo input variable, a variable that is to be optimized.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsOptimMonteCarloInput
{
    public:
        std::string                                  mName;        /**< (1) Model variable name. */
        double*                                      mAddress;     /**< (1) Model variable address. */
        double                                       mMinimum;     /**< (1) Minimum state value. */
        double                                       mMaximum;     /**< (1) Maximum state value. */
        std::vector<GunnsOptimMonteCarloConstraint*> mConstraints; /**< (1) Optional constraints on this variable relative to others. */
        /// @param[out] state (--) Model variable to be updated with the constraint value.
        /// @brief  Sets the given variable equal to the sum of the constraint functions.
        /// @details  If there are no constraints, this doesn't change the state argument.
        void applyConstraints(double* state = 0) const {
            double output = 0.0;
            for (unsigned int i=0; i<mConstraints.size(); ++i) {
                output += mConstraints.at(i)->evaluate();
                if (state) *state = output;
            }
        }
        /// @param[in] constraint (--) The constraint object to apply.
        /// @brief  Adds the given constraint object to the list of constraints applied to this MC input.
        void addNewConstraint(GunnsOptimMonteCarloConstraint* constraint) {
            mConstraints.push_back(constraint);
        }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Optimization Monte Carlo Output Variable
///
/// @details  Describes a Monte Carlo output variable, a model output variable and its target scalar
///           or trajectory that are to be compared in the optimization cost function.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsOptimMonteCarloOutput
{
    public:
        std::string         mName;           /**< (1) Model variable name. */
        double*             mAddress;        /**< (1) Pointer to the model output variable. */
        double              mCost;           /**< (1) Output value from Slave run. */
        bool                mIsScalarTarget; /**< (1) True if the target value is a scalar and not a trajectory. */
        double              mTargetScalar;   /**< (1) The scalar target value to achieve. */
        std::vector<double> mTargetTraj;     /**< (1) Target trajectory values to achieve. */
        double              mCostWeight;     /**< (1) Weight for the cost function. */
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Optimization Monte Carlo Driver Variable
///
/// @details  Describes a Monte Carlo driver variable, a model input variable with its trajectory
///           values that will drive the model along the desired path.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsOptimMonteCarloDriver
{
    public:
        double*             mAddress;    /**< (1) Pointer to the model driver variable. */
        std::vector<double> mTrajectory; /**< (1) Trajectory of values for the model driver. */
};

#endif
