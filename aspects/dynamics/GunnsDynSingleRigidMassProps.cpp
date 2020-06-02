/**
@file
@brief    GUNNS Dynamics Single Rigid-Body EOM Mass Properties implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
     (software/exceptions/TsOutOfBoundsException.o)
    )
*/

#include "GunnsDynSingleRigidMassProps.hh"
#include "aspects/dynamics/GunnsDynUtils.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include <cfloat>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] mass (kg)    Body mass.
/// @param[in] cm   (m)     Pointer to center of mass location in the body frame.
/// @param[in] ixx  (kg*m2) Body X moment of inertia about the center of mass in the body frame.
/// @param[in] ixy  (kg*m2) Body XY product of inertia about the center of mass in the body frame.
/// @param[in] ixz  (kg*m2) Body XZ product of inertia about the center of mass in the body frame.
/// @param[in] iyy  (kg*m2) Body Y moment of inertia about the center of mass in the body frame.
/// @param[in] iyz  (kg*m2) Body YZ product of inertia about the center of mass in the body frame.
/// @param[in] izz  (kg*m2) Body Z moment of inertia about the center of mass in the body frame.
///
/// @details  Contructs this mass properties with optional arguments.  Note this doesn't validate
///           the constructed state.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynSingleRigidMassProps::GunnsDynSingleRigidMassProps(const double  mass,
                                                           const double* cm,
                                                           const double  ixx,
                                                           const double  ixy,
                                                           const double  ixz,
                                                           const double  iyy,
                                                           const double  iyz,
                                                           const double  izz)
    :
    mMass(mass),
    mCM(),
    mIxx(ixx),
    mIxy(ixy),
    mIxz(ixz),
    mIyy(iyy),
    mIyz(iyz),
    mIzz(izz)
{
    if (cm) {
        GunnsDynUtils::setV(mCM, cm, 3);
    } else {
        GunnsDynUtils::zeroV(mCM, 3);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this mass properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynSingleRigidMassProps::~GunnsDynSingleRigidMassProps()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Mass properties object to be copied.
///
/// @details  Copy constructs this mass properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynSingleRigidMassProps::GunnsDynSingleRigidMassProps(const GunnsDynSingleRigidMassProps& that)
    :
    mMass(that.mMass),
    mCM(),
    mIxx(that.mIxx),
    mIxy(that.mIxy),
    mIxz(that.mIxz),
    mIyy(that.mIyy),
    mIyz(that.mIyz),
    mIzz(that.mIzz)
{
    GunnsDynUtils::setV(mCM, that.mCM, 3);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Mass properties object to be set equal to.
///
/// @details  Assigns this mass properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynSingleRigidMassProps& GunnsDynSingleRigidMassProps::operator =(const GunnsDynSingleRigidMassProps& that)
{
    mMass = that.mMass;
    GunnsDynUtils::setV(mCM, that.mCM, 3);
    mIxx = that.mIxx;
    mIxy = that.mIxy;
    mIxz = that.mIxz;
    mIyy = that.mIyy;
    mIyz = that.mIyz;
    mIzz = that.mIzz;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Mass properties object to be compared to.
///
/// @returns  bool (--) True if this mass properties is identical to that, false otherwise.
///
/// @details  Returns whether this mass properties is identical to that.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsDynSingleRigidMassProps::operator ==(const GunnsDynSingleRigidMassProps& that) const
{
    return ( (mMass  == that.mMass)  and
             (mCM[0] == that.mCM[0]) and
             (mCM[1] == that.mCM[1]) and
             (mCM[2] == that.mCM[2]) and
             (mIxx   == that.mIxx)   and
             (mIxy   == that.mIxy)   and
             (mIxz   == that.mIxz)   and
             (mIyy   == that.mIyy)   and
             (mIyz   == that.mIyz)   and
             (mIzz   == that.mIzz) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsOutOfBoundsException
///
/// @details  Checks this mass properties state for valid data and throws an exception if any terms
///           are out of valid range.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynSingleRigidMassProps::validate() const
{
    /// - Throw for mass or moments of inertia < DBL_EPSILON, or products of inertia > 0.
    if ( (mMass < DBL_EPSILON) or
         (mIxx  < DBL_EPSILON) or
         (mIyy  < DBL_EPSILON) or
         (mIzz  < DBL_EPSILON) or
         (mIxy  > 0.0)         or
         (mIxz  > 0.0)         or
         (mIyz  > 0.0) ) {
        throw TsOutOfBoundsException();
    }
}
