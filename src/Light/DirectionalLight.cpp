#include "DirectionalLight.h"

LightHit DirectionalLight::hit(Vec3& rd, RayHit& rh) {
	LightHit lh(rd, rh);
	lh.lightDir = direction;
	lh.color = color;
	return lh;
}