/*
  Class Octree.
  A lot of this code was taken from:
  http://www.videotutorialsrock.com/opengl_tutorial/collision_detection/text.php
  Coded by: Bill Jacobs
 */

#ifndef OCTREE
#define OCTREE
#include <set>
#include <vector>
#include "math/Utility.h"
#include "math/Vector3.h"
#include "objects/Sphere.h"
#include "objects/Wall.h"

using namespace std;

struct SphereWallPair
{
};

struct SpherePair
{
};

class Octree
{
private:
    //Cornes of the box and the middle
    Vector3 min, max, mid;
    
    //Thefirst dimension is for x, the second for y, the third for z
    //ex octree[0][x][x] is the division that is bettween minx and centerx
    Octree children[2][2][2];

    bool hasChildren;
    set<Sphere*> spheres;
    //The depth of this node
    int depth;
    //The number of balls this node contains (it includes its children)
    int numSpheres;

    void fileSphere(Sphere *s, Vector3 pos, bool add);
    void haveChildren(void);
    void collectSpheres(set<Spheres*> &ss);
    void destroyChildren(void);
    void remove(Sphere *s, Vector3 pos);
    void potentialSphereWallCollisions(vector<SphereWallPair> &cs, Wall w, char coord, int dir);
   
public:
    Octree(Vector3 min, Vector3 max, int d);
    ~Octree(void);
    void add(Sphere *s);
    void remove(Sphere *s);
    void ballMoved(Sphere *s, Vector3 oldPos);
    void potentialSphereCollisions(vector<SpherePair> &cs);
    void potentialSphereWallCollisions(vector<SPhereWallPair> &cs);
};

#endif
