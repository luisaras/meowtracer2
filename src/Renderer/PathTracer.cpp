#include "PathTracer.h"
#include "../Math/Util.h"

Color PathTracer::getColor(Ray &ray, float x, float y, int depth) {
	if (depth == 0)
		return Color(0, 0, 0);

	RayHit rh = tree->hit(ray);
	if (!rh.hitable)
		return backgroundColor(x, y);

// ============================================================================

	Material* mat = rh.hitable->material;

	float distance = Vec3::distance(ray.origin, rh.point);
	Color absorbed = (-ray.absorb * distance).exp();
	absorbed = absorbed.clamp(0, 1);

	Color texture = rh.hitable->getTexture(rh.uv) * mat->kd;

	return texture;

}
/*
	Vec3 o = randomUnitVec3();

	// Variables
	float nt = mat->refraction;
	float ni = ray.refraction;
	Vec3& n = rh.normal;
	Vec3 i = -ray.direction;

	// Microfacet
	float ab2 = mat->roughness * mat->roughness;
	Vec2 e = randomVec2();
	Vec2 b = random_Beckmann(e, ab2);
	Vec3 m = angle2Vec3(cos(b.x), b.y) + n;

}

// ============================================================================
// Reflection / Refraction
// ============================================================================

Color PathTracer::fr(Vec3& i, Vec3& o, Vec3& n) {
	float i_n = Vec3::dot(i, n);

	// Half vector
	Vec3 hr = (i + o) * sign(i_m);
	hr = Vec3::normalize(hr);

	float o_m = Vec3::dot(o, m);
	float o_n = Vec3::dot(o, n);
	float m_n = Vec3::dot(m, n);

	float D = D_Beckmann(m_n, ab2);
	float G = G1_Beckmann(i_m, i_n, mat->roughness) * G1_Beckmann(o_m, o_n, mat->roughness);

	float weight = (abs(i_m) * G) / (abs(i_n) * abs(m_n));

	return (F * G * D) / (4 * i_n * o_n);
}

Color PathTracer::ft() {
	// Half vector
	Vec3 ht = ni * i + n0 * o;
	ht = Vec3::normalize(ht);

}*/