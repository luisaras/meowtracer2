#include "CubeTree.h"
#include <stack>
#include "../Math/Util.h"

using std::stack;
using std::isnan;

CubeTree::CubeTree(Hitable* hit) {
	hitable = hit;
	bounds = hit->hitBox();
}

CubeTree::CubeTree(CubeTree* l, CubeTree* r) {
	children.push_back(l);
	children.push_back(r);
	depth = (l->depth > r->depth ? l->depth : r->depth) + 1;
}

CubeTree::CubeTree(vector<Hitable*>& hitables, int maxDepth, int maxNodes) {
	if (hitables.size() == 0)
		return;
	if (hitables.size() == 1) {
		hitable = hitables[0];
		bounds = hitable->hitBox();
		return;
	}
	children = vector<CubeTree*>(hitables.size());
	for (uint i = 0; i < hitables.size(); i++) {
		children[i] = new CubeTree(hitables[i]);
	}

	depth = 1;
	for (int size = children.size(); size < maxNodes; size++) {
		if (children.size() == 1) {
			hitable = children[0]->hitable;
			bounds = children[0]->bounds;
			children = children[0]->children;
			break;
		}
		if (depth >= maxDepth)
			break;
		int mini = 1, minj = 0;
		float minvol = INF;
		Box minbox;
		for (uint i = 0; i < children.size(); i++) {
			for (uint j = 0; j < i; j++) {
				Box box = children[i]->bounds.cluster(children[j]->bounds);
				float vol = box.volume();
				if (vol < minvol) {
					minvol = vol;
					mini = i;
					minj = j;
					minbox = box;
				}
			}
		}
		CubeTree* parent = new CubeTree(children[mini], children[minj]);
		parent->bounds = minbox;
		if (mini < minj) {
			children.erase(children.begin() + minj);
			children.erase(children.begin() + mini);
		} else {
			children.erase(children.begin() + mini);
			children.erase(children.begin() + minj);
		}
		children.push_back(parent);
		if (parent->depth > depth)
			depth = parent->depth;
	}
	for (uint i = 0; i < children.size(); i++) {
		bounds.bounds[0] = Vec3::min(bounds.bounds[0], children[i]->bounds.bounds[0]);
  		bounds.bounds[1] = Vec3::max(bounds.bounds[1], children[i]->bounds.bounds[1]);
	}
}

RayHit CubeTree::hit(Ray& ray, bool shadows) {
	RayHit minrh;

	stack<CubeTree*> trees;
	trees.push(this);
	while (!trees.empty()) {
		CubeTree* tree = trees.top();
		trees.pop();
		if (tree->bounds.hit(ray)) {
			if (tree->hitable) {
				if (tree->hitable->castShadows || !shadows) {
					RayHit rh = tree->hitable->hit(ray);
					if (!isnan(rh.t) && rh.t > 0 && (rh.t < minrh.t || isnan(minrh.t))) {
						minrh = rh;
					}
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
	Ray ray(lh.rayHit.point + lh.direction * ERR, lh.direction, 1);
	RayHit rh = hit(ray, true);
	return !isnan(rh.t) && rh.t > ERR && rh.t <= lh.distance;
}

CubeTree::~CubeTree() {
	for (uint i = 0; i < children.size(); i++)
		delete children[i];
}