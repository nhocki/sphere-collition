#include "Utility.h"
#include <iostream>
using namespace std;
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

/* Checks if two spheres are colliding
 */
bool areColliding(Sphere a, Sphere b)
{
  float r = (a.getR()+b.getR())*(a.getR()+b.getR());
  return r >= distPointsSquared(a.getPos(),b.getPos());
}

/* Check if a sphere is colliding with a wall
 */
bool sphereWallColliding(Sphere a, Wall b)
{
  GLfloat r = a.getR();
  return r >= distPointPlane(b.getA(), b.getB(), b.getC(), b.getD(), a.getPos());
}

/* 
   Calculates the new vel of the sphere
   If the angle is 0 is full reflection, if is 90, there is none
   So the dot product tells the angle beewteen the normal of the plane and
   The velocity so I know how much to reflect.
*/
void wallCollision(Sphere &s, Wall &w)
{
  Vector3 norm(w.getA(), w.getB(), w.getC());
  s.setVel(s.getVel() - ((-norm)*s.getVel().dot(-norm))*2);
}

/* Calculates the new vel and direction of the sphere */
void collision(Sphere &a, Sphere &b)
{
    
  /***
   * Segun: http://hyperphysics.phy-astr.gsu.edu/hbase/colsta.html
   *  -----> No se si es solo cuando se chocan "linealmente" pero igual
   * Pero creo que se puede aplicar para X y Y y sale
   * V'1 = V1*(m1 - m2) / (m1+m2)
   * V'2= (V1*2m1)/m1+m2
   *

   Vector3 va = a.getVel(), vb = b.getVel();

   float v1x = va[0], v1y = va[1], v1z=va[2];
   //float v2x = vb[0], v2y = vb[1], v2z=vb[2];
  
   GLfloat m1 = a.getMass(), m2=b.getMass();
   printf("Masa 1 ---> %f \nMasa 2 ---> %f\n",m1,m2);

   float f1x=((m1-m2)/(m1+m2))*v1x;
   float f1y=((m1-m2)/(m1+m2))*v1y;
   float f1z=((m1-m2)/(m1+m2))*v1z;
  
   float f2x=((2*m1)/(m1+m2))*v1x;
   float f2y=((2*m1)/(m1+m2))*v1y;
   float f2z=((2*m1)/(m1+m2))*v1z;
  
   printf("Velocidad 1:\n\t%f\t%f\t%f",f1x,f1y,f1z);
   puts("");
   printf("Velocidad 2:\n\t%f\t%f\t%f",f2x,f2y,f2z);
   puts("");

   a.setVel(Vector3(f1x,f1y,f1z));
   b.setVel(Vector3(f2x,f2y,f2z));
  */

  //TEMPORAL MEHTOD
  Vector3 disp = (a.getPos() - b.getPos()).normalize();
  a.setVel(a.getVel() - (disp * a.getVel().dot(disp) * 2));
  b.setVel(b.getVel() - (disp * b.getVel().dot(disp) * 2));
}
