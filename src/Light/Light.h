#ifndef __LIGHT__
#define __LIGHT__

#include "../Math/Vec3.h"
#include "../Math/Matrix4.h"
#include "../Hitable/Hitable.h"
#include <cmath>

struct LightHit {
  RayHit& rayHit;
  Ray& ray;
  Vec3 direction;
  Color color;
  LightHit(Ray& r, RayHit& rh) : ray(r), rayHit(rh) {}
};

class Light {
public:
	Color color;
	Light(Color &c) : color(c) {}

	virtual LightHit hit(Ray& ray, RayHit& rh) = 0;
	virtual ~Light() {}

};

#endif