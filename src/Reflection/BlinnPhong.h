
#pragma once

#include "../Light/Light.h"
#include "../Tree/CubeTree.h"
#include "../Renderer/Scene.h"

class BlinnPhong {
protected:
	Color localDiffuse(Light* light, LightHit &lh, Color& color);
	Color localSpecular(Light* light, LightHit &lh);
public:
	Color localColor(CubeTree* tree, Scene& scene, Ray& ray, RayHit& rh, Color& texture);
};