/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
     (Provides implementation for TsSurfaceHeatFlux class.)

REFERENCE:
-    (Minimum Parametric Value Calculations for Line of Sight With Sun --
      Fundamentals of Astrodynamics and Applications, Second Edition, David A. Vallado)

ASSUMPTIONS AND LIMITATIONS:
-    (The radiance vectors, radianceSun, radianceIR, and radianceAlbedo, are assumed to be set by
      the environment model via the sim bus. They are initialized with a trivially small value in
      case no communication with the environment exists.)

LIBRARY DEPENDENCY:
   (
   (TsSurfaceHeatFlux.o)
   (simulation/hs/TsHsMsg)
   (software/exceptions/TsOutOfBoundsException.o)
   )

PROGRAMMERS:
-    ((Kevin Supak) (L-3 Titan) (May 2008) (Initial prototype)
      (Joe Valerioti) (L-3)     (May 2011) (compatible with new env model, coding standards))
 **************************************************************************************************/

#include "TsSurfaceHeatFlux.hh"
#include <cfloat>
#include <cmath>

#include "math/UnitConversion.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this TsSurfaceHeatFlux. Initializes radiance vectors to a small
///           trivial value. Actual values should be set from the environment sim_bus every time-
///           step. Default components of 1.0 were chosen so it could be determined whether or not
///           the radiance values were overridden, since a zero value could be caused by a vehicle
///           in eclipse.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsSurfaceHeatFlux::TsSurfaceHeatFlux():
    minLighting(0.1),
    lighting(1.0)
{
    /// - calls the setRadiance method for each radiance vector (sun, infrared, albedo)
    setRadiance(   radianceSun, 1.0, 1.0, 1.0);
    setRadiance(    radianceIR, 1.0, 1.0, 1.0);
    setRadiance(radianceAlbedo, 1.0, 1.0, 1.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this TsSurfaceHeatFlux.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsSurfaceHeatFlux::~TsSurfaceHeatFlux()
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    lightset   requested vehicle lighting fraction (range 0-1)
///
/// @details  Sets lighting, fraction of vehicle in sunlight (range 0-1), usu. read from env sim_bus
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsSurfaceHeatFlux::setLighting( const double lightset)
{
    /// - check bounds of input lighting
    if (lightset < 0.0) {
        lighting = 0.0;

        TsHsMsg(TS_HS_WARNING, "PTCS",
                "TsSurfaceHeatFLux::Request to set vehicle lighting to less than 0, will be set to 0.");

    } else if (lightset > 1.0) {
        lighting = 1.0;

        TsHsMsg(TS_HS_WARNING, "PTCS",
                "TsSurfaceHeatFLux::Request to set vehicle lighting to greater than 1, will be set to 1.");

    } else {
        /// - set lighting if value is appropriate
        lighting = lightset;
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    radiancePointer    --   pointer to radiance vector of interest (sun, IR, or albedo)
///                   x          --   x-component of input radiance vector
///                   y          --   y-component of input radiance vector
///                   z          --   z-component of input radiance vector
///
/// @details  Sets a particular radiance vector (can be used when not connected to env model)
////////////////////////////////////////////////////////////////////////////////////////////////////
const void TsSurfaceHeatFlux::setRadiance( double* radiancePointer, // --   pointer to radiance vector of
                                                              //      interest (sun, IR, or albedo)
        double x,   // (W/m2)     x-component of input radiance vector
        double y,   // (W/m2)     y-component of input radiance vector
        double z)   // (W/m2)     z-component of input radiance vector
{
    /// - set each component of the desired radiance vector
    radiancePointer[0] = x; // (W/m2)     sets x-component of desired radiance vector
    radiancePointer[1] = y; // (W/m2)     sets y-component of desired radiance vector
    radiancePointer[2] = z; // (W/m2)     sets z-component of desired radiance vector
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    givenFluxVector[3]       (W/m2)    radiance vector to be used in vector comparison
///           givenSurfaceNormal[3]      --      the surface normal vector of an exterior surface
///                                              in structural coordinate frame
///
/// @return   directFlux           (W/m2)   component of flux in surf-normal direction
///
/// @details  Finds the component of a given flux that is in the direction of the given surface's
///           normal vector
////////////////////////////////////////////////////////////////////////////////////////////////////
double TsSurfaceHeatFlux::findDirectFlux(
        double givenSourceVector[3],
        double givenSurfaceNormal[3])
{
    /// - check that the magnitude of the vectors is not zero, so that the dot product calculation
    ///   does not divide by zero
    vectorValidate(givenSourceVector);
    vectorValidate(givenSurfaceNormal);

    /// - initialize directFlux, the component of flux in surface normal direction
    double directFlux = 0.0;          //  (W/m2)  the component of flux in surface normal direction

    /// - find magnitude of flux vector that was passed in as a parameter
    double radianceMag = V_MAG(givenSourceVector);

    /// - find the angle between the source vector and the surface normal using a dot product
    double angleBetween = acos(   V_DOT(givenSourceVector, givenSurfaceNormal)
                                    /  (radianceMag * V_MAG(givenSurfaceNormal))    );


    /// - if the surface normal points toward the source, calculate the direct flux
    if (angleBetween < UnitConversion::PI_OVER_2) {

        directFlux = fabs(radianceMag * cos(angleBetween));

    /// - if the surface normal points away from the source, set directFlux = 0
    } else {

        directFlux = 0.0;
    }

    directFlux = fluxValidate(directFlux,radianceMag);


    /// - return directFlux after calculation
    return directFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   vector  --   pointer to a 3D vector
///
/// @details  Validates that the magnitude of a vector is not zero, so that a dot product
///           calculation will not divide by zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsSurfaceHeatFlux::vectorValidate(
        double* vector)                   //        pointer to a 3D vector
{
    if ( V_MAG(vector) == 0.0 ) {
        /// - create error report
        TsHsMsg msg(TS_HS_ERROR, "PTCS");
        msg << "zero magnitude of vector used in dot product";
        hsSendMsg(msg);
        throw TsOutOfBoundsException("Invalid vector Data",
                                        "TsSurfaceHeatFlux::vectorValidate",
                                        "Magnitude of vector = 0.");
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   directFlux  (W/m2)   component of flux in surf-normal direction
/// @param   radianceMag (W/m2)   magnitude of radiance flux vector
///
/// @return  directFlux  (W/m2)   validated component of flux in surf-normal direction
///
/// @details  Validates that the direct flux calculation is within the correct bounds
////////////////////////////////////////////////////////////////////////////////////////////////////
double TsSurfaceHeatFlux::fluxValidate(
        double directFlux,
        double radianceMag)
{
    if ( directFlux < 0.0 ) {

        directFlux = 0.0;
        TsHsMsg(TS_HS_WARNING, "PTCS",
                "TsSurfaceHeatFlux::findDirectFlux - "
                "directFlux was calculated to be less than 0, will be set to 0.");

    } else if (directFlux > radianceMag) {

        directFlux = radianceMag;
        TsHsMsg(TS_HS_WARNING, "PTCS",
                "TsSurfaceHeatFlux::findDirectFlux - "
                "directFlux was calculated to be greater than the maximum, "
                "will be set to magnitude of the flux vector.");
    }

    return directFlux;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    givenSurfaceNormal[3]   the surface normal vector of an exterior surface
///                                   in structural reference frame
///
/// @return   totalHeatFlux    (W/m2)   total heat flux into the surface
///
/// @details  computes Heat flux in from the environment based on env radiance vectors and an input
///           surface normal vector
////////////////////////////////////////////////////////////////////////////////////////////////////
double TsSurfaceHeatFlux::heatFlux(
        double givenSurfaceNormal[3])
{

    /// - compute the amount of IR heat incident to the surface regardless of eclipse condition
    const double directIR      = findDirectFlux(   radianceIR, givenSurfaceNormal);

    /// - total heat flux is equal only to infrared flux if the vehicle is not in sunlight
    double totalHeatFlux = directIR;


    /// - "lighting" is read from the env sim_bus , based on orbital position at every time step.
    ///   lighting >= minLighting means that the vehicle is exposed to sunlight.
    if (lighting >= minLighting) {

        /// - compute the amount of solar heat flux incident to the surface
        const double directSolar  = findDirectFlux(   radianceSun, givenSurfaceNormal);

        /// - compute the amount of albedo heat incident to the surface
        const double directAlbedo = findDirectFlux(radianceAlbedo, givenSurfaceNormal);

        /// - sum magnitudes of the three heat fluxes that are exposed to the surface
        totalHeatFlux = directSolar + directAlbedo + directIR;
    }

    /// - return the total heat flux (W/m2)
    return totalHeatFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    givenSurfaceNormal[3]   the surface normal vector of an exterior surface
///                                   in structural reference frame
///
/// @return   totalHeatFlux    (W/m2)   total heat flux into the surface
///
/// @details  computes Heat flux in from the environment based on env radiance vectors and an input
///           surface normal vector
////////////////////////////////////////////////////////////////////////////////////////////////////
double TsSurfaceHeatFlux::heatFlux(
        double x, double y, double z)
{
    /// - set a surface normal array
    double surfaceNormal[3] = {x,y,z};

    /// call heatFlux()
    double totalHeatFlux = heatFlux(surfaceNormal);

    /// - return the total heat flux (W/m2)
    return totalHeatFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  vect   (--)   An array of 3 doubles
///
/// @return   double  (--)  The magnitude of the 3-vector
///
/// @details  Computes the magnitude of a 3-vector.  This is identical to the trick_utils macro
///           except we use DBL_EPSILON for the minimum absolute value of each component instead of
///           GSL_SQRT_DBL_MIN.
////////////////////////////////////////////////////////////////////////////////////////////////////
double TsSurfaceHeatFlux::V_MAG(double* vect)
{
    return ( sqrt( (((vect[0] < 0 ? -vect[0] : vect[0]) < DBL_EPSILON) ? 0.0 : vect[0]*vect[0]) + \
                   (((vect[1] < 0 ? -vect[1] : vect[1]) < DBL_EPSILON) ? 0.0 : vect[1]*vect[1]) + \
                   (((vect[2] < 0 ? -vect[2] : vect[2]) < DBL_EPSILON) ? 0.0 : vect[2]*vect[2]) ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  vect1   (--)   An array of 3 doubles
/// @param[in]  vect2   (--)   An array of 3 doubles
///
/// @return   double  (--)  The dot-product of the two supplied 3-vectors
///
/// @details  Computes the dot-product of the two supplied 3-vectors.  This is identical to the
///           trick_utils macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
double TsSurfaceHeatFlux::V_DOT(double* vect1, double* vect2)
{
    return ( vect1[0]*vect2[0] + vect1[1]*vect2[1] + vect1[2]* vect2[2] );
}
