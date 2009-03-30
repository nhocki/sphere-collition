#include "Utility.h"

/* Calculates the distance between two points
 */
float distPoints(Vector3 v1, Vector3 v2)
{
    return sqrt((v1[0] - v2[0])*(v1[0] - v2[0]) +
                (v1[1] - v2[1])*(v1[1] - v2[1]) +
                (v1[2] - v2[2])*(v1[2] - v2[2]));
}

/* Calculates the distance squared, to save one sqrt function
 */
float distPointsSquared(Vector3 v1, Vector3 v2)
{
    return (v1[0] - v2[0])*(v1[0] - v2[0]) +
           (v1[1] - v2[1])*(v1[1] - v2[1]) +
           (v1[2] - v2[2])*(v1[2] - v2[2]);
}

/* Calculates the distance from a plane to a point
   -If its 0 the point is on the plane
   -If its positive the point is on the same side of the normal of the plane
   -If its negative the point is on the other side of the nomral of the plane
 */
float distPointPlane(float a, float b, float c, float d, Vector3 v)
{
    return (a*v[0] + b*v[1] + c*v[2] + d)/sqrt(a*a + b*b + c*c);
}

/* Calculates the new vel and direction of the sphere */
void collision (Sphere &a, Sphere &b)
{
  
}
