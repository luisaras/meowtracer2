#include "RayTracer.h"
#include "../Math/Util.h"

void RayTracer::preprocess() {
	tree = new CubeTree(scene.hitables, treeDepth, treeSize);
}

Color RayTracer::getColor(Ray &initRay, float x, float y) {
	int depth = 0;
	Ray ray = initRay;
	Color color(1, 1, 1);
	while (true) {
		RayHit rh = tree->hit(ray);
		if (rh.hitable) {
			color = color * reflectionModel->getColor(tree, scene, ray, rh);
			if (depth >= rayCount) {
				break;
			} else {
				Point3 d = rh.normal + randomUnitVec3();
				ray = Ray(rh.point, d);
			}
		} else {
			color = color * backgroundColor(x, y);
			break;
		}
	}
	return color;
}

RayTracer::~RayTracer() {
	delete tree;
	delete reflectionModel;
}