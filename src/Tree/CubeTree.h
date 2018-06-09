#ifndef __CT__
#define __CT__

#include "../Hitable/Hitable.h"
#include "../Light/Light.h"
#include <vector>
#include <climits>
using std::vector;

class CubeTree {
private:
	int depth = 0;
	vector<CubeTree*> children;
	Hitable* hitable = 0;
	Box bounds;
public:
	CubeTree(CubeTree* left, CubeTree* right);
	CubeTree(Hitable* hitable);
	CubeTree(vector<Hitable*>& triangles, int maxDepth, int maxLeaves);

	RayHit hit(Ray& ray);
	bool hitsLight(Light* light, LightHit& lh);

	~CubeTree();
};

#endif