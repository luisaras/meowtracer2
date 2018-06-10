
#pragma once

#include "ReflectionModel.h"

class BlinnPhong : public ReflectionModel {
protected:
	Color diffuseColor (Light* light, LightHit &lh);
	Color specularColor(Light* light, LightHit &lh);
public:
	Color getColor(CubeTree* tree, Scene& scene, Ray& ray, RayHit& rayHit);
};