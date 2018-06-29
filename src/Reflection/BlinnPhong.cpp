#include "BlinnPhong.h"
#define ERR 0.00001

Color BlinnPhong::diffuseColor (Light* light, LightHit &lh, Color& color) {
	float r = Vec3::dot(lh.direction, lh.rayHit.normal);
	if (r > ERR) {
		return (color * lh.color) * fmin(1.0, r);
	}
	return Color(0, 0, 0);
}

Color BlinnPhong::specularColor(Light* light, LightHit &lh) {
	Vec3 half = lh.direction - lh.ray.direction;
	half = Vec3::normalize(half);
	float r = Vec3::dot(half, lh.rayHit.normal);
	if (r > ERR) {
		float s = lh.rayHit.hitable->material->shininess;
		Color &spec = lh.rayHit.hitable->material->ks;
		return (spec * lh.color) * fmin(1.0, pow(r, s));
	}
	return Color(0, 0, 0);
}