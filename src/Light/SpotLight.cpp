#include "SpotLight.h"
#include <iostream>

LightHit SpotLight::hit(Ray& ray, RayHit& rh) {
	LightHit lh(ray, rh);
	lh.direction = origin - rh.point;
	float cos = Vec3::Dot(lh.direction, direction) / lh.direction.Length();
	if (cos < angle) {
		lh.color = Color(0, 0, 0);
	} else {
		lh.color = (cos - angle) / (1 - angle) * color;
	}
	return lh;
}