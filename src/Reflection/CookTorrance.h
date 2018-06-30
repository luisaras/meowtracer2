
#pragma once

#include "../Math/Util.h"
#include "../Light/Light.h"
#include "../Tree/CubeTree.h"
#include "../Renderer/Scene.h"

class CookTorrance {
protected:
	Color lightColor(Light* light, LightHit& lh, Color& texture);
public:
	Color localColor(CubeTree* tree, Scene& scene, Ray& ray, RayHit& rh, Color& texture);
};