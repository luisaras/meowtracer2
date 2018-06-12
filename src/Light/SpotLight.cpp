#include "SpotLight.h"
#include <iostream>

LightHit SpotLight::hit(Ray& ray, RayHit& rh) {
	LightHit lh(ray, rh);
	lh.direction = origin - rh.point;
	float distance = lh.direction.length();
	float cos = Vec3::dot(lh.direction, direction) / distance;
	if (cos < angle) {
		lh.color = Color(0, 0, 0);
	} else {
		lh.color = (cos - angle) / (1 - angle) * color;
	}
	lh.distance = distance;
	return lh;
}