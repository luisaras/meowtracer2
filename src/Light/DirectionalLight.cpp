#include "DirectionalLight.h"
#include "../Math/Util.h"

LightHit DirectionalLight::hit(Ray& ray, RayHit& rh) {
	LightHit lh(ray, rh);
	lh.direction = direction;
	lh.color = color;
	lh.distance = INF;
	return lh;
}