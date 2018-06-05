#include "BlinnPhong.h"

Color BlinnPhong::getColor(CubeTree* tree, Scene& scene, Ray& ray, RayHit& rh) {
	LightHit lh(ray.direction, rh);
	Color finalColor(0, 0, 0);
	for (uint i = 0; i < scene.lights.size(); i++) {
		lh.lightDir = scene.lights[i]->getDirection(lh);
    if (!tree->hitsLight(scene.lights[i], lh)) {
  		finalColor += diffuseColor(scene.lights[i], lh) * rh.hitable->getTexture(rh.uv);
  		finalColor += specularColor(scene.lights[i], lh);
    }
	}
	finalColor = finalColor + rh.hitable->material->ka * scene.ambientColor;
  finalColor += rh.hitable->material->ke;
	return finalColor;
}

Color BlinnPhong::diffuseColor (Light* light, LightHit &lh) {
  float r = Vec3::Dot(lh.lightDir, lh.rayHit.normal) / lh.lightDir.Length();
  if (r > 0) {
    float a = light->getIntensity(lh);
    if (a > 0) {
      Color &diff = lh.rayHit.hitable->material->kd;
      return (diff * light->color) * fmin(1.0, r * a);
    }
  }
  return Color(0, 0, 0);
}

Color BlinnPhong::specularColor(Light* light, LightHit &lh) {
  Vec3 half = lh.lightDir - lh.rayDir;
  half = Vec3::Normalize(half);
  float r = Vec3::Dot(half, lh.rayHit.normal);
  if (r > 0) {
    float a = light->getIntensity(lh);
    if (a > 0) {
      float s = lh.rayHit.hitable->material->shininess;
      Color &spec = lh.rayHit.hitable->material->ks;
      return (spec * light->color) * fmin(1.0, pow(r * a, s));
    }
  }
  return Color(0, 0, 0);
}