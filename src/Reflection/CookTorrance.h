
#pragma once

#include "../Math/Util.h"
#include "../Light/Light.h"

class CookTorrance {
public:
	Color lightColor(Light* light, LightHit& lh, Color& texture);
};