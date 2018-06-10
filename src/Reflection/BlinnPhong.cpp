#include "BlinnPhong.h"

Color BlinnPhong::getColor(CubeTree* tree, Scene& scene, Ray& ray, RayHit& rh) {
	Color texture = rh.hitable->getTexture(rh.uv);
	Color finalColor(0, 0, 0);
	for (uint i = 0; i < scene.lights.size(); i++) {
		LightHit lh = scene.lights[i]->hit(ray, rh);
		if (!tree->hitsLight(scene.lights[i], lh)) {
			finalColor += diffuseColor(scene.lights[i], lh) * texture;
			finalColor += specularColor(scene.lights[i], lh);
		}
	}
	return finalColor + rh.hitable->material->ka * scene.ambientColor;
}

Color BlinnPhong::diffuseColor (Light* light, LightHit &lh) {
	float r = Vec3::Dot(lh.direction, lh.rayHit.normal);
	if (r > 0) {
		Color &diff = lh.rayHit.hitable->material->kd;
		return (diff * lh.color) * fmin(1.0, r);
	}
	return Color(0, 0, 0);
}

Color BlinnPhong::specularColor(Light* light, LightHit &lh) {
	Vec3 half = lh.direction - lh.ray.direction;
	half = Vec3::Normalize(half);
	float r = Vec3::Dot(half, lh.rayHit.normal);
	if (r > 0) {
		float s = lh.rayHit.hitable->material->shininess;
		Color &spec = lh.rayHit.hitable->material->ks;
		return (spec * lh.color) * fmin(1.0, pow(r, s));
	}
	return Color(0, 0, 0);
}