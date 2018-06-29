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


