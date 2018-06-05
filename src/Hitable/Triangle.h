#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "../Math/Vec2.h"
#include "../Math/Vec3.h"
#include "Hitable.h"

class Triangle : public Hitable {
public:
	Point3 origin;
	Vec3 e1, e2;
	Vec3 norm[3];
	Vec2 tex[3];
	Vec3 rad[3];

	bool culling = true;

	Triangle (Vec3& p0, Vec3& p1, Vec3& p2);
	
	Vec3 calculateNormal();
	bool isInside(Vec3& p);

	// Hitable
	RayHit hit(Ray& ray);
	Box hitBox();
	Color getTexture(Vec2& uv);
};



#endif