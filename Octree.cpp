#include "Octree.h"

/* Private methods */
void Octree::fileSphere(Sphere *s, Vector3 pos, bool add)
{
	//Figure where does the sphere belongs to
	for(int x = 0; x < 2; ++x)
	{
		if(x == 0)
		{
			if(pos[0] - s->getR() > center[0])
				continue;
		}
		else if (pos[0] + s->getR() < center[0])
			continue;

		for(int y = 0; y < 2; y++)
		{
			if (y == 0)
			{
				if (pos[1] - s->getR() > center[1])
					continue;
			}
			else if (pos[1] + s->getR() < center[1])
				continue;

			for(int z = 0; z < 2; z++)
			{
				if (z == 0)
				{
					if (pos[2] - s->getR() > center[2])
						continue;
				}
				else if (pos[2] + s->getR() < center[2])
					continue;

				//Add or remove the ball
				if (add)
					children[x][y][z]->add(s);
				else
					children[x][y][z]->remove(s, pos);
			}
		}
	}
}

//Creates children of this node, and moves the balls tho the children
void Octree::haveChildren()
{
	for(int x = 0; x < 2; x++)
	{
		float minX;
		float maxX;
		if (x == 0)
		{
			minX = corner1[0];
			maxX = center[0];
		}
		else
		{
			minX = center[0];
			maxX = corner2[0];
		}

		for(int y = 0; y < 2; y++)
		{
			float minY;
			float maxY;
			if (y == 0)
			{
				minY = corner1[1];
				maxY = center[1];
			}
			else
			{
				minY = center[1];
				maxY = corner2[1];
			}

			for(int z = 0; z < 2; z++)
			{
				float minZ;
				float maxZ;
				if (z == 0)
				{
					minZ = corner1[2];
					maxZ = center[2];
				}
				else
				{
					minZ = center[2];
					maxZ = corner2[2];
				}

				children[x][y][z] = new Octree(Vec3f(minX, minY, minZ),
											   Vec3f(maxX, maxY, maxZ),
											   depth + 1);
			}
		}
	}
	//Remove all balls from "balls" and add them to the new children
	for(set<Sphere*>::iterator it = spheres.begin(); it != balls.end();it++)
	{
		Sphere* s = *it;
		fileBall(s, s->getPos(), true);
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
					children[x][y][z]->collectSPheres(ss);
				}
			}
		}
	}
	else {
		for(set<Sphere*>::iterator it = spheress.begin(); it != spheres.end();
			it++) {
			Sphere* s = *it;
			bs.insert(s);
		}
	}
}

//Destroys its children, and move the balls to this node.
void Octree::destroyChildren() {
	//Move all balls in descendants of this to the "balls" set
	collectBalls(balls);

	for(int x = 0; x < 2; x++)
		for(int y = 0; y < 2; y++)
			for(int z = 0; z < 2; z++)
				delete children[x][y][z];

	hasChildren = false;
}

//Removes the specified ball at the indicated position
void Octree::remove(Ball* ball, Vec3f pos) {
	numBalls--;

	if (hasChildren && numBalls < MIN_BALLS_PER_OCTREE)
		destroyChildren();
	if (hasChildren)
		fileBall(ball, pos, false);
	else
		balls.erase(ball);
}

/* Helper fuction for potentialBallWallCollision.Adds potential ball-wall
   collisions to ss, where w is the type of wall, coord is the relevant
   coordinate of the wall ('x', 'y', or 'z'), and dir is 0 if
   the wall is in the negative direction and 1 if it is in the positive
   direction.  Assumes that this is in the extreme direction of
   the coordinate, e.g. if w is WALL_TOP, the function assumes that
   this is in the far upward direction.
 */
void Octree::potentialSphereWallCollisions(vector<SphereWallPair> &cs, Wall w, char coord, int dir)
{
	if (hasChildren)
	{
		//Recursively call potentialBallWallCollisions on the correct
		//half of the children (e.g. if w is WALL_TOP, call it on
		//children above centerY)
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
				child->potentialSphereWallCollisions(cs, w, coord, dir);
			}
		}
	}
	else {
		//Add (ball, w) for all balls in this
		for(set<Sphere*>::iterator it = spheres.begin(); it != spheres.end();it++)
		{
			Shpere* s = *it;
			SphereWallPair swp;
			swp.sphere = s;
			swp.wall = w;
			cs.push_back(swp);
		}
	}
}

/* Public Methods */
//Constructs a new Octree.  c1 is (minX, minY, minZ), c2 is (maxX, maxY,
//maxZ), and d is the depth, which starts at 1.
Octree::Octree(Vector3 c1, Vector3 c2, int d) {
	corner1 = c1;
	corner2 = c2;
	center = (c1 + c2) / 2;
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
		numSpheres > MAX_SHPERES_PER_OCTREE) {
		haveChildren();
	}

	if (hasChildren) {
		fileSphere(s, s>getPos(), true);
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
void Octree::shpereMoved(Sphere* s, Vector3 oldPos) {
	remove(s, oldPos);
	add(s);
}

//Adds potential shpere-shpere collisions to the specified set
void Octree::potentialSphereSphereCollisions(vector<SpherePair> &collisions) {
	if (hasChildren)
		for(int x = 0; x < 2; x++)
			for(int y = 0; y < 2; y++)
				for(int z = 0; z < 2; z++)
					children[x][y][z]->potentialSphereSphereCollisions(collisions);

	else
	{
		//Add all pairs (shpere1, shpere2) from shperes
		for(set<Sphere*>::iterator it = shperes.begin(); it != shperes.end();
			it++) {
			Sphere* s1 = *it;
			for(set<Sphere*>::iterator it2 = shperes.begin();
				it2 != shperes.end(); it2++) {
				Sphere* s2 = *it2;
				//This test makes sure that we only add each pair once
				if (s1 < s2) {
					SpherePair sp;
					sp.s1 = s1;
					sp.s2 = s2;
					collisions.push_back(sp);
				}
			}
		}
	}
}

//Adds potential shpere-wall collisions to the specified set
void Octree::potentialSphereWallCollisions(vector<SphereWallPair> &collisions) {
	potentialSphereWallCollisions(collisions, WALL_LEFT, 'x', 0);
	potentialSphereWallCollisions(collisions, WALL_RIGHT, 'x', 1);
	potentialSphereWallCollisions(collisions, WALL_BOTTOM, 'y', 0);
	potentialSphereWallCollisions(collisions, WALL_TOP, 'y', 1);
	potentialSphereWallCollisions(collisions, WALL_FAR, 'z', 0);
	potentialSphereWallCollisions(collisions, WALL_NEAR, 'z', 1);
}
