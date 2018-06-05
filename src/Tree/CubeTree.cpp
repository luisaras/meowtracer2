#include "CubeTree.h"
#include <stack>
#include "../Math/Util.h"

using std::stack;
using std::isnan;

CubeTree::CubeTree(Hitable* hit) {
	hitable = hit;
	bounds = hitable->hitBox();
}

CubeTree::CubeTree(CubeTree* l, CubeTree* r) {
	children.push_back(l);
	children.push_back(r);
	depth = (l->depth > r->depth ? l->depth : r->depth) + 1;
}

CubeTree::CubeTree(vector<Hitable*>& hitables, int maxDepth, int maxLeaves) {
	if (hitables.size() == 1) {
		hitable = hitables[0];
		bounds = hitable->hitBox();
		return;
	}
	children = vector<CubeTree*>(hitables.size());
	for (uint i = 0; i < hitables.size(); i++) {
		children[i] = new CubeTree(hitables[i]);
	}
	int depth = 0;
	for (int size = children.size(); depth < maxDepth && size < maxLeaves; size++) {
		if (children.size() == 1)
			break;
		int mini = 1, minj = 0;
		float minvol = INF;
		for (uint i = 0; i < children.size(); i++) {
			for (uint j = 0; j < i; j++) {
				float vol = children[i]->bounds.cluster(children[j]->bounds).volume();
				if (vol < minvol) {
					minvol = vol;
					mini = i;
					minj = j;
				}
			}
		}
		CubeTree* parent = new CubeTree(children[mini], children[minj]);
		if (mini < minj) {
			children.erase(children.begin() + mini);
			children.erase(children.begin() + minj);
		} else {
			children.erase(children.begin() + minj);
			children.erase(children.begin() + mini);
		}
		children.push_back(parent);
		if (parent->depth > depth)
			depth = parent->depth;
	}
}

RayHit CubeTree::hit(Ray& ray) {
	RayHit minrh;

	stack<CubeTree*> trees;
	trees.push(this);
	while (!trees.empty()) {
		CubeTree* tree = trees.top();
		trees.pop();
		if (tree->bounds.hit(ray)) {
			if (tree->hitable) {
				RayHit rh = tree->hitable->hit(ray);
				if (!std::isnan(rh.t) && rh.t < minrh.t && rh.t > 0) {
					minrh = rh;
				}
			} else {
				for (uint i = 0; i < tree->children.size(); i++)
					trees.push(tree->children[i]);
			}
		}
	}
	return minrh;
}

bool CubeTree::hitsLight(Light* light, LightHit& lh) {
	Ray ray(lh.rayHit.point, lh.lightDir);
	RayHit rh = hit(ray);
	return !isnan(rh.t) && rh.t > 0.00001 && rh.t <= 1;
}

CubeTree::~CubeTree() {
	for (uint i = 0; i < children.size(); i++)
		delete children[i];
}