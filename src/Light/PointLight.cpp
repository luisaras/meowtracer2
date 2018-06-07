#include "PointLight.h"

LightHit PointLight::hit(Vec3& rd, RayHit& rh) {
	LightHit lh(rd, rh);
	Vec3 dir = origin - lh.rayHit.point;
	float distance = dir.Length();
	lh.lightDir = Vec3::Normalize(dir);
	lh.color = color * length / (distance * distance);
	return lh;
}