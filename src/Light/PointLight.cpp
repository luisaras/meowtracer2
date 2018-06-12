#include "PointLight.h"

LightHit PointLight::hit(Ray& ray, RayHit& rh) {
	LightHit lh(ray, rh);
	Vec3 dir = origin - rh.point;
	float distance = dir.length();
	lh.direction = Vec3::normalize(dir);
	lh.color = color * length / (distance * distance);
	lh.distance = distance;
	return lh;
}