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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Optimization Monte Carlo Input Variable
///
/// @details  Describes a Monte Carlo input variable, a variable that is to be optimized.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsOptimMonteCarloInput
{
    public:
        std::string mName;    /**< (1) Model variable name. */
        double*     mAddress; /**< (1) Model variable address. */
        double      mMinimum; /**< (1) Minimum state value. */
        double      mMaximum; /**< (1) Maximum state value. */
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
