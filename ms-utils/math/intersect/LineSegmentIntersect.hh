#ifndef LineSegmentIntersect_EXISTS
#define LineSegmentIntersect_EXISTS
/**

@defgroup TSM_UTILITIES_MATH_INTERSECT_LINE Line Intersection Computations
@ingroup TSM_UTILITIES_MATH_INTERSECT

@details

PURPOSE:
    (Provides intersect functions for a line segment against primitive shapes)

REFERENCE:
    (none)

ASSUMPTIONS AND LIMITATIONS:
    (none)

PROGRAMMERS:
    ((Shaun Williams) (L-3) (June 2011) (Initial version))

@{
*/


/// @brief Provides intersect functions for a line segment against primitive 3D shapes
class LineSegmentIntersect
{
    public:

    /// @brief determines if the given line segment intersects the given triangle
    static bool triangle(
            const double p[3],          // line endpoint 1
            const double q[3],          // line endpoint 2
            const double a[3],          // triangle point 1
            const double b[3],          // triangle point 2
            const double c[3],          // triangle point 3
            double x[3] = 0);           // output of intersection point

    /// @brief determines if the given line segment intersects the given cylinder
    static bool cylinder(
            const double sa[3],         // line endpoint 1
            const double sb[3],         // line endpoint 2
            const double p[3],          // cylinder point 1
            const double q[3],          // cylinder point 2
            double r,                   // cylinder radius
            double x[3] = 0);           // output of intersection point

    /// @brief determines if the given line segment intersects the given sphere
    static bool sphere(
            const double p[3],          // line endpoint 1
            const double q[3],          // line endpoint 2
            const double sc[3],         // sphere center
            double sr,                  // sphere radius
            double x[3] = 0);           // output of intersection point

    /// @brief determines if the given line segment intersects the given spheroid
    static bool spheroid(
            const double p[3],          // line endpoint 1
            const double q[3],          // line endpoint 2
            const double sc[3],         // spheroid center
            double srx,                 // spheroid x radius
            double sry,                 // spheroid y radius
            double srz,                 // spheroid z radius
            double x[3] = 0);           // output of intersection point
};

/// @}

#endif // LineSegmentIntersects_EXISTS
