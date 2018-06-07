#include "RayTracer.h"
#include "../Math/Util.h"

void RayTracer::preprocess() {
	tree = new CubeTree(scene.hitables, treeDepth, treeSize);
}

Color RayTracer::getColor(Ray &initRay, float x, float y) {
	int depth = 0;
	Ray ray = initRay;
	Color color(1, 1, 1);
	RayHit rh = tree->hit(ray);
	if (rh.hitable) {
		color = color * reflectionModel->getColor(tree, scene, ray, rh);
	} else {
		color = color * backgroundColor(x, y);
	}
	return color;
}

RayTracer::~RayTracer() {
	delete tree;
	delete reflectionModel;
}