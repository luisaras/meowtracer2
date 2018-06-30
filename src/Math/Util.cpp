#include "Util.h"

float saturate(float x) {
	return clamp(x, -1, 1);
}

float clamp(float x, float min, float max) {
	return fmin(max, fmax(min, x));
}

float chi(float val) {
	return val > 0 ? 1 : 0;
}

Vec2 randomVec2() {
	return Vec2(rand() / RAND_MAX, rand() / RAND_MAX);
}

Vec3 angle2Vec3(float cost, float phi) {
	float sint = sqrt(1 - cost * cost);
	float x = sint * cos(phi);
	float y = sint * sin(phi);
	return Vec3(x, y, cost);
}

Vec3 randomUnitVec3() {
	float phi = PI2 * rand() / RAND_MAX;
	float cost = 1.0 * rand() / RAND_MAX;
	return angle2Vec3(cost, phi);
}

Vec3 schlick(float h_wi, Vec3& f0) {
	float f = pow(1.0 - h_wi, 5.0);
	return Vec3(f0.x + (1 - f0.x)*f, 
				f0.y + (1 - f0.y)*f, 
				f0.z + (1 - f0.z)*f);
}

float fresnel(float c, float nt, float ni) {
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

// ============================================================================
// Beckmann
// ============================================================================

float G_Beckmann(float n_h, float n_wi, float n_w0, float h_w0) {
	float g = 2.0 * n_h / h_w0;
	return fmin(1.0, fmin(n_w0 * g, n_wi * g));
}

float G1_Beckmann(float v_m, float v_n, float rough) {
	float tg = tan(acos(v_n));
	float a = 1 / (rough * tg);
	float G1 = chi(v_m / v_n);
	if (a < 1.6) {
		float a2 = a*a;
		G1 *= (3.535*a + 2.181*a2) / (1 + 2.27*a + 2.577*a2);
	}
	return G1;
}

/*
float D_Beckmann(float ab2, float m_n) {
	float cos2 = m_n * m_n;
	float tg = tan(acos(m_n));
	float r1 = PI * ab2 * cos2*cos2;
	float r2 = -tg*tg / ab2;
	return chi(m_n) * exp(r2) / r1;
}*/
float D_Beckmann(float m2, float n_h) {
	float n_h2 = n_h * n_h;
	float r1 = PI * m2 * n_h2 * n_h2;
	float r2 = (n_h2 - 1.0) / (m2 * n_h2);
	return exp(r2) / r1;
}

Vec2 random_Beckmann(Vec2& e, float ab2) {
	float tg = sqrt(atan(-ab2 * log(1 - e.x)));
	float tetha = atan(tg);
	float phi = 2 * PI * e.y;
	return Vec2(tetha, phi);
}