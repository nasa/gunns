/****************************** TRICK HEADER ******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (MaterialProperties: Properties of a material)

 ASSUMPTIONS AND LIMITATIONS:
 ()


 REFERENCE:
 ((Thermophysical Properties of FLuids and Materials, Jacobsen, Lemmon, et al)
  (TPSX Material Properties Database: http://tpsx.arc.nasa.gov/)
  )

 LIBRARY DEPENDENCY:
 ()

 PROGRAMMERS:
 (
  ((Chuck Sorensen) (LZT) (Jan 2011) (Conversion from C to C++))
 )
 **************************************************************************************************/

#include "MaterialProperties.hh"
//
// Initialize defined materials
//

/*
//  no validation for roughness from C code.
//  TPSX cited two values for steel304:
//             1         2
// CP        477        502
// conduc     14.9       14.6
// rough     ---        ---
// density  7900       7920
const MaterialProperties MaterialProperties::steel304(
        490.0, 14.75,  2.13360E-06, 7910.0);
*/
//*********************************************************************************
//   Not known what aluminum alloy to be
//   6061 not found in TPSX database, kept values from C code (no citations)
//*********************************************************************************
// Cp roughly 880, density consistent across all values seen, (web search)
// thermal conductivity has a range and depends on manufacturing method used.
// no validation for roughness from C code.
//const MaterialProperties MaterialProperties::aluminum6061(
//        879.249, 173.10461, 3.81E-07, 2712.55219);



