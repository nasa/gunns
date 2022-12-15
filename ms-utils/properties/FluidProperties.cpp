/*
@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Provides the class for modeling the properties of liquids and gases.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     (The constructor is protected, so only those static Fluid Properties objects in its friend,
      the Defined Fluid Properties class are available.)

 LIBRARY DEPENDENCY:
     (
      (math/approximation/QuinticFit.o)
      (math/approximation/QuarticFit.o)
      (math/approximation/CubicFit.o)
      (math/approximation/TsApproximation.o)
      (math/approximation/LinearFit.o)
      (math/approximation/PowerFit.o)
      (math/approximation/ProductFit.o)
      (math/approximation/QuadLinFit.o)
      (math/approximation/QuadLinInvFit.o)
      (math/approximation/QuadraticFit.o)
      (math/approximation/QuadraticRootFit.o)
      (math/approximation/QuotientFit.o)
      (math/approximation/TsBilinearInterpolatorReverse.o)
      (properties/FluidTsatFit.o)
      (properties/FluidHvapFit.o)
      (properties/FluidPropertiesDataWaterPvt.o)
      (math/UnitConversion.o)
     )

 PROGRAMMERS:
     ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-05))
*/

#include "FluidProperties.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] type                  Type of fluid (from FluidType enum)
/// @param[in] phase                 Phase of fluid (from FluidPhase enum)
/// @param[in] mWeight               Molecular weight (1/mol) of the fluid
/// @param[in] criticalTemperature   Critical temperature (K) of the fluid
/// @param[in] density               Pointer to curve fit of density for the fluid
/// @param[in] viscosity             Pointer to curve fit of dynamic viscosity for the fluid
/// @param[in] specificHeat          Pointer to curve fit of specific heat for the fluid
/// @param[in] thermalConductivity   Pointer to curve fit of thermal conductivity for the fluid
/// @param[in] prandtlNumber         Pointer to curve fit of Prandtl number for the fluid
/// @param[in] adiabaticIndex        Pointer to curve fit of adiabatic index for the fluid
/// @param[in] pressure              Pointer to curve fit of pressure for the fluid
/// @param[in] temperature           Pointer to curve fit of temperature for the fluid
/// @param[in] saturationPressure    Pointer to curve fit of saturation pressure for the fluid
/// @param[in] saturationTemperature Pointer to curve fit of saturation temperature for the fluid
/// @param[in] heatOfVaporization    Pointer to curve fit of heat of vaporization for the fluid
///
/// @details  Constructs the fluid properties by specifying values for each attribute. Also serves
///           as the default constructor since a default value is specified for each argument.
////////////////////////////////////////////////////////////////////////////////////////////////////
FluidProperties::FluidProperties(const FluidProperties::FluidType&  type,
                                 const FluidProperties::FluidPhase& phase,
                                 const double                       mWeight,
                                 const double                       criticalTemperature,
                                 TsApproximation*                   density,
                                 TsApproximation*                   viscosity,
                                 TsApproximation*                   specificHeat,
                                 TsApproximation*                   thermalConductivity,
                                 TsApproximation*                   prandtlNumber,
                                 TsApproximation*                   adiabaticIndex,
                                 TsApproximation*                   pressure,
                                 TsApproximation*                   temperature,
                                 TsApproximation*                   saturationPressure,
                                 TsApproximation*                   saturationTemperature,
                                 TsApproximation*                   heatOfVaporization)
    :
    mType(type),
    mPhase(phase),
    mMWeight(mWeight),
    mCriticalTemperature(criticalTemperature),
    mDensity(density),
    mViscosity(viscosity),
    mSpecificHeat(specificHeat),
    mThermalConductivity(thermalConductivity),
    mPrandtlNumber(prandtlNumber),
    mAdiabaticIndex(adiabaticIndex),
    mPressure(pressure),
    mTemperature(temperature),
    mSaturationPressure(saturationPressure),
    mSaturationTemperature(saturationTemperature),
    mHeatOfVaporization(heatOfVaporization)
{
    // nothing left to do
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs a fluid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
FluidProperties::~FluidProperties()
{
    // nothing to do
}

/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mMWeightCO         = 28.0101;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mMWeightCO2        = 44.0095;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mMWeightH2O        = 18.0153;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mMWeightN2         = 28.0134;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mMWeightO2         = 31.9988;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mMWeightNH3        = 17.0305;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mMWeightH2         = 2.01588;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mMWeightCH4        = 16.0425;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mMWeightHCL        = 36.4611;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mMWeightHCN        = 27.0254;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mMWeightHe         = 4.002602;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mMWeightXe         = 131.293;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mMWeightN2O4       = 92.011;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mMWeightN2H4       = 32.0452;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mMWeightCH3N2H3    = 46.0717;
/// @details  Reference: 3M (TM) Novec (TM) 7000 Engineering Fluid Product Information.
const double DefinedFluidProperties::mMWeightHFE7000    = 200.0;
/// @details  Reference: 3M (TM) Novec (TM) Engineered Fluid HFE-7100 for Heat Transfer.
const double DefinedFluidProperties::mMWeightHFE7100    = 250.0;
/// @details  Reference: NIST Chemistry Webbook, average of propylene glycol & water mol weights for
///           a 30% solution.
const double DefinedFluidProperties::mMWeightPG30       = 35.43903;
/// @details  Reference: NIST Chemistry Webbook, average of propylene glycol & water mol weights for
///           a 40% solution.
const double DefinedFluidProperties::mMWeightPG40       = 41.24694;
/// @details  Reference: NIST Chemistry Webbook, average of propylene glycol & water mol weights for
///           a 50% solution.
const double DefinedFluidProperties::mMWeightPG50       = 47.05485;
/// @details  Reference: the Periodic Table.
const double DefinedFluidProperties::mMWeightNAK78      = 35.5544;
/// @details  Reference: http://www.solvay.com/en/markets-and-products/featured-products/Galden.html
const double DefinedFluidProperties::mMWeightGALDEN170  = 760.0;

/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mCriticalTemperatureCO         = 132.86;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mCriticalTemperatureCO2        = 304.128;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mCriticalTemperatureH2O        = 647.096;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mCriticalTemperatureN2         = 126.192;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mCriticalTemperatureO2         = 154.581;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mCriticalTemperatureNH3        = 405.4;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mCriticalTemperatureH2         = 33.145;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mCriticalTemperatureCH4        = 190.564;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mCriticalTemperatureHCL        = 324.68;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mCriticalTemperatureHCN        = 319.38;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mCriticalTemperatureHe         = 5.1953;
/// @details  Reference: NIST Chemistry Webbook.
const double DefinedFluidProperties::mCriticalTemperatureXe         = 289.733;
/// @details  Reference: "USAF Propellant Handbooks, Nitric Acid/Nitrogen Tetroxide Oxidizers, Vol. II",
///           Martin Marietta Corp., Feb. 1977.
const double DefinedFluidProperties::mCriticalTemperatureN2O4       = 431.35;
/// @details  Reference: "USAF Propellant Handbooks, Nitric Acid/Nitrogen Tetroxide Oxidizers, Vol. II",
///           Martin Marietta Corp., Feb. 1977.
const double DefinedFluidProperties::mCriticalTemperatureN2H4       = 653.0;
/// @details  Reference: "USAF Propellant Handbooks, Hydrazine Fuels, Vol. I", Bell Aerospace Corp., Mar. 1970.
const double DefinedFluidProperties::mCriticalTemperatureCH3N2H3    = 585.0;
/// @details  Reference: 3M (TM) Novec (TM) 7000 Engineering Fluid Product Information.
const double DefinedFluidProperties::mCriticalTemperatureHFE7000    = 438.15;
/// @details  Reference: 3M (TM) Novec (TM) Engineered Fluid HFE-7100 for Heat Transfer.
const double DefinedFluidProperties::mCriticalTemperatureHFE7100    = 468.45;
/// @details  Reference: www.chemyq.com, average of propylene glycol & water critical temperatures
///           for a 30% solution.
const double DefinedFluidProperties::mCriticalTemperaturePG30       = 640.5122;
/// @details  Average of PG30 & PG50 values.
const double DefinedFluidProperties::mCriticalTemperaturePG40       = 638.3176;
/// @details  Reference: www.chemyq.com, average of propylene glycol & water critical temperatures
///           for a 50% solution.
const double DefinedFluidProperties::mCriticalTemperaturePG50       = 636.123;
/// @details  Until we can find complete data for the saturation curve of NaK-78, we are using the
///           values for H2O instead.  These properties should not be used as they will obviously
///           be wrong.
const double DefinedFluidProperties::mCriticalTemperatureNAK78      = 647.096;
/// @details  Lacking data, we just guess the same as water.
const double DefinedFluidProperties::mCriticalTemperatureGALDEN170  = 647.096;

/// @details  Reference: NIST Chemistry Webbook.  The distribution of data points is concentrated
///           more closely around the Helium critical point (T = 5.2 K, P = 228 kPa), near which
///           the data curvature is greatest.  The data points spread out more widely towards the
///           edges, where the fluid density is more linear.  Note the values in the ranges of
///           T 2.2 - 5.2 and P > 177735 represent liquid phase, and some were interpolated due to
///           NIST not providing values.
const double DefinedFluidProperties::mHeTemperatureScale[] =
            {2.1768,  3.68605, 4.7909,  5.1953,  24.3102, 80.9203, 172.850, 296.567, 447.316, 619.305, 805.923, 1000.0};
const double DefinedFluidProperties::mHePressureScale[] =
            {0.0,     70.5548, 134.203, 184.715, 217.145, 228.32,  2264.99, 8236.21, 17735.0, 30114.2, 44530.0, 60000.0};
const double DefinedFluidProperties::mHeDensityTable[] =
            {0.0, 1.473278e+02, 1.485356e+02, 1.494532e+02, 1.500251e+02, 1.502192e+02, 1.734820e+02, 2.059806e+02, 2.345207e+02, 2.594388e+02, 2.819551e+02, 3.034472e+02,  // T = 2.1768  K
             0.0, 1.339652e+02, 1.362311e+02, 1.378326e+02, 1.387897e+02, 1.391083e+02, 1.686366e+02, 2.027058e+02, 2.313511e+02, 2.559799e+02, 2.787945e+02, 3.002188e+02,  // T = 3.68605 K
             0.0, 8.198329e+00, 1.942553e+01, 1.127987e+02, 1.166740e+02, 1.177481e+02, 1.630140e+02, 1.998267e+02, 2.290307e+02, 2.534479e+02, 2.764807e+02, 2.978555e+02,  // T = 4.7909  K
             0.0, 7.339563e+00, 1.621677e+01, 2.707308e+01, 4.092678e+01, 6.678098e+01, 1.605468e+02, 1.986495e+02, 2.281814e+02, 2.525211e+02, 2.756338e+02, 2.969905e+02,  // T = 5.1953  K
             0.0, 1.396860e+00, 2.656320e+00, 3.655263e+00, 4.296293e+00, 4.517121e+00, 4.303683e+01, 1.175899e+02, 1.719181e+02, 2.087142e+02, 2.356039e+02, 2.561026e+02,  // T = 24.3102 K
             0.0, 4.192509e-01, 7.966278e-01, 1.095557e+00, 1.287216e+00, 1.353212e+00, 1.298420e+01, 4.291370e+01, 8.037543e+01, 1.168707e+02, 1.486875e+02, 1.751268e+02,  // T = 80.9203 K
             0.0, 1.963837e-01, 3.733417e-01, 5.136419e-01, 6.036548e-01, 6.346608e-01, 6.189361e+00, 2.144961e+01, 4.301894e+01, 6.717237e+01, 9.105099e+01, 1.128770e+02,  // T = 172.850 K
             0.0, 1.144893e-01, 2.177047e-01, 2.995728e-01, 3.521133e-01, 3.702144e-01, 3.637156e+00, 1.286504e+01, 2.656908e+01, 4.287828e+01, 6.002316e+01, 7.660122e+01,  // T = 296.567 K
             0.0, 7.591508e-02, 1.443711e-01, 1.986800e-01, 2.335390e-01, 2.455494e-01, 2.421074e+00, 8.650448e+00, 1.813155e+01, 2.977416e+01, 4.243294e+01, 5.507688e+01,  // T = 447.316 K
             0.0, 5.483619e-02, 1.042908e-01, 1.435294e-01, 1.687173e-01, 1.773960e-01, 1.752481e+00, 6.296107e+00, 1.330610e+01, 2.206841e+01, 3.178564e+01, 4.168597e+01,  // T = 619.305 K
             0.0, 4.214009e-02, 8.014745e-02, 1.103054e-01, 1.296652e-01, 1.363359e-01, 1.348376e+00, 4.860012e+00, 1.032188e+01, 1.722306e+01, 2.497067e+01, 3.296439e+01,  // T = 805.923 K
             0.0, 3.396250e-02, 6.459569e-02, 8.890337e-02, 1.045080e-01, 1.098850e-01, 1.087539e+00, 3.927829e+00, 8.368146e+00, 1.401736e+01, 2.041007e+01, 2.706089e+01}; // T = 1000    K

/// @details  Reference: NIST Chemistry Webbook.  The distribution of data points is concentrated
///           more closely around the Xenon critical point (T = 289.733, P = 5841.97), near which
///           the data curvature is greatest.  The data points spread out more widely towards the
///           edges, where the fluid density is more linear.  Above T ~650, the table agrees with
///           ideal gas law.
const double DefinedFluidProperties::mXeTemperatureScale[] =
            {170.0,   213.104, 246.629, 270.576, 284.944, 289.733, 302.518, 340.874, 404.800, 494.296, 609.363, 750.000};
const double DefinedFluidProperties::mXePressureScale[] =
            {0.0,     2103.11, 3738.86, 4907.25, 5608.29, 5841.97, 6637.30, 9023.28, 12999.9, 18567.2, 25725.2, 34473.8};
const double DefinedFluidProperties::mXeDensityTable[] =
            {0.0,     2918.95, 2927.18, 2932.94, 2936.35, 2937.48, 2941.29, 2952.47, 2970.34, 2993.91, 3021.82, 3052.36,  // T = 170.0   K
             0.0,     2602.12, 2617.62, 2628.21, 2634.39, 2636.42, 2643.23, 2662.81, 2692.93, 2730.92, 2774.25, 2820.91,  // T = 213.104 K
             0.0,     177.568, 2305.11, 2327.04, 2339.33, 2343.30, 2356.38, 2392.19, 2443.36, 2503.02, 2566.50, 2631.02,  // T = 246.629 K
             0.0,     147.517, 346.732, 2007.46, 2038.29, 2047.60, 2076.54, 2146.32, 2231.92, 2320.34, 2406.62, 2489.14,  // T = 270.576 K
             0.0,     135.497, 289.303, 488.494, 1689.94, 1728.16, 1814.06, 1953.30, 2083.26, 2199.88, 2305.19, 2401.30,  // T = 284.944 K
             0.0,     132.057, 276.736, 442.118, 649.175, 1118.36, 1670.03, 1874.56, 2028.62, 2157.58, 2270.40, 2371.57,  // T = 289.733 K
             0.0,     123.903, 250.444, 373.333, 473.438, 514.568, 714.515, 1591.23, 1866.25, 2038.76, 2175.07, 2291.17,  // T = 302.518 K
             0.0,     105.511, 201.749, 281.037, 334.051, 352.784, 421.089, 679.253, 1220.48, 1630.20, 1870.08, 2043.05,  // T = 340.874 K
             0.0,     85.7261, 157.983, 212.936, 247.341, 259.056, 299.877, 431.316, 676.857, 1028.05, 1372.54, 1637.78,  // T = 404.800 K
             0.0,     68.6000, 123.901, 164.425, 189.138, 197.441, 225.939, 313.502, 464.931, 680.754, 942.001, 1207.43,  // T = 494.296 K
             0.0,     54.9225, 98.1745, 129.318, 148.092, 154.364, 175.756, 240.293, 348.504, 499.207, 686.197, 895.581,  // T = 609.363 K
             0.0,     44.3181, 78.8113, 103.442, 118.211, 123.133, 139.873, 189.979, 272.895, 387.022, 528.823, 692.003}; // T = 750.000 K

/// @details  Reference: NIST Chemistry Webbook.  The temperature and pressure scales are evenly
///           distributed.
const double DefinedFluidProperties::mN2TemperatureScale[] =
            {160.0,   213.636, 267.273, 320.909, 374.545, 428.182, 481.818, 535.455, 589.091, 642.727, 696.364, 750.0};
const double DefinedFluidProperties::mN2PressureScale[] =
            {0.0,     4545.45, 9090.9,  13636.4, 18181.8, 22727.3, 27272.7, 31818.2, 36363.6, 40909.1, 45454.5, 50000.0, 54545.5, 59090.9};
const double DefinedFluidProperties::mN2DensityTable[] =
            {0.0,     123.865, 311.591, 435.162, 500.062, 542.473, 573.98,  599.137, 620.151, 638.253, 654.193, 668.464, 681.406, 693.263,  // T = 160.0   K
             0.0,     76.8427, 160.798, 242.321, 312.211, 368.39,  413.222, 449.66,  480.01,  505.871, 528.335, 548.163, 565.896, 581.93,   // T = 213.636 K
             0.0,     58.3424, 117.166, 173.802, 226.019, 272.678, 313.684, 349.55,  380.999, 408.749, 433.43,  455.564, 475.576, 493.803,  // T = 267.273 K
             0.0,     47.602,  94.2073, 138.779, 180.543, 219.078, 254.281, 286.28,  315.328, 341.732, 365.801, 387.823, 408.056, 426.724,  // T = 320.909 K
             0.0,     40.403,  79.4675, 116.738, 151.905, 184.809, 215.418, 243.798, 270.074, 294.407, 316.966, 337.921, 357.431, 375.643,  // T = 374.545 K
             0.0,     35.1826, 69.0144, 101.287, 131.875, 160.723, 187.839, 213.274, 237.11,  259.447, 280.393, 300.058, 318.547, 335.96,   // T = 428.182 K
             0.0,     31.1998, 61.139,  89.7252, 116.912, 142.694, 167.093, 190.155, 211.944, 232.529, 251.987, 270.395, 287.828, 304.358,  // T = 481.818 K
             0.0,     28.0503, 54.9571, 80.6849, 105.226, 128.594, 150.819, 171.944, 192.017, 211.095, 229.235, 246.494, 262.93,  278.597,  // T = 535.455 K
             0.0,     25.4918, 49.9573, 73.3899, 95.8,    117.21,  137.652, 157.164, 175.788, 193.569, 210.552, 226.783, 242.306, 257.165,  // T = 589.091 K
             0.0,     23.3693, 45.8202, 67.3608, 88.0099, 107.793, 126.742, 144.89,  162.274, 178.93,  194.897, 210.212, 224.91,  239.026,  // T = 642.727 K
             0.0,     21.5784, 42.3345, 62.2837, 81.4481, 99.8539, 117.53,  134.508, 150.818, 166.492, 181.562, 196.058, 210.012, 223.45,   // T = 696.364 K
             0.0,     20.0459, 39.3539, 57.9427, 75.8351, 93.0564, 109.633, 125.593, 140.963, 155.772, 170.044, 183.808, 197.089, 209.91 }; // T = 750.000 K

/// @details  Reference: NIST Chemisty Webbook.  The temperature and pressure scales are evenly
///           distributed.
const double DefinedFluidProperties::mO2TemperatureScale[] =
            {160.0,   213.636, 267.273, 320.909, 374.545, 428.182, 481.818, 535.455, 589.091, 642.727, 696.364, 750.0};
const double DefinedFluidProperties::mO2PressureScale[] =
            {0.0,     4545.45, 9090.9,  13636.4, 18181.8, 22727.3, 27272.7, 31818.2, 36363.6, 40909.1, 45454.5, 50000.0, 54545.5, 59090.9};
const double DefinedFluidProperties::mO2DensityTable[] =
            {0.0,     170.956, 692.903, 776.857, 824.302, 858.682, 886.107, 909.143, 929.126, 946.847, 962.817, 977.386, 990.804, 1003.26,  // T = 160.0   K
             0.0,     92.0654, 208.324, 341.79,  461.592, 550.727, 615.778, 665.461, 705.239, 738.302, 766.57,  791.267, 813.208, 832.96,   // T = 213.636 K
             0.0,     68.5238, 142.423, 219.084, 294.36,  364.204, 426.301, 480.28,  526.92,  567.372, 602.753, 634.008, 661.892, 687.0,    // T = 267.273 K
             0.0,     55.4577, 112.188, 169.001, 224.54,  277.575, 327.229, 373.05,  414.959, 453.127, 487.86,  519.515, 548.447, 574.989,  // T = 320.909 K
             0.0,     46.8626, 93.6751, 139.848, 184.794, 228.011, 269.126, 307.917, 344.295, 378.279, 409.963, 439.483, 467.0,   492.68,   // T = 374.545 K
             0.0,     40.6932, 80.8557, 120.178, 158.372, 195.202, 230.492, 264.134, 296.078, 326.327, 354.92,  381.925, 407.425, 431.509,  // T = 428.182 K
             0.0,     36.0167, 71.3345, 105.785, 139.219, 171.517, 202.592, 232.39,  260.89,  288.097, 314.034, 338.742, 362.272, 384.681,  // T = 481.818 K
             0.0,     32.3348, 63.9314, 94.6967, 124.551, 153.433, 181.299, 208.125, 233.904, 258.644, 282.363, 305.092, 326.865, 347.721,  // T = 535.455 K
             0.0,     29.3534, 57.9841, 85.8413, 112.883, 139.077, 164.405, 188.857, 212.434, 235.147, 257.011, 278.051, 298.294, 317.768,  // T = 589.091 K
             0.0,     26.886,  53.0879, 78.5789, 103.338, 127.351, 150.61,  173.115, 194.871, 215.89,  236.187, 255.781, 274.694, 292.949,  // T = 642.727 K
             0.0,     24.8079, 48.9786, 72.4994, 95.3613, 117.56,  139.094, 159.968, 180.191, 199.772, 218.727, 237.072, 254.825, 272.006,  // T = 696.364 K
             0.0,     23.0325, 45.476,  67.3262, 88.5811, 109.242, 129.311, 148.796, 167.705, 186.05,  203.843, 221.099, 237.834, 254.064}; // T = 750.000 K

/// @details  Reference: NIST Chemisty Webbook.  The temperature and pressure scales are evenly
///           distributed.
const double DefinedFluidProperties::mH2TemperatureScale[] =
            {64.0,  142.0,   220.0,   298.0,   376.0,   454.0,   532.0,   610.0,   688.0,   766.0,   844.0,   922.0,  1000.0};
const double DefinedFluidProperties::mH2PressureScale[] =
            {0.0, 6666.67, 13333.3, 20000.0, 26666.7, 33333.3, 40000.0, 46666.7, 53333.3, 60000.0, 66666.7, 73333.3, 80000.0};
const double DefinedFluidProperties::mH2DensityTable[] =
            {0.0, 28.4216, 47.3743, 57.4974, 64.1413, 69.1038, 73.0943, 76.4540, 79.3710, 81.9592, 84.2930, 86.4234, 88.3871,   // T =   64.0 K
             0.0, 10.8824, 20.4408, 28.5558, 35.3990, 41.2067, 46.1947, 50.5367, 54.3663, 57.7846, 60.8684, 63.6764, 66.2540,   // T =  142.0 K
             0.0, 7.01950, 13.3806, 19.1107, 24.2693, 28.9258, 33.1467, 36.9909, 40.5096, 43.7463, 46.7378, 49.5155, 52.1057,   // T =  220.0 K
             0.0, 5.21690, 10.0369, 14.4884, 18.6038, 22.4163, 25.9577, 29.2567, 32.3391, 35.2273, 37.9412, 40.4984, 42.9140,   // T =  298.0 K
             0.0, 4.15958, 8.05492, 11.7065, 15.1343, 18.3574, 21.3942, 24.2611, 26.9733, 29.5444, 31.9865, 34.3103, 36.5257,   // T =  376.0 K
             0.0, 3.46141, 6.73522, 9.83564, 12.7758, 15.5680, 18.2238, 20.7538, 23.1676, 25.4742, 27.6815, 29.7968, 31.8267,   // T =  454.0 K
             0.0, 2.96500, 5.79056, 8.48663, 11.0623, 13.5260, 15.8854, 18.1478, 20.3198, 22.4076, 24.4167, 26.3523, 28.2189,   // T =  532.0 K
             0.0, 2.59359, 5.07986, 7.46597, 9.75839, 11.9631, 14.0856, 16.1311, 18.1043, 20.0096, 21.8511, 23.6325, 25.3573,   // T =  610.0 K
             0.0, 2.30509, 4.52531, 6.66591, 8.73165, 10.7269, 12.6558, 14.5221, 16.3293, 18.0806, 19.7792, 21.4279, 23.0292,   // T =  688.0 K
             0.0, 2.07447, 4.08032, 6.02148, 7.90156, 9.72387, 11.4915, 13.2073, 14.8739, 16.4939, 18.0695, 19.6030, 21.0965,   // T =  766.0 K
             0.0, 1.88585, 3.71521, 5.49108, 7.21624, 8.89323, 10.5244, 12.1121, 13.6582, 15.1649, 16.6338, 18.0667, 19.4653,   // T =  844.0 K
             0.0, 1.72871, 3.41018, 5.04678, 6.64066, 8.19382, 9.70813, 11.1854, 12.6272, 14.0350, 15.4105, 16.7549, 18.0695,   // T =  922.0 K
             0.0, 1.59575, 3.15151, 4.66912, 6.15031, 7.59666, 9.00969, 10.3908, 11.7413, 13.0625, 14.3554, 15.6214, 16.8613};  // T = 1000.0 K

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Predefined Fluid Properties. Initializes the array of fluid
///           properties, indexed by fluid type, specifying:
///           - Type of fluid (enumeration of chemical compositions/names)
///           - Fluid phase (enumeration of gas, liquid or solid)
///           - Molecular weight (1/mol)
///           - Critical temperature Tc (K)
///           - Curve fit for density (kg/m3) as a function of temperature and pressure
///           - Curve fit for dynamic viscosity (Pa*s) as a function of temperature and pressure
///           - Curve fit for specific heat (J/kg/K) as a function of temperature and pressure
///           - Curve fit for thermal conductivity (W/m/K) as a function of temperature and pressure
///           - Curve fit for Prandtl number -- as a function of temperature and pressure
///           - Curve fit for adiabatic index -- as a function of temperature and pressure
///           - Curve fit for pressure (kPa) as a function of temperature and density
///           - Curve fit for temperature (K) as a function of specific enthalpy and pressure
///           - Curve fit for log10 of saturation pressure (kPa) as a function of inverse of reduced
///             temperature (Tc/T)
///           - Curve fit for reduced saturation temperature as a function of saturation pressure
///           - Curve fit for latent heat of vaporization as a function of reduced temperature
///           GUNNS demands an exactly invertible specific heat curve fit of the form
///           - Cp  = a + bT (for both liquids and gases)
///           GUNNS demands an exactly invertible density curve fit
///           - rho = a P / T (for ideal gases)
///           - rho = (a + b*P) + (c + d*P)*T + (e + f*P)*T^2 (for liquids)
///           - rho = f(T,P)  (table lookup for real gases)
///           all of which have exact (isothermal) inverses between pressure and density, so
///           - rho = f(T, <P = f(T, rho)>)
///           GUNNS demands exactly invertible saturation pressure & temperature curve fits
///           - Ts = f(Ps = f(Ts)
////////////////////////////////////////////////////////////////////////////////////////////////////
DefinedFluidProperties::DefinedFluidProperties()
    :
    /************************************************************************
                 Universal gas constant
    ************************************************************************/
    mGasK(UnitConversion::UNIV_GAS_CONST_SI),

    /************************************************************************
                   Properties for carbon monoxide
    ************************************************************************/
    mDensityCO(mMWeightCO / mGasK, 68.16, 2000.0, 1.0e-64, 1.0e+06),
    mViscosityCO(3.14819375e-6, 4.65571250e-8, 68.16, 2000.0),
    mSpecificHeatCO(1.06251339e+3, -3.04107143e-2, 68.16, 2000.0),
    mThermalConductivityCO(3.96573214e-3, 7.27682143e-5, 68.160, 2000.0),
    mPrandtlNumberCO(7.36597866e-1, -1.11462612e-4, 68.16, 2000.0),
    mAdiabaticIndexCO(1.43242613, -9.44340853e-5, 68.16, 2000.0),
    mPressureCO(mGasK / mMWeightCO, 68.16, 2000.00, 1.0e-64, 1.0e+06),
    mTemperatureCO(1.06251339e+3, -3.04107143e-2, 1.0e+2, 3.0e+06),
    mSaturationPressureCO(5.904077, -2.314267, -0.05184318, 1.0, mCriticalTemperatureCO/68.16),
    mSaturationTemperatureCO(5.904077, -2.314267, -0.05184318, 1.570513e1, 3.451170e3),
    mHeatOfVaporizationCO(282.398, -0.191, 0.409709, 68.16/mCriticalTemperatureCO, 1.0),


    /************************************************************************
                   Properties for carbon dioxide
    ************************************************************************/
    mDensityCO2(mMWeightCO2 / mGasK, 100.0, 2000.0, 1.0e-64, 1.0e+06),
    mViscosityCO2(3.88349774e-6, 3.68261797e-8, 100.0, 2000.0),
    mSpecificHeatCO2(6.94715529e+2, 5.22732574e-1, 100.0, 2000.0),
    mThermalConductivityCO2(-5.52142048e-3, 7.51590898e-5, 100.0, 2000.0),
    mPrandtlNumberCO2(7.85757975e-1, -6.53671274e-5, 100.0, 2000.0),
    mAdiabaticIndexCO2(1.35140794, -1.67755018e-4, 100.0, 2000.0),
    mPressureCO2(mGasK / mMWeightCO2, 100.0, 2000.0, 1.0e-64, 1.0e+06),
    mTemperatureCO2(6.94715529e+2, 5.22732574e-1, 1.0e+2, 1.0e+07),
    mSaturationPressureCO2(6.671213, -2.778967, -0.02691220, 1.0, mCriticalTemperatureCO2/100.0),
    mSaturationTemperatureCO2(6.671213, -2.778967, -0.02691220, 9.347056e-3, 7.333882e3),
    mHeatOfVaporizationCO2(573.863, -0.014, 0.404154, 100.0/mCriticalTemperatureCO2, 1.0),


    /************************************************************************
                   Properties for water
    ************************************************************************/
    mDensityH2O(mMWeightH2O / mGasK, 100.0, 2000.0, 1.0e-64, 1.0e+06),
    mViscosityH2O(-2.48944407e-6, 3.94663357e-8, 100.0, 2000.0),
    mSpecificHeatH2O(1.81255704e+3, 5.15364152e-1, 100.0, 2000.0),
    mThermalConductivityH2O(-0.00999, 0.0001, 100.0, 2000.0),
    mPrandtlNumberH2O(1.05092089, 1.47582216e-4, 100.0, 2000.0),
    mAdiabaticIndexH2O(1.37818452, -1.18125416e-4, 100.0, 2000.0),
    mPressureH2O(mGasK / mMWeightH2O, 100.0, 2000.0, 1.0e-64, 1.0e+06),
    mTemperatureH2O(1.81255704e+3, 5.15364152e-1, 1.0e+2, 1.0e+07),
    mSaturationPressureH2O(7.105911, -2.543893, -0.2302306, 1.0, mCriticalTemperatureH2O/100.0),
    mSaturationTemperatureH2O(7.105911, -2.543893, -0.2302306, 1.009159e-19, 2.146776e4),
    mHeatOfVaporizationH2O(2899.80, -0.178, 0.406871, 100.0/mCriticalTemperatureH2O, 1.0),


    /************************************************************************
                   Properties for nitrogen
    ************************************************************************/
    mDensityN2(mMWeightN2 / mGasK, 64.0, 2000.0, 1.0e-64, 1.0e+06),
    mViscosityN2(8.11293165e-6, 2.88907996e-8, 64.0, 2000.0),
    mSpecificHeatN2(1.01354736e+3, 1.31741253e-1, 64.0, 2000.0),
    mThermalConductivityN2(9.09145525e-3, 5.17493648e-5, 64.0, 2000.0),
    mPrandtlNumberN2(7.25068578e-1, 9.66001186e-6, 64.0, 2000.0),
    mAdiabaticIndexN2(1.42161068e+0, -6.08287084e-5, 64.0, 2000.0),
    mPressureN2(mGasK / mMWeightN2, 64.0, 2000.0, 1.0e-64, 1.0e+06),
    mTemperatureN2(1.01354736e+3, 1.31741253e-1, 1.0e+2, 1.0e+07),
    mSaturationPressureN2(5.891833, -2.335710, -0.03054780, 1.0, mCriticalTemperatureN2/64.0),
    mSaturationTemperatureN2(5.891833, -2.335710, -0.03054780, 1.471071e1, 3.354095e3),
    mHeatOfVaporizationN2(254.280, -0.245, 0.415032, 64.0/mCriticalTemperatureN2, 1.0),


    /************************************************************************
                   Properties for oxygen
    ************************************************************************/
    mDensityO2(mMWeightO2 / mGasK, 54.5, 2000.0, 1.0e-64, 1.0e+06),
    mViscosityO2(6.30712930E-6, 4.40945750E-008, 54.5, 2000.0),
    mSpecificHeatO2(8.74505007E+2, 2.01386520E-1 , 54.5, 2000.0),
    mThermalConductivityO2(3.03907634E-3, 7.72063714E-5, 54.50, 2000.0),
    mPrandtlNumberO2(7.48579950E-1, -8.93526407E-5, 54.5, 2000.0),
    mAdiabaticIndexO2(1.43468762E+0, -1.24059159E-4, 54.5, 2000.0),
    mPressureO2(mGasK / mMWeightO2, 54.5, 2000.0, 1.0e-64, 1.0e+06),
    mTemperatureO2(8.74505007E+2, 2.01386520E-1, 1.0e+2, 1.0e+07),
    mSaturationPressureO2(5.923722, -2.154806, -0.07725064, 1.0, mCriticalTemperatureO2/54.5),
    mSaturationTemperatureO2(5.923722, -2.154806, -0.07725064, 1.550494e-1, 4.916608e3),
    mHeatOfVaporizationO2(268.086, -0.217, 0.405495, 54.5/mCriticalTemperatureO2, 1.0),


    /************************************************************************
                   Properties for ammonia
    ************************************************************************/
    mDensityNH3(mMWeightNH3 / mGasK, 100.0, 2000.0, 1.0e-64, 1.0e+06),
    mViscosityNH3(-1.10690850e-6, 3.77945896e-8, 100.0, 2000.0),
    mSpecificHeatNH3(1.77245664e+3, 1.55734170, 100.0, 2000.0),
    mThermalConductivityNH3(-1.17887603e-2, 1.26920717e-4, 100.0, 2000.0),
    mPrandtlNumberNH3(8.24613070e-1, 1.63210100e-4, 100.0, 2000.0),
    mAdiabaticIndexNH3(1.41192763, -3.10465770e-4, 100.0, 2000.0),
    mPressureNH3(mGasK / mMWeightNH3, 100.0, 2000.0, 1.0e-64, 1.0e+06),
    mTemperatureNH3(1.77245664e+3, 1.55734170, 1.0e+2, 2.0e+07),
    mSaturationPressureNH3(6.686549, -2.451487, -0.1891818, 1.0, mCriticalTemperatureNH3/100.0),
    mSaturationTemperatureNH3(6.686549, -2.451487, -0.1891818, 4.355480e-7, 1.111426e4),
    mHeatOfVaporizationNH3(1795.73, -0.202, 0.437261, 100.0/mCriticalTemperatureNH3, 1.0),


    /************************************************************************
                   Properties for hydrogen
    ************************************************************************/
    mDensityH2(mMWeightH2 / mGasK, 13.957, 2000.0, 1.0e-64, 1.0e+06),
    mViscosityH2(3.44431431e-6, 1.72499615e-8, 13.957, 2000.0),
    mSpecificHeatH2(8.16682118e+3, 2.11904814e+1, 13.957, 2000.0),
    mThermalConductivityH2(3.20511417e-2, 4.96902311e-4, 13.957, 2000.0),
    mPrandtlNumberH2(5.40516454e-1, 5.78550413e-4, 13.957, 2000.0),
    mAdiabaticIndexH2(1.48410357e+0, -1.39888479e-4, 13.957, 2000.0),
    mPressureH2(mGasK / mMWeightH2, 13.957, 2000.0, 1.0e-64, 1.0e+06),
    mTemperatureH2 (8.16682118e+3, 2.11904814e+1, 1.0e+2, 2.0e+08),
    mSaturationPressureH2(5.137677, -2.211171, 0.1752336, 1.0, mCriticalTemperatureH2/13.957),
    mSaturationTemperatureH2(5.137677, -2.211171, 0.1752336, 7.496468e0, 1.263978e3),
    mHeatOfVaporizationH2(402.392, -0.893, 0.468214, 13.957/mCriticalTemperatureH2, 1.0),


    /************************************************************************
                   Properties for methane
    ************************************************************************/
    mDensityCH4(mMWeightCH4 / mGasK, 91.0, 2000.0, 1.0e-64, 1.0e+06),
    mViscosityCH4(-9.69180981e-8, 4.59056870e-8, 91.0, 2000.0),
    mSpecificHeatCH4(1.57965714e+3, 3.35717653e+0, 91.0, 2000.0),
    mThermalConductivityCH4(-1.85036584e-2, 2.33789538e-4, 91.0, 2000.0),
    mPrandtlNumberCH4(8.34043842e-1, -3.47214804e-4, 91.0, 2000.0),
    mAdiabaticIndexCH4(1.43685484e+0, -5.64753568e-4, 91.0, 2000.0),
    mPressureCH4(mGasK / mMWeightCH4, 91.0, 2000.0, 1.0e-64, 1.0e+06),
    mTemperatureCH4(1.57965714e+3, 3.35717653e+0, 1.0e+2, 2.0e+07),
    mSaturationPressureCH4(5.978649, -2.316399, -0.008641538, 1.0, mCriticalTemperatureCH4/91.0),
    mSaturationTemperatureCH4(5.978649, -2.316399, -0.008641538, 1.230152e1, 4.504100e3),
    mHeatOfVaporizationCH4(622.485, -0.272, 0.408189, 91.0/mCriticalTemperatureCH4, 1.0),


    /************************************************************************
                   Properties for hydrochloric acid
    ************************************************************************/
    mDensityHCL(mMWeightHCL / mGasK, 100.0, 2000.0, 1.0e-64, 1.0e+06),
    mViscosityHCL(2.20054874e-6, 1.62474889e-9, 100.0, 2000.0),
    mSpecificHeatHCL(7.72414550e+2, 1.00351086e-1, 100.0, 2000.0),
    mThermalConductivityHCL(5.52005444e-4, 4.61274044e-5, 100.0, 2000.0),
    mPrandtlNumberHCL(0.86, 0.0, 100.0, 2000.0),
    mAdiabaticIndexHCL (1.4, 0.0, 100.0, 2000.0),
    mPressureHCL(mGasK / mMWeightHCL, 100.0, 2000.0, 1.0e-64, 1.0e+06),
    mTemperatureHCL(7.72414550e+2, 1.00351086e-1, 1.0e+2, 1.0e+07),
    mSaturationPressureHCL(6.423687, -2.436504, -0.07698240, 1.0, mCriticalTemperatureHCL/100.0),
    mSaturationTemperatureHCL(6.423687, -2.436504, -0.07698240, 5.027122e-3, 8.132054e3),
    mHeatOfVaporizationHCL(816.0349, 0.4209399, 0.4209399, 100.0/mCriticalTemperatureHCL, 1.0),


    /************************************************************************
                   Properties for hydrogen cyanide
    ************************************************************************/
    mDensityHCN(mMWeightHCN / mGasK, 100.0, 2000.0, 1.0e-64, 1.0e+06),
    mViscosityHCN(-1.4e-6, 1.43666098e-8, 100.0, 2000.0),
    mSpecificHeatHCN(1.13178924e+3, 5.23263873e-1, 100.0, 2000.0),
    mThermalConductivityHCN(-7.8e-3, 7.82792640e-5, 100.0, 2000.0),
    mPrandtlNumberHCN(0.79, 0.0, 100.0, 2000.0),
    mAdiabaticIndexHCN(1.3, 0.0, 100.0, 2000.0),
    mPressureHCN(mGasK / mMWeightHCN, 100.0, 2000.0, 1.0e-64, 1.0e+06),
    mTemperatureHCN(1.13178924e+3, 5.23263873e-1, 1.0e+2, 1.0e+07),
    mSaturationPressureHCN(6.665582, -4.175167, -0.1724629, 1.0, mCriticalTemperatureHCN/100.0),
    mSaturationTemperatureHCN(6.665582, -4.175167, -0.1724629, 3.730347e-9, 2.079466e2),
    mHeatOfVaporizationHCN(1309.447, 0.09207613, 0.09207613, 100.0/mCriticalTemperatureHCN, 1.0),


    /************************************************************************
                   Properties for helium
    ************************************************************************/
    mDensityHe(mMWeightHe / mGasK, 2.1768, 1500.0, 1.0e-64, 1.0e+06),
    mViscosityHe(3.904441e-6, 5.985034e-8, -2.490652e-11, 7.095312e-15, 2.1768, 1500.0),
    mSpecificHeatHe(2.079064e4, -6.270587e-3, 2.1768, 1500.0),
    mThermalConductivityHe(2.710026e-2, 4.852526e-4, -2.162608e-7, 6.251157e-11, 2.1768, 1500.0),
    mPrandtlNumberHe(2.808356, -7.476181e-4, 1.00141e-6, -3.774658e-10, 2.1768, 1500.0),
    mAdiabaticIndexHe(1.6667, 0.0, 2.1768, 1500.0),
    mPressureHe(mGasK / mMWeightHe, 2.1768, 1500.0, 1.0e-64, 1.0e+06),
    mTemperatureHe(2.079064e4, -6.270587e-3, 1.0e+2, 1.0e+08),
    mSaturationPressureHe(4.055375, -1.930662, 0.2191484, 1.0, mCriticalTemperatureHe/2.1768),
    mSaturationTemperatureHe(4.055375, -1.930662, 0.2191484, 4.964031e0, 2.207300e2),
    mHeatOfVaporizationHe(19.800, -0.927, 0.421274, 2.1768/mCriticalTemperatureHe, 1.0),


    /************************************************************************
                   Properties for helium (real-gas compressibility)
    ************************************************************************/
    mDensityHeReal(mHeTemperatureScale, mHePressureScale, mHeDensityTable, 12, 12, 2.1768, 1000.0, 0.0, 60000.0),
    mPressureHeReal(mHeTemperatureScale, mHePressureScale, mHeDensityTable, 12, 12, 2.1768, 1000.0, 0.0, 60000.0),


    /************************************************************************
                   Properties for xenon (real-gas compressibility)
    ************************************************************************/
    mDensityXe(mXeTemperatureScale, mXePressureScale, mXeDensityTable, 12, 12, 170.0, 750.0, 0.0, 34473.8),
    mViscosityXe(3.20493e-6, 6.57203e-8, 100.0, 2000.0),
    mSpecificHeatXe(1.69768e+2, -2.09841e-2, 100.0, 2000.0),
    mThermalConductivityXe(7.62627e-4, 1.57374e-5, 100.0, 2000.0),
    mPrandtlNumberXe(7.14862e-1, -1.00208e-4, 100.0, 2000.0),
    mAdiabaticIndexXe(1.70353, -6.22410e-5, 100.0, 2000.0),
    mPressureXe(mXeTemperatureScale, mXePressureScale, mXeDensityTable, 12, 12, 170.0, 750.0, 0.0, 34473.8),
    mTemperatureXe(1.69768e+2, -2.09841e-2, 1.0e+2, 3.0e+05),
    mSaturationPressureXe(6.141935, -2.416296, 0.03483905, 1.0, mCriticalTemperatureXe/100.0),
    mSaturationTemperatureXe(6.141935, -2.416296, 0.03483905, 2.711090e-1, 5.754884e3),
    mHeatOfVaporizationXe(115.869, -0.281, 0.418031, 100.0/mCriticalTemperatureXe, 1.0),


    /************************************************************************
                   Properties for nitrogen (real-gas compressibility)
    ************************************************************************/
    mDensityN2Real (mN2TemperatureScale, mN2PressureScale, mN2DensityTable, 12, 14, 160.0, 750.0, 0.0, 59090.9),
    mPressureN2Real(mN2TemperatureScale, mN2PressureScale, mN2DensityTable, 12, 14, 160.0, 750.0, 0.0, 59090.9),

    /************************************************************************
                   Properties for oxygen (real-gas compressibility)
    ************************************************************************/
    mDensityO2Real (mO2TemperatureScale, mO2PressureScale, mO2DensityTable, 12, 14, 160.0, 750.0, 0.0, 59090.9),
    mPressureO2Real(mO2TemperatureScale, mO2PressureScale, mO2DensityTable, 12, 14, 160.0, 750.0, 0.0, 59090.9),

    /************************************************************************
                   Properties for hydrogen (real-gas compressibility)
    ************************************************************************/
    mDensityH2Real (mH2TemperatureScale, mH2PressureScale, mH2DensityTable, 13, 13, 64.0, 1000.0, 0.0, 80000.0),
    mPressureH2Real(mH2TemperatureScale, mH2PressureScale, mH2DensityTable, 13, 13, 64.0, 1000.0, 0.0, 80000.0),

    /************************************************************************
                   Properties for N2O4
    ************************************************************************/
    mSaturationPressureN2O4(8.949029, -5.406766, 0.4546795, 1.0, mCriticalTemperatureN2O4/261.95),
    mSaturationTemperatureN2O4(8.949029, -5.406766, 0.4546795, 18.996, 9929.85),
    mHeatOfVaporizationN2O4(1258.40074661114, -2.91735422884232, 261.95, 431.35),

    /************************************************************************
                   Properties for N2H4
    ************************************************************************/
    mSaturationPressureN2H4(6.968218448, -2.586591091, -0.211863158, 1.0, mCriticalTemperatureN2H4/274.68),
    mSaturationTemperatureN2H4(6.968218448, -2.586591091, -0.211863158, 0.418, 14784.0),
    mHeatOfVaporizationN2H4(1859.5, -1.644, 274.68, 387.37),

    /************************************************************************
                   Properties for CH3N2H3
    ************************************************************************/
    mSaturationPressureCH3N2H3(6.263433, -1.923808, -0.434885, 1.0, mCriticalTemperatureCH3N2H3/220.0),
    mSaturationTemperatureCH3N2H3(6.263433, -1.923808, -0.434885, 0.011827, 8030.46),
    mHeatOfVaporizationCH3N2H3(1788.0772786368, -3.05654235664393, 220.0, 585.0),

    /************************************************************************
                   Properties for liquid water
    ************************************************************************/
    mDensityWATER(7.971248e+2, 7.557045e-3, 1.591560, -4.273648e-5, -3.100791e-3,
                  6.311271e-8, 200.0, 470.0, 1.0e-64, 1.0e+06),
    mViscosityWATER(4.702520e-1, -5.996482e-3, 3.061785e-5, -7.808668e-8, 9.936067e-11, -5.042842e-14, 200.0, 470.0),
    mSpecificHeatWATER(4.16924282e+3, 7.58586482e-2, 200.0, 470.0),
    mThermalConductivityWATER(2.49898518E-1, 1.187923e-3, 200.0, 470.0),
    mPrandtlNumberWATER(4.154213e+3, -5.320634e+1, 2.725191e-1, -6.966592e-4, 8.880820e-7, -4.513810e-10, 200.0, 470.0),
    mAdiabaticIndexWATER(0.64975127, 1.227552e-3, 200.0, 470.0),
    mPressureWATER(7.971248e+2, 7.557045e-3, 1.591560, -4.273648e-5, -3.100791e-3,
                  6.311271e-8, 200.0, 470.0, 1.0e-64, 1.0e+06),
    mTemperatureWATER(4.16924282e+3, 7.58586482e-2, 1.0e+02, 1.0e+07),


    /************************************************************************
                   Properties for liquid HFE7000
    ************************************************************************/
    mDensityHFE7000 (1.4 * 7.971248e+2, 1.4 * 7.557045e-3, 1.4 * 1.591560, -1.4 * 4.273648e-5,
                     -1.4 * 3.100791e-3, 1.4 * 6.311271e-8, 150.65, 470.0, 1.0e-64, 1.0e+06),
    mViscosityHFE7000(1.775403E-001, -8.381752E-004, -2.503906E-006, 1.126480E-008, 150.65, 320.0),
    mSpecificHeatHFE7000(3.8181606E+2, 3.08030000E+0, 150.65, 470.0),
    mThermalConductivityHFE7000(1.3333740e-1, -1.9600000E-4, 150.65, 470.0),
    mPrandtlNumberHFE7000(7.5, 0.0, 150.65, 470.0),
    mAdiabaticIndexHFE7000(1.4, 0.0, 150.65, 470.0),
    mPressureHFE7000(1.4 * 7.971248e+2, 1.4 * 7.557045e-3, 1.4 * 1.591560, -1.4 * 4.273648e-5,
                     -1.4 * 3.100791e-3, 1.4 * 6.311271e-8, 150.65, 470.0, 1.0e-64, 1.0e+06),
    mTemperatureHFE7000(3.8181606E+2, 3.08030000E+0, 1.0e+2, 2.0e+06),
    mSaturationPressureHFE7000(3.560395, 2.010689, -2.176239, 1.0, mCriticalTemperatureHFE7000/150.65),
    mSaturationTemperatureHFE7000(3.560395, 2.010689, -2.176239, 9.999151e-10, 2.482244e3),
    mHeatOfVaporizationHFE7000(155.6048, 0.04794244, 0.04794244, 150.65/mCriticalTemperatureHFE7000, 1.0),


    /************************************************************************
                   Properties for liquid HFE7100
    ************************************************************************/
    mDensityHFE7100 (2.157312e3, 7.557045e-3, -2.26467e0, -4.273648e-5, -6.394895e-6, 6.311271e-8,
                     138.15, mCriticalTemperatureHFE7100, 1.0e-64, 1.0e+06),
    mViscosityHFE7100(1.074738E-001, -1.0435097E-003, 3.393889E-006, -3.679803E-009, 138.15, 334.15),
    mSpecificHeatHFE7100(5.867E+2, 2.0E+0, 138.15, mCriticalTemperatureHFE7100),
    mThermalConductivityHFE7100(1.271094e-1, -1.9548E-4, 138.15, mCriticalTemperatureHFE7100),
    mPrandtlNumberHFE7100(8.453566e2, -7.627354e0, 2.298012e-2, -2.285688e-5, 138.15, 334.15),
    mAdiabaticIndexHFE7100(1.4, 0.0, 138.15, mCriticalTemperatureHFE7100),
    mPressureHFE7100(2.157312e3, 7.557045e-3, -2.26467e0, -4.273648e-5, -6.394895e-6, 6.311271e-8,
                     138.15, mCriticalTemperatureHFE7100, 1.0e-64, 1.0e+06),
    mTemperatureHFE7100(5.867E+2, 2.0E+0, 1.0e+2, 2.0e+06),
    mSaturationPressureHFE7100(6.741905, -3.38726, 0.003905253, 1.0, mCriticalTemperatureHFE7100/138.15),
    mSaturationTemperatureHFE7100(6.741905, -3.38726, 0.003905253, 1.99994e-5, 2.283231e3),
    mHeatOfVaporizationHFE7100(130.55269, 0.07991991, 0.07991991, 138.15/mCriticalTemperatureHFE7100, 1.0),


    /************************************************************************
                   Properties for liquid propylene glycol 30%
    ************************************************************************/
    mDensityPG30(1.026482e3, 7.557045e-3, 3.783358e-1, -4.273648e-5, -1.392346e-3, 6.311271e-8,
                 260.45, mCriticalTemperaturePG30, 1.0e-64, 1.0e+06),
    mViscosityPG30(4.729396e2, 9.608948e-1, 260.45, mCriticalTemperaturePG30),
    mSpecificHeatPG30(3.520761e3, 1.184836, 260.45, mCriticalTemperaturePG30),
    mThermalConductivityPG30(1.294089e-1, 1.561264e-3, -1.392068e-6, 260.45, mCriticalTemperaturePG30),
    mPrandtlNumberPG30(5.439379e6, 9.598727e-1, 260.45, mCriticalTemperaturePG30),
    mAdiabaticIndexPG30(1.4, 0.0, 260.45, mCriticalTemperaturePG30),
    mPressurePG30(1.026482e3, 7.557045e-3, 3.783358e-1, -4.273648e-5, -1.392346e-3, 6.311271e-8,
                  260.45, mCriticalTemperaturePG30, 1.0e-64, 1.0e+06),
    mTemperaturePG30(3.520761e3, 1.184836, 1.0e+2, 1.0e+07),
    mSaturationPressurePG30(6.981063, -2.499521, -0.2438117, 1.0, mCriticalTemperaturePG30/260.45),
    mSaturationTemperaturePG30(6.981063, -2.499521, -0.2438117, 2.288534e-1, 1.728742e4),
    mHeatOfVaporizationPG30(903.4089, 0.07395264, 0.07395264, 260.45/mCriticalTemperaturePG30, 1.0),


    /************************************************************************
                   Properties for liquid propylene glycol 40%
    ************************************************************************/
    mDensityPG40(1.080332e3, 7.557045e-3, 1.588472e-1, -4.273648e-5, -1.176238e-3, 6.311271e-8,
                 251.56, mCriticalTemperaturePG40, 1.0e-64, 1.0e+06),
    mViscosityPG40(2.78637e3, 9.563338e-1, 251.56, mCriticalTemperaturePG40),
    mSpecificHeatPG40(3.240034e3, 1.71608, 251.56, mCriticalTemperaturePG40),
    mThermalConductivityPG40(2.725643e-1, 6.714341e-4, -5.159441e-7, 251.56, mCriticalTemperaturePG40),
    mPrandtlNumberPG40(2.647771e7, 9.561267e-1, 251.56, mCriticalTemperaturePG40),
    mAdiabaticIndexPG40(1.4, 0.0, 251.56, mCriticalTemperaturePG40),
    mPressurePG40(1.080332e3, 7.557045e-3, 1.588472e-1, -4.273648e-5, -1.176238e-3, 6.311271e-8,
                  251.56, mCriticalTemperaturePG40, 1.0e-64, 1.0e+06),
    mTemperaturePG40(3.240034e3, 1.71608, 1.0e+2, 1.0e+07),
    mSaturationPressurePG40(6.943746, -2.505542, -0.240262, 1.0, mCriticalTemperaturePG40/251.56),
    mSaturationTemperaturePG40(6.943746, -2.505542, -0.240262, 1.09513e-1, 1.577863e4),
    mHeatOfVaporizationPG40(903.4089, 0.07395264, 0.07395264, 251.56/mCriticalTemperaturePG40, 1.0),


    /************************************************************************
                   Properties for liquid propylene glycol 50%
    ************************************************************************/
    mDensityPG50(1.134182e3, 7.557045e-3, -6.064128e-2, -4.273648e-5, -9.601303e-4, 6.311271e-8,
                 239.65, mCriticalTemperaturePG50, 1.0e-64, 1.0e+06),
    mViscosityPG50(1.641618e+4, 9.517944e-1, 239.65, mCriticalTemperaturePG50),
    mSpecificHeatPG50(2.959307e+3, 2.247201, 239.65, mCriticalTemperaturePG50),
    mThermalConductivityPG50(4.157197e-1, -2.183961e-4, 3.601802e-7, 239.65, mCriticalTemperaturePG50),
    mPrandtlNumberPG50(1.288877e+8, 9.523953e-1, 239.65, mCriticalTemperaturePG50),
    mAdiabaticIndexPG50(1.4, 0.0, 239.65, mCriticalTemperaturePG50),
    mPressurePG50(1.134182e3, 7.557045e-3, -6.064128e-2, -4.273648e-5, -9.601303e-4, 6.311271e-8,
                  239.65, mCriticalTemperaturePG50, 1.0e-64, 1.0e+06),
    mTemperaturePG50(2.959307e+3, 2.247201, 1.0e+2, 1.0e+07),
    mSaturationPressurePG50(6.734702, -2.261483, -0.3222344, 1.0, mCriticalTemperaturePG50/239.65),
    mSaturationTemperaturePG50(6.734702, -2.261483, -0.3222344, 2.893852e-2, 1.415745e4),
    mHeatOfVaporizationPG50(903.4089, 0.07395264, 0.07395264, 239.65/mCriticalTemperaturePG50, 1.0),


    /************************************************************************
                   Properties for liquid ammonia
    ************************************************************************/
    mDensityAMMONIA(8.575560e+2, -3.322898e-3, -2.196556e-1, 2.682584e-5, -2.137907e-3,
                    -4.373326e-8, 200.0, 320.0, 1.0e-64, 1.0e+06),
    mViscosityAMMONIA(8.257568e-3, -8.069591e-5, 2.711720e-7, -3.080228e-10, 200.0, 320.0),
    mSpecificHeatAMMONIA(3.232675e+3, 5.032308, 200.0, 320.0),
    mThermalConductivityAMMONIA(1.409424, -3.073545e-3, 200.0, 320.0),
    mPrandtlNumberAMMONIA(3.284527e+1, -3.140536e-1, 1.053092e-3, -1.188966e-6, 200.0, 320.0),
    mAdiabaticIndexAMMONIA(9.180049e-1, 2.654521e-3, 200.0, 320.0),
    mPressureAMMONIA(8.575560e+2, -3.322898e-3, -2.196556e-1, 2.682584e-5, -2.137907e-3,
                    -4.373326e-8, 200.0, 320.0, 1.0e-64, 1.0e+06),
    mTemperatureAMMONIA(3.232675e+3, 5.032308, 1.0e+2, 1.0e+07),


    /************************************************************************
                   Properties for liquid oxygen
    ************************************************************************/
    mDensityOXYGEN(1.484157e+3, -8.587042e-3, -2.450391, 1.987369e-4, -1.522467e-2,
                   -7.831438e-7, 54.5, 160.0, 1.0e-64, 1.0e+06),
    mViscosityOXYGEN(4.980476e-3, -1.23148e-4, 1.028597e-6, -2.833549e-9, 54.5, 160.0),
    mSpecificHeatOXYGEN(1.236465e+3, 5.952953, 54.5, 160.0),
    mThermalConductivityOXYGEN(2.702649e-1, -1.296934e-3, 54.5, 160.0),
    mPrandtlNumberOXYGEN(3.832281e+1, -9.263346e-1, 7.654178e-3, -2.057837e-5, 54.5, 160.0),
    mAdiabaticIndexOXYGEN(6.535915e-1, 1.351563e-2, 54.5, 160.0),
    mPressureOXYGEN(1.484157e+3, -8.587042e-3, -2.450391, 1.987369e-4, -1.522467e-2,
                    -7.831438e-7, 54.5, 160.0, 1.0e-64, 1.0e+06),
    mTemperatureOXYGEN(1.236465e+3, 5.952953, 1.0e+2, 1.0e+06),


    /************************************************************************
                   Properties for liquid hydrogen
    ************************************************************************/
    mDensityHYDROGEN(8.523911e+1, 3.848172e-4, -2.505004e-1, -1.498004e-6, -2.275759e-2,
                     1.385381e-6, 13.957, 33.145, 1.0e-64, 1.0e+05),
    mViscosityHYDROGEN(-3.313185e-4, 5.649623e-5, -2.874943e-6, 4.492284e-8, 13.957, 33.145),
    mSpecificHeatHYDROGEN(1075.252, 397.4336, 13.957, 33.145),
    mThermalConductivityHYDROGEN(6.970838e-2, 1.438866e-3, 13.957, 33.145),
    mPrandtlNumberHYDROGEN(-2.957755, 1.071455, -6.652422e-2, 1.197382e-3, 13.957, 33.145),
    mAdiabaticIndexHYDROGEN(2.970049e-1, 6.793151e-2, 13.957, 33.145),
    mPressureHYDROGEN(8.523911e+1, 3.848172e-4, -2.505004e-1, -1.498004e-6, -2.275759e-2,
                      1.385381e-6, 13.957, 33.145, 1.0e-64, 1.0e+05),
    mTemperatureHYDROGEN(1075.252, 397.4336, 9.0e+4, 5.0e+05),


    /************************************************************************
                   Properties for liquid methane
    ************************************************************************/
    mDensityMETHANE(5.18077e+2, -1.218128e-3, -2.121587e-1, 1.718584e-5, -5.772998e-3,
                    2.199488e-8, 91.0, 190.0, 1.0e-64, 1.0e+06),
    mViscosityMETHANE(1.570362e-3, -2.758163e-5, 1.700744e-7, -3.554044e-10, 91.0, 190.0),
    mSpecificHeatMETHANE(2.396006e+3, 1.012118e+1, 91.0, 190.0),
    mThermalConductivityMETHANE(3.18017e-1, -1.19349e-3, 91.0, 190.0),
    mPrandtlNumberMETHANE(1.993432e+1, -3.382524e-1, 2.086565e-3, -4.282414e-6, 91.0, 190.0),
    mAdiabaticIndexMETHANE(7.96846e-1, 8.1612e-3, 91.0, 190.0),
    mPressureMETHANE(5.18077e+2, -1.218128e-3, -2.121587e-1, 1.718584e-5, -5.772998e-3,
                     2.199488e-8, 91.0, 190.0, 1.0e-64, 1.0e+06),
    mTemperatureMETHANE(2.396006e+3, 1.012118e+1, 1.0e+2, 1.0e+06),


    /************************************************************************
                   Properties for liquid NaK-78
    ************************************************************************/
    mDensityNAK78(9.43057e+2, 7.557045e-3, -2.536697e-1, -4.273648e-5, -6.394895e-6,
                  6.311271e-8, 260.55, 1058.15, 1.0e-64, 1.0e+06),
    mViscosityNAK78(1.094168e-2, 9.915936e-1, 273.15, 393.0),
    mSpecificHeatNAK78(8.38991e+2, 4.298676e-2, 260.55, 1058.15),
    mThermalConductivityNAK78(1.410435e+1, 3.27186e-2, -2.2e-5, 273.15, 1058.15),
    mPrandtlNumberNAK78(4.353545e1, -2.2239e-3, 3e-6, 273.15, 373.0),
//    mAdiabaticIndexNAK78(7.96846e-1, 8.1612e-3, 91.0, 190.0),
    mPressureNAK78(9.43057e+2, 7.557045e-3, -2.536697e-1, -4.273648e-5, -6.394895e-6,
                   6.311271e-8, 260.55, 1058.15, 1.0e-64, 1.0e+06),
    mTemperatureNAK78(8.38991e+2, 4.298676e-2, 2.0e+5, 2.0e+6),

    /************************************************************************
                   Properties for liquid Galden HT-170
    ************************************************************************/
    mDensityGALDEN170(2.448909e+3, 7.557045e-3, -2.273338e+0, -4.273648e-5, -6.394895e-6,
                      6.311271e-8, 176.15, 443.15, 1.0e-64, 1.0e+06),
    mViscosityGALDEN170(2.399253e+0, 9.795713e-1, 223.15, 443.15),
    mSpecificHeatGALDEN170(959.33, 3.3983e-3, 176.15, 443.15),
    mThermalConductivityGALDEN170(8.834155e-2, -7.737154e-5, 176.15, 443.15),
    mPrandtlNumberGALDEN170(1.043326e+4, 9.835799e-1, 223.15, 443.15),
//    mAdiabaticIndexGALDEN170(7.96846e-1, 8.1612e-3, 91.0, 190.0),
    mPressureGALDEN170(2.448909e+3, 7.557045e-3, -2.273338e+0, -4.273648e-5, -6.394895e-6,
                       6.311271e-8, 176.15, 443.15, 1.0e-64, 1.0e+06),
    mTemperatureGALDEN170(959.33, 3.3983e-3, 1.0e+5, 1.0e+6),
    mSaturationPressureGALDEN170(6.901611, -2.958793, -0.3139779, 1.0, mCriticalTemperatureGALDEN170/223.15),
    mSaturationTemperatureGALDEN170(6.901611, -2.958793, -0.3139779, 4.801679e-5, 4.254419e+3),
    mHeatOfVaporizationGALDEN170(85.343219, 0.132045, 0.132045, 176.15/mCriticalTemperatureGALDEN170, 1.0),

    /************************************************************************
                   Properties for liquid water (density table lookup)
    ************************************************************************/
    mDataWATERPVT(),
    mDensityWATERPVT(mDataWATERPVT.mScaleTemperature,
                     mDataWATERPVT.mScalePressure,
                     mDataWATERPVT.mDensityTable,
                     mDataWATERPVT.getTscaleSize(),
                     mDataWATERPVT.getPscaleSize(),
                     mDataWATERPVT.mScaleTemperature[0],
                     mDataWATERPVT.mScaleTemperature[mDataWATERPVT.getTscaleSize() - 1],
                     mDataWATERPVT.mScalePressure[0],
                     mDataWATERPVT.mScalePressure[mDataWATERPVT.getPscaleSize() - 1]),
     mPressureWATERPVT(mDataWATERPVT.mScaleTemperature,
                       mDataWATERPVT.mScalePressure,
                       mDataWATERPVT.mDensityTable,
                       mDataWATERPVT.getTscaleSize(),
                       mDataWATERPVT.getPscaleSize(),
                       mDataWATERPVT.mScaleTemperature[0],
                       mDataWATERPVT.mScaleTemperature[mDataWATERPVT.getTscaleSize() - 1],
                       mDataWATERPVT.mScalePressure[0],
                       mDataWATERPVT.mScalePressure[mDataWATERPVT.getPscaleSize() - 1]),

    /************************************************************************
                   Properties for liquid NTO
    ************************************************************************/
    mDensityNTO(3.241299e+3, 1.140648e-1, 2.817185e+1, -6.391916e-4, -4.342774e-2, 9.113739e-7,
                261.95, 427.6, 1.0e-64, 1.0e+06),
    mViscosityNTO(7.533e-3, -6.167e-5, 2.055e-7, -3.234e-10, 1.966e-13, 261.95, 427.6),
    mSpecificHeatNTO(-2215.7, 13.372, 261.95, 427.6),
    mThermalConductivityNTO(-1.3782e-1, 2.3288e-3, -4.7865e-6, 261.95, 350.0),
    mPrandtlNumberNTO(-2.364171e+1, 2.898713e-1, -9.459955e-4, 9.33181e-7, 2.44165e-10, 261.95, 350.0),
    mAdiabaticIndexNTO(0.64975127, 1.227552e-3, 200.0, 470.0),
    mPressureNTO(3.241299e+3, 1.140648e-1, 2.817185e+1, -6.391916e-4, -4.342774e-2, 9.113739e-7,
                 261.95, 427.6, 1.0e-64, 1.0e+06),
    mTemperatureNTO(-2215.7, 13.372, 1214.0*261.95, 3502.0*427.6),

    /************************************************************************
                   Properties for liquid MMH
    ************************************************************************/
    mDensityMMH(9.935507e+2, 4.356428e-3, 7.055863e-2, -2.411805e-5, -1.605997e-3, 4.182414e-8,
                220.0, 585.0, 1.0e-64, 1.0e+06),
    mViscosityMMH(1.081484e1, -1.796232e-1, 1.189885e-3, -3.928065e-6, 6.460698e-9, -4.234832e-12, 220.0, 360.0),
    mSpecificHeatMMH(2480.7, 1.2624, 220.0, 585.0),
    mThermalConductivityMMH(1.4236e-1, 9.2048e-4, -1.9016e-6, 220.0, 585.0),
    mPrandtlNumberMMH(1.73757e3, -1.634448e1, 5.688812e-2, -8.669108e-5, 4.878808e-8, 220.0, 500.0),
    mAdiabaticIndexMMH(0.64975127, 1.227552e-3, 200.0, 470.0),
    mPressureMMH(9.935507e+2, 4.356428e-3, 7.055863e-2, -2.411805e-5, -1.605997e-3, 4.182414e-8,
                 220.0, 585.0, 1.0e-64, 1.0e+06),
    mTemperatureMMH(2480.7, 1.2624, 2758.4*220.0, 3219.2*585.0),

    /************************************************************************
                   Properties for liquid HYDRAZINE
    ************************************************************************/
    mDensityHYDRAZINE(1230.774, 5.91931e-5, -0.626700942, 2.06685e-7, -0.000452968, 1.25893e-9,
                      274.68, 387.4, 1.0e-64, 1.0e+06),
    mViscosityHYDRAZINE(0.032949161, 0.988079347, 274.68, 387.4),
    mSpecificHeatHYDRAZINE(2343.3, 2.4825, 274.68, 387.4),
    mThermalConductivityHYDRAZINE(0.088203316, 0.002744071, -4.68042e-6, 274.68, 387.4),
    mPrandtlNumberHYDRAZINE(131.8770027, 0.989586693, 274.68, 387.4),
    mAdiabaticIndexHYDRAZINE(0.64975127, 1.227552e-3, 200.0, 470.0),
    mPressureHYDRAZINE(1230.774, 5.91931e-5, -0.626700942, 2.06685e-7, -0.000452968, 1.25893e-9,
                       274.68, 387.4, 1.0e-64, 1.0e+06),
    mTemperatureHYDRAZINE(2343.3, 2.4825, 3025.0*274.68, 3806.0*387.4),

    /************************************************************************
                   Properties for carbon monoxide (Ideal Gas)
    ************************************************************************/
    mPropertiesCO(FluidProperties::GUNNS_CO,   FluidProperties::GAS,
                  mMWeightCO,                  mCriticalTemperatureCO,
                  &mDensityCO,                 &mViscosityCO,        &mSpecificHeatCO,
                  &mThermalConductivityCO,     &mPrandtlNumberCO,    &mAdiabaticIndexCO,
                  &mPressureCO,                &mTemperatureCO,      &mSaturationPressureCO,
                  &mSaturationTemperatureCO,   &mHeatOfVaporizationCO),

    /************************************************************************
                   Properties for carbon dioxide (Ideal Gas)
    ************************************************************************/
    mPropertiesCO2(FluidProperties::GUNNS_CO2, FluidProperties::GAS,
                  mMWeightCO2,                 mCriticalTemperatureCO2,
                  &mDensityCO2,                &mViscosityCO2,       &mSpecificHeatCO2,
                  &mThermalConductivityCO2,    &mPrandtlNumberCO2,   &mAdiabaticIndexCO2,
                  &mPressureCO2,               &mTemperatureCO2,     &mSaturationPressureCO2,
                  &mSaturationTemperatureCO2,  &mHeatOfVaporizationCO2),

    /************************************************************************
                   Properties for water vapor  (Ideal Gas)
    ************************************************************************/
    mPropertiesH2O(FluidProperties::GUNNS_H2O, FluidProperties::GAS,
                  mMWeightH2O,                 mCriticalTemperatureH2O,
                  &mDensityH2O,                &mViscosityH2O,       &mSpecificHeatH2O,
                  &mThermalConductivityH2O,    &mPrandtlNumberH2O,   &mAdiabaticIndexH2O,
                  &mPressureH2O,               &mTemperatureH2O,     &mSaturationPressureH2O,
                  &mSaturationTemperatureH2O,  &mHeatOfVaporizationH2O),

    /************************************************************************
                   Properties for nitrogen (Ideal Gas)
    ************************************************************************/
    mPropertiesN2(FluidProperties::GUNNS_N2,   FluidProperties::GAS,
                  mMWeightN2,                  mCriticalTemperatureN2,
                  &mDensityN2,                 &mViscosityN2,        &mSpecificHeatN2,
                  &mThermalConductivityN2,     &mPrandtlNumberN2,    &mAdiabaticIndexN2,
                  &mPressureN2,                &mTemperatureN2,      &mSaturationPressureN2,
                  &mSaturationTemperatureN2,   &mHeatOfVaporizationN2),

    /************************************************************************
                   Properties for oxygen (Ideal Gas)
    ************************************************************************/
    mPropertiesO2(FluidProperties::GUNNS_O2,   FluidProperties::GAS,
                  mMWeightO2,                  mCriticalTemperatureO2,
                  &mDensityO2,                 &mViscosityO2,        &mSpecificHeatO2,
                  &mThermalConductivityO2,     &mPrandtlNumberO2,    &mAdiabaticIndexO2,
                  &mPressureO2,                &mTemperatureO2,      &mSaturationPressureO2,
                  &mSaturationTemperatureO2,   &mHeatOfVaporizationO2),

    /************************************************************************
                   Properties for ammonia (Ideal Gas)
    ************************************************************************/
    mPropertiesNH3(FluidProperties::GUNNS_NH3, FluidProperties::GAS,
                  mMWeightNH3,                 mCriticalTemperatureNH3,
                  &mDensityNH3,                &mViscosityNH3,       &mSpecificHeatNH3,
                  &mThermalConductivityNH3,    &mPrandtlNumberNH3,   &mAdiabaticIndexNH3,
                  &mPressureNH3,               &mTemperatureNH3,     &mSaturationPressureNH3,
                  &mSaturationTemperatureNH3,  &mHeatOfVaporizationNH3),

    /************************************************************************
                  Properties for hydrogen (Ideal Gas)
    ************************************************************************/
    mPropertiesH2(FluidProperties::GUNNS_H2,   FluidProperties::GAS,
                  mMWeightH2,                  mCriticalTemperatureH2,
                  &mDensityH2,                 &mViscosityH2,        &mSpecificHeatH2,
                  &mThermalConductivityH2,     &mPrandtlNumberH2,    &mAdiabaticIndexH2,
                  &mPressureH2,                &mTemperatureH2,      &mSaturationPressureH2,
                  &mSaturationTemperatureH2,   &mHeatOfVaporizationH2),

    /************************************************************************
                   Properties for methane (Ideal Gas)
    ************************************************************************/
    mPropertiesCH4(FluidProperties::GUNNS_CH4, FluidProperties::GAS,
                  mMWeightCH4,                 mCriticalTemperatureCH4,
                  &mDensityCH4,                &mViscosityCH4,       &mSpecificHeatCH4,
                  &mThermalConductivityCH4,    &mPrandtlNumberCH4,   &mAdiabaticIndexCH4,
                  &mPressureCH4,               &mTemperatureCH4,     &mSaturationPressureCH4,
                  &mSaturationTemperatureCH4,  &mHeatOfVaporizationCH4),

    /************************************************************************
                   Properties for hydrocloric acid (Ideal Gas)
    ************************************************************************/
    mPropertiesHCL(FluidProperties::GUNNS_HCL, FluidProperties::GAS,
                  mMWeightHCL,                 mCriticalTemperatureHCL,
                  &mDensityHCL,                &mViscosityHCL,       &mSpecificHeatHCL,
                  &mThermalConductivityHCL,    &mPrandtlNumberHCL,   &mAdiabaticIndexHCL,
                  &mPressureHCL,               &mTemperatureHCL,     &mSaturationPressureHCL,
                  &mSaturationTemperatureHCL,  &mHeatOfVaporizationHCL),

    /************************************************************************
                   Properties for hydrogen cyanide (Ideal Gas)
    ************************************************************************/
    mPropertiesHCN(FluidProperties::GUNNS_HCN, FluidProperties::GAS,
                  mMWeightHCN,                 mCriticalTemperatureHCN,
                  &mDensityHCN,                &mViscosityHCN,       &mSpecificHeatHCN,
                  &mThermalConductivityHCN,    &mPrandtlNumberHCN,   &mAdiabaticIndexHCN,
                  &mPressureHCN,               &mTemperatureHCN,     &mSaturationPressureHCN,
                  &mSaturationTemperatureHCN,  &mHeatOfVaporizationHCN),

    /************************************************************************
                   Properties for helium (Ideal Gas)
    ************************************************************************/
    mPropertiesHe(FluidProperties::GUNNS_HE, FluidProperties::GAS,
                  mMWeightHe,                 mCriticalTemperatureHe,
                  &mDensityHe,                &mViscosityHe,       &mSpecificHeatHe,
                  &mThermalConductivityHe,    &mPrandtlNumberHe,   &mAdiabaticIndexHe,
                  &mPressureHe,               &mTemperatureHe,     &mSaturationPressureHe,
                  &mSaturationTemperatureHe,  &mHeatOfVaporizationHe),

    /************************************************************************
                   Properties for helium (Real-Gas compressibility)
    ************************************************************************/
    mPropertiesHeReal(FluidProperties::GUNNS_HE_REAL_GAS, FluidProperties::GAS,
                  mMWeightHe,                 mCriticalTemperatureHe,
                  &mDensityHeReal,            &mViscosityHe,       &mSpecificHeatHe,
                  &mThermalConductivityHe,    &mPrandtlNumberHe,   &mAdiabaticIndexHe,
                  &mPressureHeReal,           &mTemperatureHe,     &mSaturationPressureHe,
                  &mSaturationTemperatureHe,  &mHeatOfVaporizationHe),

    /************************************************************************
                   Properties for xenon, (Real-Gas compressibility)
    ************************************************************************/
    mPropertiesXe(FluidProperties::GUNNS_XE_REAL_GAS, FluidProperties::GAS,
                  mMWeightXe,                mCriticalTemperatureXe,
                  &mDensityXe,               &mViscosityXe,        &mSpecificHeatXe,
                  &mThermalConductivityXe,   &mPrandtlNumberXe,    &mAdiabaticIndexXe,
                  &mPressureXe,              &mTemperatureXe,      &mSaturationPressureXe,
                  &mSaturationTemperatureXe, &mHeatOfVaporizationXe),

    /************************************************************************
                   Properties for nitrogen, (Real-Gas compressibility).  These are the same as the
                   GUNNS_N2, except the density & pressures are table look-ups that model real-gas
                   compression.
    ************************************************************************/
    mPropertiesN2Real(FluidProperties::GUNNS_N2_REAL_GAS, FluidProperties::GAS,
                      mMWeightN2,                  mCriticalTemperatureN2,
                      &mDensityN2Real,             &mViscosityN2,        &mSpecificHeatN2,
                      &mThermalConductivityN2,     &mPrandtlNumberN2,    &mAdiabaticIndexN2,
                      &mPressureN2Real,            &mTemperatureN2,      &mSaturationPressureN2,
                      &mSaturationTemperatureN2,   &mHeatOfVaporizationN2),

    /************************************************************************
                   Properties for oxygen, (Real-Gas compressibility).  These are the same as the
                   GUNNS_O2, except the density & pressures are table look-ups that model real-gas
                   compression.
    ************************************************************************/
    mPropertiesO2Real(FluidProperties::GUNNS_O2_REAL_GAS, FluidProperties::GAS,
                      mMWeightO2,                  mCriticalTemperatureO2,
                      &mDensityO2Real,             &mViscosityO2,        &mSpecificHeatO2,
                      &mThermalConductivityO2,     &mPrandtlNumberO2,    &mAdiabaticIndexO2,
                      &mPressureO2Real,            &mTemperatureO2,      &mSaturationPressureO2,
                      &mSaturationTemperatureO2,   &mHeatOfVaporizationO2),

    /************************************************************************
                   Properties for hydrogen, (Real-Gas compressibility).  These are the same as the
                   GUNNS_H2, except the density & pressures are table look-ups that model real-gas
                   compression.
    ************************************************************************/
    mPropertiesH2Real(FluidProperties::GUNNS_H2_REAL_GAS, FluidProperties::GAS,
                      mMWeightH2,                  mCriticalTemperatureH2,
                      &mDensityH2Real,             &mViscosityH2,        &mSpecificHeatH2,
                      &mThermalConductivityH2,     &mPrandtlNumberH2,    &mAdiabaticIndexH2,
                      &mPressureH2Real,            &mTemperatureH2,      &mSaturationPressureH2,
                      &mSaturationTemperatureH2,   &mHeatOfVaporizationH2),

    /************************************************************************
                   Properties for water (Liquid)
    ************************************************************************/
    mPropertiesWATER(FluidProperties::GUNNS_WATER, FluidProperties::LIQUID,
                     mMWeightH2O,                     mCriticalTemperatureH2O,
                     &mDensityWATER,                  &mViscosityWATER,
                     &mSpecificHeatWATER,             &mThermalConductivityWATER,
                     &mPrandtlNumberWATER,            &mAdiabaticIndexWATER,
                     &mPressureWATER,                 &mTemperatureWATER,
                     &mSaturationPressureH2O,         &mSaturationTemperatureH2O,
                     &mHeatOfVaporizationH2O),

    /************************************************************************
                   Properties for HFE7000 (Liquid)
    ************************************************************************/
    mPropertiesHFE7000(FluidProperties::GUNNS_HFE7000, FluidProperties::LIQUID,
                       mMWeightHFE7000,                mCriticalTemperatureHFE7000,
                       &mDensityHFE7000,               &mViscosityHFE7000,
                       &mSpecificHeatHFE7000,          &mThermalConductivityHFE7000,
                       &mPrandtlNumberHFE7000,         &mAdiabaticIndexHFE7000,
                       &mPressureHFE7000,              &mTemperatureHFE7000,
                       &mSaturationPressureHFE7000,    &mSaturationTemperatureHFE7000,
                       &mHeatOfVaporizationHFE7000),

    /************************************************************************
                   Properties for HFE7100 (Liquid)
    ************************************************************************/
    mPropertiesHFE7100(FluidProperties::GUNNS_HFE7100, FluidProperties::LIQUID,
                       mMWeightHFE7100,                mCriticalTemperatureHFE7100,
                       &mDensityHFE7100,               &mViscosityHFE7100,
                       &mSpecificHeatHFE7100,          &mThermalConductivityHFE7100,
                       &mPrandtlNumberHFE7100,         &mAdiabaticIndexHFE7100,
                       &mPressureHFE7100,              &mTemperatureHFE7100,
                       &mSaturationPressureHFE7100,    &mSaturationTemperatureHFE7100,
                       &mHeatOfVaporizationHFE7100),

   /************************************************************************
                 Properties for propylene glycol 30% (Liquid)
   ************************************************************************/
   mPropertiesPG30(FluidProperties::GUNNS_PG30, FluidProperties::LIQUID,
                   mMWeightPG30,                mCriticalTemperaturePG30,
                   &mDensityPG30,               &mViscosityPG30,
                   &mSpecificHeatPG30,          &mThermalConductivityPG30,
                   &mPrandtlNumberPG30,         &mAdiabaticIndexPG30,
                   &mPressurePG30,              &mTemperaturePG30,
                   &mSaturationPressurePG30,    &mSaturationTemperaturePG30,
                   &mHeatOfVaporizationPG30),

/************************************************************************
                 Properties for propylene glycol 40% (Liquid)
************************************************************************/
   mPropertiesPG40(FluidProperties::GUNNS_PG40, FluidProperties::LIQUID,
                   mMWeightPG40,                mCriticalTemperaturePG40,
                   &mDensityPG40,               &mViscosityPG40,
                   &mSpecificHeatPG40,          &mThermalConductivityPG40,
                   &mPrandtlNumberPG40,         &mAdiabaticIndexPG40,
                   &mPressurePG40,              &mTemperaturePG40,
                   &mSaturationPressurePG40,    &mSaturationTemperaturePG40,
                   &mHeatOfVaporizationPG40),

   /************************************************************************
                Properties for propylene glycol 50% (Liquid)
   ************************************************************************/
   mPropertiesPG50(FluidProperties::GUNNS_PG50, FluidProperties::LIQUID,
                   mMWeightPG50,                mCriticalTemperaturePG50,
                   &mDensityPG50,               &mViscosityPG50,
                   &mSpecificHeatPG50,          &mThermalConductivityPG50,
                   &mPrandtlNumberPG50,         &mAdiabaticIndexPG50,
                   &mPressurePG50,              &mTemperaturePG50,
                   &mSaturationPressurePG50,    &mSaturationTemperaturePG50,
                   &mHeatOfVaporizationPG50),

    /************************************************************************
                   Properties for ammonia (Liquid)
    ************************************************************************/
    mPropertiesAMMONIA(FluidProperties::GUNNS_AMMONIA, FluidProperties::LIQUID,
                       mMWeightNH3,                    mCriticalTemperatureNH3,
                       &mDensityAMMONIA,               &mViscosityAMMONIA,
                       &mSpecificHeatAMMONIA,          &mThermalConductivityAMMONIA,
                       &mPrandtlNumberAMMONIA,         &mAdiabaticIndexAMMONIA,
                       &mPressureAMMONIA,              &mTemperatureAMMONIA,
                       &mSaturationPressureNH3,        &mSaturationTemperatureNH3,
                       &mHeatOfVaporizationNH3),

    /************************************************************************
                   Properties for oxygen (Liquid)
    ************************************************************************/
    mPropertiesOXYGEN(FluidProperties::GUNNS_OXYGEN, FluidProperties::LIQUID,
                      mMWeightO2,                    mCriticalTemperatureO2,
                      &mDensityOXYGEN,               &mViscosityOXYGEN,
                      &mSpecificHeatOXYGEN,          &mThermalConductivityOXYGEN,
                      &mPrandtlNumberOXYGEN,         &mAdiabaticIndexOXYGEN,
                      &mPressureOXYGEN,              &mTemperatureOXYGEN,
                      &mSaturationPressureO2,        &mSaturationTemperatureO2,
                      &mHeatOfVaporizationO2),

    /************************************************************************
                   Properties for hydrogen (Liquid)
    ************************************************************************/
    mPropertiesHYDROGEN(FluidProperties::GUNNS_HYDROGEN, FluidProperties::LIQUID,
                        mMWeightH2,                    mCriticalTemperatureH2,
                        &mDensityHYDROGEN,             &mViscosityHYDROGEN,
                        &mSpecificHeatHYDROGEN,        &mThermalConductivityHYDROGEN,
                        &mPrandtlNumberHYDROGEN,       &mAdiabaticIndexHYDROGEN,
                        &mPressureHYDROGEN,            &mTemperatureHYDROGEN,
                        &mSaturationPressureH2,        &mSaturationTemperatureH2,
                        &mHeatOfVaporizationH2),

/************************************************************************
               Properties for methane (Liquid)
************************************************************************/
    mPropertiesMETHANE(FluidProperties::GUNNS_METHANE, FluidProperties::LIQUID,
                       mMWeightCH4,                    mCriticalTemperatureCH4,
                       &mDensityMETHANE,               &mViscosityMETHANE,
                       &mSpecificHeatMETHANE,          &mThermalConductivityMETHANE,
                       &mPrandtlNumberMETHANE,         &mAdiabaticIndexMETHANE,
                       &mPressureMETHANE,              &mTemperatureMETHANE,
                       &mSaturationPressureCH4,        &mSaturationTemperatureCH4,
                       &mHeatOfVaporizationCH4),

/************************************************************************
               Properties for NaK-78 (Liquid).  Some properties we don't have any data for, so we
               just substitute WATER/H2O for them.  They should not be used.
************************************************************************/
    mPropertiesNAK78(FluidProperties::GUNNS_NAK78, FluidProperties::LIQUID,
                     mMWeightNAK78,                mCriticalTemperatureNAK78,
                     &mDensityNAK78,               &mViscosityNAK78,
                     &mSpecificHeatNAK78,          &mThermalConductivityNAK78,
                     &mPrandtlNumberNAK78,         &mAdiabaticIndexWATER,
                     &mPressureNAK78,              &mTemperatureNAK78,
                     &mSaturationPressureH2O,      &mSaturationTemperatureH2O,
                     &mHeatOfVaporizationH2O),

/************************************************************************
              Properties for Galden HT-170 (Liquid)
************************************************************************/
    mPropertiesGALDEN170(FluidProperties::GUNNS_GALDEN170, FluidProperties::LIQUID,
                         mMWeightGALDEN170,                mCriticalTemperatureGALDEN170,
                         &mDensityGALDEN170,               &mViscosityGALDEN170,
                         &mSpecificHeatGALDEN170,          &mThermalConductivityGALDEN170,
                         &mPrandtlNumberGALDEN170,         &mAdiabaticIndexWATER,
                         &mPressureGALDEN170,              &mTemperatureGALDEN170,
                         &mSaturationPressureGALDEN170,    &mSaturationTemperatureGALDEN170,
                         &mHeatOfVaporizationGALDEN170),

/************************************************************************
              Properties for liquid water (density table lookup)
************************************************************************/
    mPropertiesWATERPVT(FluidProperties::GUNNS_WATER_PVT, FluidProperties::LIQUID,
                        mMWeightH2O,                      mCriticalTemperatureH2O,
                        &mDensityWATERPVT,                &mViscosityWATER,
                        &mSpecificHeatWATER,              &mThermalConductivityWATER,
                        &mPrandtlNumberWATER,             &mAdiabaticIndexWATER,
                        &mPressureWATERPVT,               &mTemperatureWATER,
                        &mSaturationPressureH2O,          &mSaturationTemperatureH2O,
                        &mHeatOfVaporizationH2O),

/************************************************************************
               Properties for NTO (Liquid)
************************************************************************/
    mPropertiesNTO(FluidProperties::GUNNS_NTO, FluidProperties::LIQUID,
                   mMWeightN2O4,                   mCriticalTemperatureN2O4,
                   &mDensityNTO,                   &mViscosityNTO,
                   &mSpecificHeatNTO,              &mThermalConductivityNTO,
                   &mPrandtlNumberNTO,             &mAdiabaticIndexNTO,
                   &mPressureNTO,                  &mTemperatureNTO,
                   &mSaturationPressureN2O4,       &mSaturationTemperatureN2O4,
                   &mHeatOfVaporizationN2O4),

/************************************************************************
                Properties for MMH (Liquid)
************************************************************************/
    mPropertiesMMH(FluidProperties::GUNNS_MMH, FluidProperties::LIQUID,
                   mMWeightCH3N2H3,                mCriticalTemperatureCH3N2H3,
                   &mDensityMMH,                   &mViscosityMMH,
                   &mSpecificHeatMMH,              &mThermalConductivityMMH,
                   &mPrandtlNumberMMH,             &mAdiabaticIndexMMH,
                   &mPressureMMH,                  &mTemperatureMMH,
                   &mSaturationPressureCH3N2H3,    &mSaturationTemperatureCH3N2H3,
                   &mHeatOfVaporizationCH3N2H3),

/************************************************************************
                Properties for HYDRAZINE (Liquid)
************************************************************************/
    mPropertiesHYDRAZINE(FluidProperties::GUNNS_HYDRAZINE, FluidProperties::LIQUID,
                   mMWeightN2H4,                         mCriticalTemperatureN2H4,
                   &mDensityHYDRAZINE,                   &mViscosityHYDRAZINE,
                   &mSpecificHeatHYDRAZINE,              &mThermalConductivityHYDRAZINE,
                   &mPrandtlNumberHYDRAZINE,             &mAdiabaticIndexHYDRAZINE,
                   &mPressureHYDRAZINE,                  &mTemperatureHYDRAZINE,
                   &mSaturationPressureN2H4,             &mSaturationTemperatureN2H4,
                   &mHeatOfVaporizationN2H4)
{
    /// - Load the fluid properties array with the pointer to the properties for each fluid type
    mProperties[FluidProperties::GUNNS_CO]          = &mPropertiesCO;
    mProperties[FluidProperties::GUNNS_CO2]         = &mPropertiesCO2;
    mProperties[FluidProperties::GUNNS_H2O]         = &mPropertiesH2O;
    mProperties[FluidProperties::GUNNS_N2]          = &mPropertiesN2;
    mProperties[FluidProperties::GUNNS_O2]          = &mPropertiesO2;
    mProperties[FluidProperties::GUNNS_NH3]         = &mPropertiesNH3;
    mProperties[FluidProperties::GUNNS_H2]          = &mPropertiesH2;
    mProperties[FluidProperties::GUNNS_CH4]         = &mPropertiesCH4;
    mProperties[FluidProperties::GUNNS_HCL]         = &mPropertiesHCL;
    mProperties[FluidProperties::GUNNS_HCN]         = &mPropertiesHCN;
    mProperties[FluidProperties::GUNNS_HE]          = &mPropertiesHe;
    mProperties[FluidProperties::GUNNS_HE_REAL_GAS] = &mPropertiesHeReal;
    mProperties[FluidProperties::GUNNS_XE_REAL_GAS] = &mPropertiesXe;
    mProperties[FluidProperties::GUNNS_N2_REAL_GAS] = &mPropertiesN2Real;
    mProperties[FluidProperties::GUNNS_O2_REAL_GAS] = &mPropertiesO2Real;
    mProperties[FluidProperties::GUNNS_H2_REAL_GAS] = &mPropertiesH2Real;
    mProperties[FluidProperties::GUNNS_WATER]       = &mPropertiesWATER;
    mProperties[FluidProperties::GUNNS_HFE7000]     = &mPropertiesHFE7000;
    mProperties[FluidProperties::GUNNS_HFE7100]     = &mPropertiesHFE7100;
    mProperties[FluidProperties::GUNNS_PG30]        = &mPropertiesPG30;
    mProperties[FluidProperties::GUNNS_PG40]        = &mPropertiesPG40;
    mProperties[FluidProperties::GUNNS_PG50]        = &mPropertiesPG50;
    mProperties[FluidProperties::GUNNS_AMMONIA]     = &mPropertiesAMMONIA;
    mProperties[FluidProperties::GUNNS_OXYGEN]      = &mPropertiesOXYGEN;
    mProperties[FluidProperties::GUNNS_HYDROGEN]    = &mPropertiesHYDROGEN;
    mProperties[FluidProperties::GUNNS_METHANE]     = &mPropertiesMETHANE;
    mProperties[FluidProperties::GUNNS_NAK78]       = &mPropertiesNAK78;
    mProperties[FluidProperties::GUNNS_GALDEN170]   = &mPropertiesGALDEN170;
    mProperties[FluidProperties::GUNNS_WATER_PVT]   = &mPropertiesWATERPVT;
    mProperties[FluidProperties::GUNNS_NTO]         = &mPropertiesNTO;
    mProperties[FluidProperties::GUNNS_MMH]         = &mPropertiesMMH;
    mProperties[FluidProperties::GUNNS_HYDRAZINE]   = &mPropertiesHYDRAZINE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Defined Fluid Properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
DefinedFluidProperties::~DefinedFluidProperties()
{
    // nothing to do
}
