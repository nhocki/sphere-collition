#include "Octree.h"

/* Private methods */
void Octree::fileSphere(Sphere *s, Vector3 pos, bool add)
{
	//Figure where does the sphere belongs to
	for(int x = 0; x < 2; ++x)
	{
		if(x == 0)
		{
			if(pos[0] - s->getR() > mid[0])
				continue;
		}
		else if (pos[0] + s->getR() < mid[0])
			continue;

		for(int y = 0; y < 2; y++)
		{
			if (y == 0)
			{
				if (pos[1] - s->getR() > mid[1])
					continue;
			}
			else if (pos[1] + s->getR() < mid[1])
				continue;

			for(int z = 0; z < 2; z++)
			{
				if (z == 0)
				{
					if (pos[2] - s->getR() > mid[2])
						continue;
				}
				else if (pos[2] + s->getR() < mid[2])
					continue;

				//Add or remove the sphere
				if (add)
					children[x][y][z]->add(s);
				else
					children[x][y][z]->remove(s, pos);
			}
		}
	}
}

//Creates children of this node, and moves the spheres tho the children
void Octree::haveChildren()
{
	for(int x = 0; x < 2; x++)
	{
		float minX;
		float maxX;
		if (x == 0)
		{
			minX = min[0];
			maxX = mid[0];
		}
		else
		{
			minX = mid[0];
			maxX = max[0];
		}

		for(int y = 0; y < 2; y++)
		{
			float minY;
			float maxY;
			if (y == 0)
			{
				minY = min[1];
				maxY = mid[1];
			}
			else
			{
				minY = mid[1];
				maxY = max[1];
			}

			for(int z = 0; z < 2; z++)
			{
				float minZ;
				float maxZ;
				if (z == 0)
				{
					minZ = min[2];
					maxZ = mid[2];
				}
				else
				{
					minZ = mid[2];
					maxZ = max[2];
				}

				children[x][y][z] = new Octree(Vector3(minX, minY, minZ),
											   Vector3(maxX, maxY, maxZ),
											   depth + 1);
			}
		}
	}
	//Remove all spheres from "spheres" and add them to the new children
	for(set<Sphere*>::iterator it = spheres.begin(); it != spheres.end();it++)
	{
		Sphere* s = *it;
		fileSphere(s, s->getPos(), true);
	}
	spheres.clear();

	hasChildren = true;
}

//Adds all spheres in this or one of its descendants to the specified set
void Octree::collectSpheres(set<Sphere*> &ss) {
	if (hasChildren) {
		for(int x = 0; x < 2; x++) {
			for(int y = 0; y < 2; y++) {
				for(int z = 0; z < 2; z++) {
					children[x][y][z]->collectSpheres(ss);
				}
			}
		}
	}
	else {
		for(set<Sphere*>::iterator it = spheres.begin(); it != spheres.end();
			it++) {
			Sphere* s = *it;
			ss.insert(s);
		}
	}
}

//Destroys its children, and move the spheres to this node.
void Octree::destroyChildren() {
	//Move all spheres in descendants of this to the "spheres" set
	collectSpheres(spheres);

	for(int x = 0; x < 2; x++)
		for(int y = 0; y < 2; y++)
			for(int z = 0; z < 2; z++)
				delete children[x][y][z];

	hasChildren = false;
}

//Removes the specified sphere at the indicated position
void Octree::remove(Sphere* sphere, Vector3 pos) {
	numSpheres--;

	if (hasChildren && numSpheres < MIN_SPHERES_PER_OCTREE)
		destroyChildren();
	if (hasChildren)
		fileSphere(sphere, pos, false);
	else
		spheres.erase(sphere);
}

/* Helper fuction for potentialSphereWallCollision.Adds potential sphere-wall
   collisions to ss, where w is the type of wall, coord is the relevant
   coordinate of the wall ('x', 'y', or 'z'), and dir is 0 if
   the wall is in the negative direction and 1 if it is in the positive
   direction.  Assumes that this is in the extreme direction of
   the coordinate, e.g. if w is WALL_TOP, the function assumes that
   this is in the far upward direction.
 */
void Octree::potentialSphereWallCollisions(vector<SphereWallPair> &cs, Wall *w)
{
	int dir = 0;
	char coord = w->getCoord();
	if(coord == 'x')dir = (w->getA() > 0)?0:1;
	if(coord == 'y')dir = (w->getB() > 0)?0:1;
	if(coord == 'z')dir = (w->getC() > 0)?0:1;

	//cout << coord << << " " dir << endl;
	if (hasChildren)
	{
		//Recursively call potentialSphereWallCollisions on the correct
		//half of the children (e.g. if w is WALL_TOP, call it on
		//children above midY)
		for(int dir2 = 0; dir2 < 2; dir2++)
		{
			for(int dir3 = 0; dir3 < 2; dir3++)
			{
				Octree *child;
				switch (coord)
				{
				case 'x':
					child = children[dir][dir2][dir3];
					break;
				case 'y':
					child = children[dir2][dir][dir3];
					break;
				case 'z':
					child = children[dir2][dir3][dir];
					break;
				}
				child->potentialSphereWallCollisions(cs, w);
			}
		}
	}
	else {
		//Add (sphere, w) for all spheres in this
		for(set<Sphere*>::iterator it = spheres.begin(); it != spheres.end();it++)
		{
			Sphere* s = *it;
			SphereWallPair swp;
			swp.first = s;
			swp.second = w;
			cs.push_back(swp);
		}
	}
}

/* Public Methods */
//Constructs a new Octree.  c1 is (minX, minY, minZ), c2 is (maxX, maxY,
//maxZ), and d is the depth, which starts at 1.
Octree::Octree(Vector3 c1, Vector3 c2, int d) {
	min = c1;
	max = c2;
	mid = (c1 + c2) / 2;
	depth = d;
	numSpheres = 0;
	hasChildren = false;
}

//Destructor
Octree::~Octree() {
	if (hasChildren) {
		destroyChildren();
	}
}

//Adds a shpere to this
void Octree::add(Sphere* s) {
	numSpheres++;
	if (!hasChildren && depth < MAX_OCTREE_DEPTH &&
		numSpheres > MAX_SPHERES_PER_OCTREE) {
		haveChildren();
	}

	if (hasChildren) {
		fileSphere(s, s->getPos(), true);
	}
	else {
		spheres.insert(s);
	}
}

//Removes a shpere from this
void Octree::remove(Sphere* s) {
	remove(s, s->getPos());
}

//Changes the position of a shpere in this from oldPos to shpere->pos
void Octree::sphereMoved(Sphere* s, Vector3 oldPos) {
	remove(s, oldPos);
	add(s);
}

//Adds potential shpere-shpere collisions to the specified set
void Octree::potentialSphereCollisions(vector<SpherePair> &collisions) {
	if (hasChildren)
		for(int x = 0; x < 2; x++)
			for(int y = 0; y < 2; y++)
				for(int z = 0; z < 2; z++)
					children[x][y][z]->potentialSphereCollisions(collisions);

	else
	{
		//Add all pairs (shpere1, shpere2) from shperes
		for(set<Sphere*>::iterator it = spheres.begin(); it != spheres.end();
			it++) {
			Sphere* s1 = *it;
			for(set<Sphere*>::iterator it2 = spheres.begin();
				it2 != spheres.end(); it2++) {
				Sphere* s2 = *it2;
				//This test makes sure that we only add each pair once
				if (s1 < s2) {
					SpherePair sp;
					sp.first = s1;
					sp.second = s2;
					collisions.push_back(sp);
				}
			}
		}
	}
}

//Adds potential shpere-wall collisions to the specified set
void Octree::potentialSphereWallCollisions(vector<SphereWallPair> &cs, vector<Wall*>walls){
	for(unsigned int i = 0; i < walls.size(); ++i)
		potentialSphereWallCollisions(cs, walls[i]);
}
