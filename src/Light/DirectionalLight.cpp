#include "DirectionalLight.h"

LightHit DirectionalLight::hit(Ray& ray, RayHit& rh) {
	LightHit lh(ray, rh);
	lh.direction = direction;
	lh.color = color;
	return lh;
}