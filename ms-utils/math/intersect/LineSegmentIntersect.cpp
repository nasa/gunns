/************************ TRICK HEADER *****************************************************
PURPOSE:
    (Provides intersect functions for a line segment against primitive shapes)

REFERENCE:
    (none)

ASSUMPTIONS AND LIMITATIONS:
    (none)

PROGRAMMERS:
    ((Shaun Williams) (L-3) (June 2011) (Initial version))
********************************************************************************************/

// System includes
#include <cmath>
#include <cstddef>

// JEOD 2.0 includes
#include "utils/math/include/vector3.hh"

// TS includes
#include "LineSegmentIntersect.hh"


// macros to decouple the algorithms from implementation-specific vector operations
// (currently using JEOD2's Vector3 class)
#define CROSS(dest,v1,v2)   Vector3::cross(v1,v2,dest)
#define DOT(v1,v2)          Vector3::dot(v1,v2)
#define SUB(dest,v1,v2)     Vector3::diff(v1,v2,dest)
#define MAG(v)              Vector3::vmag(v);
#define SCALE(v, s)         Vector3::scale(s,v);

// interpolates a point between two vectors
#define POINT_ON_LINE(result, v1,v2,t) \
    result[0] = v1[0] + (v2[0]-v1[0])*t; \
    result[1] = v1[1] + (v2[1]-v1[1])*t; \
    result[2] = v1[2] + (v2[2]-v1[2])*t;

// tolerance radius around zero
#define EPSILON 0.00001

/// @param p line endpoint 1
/// @param q line endpoint 2
/// @param a triangle point 1
/// @param b triangle point 2
/// @param c triangle point 3
/// @param x output of intersection point
///
/// @details
///    Algorithm from:
///        _Real-Time Collision Detection_ by Christer Ericson
///        Section 5.3.6
///
///    Credits:
///        Christer Ericson's variant of the algorithm by Tomas Möller and Ben Trumbore
///        Original Algorithm:
///            "Fast, minimum storage ray-triangle intersection"
///            http://www.graphics.cornell.edu/pubs/1997/MT97.html
///
/// @note
///    The original algorithm returned false if d < 0,
///    causing an intersection detection if and only if the triangle's
///    points were specified in ClockWise orientation
///    when viewed from the origin of the directed line
///    segment.  This is useful if triangles are specified
///    in a standard way and the triangle is a part of a
///    continuous mesh.  If this is true, then you can ignore
///    all triangles that are facing away from the line segment.
///
///    The "if (d < 0)" branch was added in order to
///    detect intersection regardless of the order
///    of the points in the triangle and line.
///
/// @return intersection found is true|false
bool LineSegmentIntersect::triangle(
            const double p[3],
            const double q[3],
            const double a[3],
            const double b[3],
            const double c[3],
            double x[3])
{
    // FIXME: ensure triangle has area
    // FIXME: ensure p != q

    double u,v,w,t;
    double ab[3], ac[3], qp[3];
    SUB(ab, b,a);
    SUB(ac, c,a);
    SUB(qp, p,q);

    double n[3];
    CROSS(n, ab,ac);

    double d = DOT(qp, n);
    if (fabs(d) < EPSILON) return false;

    if (d < 0.0) {
        double ap[3];
        SUB(ap, p,a);
        t = DOT(ap, n);
        if (t > 0) {
            return false;
        }
        if (t < d) {
            return false;
        }

        double e[3];
        CROSS(e, qp,ap);
        v = DOT(ac, e);
        if (v > 0.0 || v < d) {
            return false;
        }
        w = -DOT(ab, e);
        if (w > 0.0 || v+w < d) {
            return false;
        }
    }
    else {
        double ap[3];
        SUB(ap, p,a);
        t = DOT(ap, n);
        if (t < 0) {
            return false;
        }
        if (t > d) {
            return false;
        }

        double e[3];
        CROSS(e, qp,ap);
        v = DOT(ac, e);
        if (v < 0.0 || v > d) {
            return false;
        }
        w = -DOT(ab, e);
        if (w < 0.0 || v+w > d) {
            return false;
        }
    }

    // intersection is found

    // compute of barycentric coordinates (u,v,w)
    /*
    double ood = 1.0 / d;
    t *= ood;
    v *= ood;
    w *= ood;
    u = 1.0 - v - w;
    */

    // compute intersection point if an array was passed for it
    if (NULL != x) {
        t /= d;
        POINT_ON_LINE(x, p,q,t);
    }
    return true;
}

/// @param sa line endpoint 1
/// @param sb line endpoint 2
/// @param p center of circle face 1 on cylinder
/// @param q center of circle face 2 on cylinder
/// @param r cylinder radius
/// @param x output of intersection point
///
/// @details
///   Algorithm from:
///        _Real-Time Collision Detection_ by Christer Ericson
///        Section 5.3.7
///
/// @return intersection found is true|false
bool LineSegmentIntersect::cylinder(
            const double sa[3],
            const double sb[3],
            const double p[3],
            const double q[3],
            double r,
            double x[3])
{
    // FIXME: ensure r != 0
    // FIXME: ensure sa != sb
    // FIXME: ensure p != q

    double t;
    double d[3], m[3], n[3];
    SUB(d, q,p);
    SUB(m, sa,p);
    SUB(n, sb,sa);
    double md = DOT(m, d);
    double nd = DOT(n, d);
    double dd = DOT(d, d);

    // Test if segment fully outside either endcap of cylinder
    if (md < 0.0 && md + nd < 0.0) {
        return false; // Segment outside 'p' side of cylinder
    }
    if (md > dd && md + nd > dd) {
        return false;     // Segment outside 'q' side of cylinder
    }

    double nn = DOT(n, n);
    double mn = DOT(m, n);
    double a = dd * nn - nd * nd;
    double k = DOT(m, m) - r * r;
    double c = dd * k - md * md;

    double b,discr;

    if (fabs(a) < EPSILON) {
        // Segment runs parallel to cylinder axis
        if (c > 0.0) {
            return false; // 'a' and thus the segment lie outside cylinder
        }
        // Now known that segment intersects cylinder; figure out how it intersects
        if (md < 0.0) {
            t = -mn / nn; // Intersect segment against 'p' endcap
        }
        else if (md > dd) {
            t = (nd - mn) / nn; // Intersect segment against 'q' endcap
        }
        else {
            t = 0.0; // 'a' lies inside cylinder
        }
        if (NULL != x) {
            POINT_ON_LINE(x, sa,sb,t);
        }
        return true;
    }

    b = dd * mn - nd * md;
    discr = b * b - a * c;
    if (discr < 0.0) {
        return false; // No real roots; no intersection
    }

    t = (-b - sqrt(discr)) / a;
    if (t < 0.0 || t > 1.0f) return false; // Intersection lies outside segment

    if (md + t * nd < 0.0) {

        // Intersection outside cylinder on 'p' side
        if (nd <= 0.0) { // Segment pointing away from endcap
            return false;
        }
        t = -md / nd;

        // Keep intersection if DOT(S(t) - p, S(t) - p) <= r^2
        if (k + 2 * t * (mn + t * nn) <= 0.0) {
            if (NULL != x) {
                POINT_ON_LINE(x, sa,sb,t);
            }
            return true;
        }
        else {
            return false;
        }
    } else if (md + t * nd > dd) {

        // Intersection outside cylinder on 'q' side
        if (nd >= 0.0) {
            return false; // Segment pointing away from endcap
        }
        t = (dd - md) / nd;

        // Keep intersection if DOT(S(t) - q, S(t) - q) <= r^2
        if (k + dd - 2 * md + t * (2 * (mn - nd) + t * nn) <= 0.0) {
            if (NULL != x) {
                POINT_ON_LINE(x, sa,sb,t);
            }
            return true;
        }
        else {
            return false;
        }
    }

    if (NULL != x) {
        POINT_ON_LINE(x, sa,sb,t);
    }
    return true;
}

/// @param p line endpoint 1
/// @param q line endpoint 2
/// @param sc sphere center
/// @param sr sphere radius
/// @param x output of intersection point
///
/// @details
///   Algorithm from:
///        _Real-Time Collision Detection_ by Christer Ericson
///        Section 5.3.2
///
///   Added:
///         * take a segment instead of a ray
///         * extra case for both intersection points behind the start
///
/// @return intersection found is true|false
bool LineSegmentIntersect::sphere(
            const double p[3],
            const double q[3],
            const double sc[3],
            double sr,
            double x[3])
{
    // FIXME: ensure sr != 0
    // FIXME: ensure p != q

    double d[3];
    SUB(d, q,p);

    double dist = MAG(d);
    SCALE(d, 1.0/dist);

    double m[3];
    SUB(m, p,sc);

    double b = DOT(m, d);
    double c = DOT(m, m) - sr * sr;

    // Exit if r's origin outside s (c > 0) and r pointing away from s (b > 0)
    if (c > 0.0f && b > 0.0f) {
        return false;
    }
    double discr = b*b - c;

    // A negative discriminant corresponds to ray missing sphere
    if (discr < 0.0f) {
        return false;
    }

    // Ray now found to intersect sphere, compute smallest t value of intersection
    double sqrtd = sqrt(discr);
    double t = -b - sqrtd;

    if (t < 0.0f) {
        // ray is outside the sphere, with both intersections behind it
        if (sqrtd < b) {
            return false;
        }
        // ray started inside sphere so clamp t to zero
        t = 0.0f;
    }

    // intersection point behind the end point
    if (t > dist) {
        return false;
    }

    // compute intersection point
    if (NULL != x) {
        x[0] = p[0] + t*d[0];
        x[1] = p[1] + t*d[1];
        x[2] = p[2] + t*d[2];
    }
    return true;
}

/// @param p line endpoint 1
/// @param q line endpoint 2
/// @param sc spheroid center
/// @param srx spheroid x radius
/// @param sry spheroid y radius
/// @param srz spheroid z radius
/// @param x output of intersection point
///
/// @details
///   Algorithm by Robert J. Paul (L-3):
///     Transform the euclidean space such that the spheroid becomes a unit sphere,
///     then perform a sphere intersection check.  The inverse transform can then
///     be applied to the intersection point on the sphere to obtain the true
///     intersection point on the spheroid.
///
/// @return intersection found is true|false
bool LineSegmentIntersect::spheroid(
            const double p[3],
            const double q[3],
            const double sc[3],
            double srx,
            double sry,
            double srz,
            double x[3])
{
    // FIXME: ensure srx, sry, srz != 0
    // FIXME: ensure p != q

    // transform space to make the spheroid a unit sphere

    double p0[3];
    p0[0] = (p[0] - sc[0]) / srx;
    p0[1] = (p[1] - sc[1]) / sry;
    p0[2] = (p[2] - sc[2]) / srz;

    double q0[3];
    q0[0] = (q[0] - sc[0]) / srx;
    q0[1] = (q[1] - sc[1]) / sry;
    q0[2] = (q[2] - sc[2]) / srz;

    double sc0[3] = {0.0, 0.0, 0.0};

    bool result = sphere(p0,q0,sc0,1.0,x);

    if (NULL != x)
    {
        // inverse transform the intersection point
        x[0] = x[0] * srx + sc[0];
        x[1] = x[1] * sry + sc[1];
        x[2] = x[2] * srz + sc[2];
    }

    return result;
}
