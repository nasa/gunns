/**
@file
@brief    GUNNS Dynamics Utilities implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ((software/exceptions/TsNumericalException.o))
*/

#include "GunnsDynUtils.hh"
#include "software/exceptions/TsNumericalException.hh"
#include <cfloat>
#include <cmath>

/// @details  This sets the functions for the individual matrix element values of a quaternion-to-
///           matrix conversion into the function pointer array.  This allows a user to get a single
///           element of the converted matrix without having to compute the entire matrix.
const GunnsDynUtils::QtoM_Element GunnsDynUtils::QtoMElement[] = {
        &GunnsDynUtils::QtoM0,
        &GunnsDynUtils::QtoM1,
        &GunnsDynUtils::QtoM2,
        &GunnsDynUtils::QtoM3,
        &GunnsDynUtils::QtoM4,
        &GunnsDynUtils::QtoM5,
        &GunnsDynUtils::QtoM6,
        &GunnsDynUtils::QtoM7,
        &GunnsDynUtils::QtoM8,
};

/// @details  This tolerance value comes from Trick's implementation in quat_norm.c.
const double GunnsDynUtils::quatNormTolerance = 0.00000023842;

/// @details  Minimum vector magnitude for normalization.
const double GunnsDynUtils::vecNormTolerance = DBL_EPSILON;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] vA   (--) Pointer to the vector (an array) to be set.
/// @param[in]  vB   (--) Pointer to the Vector (an array) to be set equal to.
/// @param[in]  size (--) Size of the vector to set.
///
/// @details  Sets each element of vA equal to that element in vB, up to the given array size.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::setV(double* vA, const double* vB, const unsigned int size)
{
    for (unsigned int i=0; i<size; ++i) {
        vA[i] = vB[i];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] vA   (--) Pointer to the vector (an array) to be cleared.
/// @param[in]  size (--) Size of the vector to clear.
///
/// @details  Zeroes every element in the given array up to the given size.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::zeroV(double* vA, const unsigned int size)
{
    for (unsigned int i=0; i<size; ++i) {
        vA[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] vA   (--) Pointer to the resulting sum (an array).
/// @param[in]  vB   (--) Pointer to the first vector (an array) to be summed.
/// @param[in]  vC   (--) Pointer to the second vector (an array) to be summed.
/// @param[in]  size (--) Size of the vectors.
///
/// @details  Adds each element of the two given arrays up to the given array size.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::addV(double* vA, const double* vB, const double* vC, const unsigned int size)
{
    for (unsigned int i=0; i<size; ++i) {
        vA[i] = vB[i] + vC[i];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] vA   (--) Pointer to the resulting sum (an array).
/// @param[in]  vB   (--) Pointer to the vector (an array) to be subtracted from.
/// @param[in]  vC   (--) Pointer to the vector (an array) to be subtracted.
/// @param[in]  size (--) Size of the vectors.
///
/// @details  Subtracts each element of the second given array from the first, up to the given array
///           size.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::subtractV(double* vA, const double* vB, const double* vC, const unsigned int size)
{
    for (unsigned int i=0; i<size; ++i) {
        vA[i] = vB[i] - vC[i];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] vA   (--) Pointer to the resulting scaled vector (an array).
/// @param[in]  vB   (--) Pointer to the vector (an array) to scale.
/// @param[in]  sC   (--) The scalar value in the multiplication.
/// @param[in]  size (--) Size of the vectors.
///
/// @details  Multiplies every element of the given vector by the given scalar, up to the given
///           array size.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::scaleV(double* vA, const double* vB, const double sC, const unsigned int size)
{
    for (unsigned int i=0; i<size; ++i) {
        vA[i] = vB[i] * sC;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] vA   (--) Pointer to the resulting vector cross product (an array).
/// @param[in]  vB   (--) Pointer to the left-side vector (an array) in the operation.
/// @param[in]  vC   (--) Pointer to the right-side vector (an array) in the operation.
///
/// @details  Does the vector cross product of two size 3 vectors.  The vA can also be used as
///           either or both inputs, e.g. {A} = {A} X {C}, {A} = {B} X {A} or {A} = {A} X {A}.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::crossV3(double* vA, const double* vB, const double* vC)
{
    /// - Operate on copies of input vectors in case they point to the output.
    double B[3], C[3];
    setV(B, vB, 3);
    setV(C, vC, 3);
    vA[0] = B[1]*C[2] - B[2]*C[1];
    vA[1] = B[2]*C[0] - B[0]*C[2];
    vA[2] = B[0]*C[1] - B[1]*C[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] vA   (--) Pointer to the resulting product vector (an array).
/// @param[in]  mB   (--) Pointer to the square matrix (a single-dimension array) to be multiplied.
/// @param[in]  vC   (--) Pointer to the vector (an array) to be multiplied.
/// @param[in]  size (--) Size of the vector and the number of matrix rows.
///
/// @details  mC must be square.  Multiplies matrix mC by vector vC.  Result vA can also point to
///           the same array as vC, e.g. {A} = [B] * {A}.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::multiplyMV(double* vA, const double* mB, const double* vC, const unsigned int size)
{
    /// - Operate on copy of input vector in case it points to the output.
    double* C = new double[size];
    setV(C, vC, size);
    zeroV(vA, size);
    for (unsigned int row=0; row<size; ++row) {
        for (unsigned int col=0; col<size; ++col) {
            vA[row] += mB[row*size+col] * C[col];
        }
    }
    delete [] C;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] vA   (--) Pointer to the resulting product vector (an array).
/// @param[in]  mB   (--) Pointer to the square matrix (a single-dimension array) to be multiplied.
/// @param[in]  vC   (--) Pointer to the vector (an array) to be multiplied.
/// @param[in]  size (--) Size of the vector and the number of matrix rows.
///
/// @details  mC must be square.  Multiplies the transpose of matrix mC by vector vC.  Result vA can
///           also point to the same array as vC, e.g. {A} = [B] * {A}.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::multiplyMtV(double* vA, const double* mB, const double* vC, const unsigned int size)
{
    /// - Operate on copy of input vector in case it points to the output.
    double* C = new double[size];
    setV(C, vC, size);
    zeroV(vA, size);
    for (unsigned int row=0; row<size; ++row) {
        for (unsigned int col=0; col<size; ++col) {
            vA[row] += mB[col*size+row] * C[col];
        }
    }
    delete [] C;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] mA (--) Pointer to the resulting skew matrix (a single-dimension array of size 9).
/// @param[in]  vA (--) Pointer to the vector (an array) to compute the skew of.
///
/// @details  Computes the skew-symmetric 3x3 matrix of the given 3-vector.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::skew(double* mA, const double* vA)
{
    mA[0] =  0.0;
    mA[1] = -vA[2];
    mA[2] =  vA[1];
    mA[3] =  vA[2];
    mA[4] =  0.0;
    mA[5] = -vA[0];
    mA[6] = -vA[1];
    mA[7] =  vA[0];
    mA[8] =  0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] vA   (--) The first vector (an array) in the dot product.
/// @param[in] vB   (--) The second vector (an array) in the dot product.
/// @param[in] size (--) Size of the vectors.
///
/// @returns  double (--) The scalar result of the vector dot product.
///
/// @details  Computes and returns the vector dot product, a scalar value.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsDynUtils::dotV(const double* vA, const double* vB, const unsigned int size)
{
    double result = 0.0;
    for (unsigned int i=0; i<size; ++i) {
        result += vA[i]*vB[i];
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] qDot (--) Pointer to the resulting quaternion derivative.
/// @param[in]  q    (--) Pointer to the quaternion to differentiate.
/// @param[in]  w    (--) Pointer to the angular velocity vector in the derivative.
///
/// @details  Computes the time derivative of the input quaternion q by the angular velocity vector
///           w.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::Qdot(double* qDot, const double* q, const double* w)
{
    double q0w[3];
    double qSkew[9];
    double qSkewW[3];
    qDot[0] = dotV(&q[1], w, 3);  // will multiply by 0.5 below...
    scaleV(q0w, w, -q[0], 3);
    skew(qSkew, &q[1]);
    multiplyMV(qSkewW, qSkew, w, 3);
    addV(&qDot[1], qSkewW, q0w, 3);
    scaleV(qDot, qDot, 0.5, 4);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] mA (--) Pointer to the 3x3 rotation transformation matrix (as a single-dimension array).
/// @param[in]  qA (--) Pointer to the quaternion.
///
/// @details  Computes the rotation transformation matrix corresponding to the given left-handed
///           scalar-first quaternion.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::QtoM(double* mA, const double* qA)
{
    for (unsigned int i=0; i<9; ++i) {
        mA[i] = QtoMElement[i](qA);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] q (--) Pointer to the quaternion.
///
/// @returns  double (--) The [0][0] element of the rotation transformation matrix.
///
/// @details  Computes and returns the [0][0] element of the rotation transformation matrix
///           corresponding to the given left-handed scalar-first quaternion.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsDynUtils::QtoM0(const double *q)
{
    return 1.0 - 2.0 * (q[2]*q[2] + q[3]*q[3]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] q (--) Pointer to the quaternion.
///
/// @returns  double (--) The [0][1] element of the rotation transformation matrix.
///
/// @details  Computes and returns the [0][1] element of the rotation transformation matrix
///           corresponding to the given left-handed scalar-first quaternion.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsDynUtils::QtoM1(const double *q)
{
    return 2.0 * (q[1]*q[2] - q[0]*q[3]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] q (--) Pointer to the quaternion.
///
/// @returns  double (--) The [0][2] element of the rotation transformation matrix.
///
/// @details  Computes and returns the [0][2] element of the rotation transformation matrix
///           corresponding to the given left-handed scalar-first quaternion.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsDynUtils::QtoM2(const double *q)
{
    return 2.0 * (q[1]*q[3] + q[0]*q[2]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] q (--) Pointer to the quaternion.
///
/// @returns  double (--) The [1][0] element of the rotation transformation matrix.
///
/// @details  Computes and returns the [1][0] element of the rotation transformation matrix
///           corresponding to the given left-handed scalar-first quaternion.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsDynUtils::QtoM3(const double *q)
{
    return 2.0 * (q[1]*q[2] + q[0]*q[3]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] q (--) Pointer to the quaternion.
///
/// @returns  double (--) The [1][1] element of the rotation transformation matrix.
///
/// @details  Computes and returns the [1][1] element of the rotation transformation matrix
///           corresponding to the given left-handed scalar-first quaternion.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsDynUtils::QtoM4(const double *q)
{
    return 1.0 - 2.0 * (q[1]*q[1] + q[3]*q[3]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] q (--) Pointer to the quaternion.
///
/// @returns  double (--) The [1][2] element of the rotation transformation matrix.
///
/// @details  Computes and returns the [1][2] element of the rotation transformation matrix
///           corresponding to the given left-handed scalar-first quaternion.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsDynUtils::QtoM5(const double *q)
{
    return 2.0 * (q[2]*q[3] - q[0]*q[1]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] q (--) Pointer to the quaternion.
///
/// @returns  double (--) The [2][0] element of the rotation transformation matrix.
///
/// @details  Computes and returns the [2][0] element of the rotation transformation matrix
///           corresponding to the given left-handed scalar-first quaternion.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsDynUtils::QtoM6(const double *q)
{
    return 2.0 * (q[1]*q[3] - q[0]*q[2]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] q (--) Pointer to the quaternion.
///
/// @returns  double (--) The [2][1] element of the rotation transformation matrix.
///
/// @details  Computes and returns the [2][1] element of the rotation transformation matrix
///           corresponding to the given left-handed scalar-first quaternion.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsDynUtils::QtoM7(const double *q)
{
    return 2.0 * (q[2]*q[3] + q[0]*q[1]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] q (--) Pointer to the quaternion.
///
/// @returns  double (--) The [2][2] element of the rotation transformation matrix.
///
/// @details  Computes and returns the [2][2] element of the rotation transformation matrix
///           corresponding to the given left-handed scalar-first quaternion.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsDynUtils::QtoM8(const double *q)
{
    return 1.0 - 2.0 * (q[1]*q[1] + q[2]*q[2]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] qA (--) Pointer to the quaternion.
/// @param[in]  mA (--) Pointer to the 3x3 rotation transformation matrix (as a single-dimension array).
///
/// @throws   TsNumericalException
///
/// @details  Computes the left-handed scalar-first quaternion corresponding to the given rotation
///           transformation matrix.  This quaternion is not normalized.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::MtoQ(double* qA, const double* mA)
{
    const double diags = mA[0] + mA[4] + mA[8];
    if (diags > -1.0) {
        qA[0] = 0.5  * sqrt(1.0 + diags);
        qA[1] = 0.25 * (mA[7] - mA[5]) / qA[0];
        qA[2] = 0.25 * (mA[2] - mA[6]) / qA[0];
        qA[3] = 0.25 * (mA[3] - mA[1]) / qA[0];
    } else {
        throw TsNumericalException();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] q (--) The quaternion.
///
/// @details  Normalizes the given quaternion to have a magnitude of 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::normalizeQ(double* q)
{
    double factor;
    const double mag2 = dotV(q, q, 4);
    if (fabs(1.0 - mag2) < quatNormTolerance) {
        factor = 2.0 / (1.0 + mag2);
    } else {
        factor = 1.0 / sqrt(mag2);
    }
    q[0] *= factor;
    q[1] *= factor;
    q[2] *= factor;
    q[3] *= factor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] v (--) The vector.
///
/// @throws   TsNumericalException
///
/// @details  Normalizes the given vector to have a magnitude of 1.  If the vector magnitude is too
///           small, this throws an exception and leaves the vector unchanged.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::normalizeV(double* v)
{
    const double mag = magV(v, 3);
    if (mag < vecNormTolerance) {
        throw TsNumericalException();
    } else {
        const double factor = 1.0 / mag;
        v[0] *= factor;
        v[1] *= factor;
        v[2] *= factor;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in, out] v    (--) The vector.
/// @param[in]      size (--) Size of the vector.
///
/// @returns  bool (--) True if the normalize succeeded, false if it failed.
///
/// @details  If the given vector magnitude is sufficient for dividing, this normalizes the vector
///           to have a magnitude of 1, and returns true indicating success.  Otherwise leaves the
///           vector unchanged and returns false indicating failure.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsDynUtils::normalizeVSuccess(double* v, const unsigned int size)
{
    const double mag = magV(v, size);
    if (mag < vecNormTolerance) {
        return false;
    } else {
        scaleV(v, v, 1.0 / mag, size);
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] v    (--) Pointer to the vector (an array) to get the magnitude of.
/// @param[in] size (--) The number of elements in the vector.
///
/// @returns  double (--) The vector magnitude.
///
/// @details  Returns the magnitude as the sum of the squares of each array element.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsDynUtils::magV(const double* v, const unsigned int size)
{
    double result = 0.0;
    for (unsigned int i=0; i<size; ++i) {
        result += v[i]*v[i];
    }
    return sqrt(result);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] mA    (--) Pointer to the single-dimension array being set.
/// @param[in]  mB    (--) Pointer to the single-dimnesion array used to set.
/// @param[in]  size  (--) The number of rows of the square matrices, e.g. a 3x3 matrix is size 3.
///
/// @details  Sets mA to mB value by value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::setM(double* mA, const double* mB, const unsigned int size)
{
    const unsigned int sizeSq = size*size;
    for (unsigned int i=0; i<sizeSq; ++i) {
        mA[i] = mB[i];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] mA   (--) Pointer to the single-dimension array for the resulting product matrix.
/// @param[in]  mB   (--) Pointer to the single-dimension array for the left-side matrix in the multiplication.
/// @param[in]  mC   (--) Pointer to the single-dimension array for the right-side matrix in the multiplication.
/// @param[in]  size (--) The number of rows of the square matrices, e.g. a 3x3 matrix is size 3.
///
/// @details  All matrices must be square and have the same size.  The mA can also be used as either
///           or both inputs, e.g. [A] = [A] * [C], [A] = [B] * [A], or [A] = [A] * [A].
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynUtils::multiplyMM(double* mA, double* mB, double* mC, const unsigned int size)
{
    /// - Operate on copies of input matrices in case they point to the output.
    const unsigned int sizeSq = size*size;
    double* B = new double[sizeSq];
    double* C = new double[sizeSq];
    setV(B, mB, sizeSq);
    setV(C, mC, sizeSq);
    zeroV(mA, sizeSq);
    for (unsigned int row=0; row<size; ++row) {
        for (unsigned int col=0; col<size; ++col) {
            for (unsigned int i=0; i<size; ++i) {
                mA[row*size+col] += B[row*size+i] * C[i*size+col];
            }
        }
    }
    delete [] C;
    delete [] B;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] v    (--) Pointer to the vector (an array) to check.
/// @param[in] size (--) The number of elements in the vector.
///
/// @returns  bool (--) False when all vector elements are exactly zero, true otherwise.
///
/// @details  Returns true when any of the vector elements are non-zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsDynUtils::isNonZeroV(const double* v, const unsigned int size)
{
    for (unsigned int i=0; i<size; ++i) {
        if (v[i] != 0.0) return true;
    }
    return false;
}
