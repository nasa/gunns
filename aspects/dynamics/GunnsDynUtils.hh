#ifndef GunnsDynUtils_EXISTS
#define GunnsDynUtils_EXISTS

/**
@file
@brief    GUNNS Dynamics Utilities declarations

@defgroup  TSM_GUNNS_DYNAMICS_UTILS  GUNNS Dynamics Utilities
@ingroup   TSM_GUNNS_DYNAMICS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
  (General matrix, vector & quaternion math utilities used for dynamics.
  - All matrices are in a single-dimension array, same as the GUNNS admittance matrix convention.
  - Quaternions are left-handed, scalar first.
  - Except for cases that are only meaningful for specific array sizes, such as a 3x3 matrix in
    quanternion conversions, all functions can work on arrays of arbitrary size.
  )

REFERENCE:
  ("Adv. Dynamics", Tushar K. Ghosh, June 2016)

ASSUMPTIONS AND LIMITATIONS:
  (Pointers are assumed to actually point to something, and we don't check for NULL.)

LIBRARY_DEPENDENCY:
  (GunnsDynUtils.o)

PROGRAMMERS:
  (
   ((Jason Harvey) (CACI) (December 2016) (Initial))
  )
@{
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Dynamics Utilities.
///
/// @details  Refer to class Purpose.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsDynUtils
{
    public:
        /// @brief Function pointer type for quanternion to matrix conversion elements functions.
        typedef double (*QtoM_Element)(const double *q);
        static const  QtoM_Element QtoMElement[9]; /**< ** (--) Array of quanternion to matrix conversion functions by matrix element. */
        /// @brief {vA} = {vB}.
        static void   setV(double* vA, const double* vB, const unsigned int size);
        /// @brief {v} = 0.
        static void   zeroV(double* vA, const unsigned int size);
        /// @brief {vA} = {vB} + {vC}.
        static void   addV(double* vA, const double* vB, const double* vC, const unsigned int size);
        /// @brief {vA} = {vB} - {vC}.
        static void   subtractV(double* vA, const double* vB, const double* vC, const unsigned int size);
        /// @brief {vA} = {vB} * sC.
        static void   scaleV(double* vA, const double* vB, const double sC, const unsigned int size);
        /// @brief {vA} = {vB} X {vC}.
        static void   crossV3(double* vA, const double* vB, const double* vC);
        /// @brief {vA} = [mB] * {vC}.
        static void   multiplyMV(double* vA, const double* mB, const double* vC, const unsigned int size);
        /// @brief {vA} = transpose[mB] * {vC}.
        static void   multiplyMtV(double* vA, const double* mB, const double* vC, const unsigned int size);
        /// @brief [mA] = skew {vA}.
        static void   skew(double* mA, const double* vA);
        /// @brief Returns scalar = {vA} dot {vB}.
        static double dotV(const double* vA, const double* vB, const unsigned int size);
        /// @brief Quaternion {qDot} = d{q}/dt by {w}.
        static void   Qdot(double* qDot, const double* q, const double* w);
        /// @brief Matrix [mA] = quaternion {qA}.
        static void   QtoM(double* mA, const double* qA);
        /// @brief Quaternion {qA} = matrix [mA].
        static void   MtoQ(double* qA, const double* mA);
        /// @brief Normalize quaternion {q}.
        static void   normalizeQ(double* q);
        /// @brief Normalize a 3-vector {v}.
        static void   normalizeV(double* v);
        /// @brief Normalize vector {v} and returns success/fail flag.
        static bool   normalizeVSuccess(double* v, const unsigned int size);
        /// @brief |{v}|.
        static double magV(const double* v, const unsigned int size);
        /// @brief [mA] = [mB]
        static void   setM(double* mA, const double* mB, const unsigned int size);
        /// @brief [mA] = [mB] * [mC].
        static void   multiplyMM(double* mA, double* mB, double* mC, const unsigned int size);
        /// @brief Returns whether the vector has non-zero magnitude.
        static bool   isNonZeroV(const double* v, const unsigned int size);

    private:
        static const double quatNormTolerance; /**< ** (--) Tolerance for quaternion magnitude normalization. */
        static const double vecNormTolerance;  /**< ** (--) Tolerance for vector magnitude normalization. */
        /// @brief Returns the [0][0] matrix element of the quaternion to matrix conversion.
        static double QtoM0(const double* q);
        /// @brief Returns the [0][1] matrix element of the quaternion to matrix conversion.
        static double QtoM1(const double* q);
        /// @brief Returns the [0][2] matrix element of the quaternion to matrix conversion.
        static double QtoM2(const double* q);
        /// @brief Returns the [1][0] matrix element of the quaternion to matrix conversion.
        static double QtoM3(const double* q);
        /// @brief Returns the [1][1] matrix element of the quaternion to matrix conversion.
        static double QtoM4(const double* q);
        /// @brief Returns the [1][2] matrix element of the quaternion to matrix conversion.
        static double QtoM5(const double* q);
        /// @brief Returns the [2][0] matrix element of the quaternion to matrix conversion.
        static double QtoM6(const double* q);
        /// @brief Returns the [2][1] matrix element of the quaternion to matrix conversion.
        static double QtoM7(const double* q);
        /// @brief Returns the [2][2] matrix element of the quaternion to matrix conversion.
        static double QtoM8(const double* q);
        /// @brief Default Constructor
        GunnsDynUtils();
        /// @brief Default Destructor
        virtual ~GunnsDynUtils();
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsDynUtils(const GunnsDynUtils& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsDynUtils& operator =(const GunnsDynUtils& that);
};

/// @}

#endif
