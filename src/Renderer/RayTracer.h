#ifndef __RT__
#define __RT__

#include <vector>
#include "Renderer.h"
#include "Scene.h"
#include "ReflectionModel.h"
#include "../Tree/CubeTree.h"

using std::vector;

class RayTracer : public Renderer {
public:
	Scene scene;
	ReflectionModel* reflectionModel;
	CubeTree* tree;

	// Parameters
	int treeDepth = 1;
	int treeSize = INT_MAX;
	int rayCount = 1;

	RayTracer(ReflectionModel* rm) : reflectionModel(rm) {}
	~RayTracer();

	// Renderer
	void preprocess();
	Color getColor(Ray &initRay, float x, float y);
};

#endif