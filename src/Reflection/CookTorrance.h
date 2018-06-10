
#pragma once

#include "ReflectionModel.h"

class CookTorrance : public ReflectionModel {
protected:
	virtual Color lightColor (Light* light, LightHit& lh, Color& texture);
public:
	Color getColor(CubeTree* tree, Scene& scene, Ray& ray, RayHit& rayHit);
};