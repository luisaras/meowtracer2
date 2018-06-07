#ifndef __RM__
#define __RM__

#include "../Hitable/Hitable.h"
#include "../Tree/CubeTree.h"
#include "../Renderer/Scene.h"

class ReflectionModel {
public:
	virtual Color getColor(CubeTree* tree, Scene& scene, Ray& ray, RayHit& rayHit) = 0;
};

#endif