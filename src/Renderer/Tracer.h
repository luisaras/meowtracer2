
#pragma once

#include <vector>
#include "Renderer.h"
#include "Scene.h"
#include "../Tree/CubeTree.h"

using std::vector;

class Tracer : public Renderer {
protected:
	virtual Color getColor(Ray &ray, float x, float y, int depth) = 0;
	
public:
	Scene scene;
	CubeTree* tree;

	// Tree
	int treeDepth = 1;
	int treeSize = INT_MAX;

	// Ray path
	int spr = 1;
	int rayDepth = 1;
	
	~Tracer() { delete tree; }
	void preprocess() { tree = new CubeTree(scene.hitables, treeDepth, treeSize); }
	Color getColor(Ray &ray, float x, float y) { return getColor(ray, x, y, rayDepth); }
};