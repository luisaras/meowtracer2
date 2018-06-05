#ifndef __LIGHT__
#define __LIGHT__

#include "../Math/Vec3.h"
#include "../Math/Matrix4.h"
#include "../Hitable/Hitable.h"
#include <cmath>

struct LightHit {
  RayHit& rayHit;
  Vec3& rayDir;
  Vec3 lightDir;
  LightHit(Vec3 &rd, RayHit &rh) : rayDir(rd), rayHit(rh) {}
};

class Light {
public:
	Color color;
	Light(Color &c) : color(c) {}

	virtual Vec3 getDirection(LightHit& lh) = 0;
	virtual float getIntensity(LightHit& lh) { return 1; }

};

#endif