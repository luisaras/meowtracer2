#include "CookTorrance.h"
#include "../Math/Util.h"
#define mfCount 8

Color CookTorrance::localColor(CubeTree* tree, Scene& scene, Ray& ray, RayHit& rh, Color& texture) {
	Color lit(0, 0, 0);
	for (uint i = 0; i < scene.lights.size(); i++) {
		LightHit lh = scene.lights[i]->hit(ray, rh);
		if (!tree->hitsLight(scene.lights[i], lh)) {
			lit += lightColor(scene.lights[i], lh, texture);
		}
	}
	return lit + rh.hitable->material->ka * scene.ambientColor;
}

Color CookTorrance::lightColor(Light* light, LightHit& lh, Color& texture) {

	// Parameters
	Vec3& n = lh.rayHit.normal;
	Vec3 w0 = -lh.ray.direction;
	Vec3& wi = lh.direction;
	Material* material = lh.rayHit.hitable->material;

	// Light
	Vec3 Li = lh.color;
	float n_wi = fmax(0, Vec3::dot(n, wi));

	// Diffuse
	Color color = material->kd * Li * n_wi * texture;

	if (n_wi > 0)  {
		float n_w0 = fmax(0, Vec3::dot(n, w0));
		if (n_w0 <= 0)
			return color;

		Vec3 h = wi + w0;
		h = Vec3::normalize(h);

		float h_w0 = fmax(0, Vec3::dot(h, w0));
		if (h_w0 <= 0)
			return color;

		float h_wi = fmax(0, Vec3::dot(h, wi));
		float n_h = fmax(0, Vec3::dot(n, h));

		// Schlick reflectance
		Vec3 F = schlick(h_wi, material->ks);

		// Microfacet distribution by Beckmann
		float D = D_Beckmann(material->roughness * material->roughness, n_h);

		// Geometric shadowing
		float G = G_Beckmann(n_h, n_wi, n_w0, h_w0);

		// Result
		Vec3 Fr = F * (D * G) / (4 * n_wi * n_w0);
		color += Li * Fr * n_wi;
	}

	return color;
}