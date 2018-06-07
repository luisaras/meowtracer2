#include "SphereLight.h"
#include "../Math/Util.h"

LightHit SphereLight::hit(Vec3& rd, RayHit& rh) {
	LightHit lh(rd, rh);
	Vec3 v = origin - lh.rayHit.point;
	lh.lightDir = v + randomUnitVec3(size);
	lh.lightDir = Vec3::Normalize(lh.lightDir);
	lh.color = color;
	return lh;
}