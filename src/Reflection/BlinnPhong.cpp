#include "BlinnPhong.h"
#define ERR 0.00001

Color BlinnPhong::localColor(CubeTree* tree, Scene& scene, Ray& ray, RayHit& rh) {
	Color lit(0, 0, 0);
	for (uint i = 0; i < scene.lights.size(); i++) {
		LightHit lh = scene.lights[i]->hit(ray, rh);
		if (!tree->hitsLight(scene.lights[i], lh)) {
			lit += localDiffuse(scene.lights[i], lh);
			lit += localSpecular(scene.lights[i], lh);
		}
	}
	return lit + rh.hitable->material->ka * scene.ambientColor;
}

Color BlinnPhong::localDiffuse(Light* light, LightHit &lh) {
	float r = Vec3::dot(lh.direction, lh.rayHit.normal);
	if (r > ERR) {
		Color &diff = lh.rayHit.hitable->material->kd;
		return (diff * lh.color) * fmin(1.0, r);
	}
	return Color(0, 0, 0);
}

Color BlinnPhong::localSpecular(Light* light, LightHit &lh) {
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