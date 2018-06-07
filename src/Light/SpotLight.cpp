#include "SpotLight.h"
#include <iostream>

LightHit SpotLight::hit(Vec3& rd, RayHit& rh) {
	LightHit lh(rd, rh);
	lh.lightDir = origin - lh.rayHit.point;
	float cos = Vec3::Dot(lh.lightDir, direction) / lh.lightDir.Length();
	if (cos < angle) {
		lh.color = Color(0, 0, 0);
	} else {
		lh.color = (cos - angle) / (1 - angle) * color;
	}
	return lh;
}