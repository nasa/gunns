#ifndef ApproximationSim_EXISTS
#define ApproximationSim_EXISTS

/*
 PURPOSE:
 - (Provides the declarations for the example approximation sim class.)

 LIBRARY DEPENDENCY:
 - (
    (math/approximation/test/sim/ApproximationSim.o)
   )

 PROGRAMMERS:
 - (
    ((Eric Ferguson) (L-3 Communications) (2011-08))
   )
 */

#include "math/approximation/TsBilinearInterpolator.hh"
#include "math/approximation/TsLinearInterpolator.hh"
#include "math/approximation/CubicFit.hh"
#include "math/approximation/ExponentialFit.hh"
#include "math/approximation/InvLinearFit.hh"
#include "math/approximation/InvQuadraticFit.hh"
#include "math/approximation/LinearFit.hh"
#include "math/approximation/PowerFit.hh"
#include "math/approximation/ProductFit.hh"
#include "math/approximation/QuadLinFit.hh"
#include "math/approximation/QuadLinInvFit.hh"
#include "math/approximation/QuadraticFit.hh"
#include "math/approximation/QuadraticRootFit.hh"
#include "math/approximation/QuarticFit.hh"
#include "math/approximation/QuotientFit.hh"
#include "math/approximation/RationalFit.hh"
#include "math/approximation/ShowmateFit.hh"
#include "math/approximation/SutherlandFit.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

class ApproximationSim {
    TS_MAKE_SIM_COMPATIBLE(ApproximationSim);
public:
    ApproximationSim();
    ~ApproximationSim();
    void initialize();
    void update();
protected:
    TsLinearInterpolator   mLinear;       // -- Instantiation of TsLinearInterpolator class
    TsBilinearInterpolator mBilinear;     // -- Instantiation of TsBiLinearInterpolator class
    CubicFit               mCubic;        // -- Instantiation of CubicFit class
    ExponentialFit         mExponential;  // -- Instantiation of ExponetialFit class
    InvLinearFit           mInvLinear;    // -- Instantiation of InvLinearFit class
    InvQuadraticFit        mInvQuad;      // -- Instantiation of InvQuadraticFit class
    LinearFit              mLinearFit;    // -- Instantiation of LinearFit class
    PowerFit               mPower;        // -- Instantiation of PowerFit class
    ProductFit             mProduct;      // -- Instantiation of ProductFit class
    QuadLinFit             mQuadLin;      // -- Instantiation of QuadLinFit class
    QuadLinInvFit          mQuadLinInv;   // -- Instantiation of QuadLinInvFit class
    QuadraticFit           mQuad;         // -- Instantiation of QuadraticFit class
    QuadraticRootFit       mQuadRoot;     // -- Instantiation of QuadraticRootFit class
    QuarticFit             mQuartic;      // -- Instantiation of QuarticFit class
    QuotientFit            mQuotient;     // -- Instantiation of QuotientFit class
    RationalFit            mRational;     // -- Instantiation of RationalFit class
    ShowmateFit            mShowmate;     // -- Instantiation of ShowmateFit class
    SutherlandFit          mSutherland;   // -- Instantiation of SutherlandFit class
    static const double    X[];           // -- independent array for linear interp
    static const double    Z[];           // -- dependent array for linear interp
    static const int       n;             // -- size of linear interp arrays
    static const double    X2[];          // -- first independent array for bilinear interp
    static const double    Y2[];          // -- second independent array for bilinear interp
    static const double    Z2[];          // -- dependent array for bilinear interp
    static const int       n2;            // -- size of first independent array
    static const int       m2;            // -- size of second independent array
    static const double    minX;          // -- valid range lower limit for X variable
    static const double    maxX;          // -- valid range upper limit for X variable
    static const double    minY;          // -- valid range lower limit for Y variable
    static const double    maxY;          // -- valid range upper limit for Y variable
    static const double    minX2;         // -- valid range lower limit (> 0) for X variable
    static const double    minY2;         // -- valud range lower limit (> 0) for Y variable
    double                 mZ;            // -- return value for LinearInterp approximation
    double                 mZ2;           // -- return value for BilinearInterp approximation
    double                 mZCub;         // -- return value for CubicFit approximation
    double                 mZExp;         // -- return value for ExponetialFit approximation
    double                 mZILin;        // -- return value for InvLinearFit approximation
    double                 mZIQuad;       // -- return value for InvQuadraticFit approximation
    double                 mZLin;         // -- return value for LinearFit approximation
    double                 mZPow;         // -- return value for PowerFit approximation
    double                 mZProd;        // -- return value for ProductFit approximation
    double                 mZQuadLin;     // -- return value for QuadLinFit approximation
    double                 mZQuadLinI;    // -- return value for QuadLinInvFit approximation
    double                 mZQuad;        // -- return value for QuadraticFit approximation
    double                 mZQuadRoot;    // -- return value for QuadraticRootFit approximation
    double                 mZQuart;       // -- return value for QuarticFit approximation
    double                 mZQuotient;    // -- return value for QuotientFit approximation
    double                 mZRation;      // -- return value for ShowmateFit approximation
    double                 mZShow;        // -- return value for SutherlandFit approximation
    double                 mZSuther;      // -- approximation output for SutherlandFit
    double                 mW;            // -- first independent variable for linear interp
    double                 mX;            // -- first independent variable for bilinear interp
    double                 mY;            // -- second independent variable for bilinear interp
    static const double    a;             // -- first coefficient for curve fit models
    static const double    b;             // -- second coefficient for curve fit models
    static const double    c;             // -- third coefficient for curve fit models
    static const double    d;             // -- fourth coefficient for curve fit models
    static const double    e;             // -- fifth coefficient for curve fit models
    static const double    f;             // -- sixth coefficient for curve fit models
private:
    ApproximationSim(const ApproximationSim&);
    ApproximationSim& operator =(const ApproximationSim&);
};

#endif
