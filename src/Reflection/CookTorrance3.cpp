#include "CookTorrance.h"
#include "../Math/Util.h"
#define SamplesCount 8

inline float saturate(float x) {
	return clamp(x, -1, 1);
}

inline float X(float val) {
	return val > 0 ? 1 : 0;
}

inline float Fresnel(float c, float nt, float ni) {
	float g2 = (nt * nt) / (ni * ni) - 1 + c*c;
	if (g2 >= 0) {
		float g = sqrt(g2);
		float gmc = g-c;
		float gpc = g+c;
		float c_gpc = c*gpc - 1;
		float c_gmc = c*gmc + 1;
		return 0.5 * (gmc*gmc)/(gpc*gpc) * (1 + (c_gpc*c_gpc)/(c_gmc*c_gmc));
	} else {
		return 1;
	}
}

inline float D_Beckmann(float m_n, float ab2) {
	float cos2 = m_n * m_n;
	float tg = tan(acos(m_n));
	return X(m_n) / (PI * ab2 * cos2*cos2) * exp(-tg*tg / ab2);
}

inline float G1_Beckmann(float v_m, float v_n, float rough) {
	float tg = tan(acos(v_n));
	float a = 1 / (rough * tg);
	float G1 = X(v_m / v_n);
	if (a < 1.6) {
		float a2 = a*a;
		G1 *= (3.535*a + 2.181*a2) / (1 + 2.27*a + 2.577*a2);
	}
}

inline Vec3 random_Beckmann(Vec2& e, float ab2) {
	float tg = srqt(atan(-abs * log(1 - e.x)));
	float tetha = atan(tg);
	float phi = 2 * PI * e.y;
	return angle2Vec3(cos(tetha), phi);
}

// nt => ray refraction
// ni => obj refraction
Color CookTorrance::getColor(float nt, float ni, Vec3& n, Vec& i, Vec3& o) {

	// i => light direction
	// o => scattered light direction
	// n => normal
	// m => micro normal

	float ab2 = rough * rough;
	Vec2 e = randomVec2();
	Vec3 m = random_Beckmann(e, ab2);

	float i_m = Vec3::dot(i, m);
	float i_n = Vec3::dot(i, n);
	float o_m = Vec3::dot(o, m);
	float o_n = Vec3::dot(o, n);
	float m_n = Vec3::dot(m, n);

	float F = Fresnel(i_m);
	float D = D_Beckmann(m_n, ab2);
	float G = G1_Beckmann(i_m, i_n, rough) * G1_Beckmann(o_m, o_n, rough);

	float weight = (abs(i_m) * G) / (abs(i_n) * abs(m_n));

	float fr = (F * G * D) / (4 * i_n * o_n);

}