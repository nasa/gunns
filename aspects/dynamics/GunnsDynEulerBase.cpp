/**
@file
@brief    GUNNS Dynamics Euler Angles Base implementation

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
     (aspects/dynamics/GunnsDynUtils.o)
     (software/exceptions/TsNumericalException.o)
    )
*/

#include "GunnsDynEulerBase.hh"
#include "aspects/dynamics/GunnsDynUtils.hh"
#include "software/exceptions/TsNumericalException.hh"
#include "math/UnitConversion.hh"
#include "math/MsMath.hh"

/// @details  This is derived from the 1.0e-6 value used by the Trick implementation in
///           trick_source/trick_utils/math/deuler_123_quat.c but we build in a few more operations
///           into this constant to make the run-time code more efficient.
const double GunnsDynEulerBase::TOLSING = std::sin(UnitConversion::PI_OVER_2 - 1.0e-6);

/// @details  This tolerance is used in various places in the update method's singularity avoidance
///           logic.  This value comes from the Trick implementation in
///           trick_source/trick_utils/math/deuler_123_quat.c.
const double GunnsDynEulerBase::TOLE15  = 1.0e-15;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] sequence (--) The axis sequence (123, etc) of Euler rotations.
///
/// @details  Default constructs this GUNNS Dynamics Euler Angles Base.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynEulerBase::GunnsDynEulerBase(const unsigned int sequence)
    :
    mAngles(),
    mSequence(sequence),
    mPrevAngles(),
    mSingularity(0)
{
    GunnsDynUtils::zeroV(mAngles,     3);
    GunnsDynUtils::zeroV(mPrevAngles, 3);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Dynamics Euler Angles Base.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynEulerBase::~GunnsDynEulerBase()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] quat  (--) Pointer to the quaternion to be updated.
/// @param[in] euler (--) Optional pointer to new Euler angles to use and store.
///
/// @details  Updates the currently stored Euler angles if new angles are provided, then updates the
///           given quaternion from the currently stored angles.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynEulerBase::computeQuat(double* quat, const double* euler)
{
    /// - Store the optional input euler angles if provided.
    if (euler) {
        GunnsDynUtils::setV(mAngles,     euler, 3);
        GunnsDynUtils::setV(mPrevAngles, euler, 3);
    }

    /// - Compute sines and cosines of Euler half-angles.
    double half_angles[3];
    GunnsDynUtils::scaleV(half_angles, mAngles, 0.5, 3);

    const double s1 = std::sin(half_angles[0]);
    const double c1 = std::cos(half_angles[0]);
    const double s2 = std::sin(half_angles[1]);
    const double c2 = std::cos(half_angles[1]);
    const double s3 = std::sin(half_angles[2]);
    const double c3 = std::cos(half_angles[2]);

    computeQuatSeq(quat, s1, c1, s2, c2, s3, c3);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] quat (--) Pointer to the quaternion to compute from.
///
/// @throws   TsNumericalException
///
/// @details  This updates the Euler angles from the given quaternion.  This method is best used
///           when the previous angle values are not up-to-date.  This throws an exception and sets
///           the Euler angles to nan for a bad quaternion at a singularity condition.
////////////////////////////////////////////////////////////////////////////////////////////////////
double* GunnsDynEulerBase::update(const double* quat)
{
    const double sinRot2 = computeRot2Sin(quat);

    if (MsMath::isInRange(-1.0, sinRot2, 1.0)) {
        // Within range for asin function.
        if (std::fabs(sinRot2) < TOLSING) {
            mSingularity = 0;
            computeAnglesNoSing(quat, sinRot2);            // away from singularity
        } else if (sinRot2 > 0.0) {
            mSingularity = 1;
            computeAnglesPosSing(quat);                    // near +pi/2 singularity
        } else {
            mSingularity = 2;
            computeAnglesNegSing(quat);                    // near -pi/2 singularity
        }

    } else if (MsMath::isInRange(1.0, sinRot2, (1.0 + TOLE15))) {
        // Within acceptable tolerance outside the range for asin function.
        mSingularity = 3;
        computeAnglesPosSing(quat);                        // near +pi/2 singularity

    } else if (MsMath::isInRange((-1.0 - TOLE15), sinRot2, -1.0)) {
        // Within acceptable tolerance outside the range for asin function.
        mSingularity = 4;
        computeAnglesNegSing(quat);                        // near -pi/2 singularity

    } else {
        // Error: Out of range for asin function and beyond acceptable tolerance, set angles to nan
        // and throw exception.
        mSingularity = 5;
        const double zero = 0.0;
        mAngles[0] = mAngles[1] = mAngles[2] = 0.0 / zero;
        throw TsNumericalException();
    }

    /// - Store Euler angles for next pass
    GunnsDynUtils::setV(mPrevAngles, mAngles, 3);

    return mAngles;
}
