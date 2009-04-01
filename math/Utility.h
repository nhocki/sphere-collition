#ifndef UTILITY
#define UTILITY
#include <math.h>
#include "Vector3.h"
#include "../objects/Sphere.h"

#define PI 3.141592653589793115997963468544185161590576171875

float distance(Vector3 v1, Vector3 v2);
float distanceSquared(Vector3 v1, Vector3 v2);
//bool collision(Sphere a, Sphere b);
#endif
