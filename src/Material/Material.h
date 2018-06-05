#ifndef __MATERIAL__
#define __MATERIAL__

#include "../Math/Vec3.h"

struct Material {
	// Phong
	Color ka = Color(1, 1, 1);
	Color ke = Color(0, 0, 0);
	Color kd = Color(1, 1, 1);
	Color ks = Color(1, 1, 1);
	float shininess = 32;
};

#endif