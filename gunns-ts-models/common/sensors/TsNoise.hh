#ifndef TsNoise_EXISTS
#define TsNoise_EXISTS

/**
@defgroup TSM_SENSORS_NOISE  Noise
@ingroup  TSM_SENSORS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- ( Leva Normalized Noise over [-1, 1].\n
    This function calculates and returns pseudorandom noise.  It is based on the Leva Method and
    has been shown to be ~30% faster than the Box-Muller method.\n
    Leva's Method generates pseudorandom numbers with a normal distribution.  The method is exact
    and builds upon the Ratio of Uniforms algorithm developed by Kinderman and Monahan.  A portion
    of the bounding curves, which define the acceptance region for the selective rejection process,
    are redefined by quadratic fits.  The computational work is significantly reduced (as compared
    to algorithms which evaluate transendental functions every iteration) by limiting the number of
    evaluations of the log function such that only 0.012 evaluations are performed for every normal
    deviate generated.  The algorithm requires two independent and uniformly random variables
    defined over the interval [0,RAND_MAX]) to generate one pseudo-gaussian distributioned deviate
    (with mean zero and variance 19.51...).  Multiplication by the desired standard deviation
    1/4.41717... rescales the standard normal deviates such that 99.999 percent fall onto the
    [-1,+1] interval.  The remaining few iterates that fall outside the [-1,+1] interval are
    recomputed until they fall within the desired interval which results in a small ~1e-5 percent
    error in the probability density function.  The result, pseudo-normally distributed numbers are
    generated such that they fall between -1 to +1 (inclusive).)

REQUIREMENTS:
- ()

REFERENCE:
- ((Leva, Joseph L., A Fast Normal Random Number Generator, ACM Trans. Math. Softw. Vol 18, No 4,
    12/92, 449-453.)
   (Thomas, David B., Luk, Wayne, Leong, Philip H. W., Villasenor, John D., Gaussian Random Number
    Generators, CMN Computing Surveys, Vol. 39, No. 4, Article 11, 10/07.)
   (Press, William H., Teukolsky, Saul A., Vettering, William T., Flannery, Brian P., Numerical
    Recipes: The Art of Scientific Computing, 3rd edition, 2007.))
  )

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((TsNoise.o))

 PROGRAMMERS:
- (((Robert Paul) (L3) (Tue Dec 22, 2009 18:20:48 CST)  (initial and updates))
   ((Kenneth McMurtrie) (Tietronix Software) (Conversion to C++) (2012-08)))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Noise model.
///
/// @warning  The hardcoded values are part of Leva's method and should not be changed.
///
/// @details  Provides noise from a normal distribution with range limiting.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsNoise {
    TS_MAKE_SIM_COMPATIBLE(TsNoise);
    public:
        /// @brief  A function pointer type of signature: double function(), used for accessing getNoise.
        typedef double (*f_ptr)();
        /// @brief  Initializes this Noise model.
        static void initialize(const unsigned int seed);
        /// @brief  Gets noise from this Noise model.
        static double getNoise();
        /// @brief  Returns an object handle to the getNoise function for Python (Trick input file).
        static f_ptr getNoiseFunction() {return getNoise;}

    protected:
        static const double  S;                   /**< (--)  Curve fit coefficient 1. */
        static const double  T;                   /**< (--)  Curve fit coefficient 2. */
        static const double  A;                   /**< (--)  Curve fit coefficient 3. */
        static const double  B;                   /**< (--)  Curve fit coefficient 4. */
        static const double  R1;                  /**< (--)  Evaluation constant 1. */
        static const double  R2;                  /**< (--)  Evaluation constant 2. */
        static const double  SIGMA_INV;           /**< (--)  Inverse of sigma. */
        static const double  DENOM_INV;           /**< (--)  Inverse of denominator scale factor. */
        static const double  NUM;                 /**< (--)  Numerator scale factor. */
        static const int     LOOP_MAX;            /**< (--)  Maximum number of attempts. */
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Default constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsNoise();
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsNoise(const TsNoise&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Default destructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        ~TsNoise();
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsNoise& operator=(const TsNoise&);
};

/// @}

#endif

