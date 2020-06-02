/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((simulation/hs/TsHsMsg.o))
**************************************************************************************************/

#include <cmath>
#include <cstdlib>

#include "TsNoise.hh"
#include "simulation/hs/TsHsMsg.hh"

const double  TsNoise::S         =  0.449871;
const double  TsNoise::T         = -0.386595;
const double  TsNoise::A         =  0.19600;
const double  TsNoise::B         =  0.25472;
const double  TsNoise::R1        =  0.27597;
const double  TsNoise::R2        =  0.27846;
/// @details  Sigma is defined as 1/number of standard deviations which contain 99.999% of the
///           random variables in a standard normal distribution.  This is equivalent to sigma, the
///           desired standard deviation for the new transformed probability distribution function.
const double  TsNoise::SIGMA_INV = 1.0 / 4.417173413469;
const double  TsNoise::DENOM_INV = 1.0 / (static_cast<double>(RAND_MAX) + 2.0);
const double  TsNoise::NUM       = sqrt(2.0 / exp(1.0));
const int     TsNoise::LOOP_MAX  = 50;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  seed  (--)  Seed for uniform randon number generation.
///
/// @return     void
///
/// @details    Initializes this Noise model. Should be called once before calling getNoise.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsNoise::initialize(const unsigned int seed)
{
    std::srand(seed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     double  (--)  Gaussian noise.
///
/// @details    Gets noise.
////////////////////////////////////////////////////////////////////////////////////////////////////
double TsNoise::getNoise()
{
    /// Selective rejection algorithm (keeps the gaussian noise, rejects the rest).
    /// All constants are taken from Leva's algorithm.

    // While it is nearly certain the "while" loop will exit before too many passes
    // (statistically speaking), a loop counter was added for safety.
    for (int i = 0; i < LOOP_MAX; ++i) {

        // Uniformly distributed random variables u and z are defined as independent random numbers
        // on the interval (0,1); that is, the interval excludes 0 and 1.  The function rand() returns
        // numbers which are elements of the closed interval [0, RAND_MAX].  The computed values, u
        // and z, are constructed such that their minimum and maximum are a distance 1/(RAND_MAX + 2)
        // inside the closed interval boundary.
        const double u = (static_cast<double>(std::rand()) + 1.0) * DENOM_INV;
        const double z = (static_cast<double>(std::rand()) + 1.0) * DENOM_INV;

        const double v = NUM * (2.0 * z - 1.0);
        const double x = u - S;
        const double y = fabs(v) - T;

        /// - Compute candidate from two uniform random variates.
        const double q = x * x + y * (A * y - B * x);

        /// - Compute noise if point is within acceptance area
        //    As defined, u cannot equal zero.
        if ((q < R1) || ((q < R2) && (v * v < -4.0 * u * u * log(u)))) {
            const double noise = SIGMA_INV * v / u;
            if (fabs(noise) <= 1.0) {
                /// - Return Gaussian noise when algorithm succeeds.
                return noise;
            }
        }
    }

    /// - Issue an H&S message and return zero noise on algorithm failure after limited number of attempts.
    hsSendMsg(TS_HS_INFO, "MATH", "Leva's method failed to produce a noise value within the acceptance range, a rare but not impossible occurrence. A value of zero was returned. If this message appears more than once, then the code has been modified and is no longer functioning properly.");
    return 0.0;
}
