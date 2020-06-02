/************************ TRICK HEADER ************************************************************
 @defgroup TSM_GUNNS_THERMAL_SurfaceHeatFlux Surface Heat Flux
 @ingroup  TSM_GUNNS_THERMAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
-    (Used to determine the heat flux to an object given its orientation with a given source.
      It serves most commonly as an interface between a vehicle's exterior panel and the
      environment, and should be included once in each thermal network.)

REFERENCE:
-    (Minimum Parametric Value Calculations for Line of Sight With Sun --
      Fundamentals of Astrodynamics and Applications, Second Edition, David A. Vallado)

ASSUMPTIONS AND LIMITATIONS:
-    (The radiance vectors: radianceSun, radianceIR, and radianceAlbedo, are assumed to be set by
      the environment model via the sim bus. They are initialized with a trivially small value in
      case no communication with the environment exists. Trick and friendly unit tests can set these
      environment variables using the protected setter methods.)

LIBRARY DEPENDENCY:
   (
   (TsSurfaceHeatFlux.o)
   )

PROGRAMMERS:
-    ((Kevin Supak) (L-3 Titan) (May 2008) (Initial prototype)
      (Joe Valerioti) (L-3)     (May 2011) (compatible with new env model, coding standards))
@{
***************************************************************************************************/

#ifndef TsSurfaceHeatFlux_EXISTS
#define TsSurfaceHeatFlux_EXISTS

#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Class for TsSurfaceHeatFlux.
///
/// @details
/// Provides the class data for TsSurfaceHeatFlux. Its data members are placeholders for the state
/// of the environment as read from the sim bus.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsSurfaceHeatFlux {

    TS_MAKE_SIM_COMPATIBLE(TsSurfaceHeatFlux);

    public:
        /// @brief Default constructs this TsSurfaceHeatFlux.
        TsSurfaceHeatFlux();

        /// @brief Default destructs this TsSurfaceHeatFlux.
        virtual ~TsSurfaceHeatFlux();

        // These Class Methods are public so that they can be used by the Thermal Network
        ///////////////////////////////////////////////////////////////////////////////////////////

        /// @brief Determines the projection of the flux vector that is opposite to
        ///        the surface normal direction.
        double findDirectFlux(                  // RETURN:(W/m2) flux component in surf-normal dir
                double givenFluxVector[3],      //        (W/m2) heat flux vector
                double givenSurfaceNormal[3]);  //          --   Surface Normal Vector, magnitude
                                                //               / units are irrelevant)

        /// @brief Returns the total heat flux on a given surface.
        double heatFlux(                        // RETURN: (W/m2)  Surface Heat Flux
                double givenSurfaceNormal[3]);  //           --    Surface Normal Vector, magnitude
                                                //                 / units are irrelevant)

        /// @brief Returns the total heat flux on a given surface.
        double heatFlux(double x, double y, double z);

    public:
        // These Class Methods are protected because they are only used for testing purposes
        ///////////////////////////////////////////////////////////////////////////////////////////

        /// @brief Sets lighting.
        void  setLighting( const double lightset);

        /// @brief Sets one of the radiance vectors.
        const void  setRadiance( double* radiancePointer, double x, double y, double z);

        /// @brief Validates that the magnitude of a vector is not zero
        void  vectorValidate(double* vector);

        /// @brief Validates that the direct flux calculation is within the correct bounds
        double  fluxValidate(double directFlux, double radianceMag);

    public:
        // Primary Attributes
        ///////////////////////////////////////////////////////////////////////////////////////////

        /// @brief Initializes all data members.
        double minLighting;             // --      Minimum frac lighting for Solar Calculations


        // Environment Attributes (typically set from environment sim bus)
        ///////////////////////////////////////////////////////////////////////////////////////////

        /// @brief Fraction of vehicle in sunlight
        double lighting;                // --      frac of vehicle in sunlight: 0=Eclipse, 1.0=Lit

        /// @brief Solar flux
        double radianceSun[3];          // (W/m2)  Solar flux

        /// @brief Earth (or other body) Infrared flux
        double radianceIR[3];           // (W/m2)  Infrared flux

        /// @brief Earth (or other body) albedo flux.
        double radianceAlbedo[3];       // (W/m2)  Albedo flux

    private:

        // These are re-implementations of Trick math utilities to remove this dependency on Trick.
        ///////////////////////////////////////////////////////////////////////////////////////////

        /// @brief Computes the magnitude of a 3-vector.
        double V_MAG(double* vect);

        /// @brief Computes the dot-product of two 3-vectors.
        double V_DOT(double* vect1, double* vect2);

        /// @brief copy constructor, private and unimplemented
        TsSurfaceHeatFlux(const TsSurfaceHeatFlux &rhs);

        /// @brief operator=, private and unimplemented
        TsSurfaceHeatFlux operator=(const TsSurfaceHeatFlux &rhs);
};

/// @}

#endif
