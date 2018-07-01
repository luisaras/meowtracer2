#include "CookTorrance.h"
#include "../Math/Util.h"
#define mfCount 4

Color CookTorrance::localColor(CubeTree* tree, Scene& scene, Ray& ray, RayHit& rh) {
	Color lit(0, 0, 0);
	Vec3 rayDir = -ray.direction;
	for (uint i = 0; i < scene.lights.size(); i++) {
		LightHit lh = scene.lights[i]->hit(ray, rh);
		if (!tree->hitsLight(scene.lights[i], lh)) {
			lit += lh.color * reflectedColor(rh.normal, rayDir, lh.direction, rh.hitable->material);
		}
	}
	return lit + rh.hitable->material->ka * scene.ambientColor;
}

// ============================================================================
//  Reflection
// ============================================================================

float CookTorrance::reflectedColor(Vec3& m, Vec3& w0, Vec3& wi, 
		Vec3& h, float h_w0, float rough) {
	float m_w0 = Vec3::dot(m, w0);
	float m_wi = Vec3::dot(m, wi);
	if (m_wi < 0 || m_w0 < 0)
		return 0;
	float m_h = Vec3::dot(m, h);
	// Microfacet distribution by Beckmann
	float D = D_Beckmann(rough * rough, m_h);
	// Geometric shadowing
	float G = G_Beckmann(m_h, m_wi, m_w0, h_w0);
	return (D * G) / (4 * m_wi * m_w0);
}

Color CookTorrance::reflectedColor(Vec3& n, Vec3& w0, Vec3& wi, Material* mat) {
	float n_wi = Vec3::dot(n, wi);
	if (n_wi > ERR) {
		// Half vector
		Vec3 h = wi + w0;
		h = Vec3::normalize(h);
		float h_wi = fmax(0, Vec3::dot(h, wi));
		float h_w0 = fmax(0, Vec3::dot(h, w0));
		// Schlick reflectance
		Vec3 F = schlick(h_wi, mat->ks);
		// Reflected color
		Color color;
		Vec3 orig = n.perp();
		if (mfCount == 1) {
			color = F * reflectedColor(n, w0, wi, h, h_w0, mat->roughness);
		} else {
			color = Color(0, 0, 0);
			for(int i = 0; i < mfCount; i++) {
				
				// Microfacet
				Vec2 e = randomVec2();
				Vec2 a = random_Beckmann(e, mat->roughness * mat->roughness);
				Vec3 v = Matrix4::rotation(n, a.y).transformVector(orig);
				Vec3 k = Vec3::cross(v, n);
				Vec3 m = Matrix4::rotation(k, PI / 2 - acos(a.x)).transformVector(v);
				m = Vec3::normalize(m);

				color += F * reflectedColor(m, w0, wi, h, h_w0, mat->roughness);
			}
		}
		return n_wi * (color / mfCount + mat->kd);
	} else {
		return Color(0, 0, 0);
	}
}

// ============================================================================
//  Transmission
// ============================================================================