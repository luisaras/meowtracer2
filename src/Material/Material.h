
#pragma once

#include "../Math/Vec3.h"

class Material {
public:
	// 0 = phong, 1 = lambertian, 2 = metal, 3 = dielectric
	int type = 0;

	// Phong
	Color ka = Color(1, 1, 1);
	Color ke = Color(0, 0, 0);
	Color kd = Color(1, 1, 1);
	Color ks = Color(1, 1, 1);
	float shininess = 32;
	float reflexivity = 0;

	// Cook-Torrance
	float roughness = 0.1;

	// Dieletric
	float refraction = 1;

	// Metal
	float fuzz = 0;

};