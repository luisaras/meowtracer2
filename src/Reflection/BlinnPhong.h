
#pragma once

#include "../Light/Light.h"

class BlinnPhong {
public:
	Color diffuseColor (Light* light, LightHit &lh, Color& color);
	Color specularColor(Light* light, LightHit &lh);
};