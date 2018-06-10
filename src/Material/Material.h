
#pragma once

#include "../Math/Vec3.h"

class Material {
public:
	// 0 = lambertian, 1 = metal, 2 = dielectric
	int type = 0;

	// Phong
	Color ka = Color(1, 1, 1);
	Color ke = Color(0, 0, 0);
	Color kd = Color(1, 1, 1);
	Color ks = Color(1, 1, 1);
	float shininess = 32;

	// Cook-Torrance
	float roughness = 0.1;

	// Dieletric
	float refraction = 1;

	// Metal
	float fuzz = 0;

};