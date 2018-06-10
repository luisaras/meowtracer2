#include "CookTorrance.h"
#include "../Math/Util.h"

Color CookTorrance::getColor(CubeTree* tree, Scene& scene, Ray& ray, RayHit& rh) {
	Color texture = rh.hitable->getTexture(rh.uv);
	Color finalColor(0, 0, 0);
	for (uint i = 0; i < scene.lights.size(); i++) {
		LightHit lh = scene.lights[i]->hit(ray, rh);
		if (!tree->hitsLight(scene.lights[i], lh)) {
			finalColor += lightColor(scene.lights[i], lh, texture);
		}
	}
	return finalColor + rh.hitable->material->ka * scene.ambientColor;
}

Color CookTorrance::lightColor(Light* light, LightHit& lh, Color& texture) {
	// Parameters
	Vec3& n = lh.rayHit.normal;
	Vec3 w0 = -lh.ray.direction;
	Vec3& wi = lh.direction;
	Material* material = lh.rayHit.hitable->material;

	// Light
	Vec3 Li = lh.color;
	float n_wi = fmax(0, Vec3::Dot(n, wi));

	// Diffuse
	Color color = material->kd * Li * n_wi * texture;

	if (n_wi > 0)  {
		float n_w0 = fmax(0, Vec3::Dot(n, w0));
		if (n_w0 <= 0)
			return color;

		Vec3 h = wi + w0;
		h = Vec3::Normalize(h);

		float h_w0 = fmax(0, Vec3::Dot(h, w0));
		if (h_w0 <= 0)
			return color;

		float h_wi = fmax(0, Vec3::Dot(h, wi));
		float n_h = fmax(0, Vec3::Dot(n, h));

		// Fresnel reflectance
		float f = pow(1.0 - h_wi, 5.0);
		Vec3& spec = material->ks;
		Vec3 F(spec.x + (1 - spec.x)*f, spec.y + (1 - spec.y)*f, spec.z + (1 - spec.z)*f);

		// Microfacet distribution by Beckmann
		float m2 = material->roughness * material->roughness;
		float n_h2 = n_h * n_h;
		float r1 = PI * m2 * n_h2 * n_h2;
		float r2 = (n_h2 - 1.0) / (m2 * n_h2);
		float D = exp(r2) / r1;

		// Geometric shadowing
		float g = 2.0 * n_h / h_w0;
		float G = fmin(1.0, fmin(n_w0 * g, n_wi * g));

		// Fresnel
		Vec3 Fr = F * (D * G) / (4 * n_wi * n_w0);

		// Weight
		float p = 2 * PI;

		// Result
		color += Li * Fr * n_wi * p;
	}

	return color;
}