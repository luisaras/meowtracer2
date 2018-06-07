#ifndef __LIGHT__
#define __LIGHT__

#include "../Math/Vec3.h"
#include "../Math/Matrix4.h"
#include "../Hitable/Hitable.h"
#include <cmath>

struct LightHit {
  RayHit& rayHit;
  Vec3 rayDir;
  Vec3 lightDir;
  Color color;
  LightHit(Vec3 &rd, RayHit &rh) : rayDir(-rd), rayHit(rh) {}
};

class Light {
public:
	Color color;
	Light(Color &c) : color(c) {}

	virtual LightHit hit(Vec3& rd, RayHit& rh) = 0;

};

#endif