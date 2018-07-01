
#pragma once

#include "../Light/Light.h"
#include "../Tree/CubeTree.h"
#include "../Renderer/Scene.h"

class CookTorrance {
public:
	float reflectedColor(Vec3& m, Vec3& w0, Vec3& wi, Vec3& h, float h_w0, float rough);
	Color reflectedColor(Vec3& n, Vec3& w0, Vec3& wi, Material* mat);
	Color localColor(CubeTree* tree, Scene& scene, Ray& ray, RayHit& rh);
};