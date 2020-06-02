#ifndef GunnsDynEuler231_EXISTS
#define GunnsDynEuler231_EXISTS

/**
@file
@brief    GUNNS Dynamics Euler Angles 231-Sequence declarations

@defgroup  TSM_GUNNS_DYNAMICS_EULER_231  GUNNS Dynamics Euler Angles 231-Sequence
@ingroup   TSM_GUNNS_DYNAMICS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
  (This class defines the conversions between Euler rotations in the 2-3-1 axis sequence and other
   orientation formats.)

REFERENCE:
  ()

ASSUMPTIONS AND LIMITATIONS:
  ()

LIBRARY_DEPENDENCY:
  (GunnsDynEuler231.o)

PROGRAMMERS:
  ((Jason Harvey) (CACI) (December 2016) (Initial))
@{
*/

#include "GunnsDynEulerBase.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief GUNNS Dynamics Euler Angles 231-Sequence
///
/// @details  This class defines the conversions between Euler rotations in the aerodynamics 2-3-1
///           axis sequence (Y, Z, X or Pitch, Yaw, Roll) and other orientation formats.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsDynEuler231 : public GunnsDynEulerBase
{
    public:
        /// @brief Default constructor.
        GunnsDynEuler231();
        /// @brief Default destructor.
        virtual ~GunnsDynEuler231();

    protected:
        /// @brief Computes the quaternion elements.
        virtual void   computeQuatSeq(double* quat,
                                      const double s1, const double c1,
                                      const double s2, const double c2,
                                      const double s3, const double c3);
        /// @brief Computes the sine of the 2nd rotation.
        virtual double computeRot2Sin(const double* quat);
        /// @brief Computes Euler angles away from a singularity condition.
        virtual void   computeAnglesNoSing(const double* quat, const double rot2sin);
        /// @brief Computes Euler angles near the positive singularity condition.
        virtual void   computeAnglesPosSing(const double* quat);
        /// @brief Computes Euler angles near the negative singularity condition.
        virtual void   computeAnglesNegSing(const double* quat);

    private:
        /// @brief Copy constructor is unavailable since private and not implemented.
        GunnsDynEuler231(const GunnsDynEuler231& that);
        /// @brief Assignment operator is unavailable since private and not implemented.
        GunnsDynEuler231& operator =(const GunnsDynEuler231& that);
};

/// @}

#endif
