#ifndef GunnsDynEulerBase_EXISTS
#define GunnsDynEulerBase_EXISTS

/**
@file
@brief    GUNNS Dynamics Euler Angles Base declarations

@defgroup  TSM_GUNNS_DYNAMICS_EULER_BASE  GUNNS Dynamics Euler Angles Base Class
@ingroup   TSM_GUNNS_DYNAMICS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
  (GUNNS Dynamics Euler Angles Base Class.)

REFERENCE:
  (Trick's implementation in: trick_source/trick_utils/math/src/deuler_123_quat.c, etc.)

ASSUMPTIONS AND LIMITATIONS:
  ()

LIBRARY_DEPENDENCY:
  (GunnsDynEulerBase.o)

PROGRAMMERS:
  ((Jason Harvey) (CACI) (December 2016) (Initial))
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  GUNNS Dynamics Euler Angles Base
///
/// @details  This does conversions between a left-handed quaternion and Euler rotation angles, for
///           a defined Euler rotation sequence.  The sequence is defined by a derived class.
///
///           This only stores the Euler angles, and the corresponding quaternion is assumed to
///           reside external to this object.  This can compute the Euler angles from a given
///           quaternion, or update the given quaternion with the Euler angles.
///
///           This handles the "gimbal lock" or "singularity" situations when the 2nd rotation is
///           near 90 degrees.
///
///           This is an adaptation of Trick's implementation, modified for GUNNS architecture.
///
///           TODO Currently, only quaternions are supported and we don't have conversions for
///           transformation matrices.
///
///           TODO Currently, only "aerodynamic" Euler sequences (e.g. XYZ) are supported, and we
///           haven't yet implemented the "astronomic" sequences (e.g. XYX).
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsDynEulerBase
{
    TS_MAKE_SIM_COMPATIBLE(GunnsDynEulerBase);

    public:
        double mAngles[3];   /**< (rad) Euler rotation angles in the sequence given by mSequence. */
        /// @brief Default destructor.
        virtual ~GunnsDynEulerBase();
        /// @brief Computes the Euler rotation angles from the given quaternion.
        double* update(const double* quat);
        /// @brief Updates the given quaternion with the Euler rotation angles.
        void    computeQuat(double* quat, const double* euler = 0);
        /// @brief Returns the current singularity condition indicator value.
        int     getSingularityCondition() const;

    protected:
        unsigned int        mSequence;       /**< *o (--)  trick_chkpnt_io(*o) Euler rotation sequence for display. */
        double              mPrevAngles[3];  /**<    (rad)                     Previous values of the Euler angles used in singularity avoidance logic. */
        int                 mSingularity;    /**<    (--)                      Singularity condition indicator. */
        static const double TOLSING;         /**< ** (--)  trick_chkpnt_io(**) Tolerance for near-singularity checks. */
        static const double TOLE15;          /**< ** (--)  trick_chkpnt_io(**) 1e-15 tolerance. */
        /// @brief Default constructor.
        GunnsDynEulerBase(const unsigned int sequence);
        /// @brief Computes the quaternion elements.
        virtual void   computeQuatSeq(double* quat,
                                      const double s1, const double c1,
                                      const double s2, const double c2,
                                      const double s3, const double c3) = 0;
        /// @brief Computes the sin of the 2nd rotation.
        virtual double computeRot2Sin(const double* quat) = 0;
        /// @brief Computes Euler angles away from a singularity condition.
        virtual void   computeAnglesNoSing(const double* quat, const double rot2sin) = 0;
        /// @brief Computes Euler angles near the positive singularity condition.
        virtual void   computeAnglesPosSing(const double* quat) = 0;
        /// @brief Computes Euler angles near the negative singularity condition.
        virtual void   computeAnglesNegSing(const double* quat) = 0;

    private:
        /// @brief Copy constructor is unavailable since private and not implemented.
        GunnsDynEulerBase(const GunnsDynEulerBase& that);
        /// @brief Assignment operator is unavailable since private and not implemented.
        GunnsDynEulerBase& operator =(const GunnsDynEulerBase& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int (--) The current singularity condition indicator value.
///
/// @details  Returns the current singularity condition indicator value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsDynEulerBase::getSingularityCondition() const
{
    return mSingularity;
}

#endif
