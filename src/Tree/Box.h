#pragma once

#include "../Math/Ray.h"
#include <vector>

using std::vector;

class Box {
public:
	Vec3 bounds[2];
	Box() {}
	Box(Vec3& min, Vec3& max) {
		bounds[0] = min;
		bounds[1] = max;
	}
	bool hit(Ray& ray);
	bool hit(Vec3 other[2]);
	Box cluster(Box& other);
	float volume();
	void triangulate(vector<Point3>& vertices);
};