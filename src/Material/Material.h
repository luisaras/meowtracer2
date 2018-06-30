
#pragma once

#include "../Math/Vec3.h"

enum MaterialType {
	BLINNPHONG, COOKTORRANCE, LAMBERTIAN, METAL, DIELECTRIC
};

class Material {
public:
	Material(MaterialType t) : type(t) {}
	MaterialType type;
	bool recursive = true;

	// Phong
	Color ka = Color(1, 1, 1);
	Color ke = Color(0, 0, 0);
	Color kd = Color(1, 1, 1);
	Color ks = Color(1, 1, 1);
	float shininess = 32;
	float reflectivity = 0;

	// Cook-Torrance
	float roughness = 0.1;

	// Dieletric
	float refraction = 1;

	// Metal
	float fuzz = 0;

	// Beer's Law
	Color absorb = Color(0, 0, 0);

};