/************************** TRICK HEADER **********************************************************
 PURPOSE:
 - (Provides the definitions for the example approximation sim class.)

 REQUIREMENTS:
 ()

 REFERENCE:
 ()

 ASSUMPTIONS AND LIMITATIONS:
 ()

 LIBRARY DEPENDENCY:
 (
    (math/approximation/TsBilinearInterpolator.o)
    (math/approximation/TsLinearInterpolator.o)
    (math/approximation/CubicFit.o)
    (math/approximation/ExponentialFit.o)
    (math/approximation/InvLinearFit.o)
    (math/approximation/InvQuadraticFit.o)
    (math/approximation/LinearFit.o)
    (math/approximation/PowerFit.o)
    (math/approximation/ProductFit.o)
    (math/approximation/QuadLinFit.o)
    (math/approximation/QuadLinInvFit.o)
    (math/approximation/QuadraticFit.o)
    (math/approximation/QuadraticRootFit.o)
    (math/approximation/QuarticFit.o)
    (math/approximation/QuotientFit.o)
    (math/approximation/RationalFit.o)
    (math/approximation/ShowmateFit.o)
    (math/approximation/SutherlandFit.o)
 )

 PROGRAMMERS:
 (
     ((Eric Ferguson) (L-3 Communications) (2011-08))
  )
 ***************************************************************************************************/

#include "ApproximationSim.hh"

const double ApproximationSim::X[]    = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
const double ApproximationSim::Z[]    = {100, 200, 400, 900, 1600, 2500, 2600, 2700, 2800, 2900};
const int    ApproximationSim::n      = 10;
const double ApproximationSim::X2[5]  = {10, 20, 30, 40, 50};
const double ApproximationSim::Y2[3]  = {11, 22, 33};
const int    ApproximationSim::m2     = 5;
const int    ApproximationSim::n2     = 3;
const double ApproximationSim::Z2[15] = { 200,  400,  900,
                                          1600, 2500, 2600,
                                          2700, 2800, 2900,
                                          3000, 3100, 3200,
                                          3300, 3400, 3500 };
const double ApproximationSim::minX   = +10.0;
const double ApproximationSim::maxX   = +50.0;
const double ApproximationSim::minY   = +11.0;
const double ApproximationSim::maxY   = +33.0;
const double ApproximationSim::minX2  = +1;
const double ApproximationSim::minY2  = +1;
const double ApproximationSim::a      = +1.0;
const double ApproximationSim::b      = +2.0;
const double ApproximationSim::c      = +3.0;
const double ApproximationSim::d      = +4.0;
const double ApproximationSim::e      = +5.0;
const double ApproximationSim::f      = +6.0;

ApproximationSim::ApproximationSim()
            :
            mLinear(X, Z, n, minX, maxX),
            mBilinear(X2, Y2, Z2, m2, n2, minX, maxX, minY, maxY),
            mCubic(a, b, c, d, minX, maxX),
            mExponential(a, b, c, minX2, maxX),
            mInvLinear(a, b, c, minX2, maxX),
            mInvQuad(a, b, c, minX2, maxX),
            mLinearFit(a, b, minX, maxX),
            mPower(a, b, minX2, maxX),
            mProduct(a, minX, maxX, minY, maxY),
            mQuadLin(a, b, c, d, e, f, minX, maxX, minY, maxY),
            mQuadLinInv(a, b, c, d, e, f, minX2, maxX, minY2, maxY),
            mQuad(a, b, c, minX, maxX),
            mQuadRoot(a, b, minX2, maxX),
            mQuartic(a, b, c, d, e, minX, maxX),
            mQuotient(a, minX2, maxX, minY2, maxY),
            mRational(a, b, c, d, minX, maxX),
            mShowmate(a, b, c, d, e, minX2, maxX),
            mSutherland(a, b, minX2, maxX),
            mW(9.1),
            mX(9.1),
            mY(10.1),
            mZ(0.0),
            mZ2(0.0),
            mZCub(0.0),
            mZExp(0.0),
            mZILin(0.0),
            mZIQuad(0.0),
            mZLin(0.0),
            mZPow(0.0),
            mZProd(0.0),
            mZQuadLin(0.0),
            mZQuadLinI(0.0),
            mZQuad(0.0),
            mZQuadRoot(0.0),
            mZQuart(0.0),
            mZQuotient(0.0),
            mZRation(0.0),
            mZShow(0.0),
            mZSuther(0.0)
{
    // nothing to do
}

ApproximationSim::~ApproximationSim()
{
    // nothing to do
}

void ApproximationSim::initialize()
{
    // nothing to do
}

void ApproximationSim::update()
{
    //Interpolators
    mW += 0.1;
    mZ  = mLinear.get(mW);
    mX += 0.1;
    mY += 0.1;
    mZ2 = mBilinear.get(mX, mY);

    //Curve Fits
    mZCub = mCubic.get(-7.617);
    mZExp =  mExponential.get(13.0);
    mZILin = mInvLinear.get(200.0);
    mZIQuad = mInvQuad.get(11.0);
    mZLin = mLinearFit.get(11.437);
    mZPow = mPower.get(3.0);
    mZProd = mProduct.get(310.0,200.0);
    mZQuadLin = mQuadLin.get(8.0,11.0);
    mZQuadLinI = mQuadLinInv.get(8.0,15.0);
    mZQuad = mQuad.get(132.34);
    mZQuadRoot = mQuadRoot.get(150.0);
    mZQuart  =  mQuartic.get(6.0);
    mZQuotient = mQuotient.get(310.0,1000.0);
    mZRation = mRational.get(316.0);
    mZShow = mShowmate.get(3610.0);
    mZSuther = mSutherland.get(100.0);


}
