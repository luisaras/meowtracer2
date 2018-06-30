
#pragma once

#include "../Math/Util.h"
#include "../Light/Light.h"
#include "../Tree/CubeTree.h"
#include "../Renderer/Scene.h"

class CookTorranceMT {
public:
	Vec3 getMicrofacet();
	Color getColor();
};