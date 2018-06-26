#include "Sphere.h"
#include "../Math/Util.h"
#include <iostream>

Sphere::Sphere(int id, Matrix4 &xform, Point3 p, float rad) : center(p), radius(rad) {	
	this->id = id;
	Matrix4 translation = Matrix4::translation(center);
	Matrix4 scaling = Matrix4::scaling(radius);
	transform = scaling * translation;
	transform = transform * xform;
	invt = transform.inverse();
}

Box Sphere::hitBox() {
	// Bounding box
	Matrix4 s = Matrix4::identity();
	s[3][3] = -1;
	Matrix4 si = s.inverse();

	Matrix4 &m = transform;
	Matrix4 mt = m.transposed();

	Vec3 min, max;
	Matrix4 r = mt * si * m;
	min.x = (r[0][3] + sqrt(pow(r[0][3],2) - (r[3][3]*r[0][0])) ) * 1.0 / r[3][3]; 
	max.x = (r[0][3] - sqrt(pow(r[0][3],2) - (r[3][3]*r[0][0])) ) * 1.0 / r[3][3];
	min.y = (r[1][3] + sqrt(pow(r[1][3],2) - (r[3][3]*r[1][1])) ) * 1.0 / r[3][3]; 
	max.y = (r[1][3] - sqrt(pow(r[1][3],2) - (r[3][3]*r[1][1])) ) * 1.0 / r[3][3]; 
	min.z = (r[2][3] + sqrt(pow(r[2][3],2) - (r[3][3]*r[2][2])) ) * 1.0 / r[3][3]; 
	max.z = (r[2][3] - sqrt(pow(r[2][3],2) - (r[3][3]*r[2][2])) ) * 1.0 / r[3][3]; 

	return Box(min, max);
}

Color Sphere::getTexture(Vec2& uv) {
	return Color(1, 1, 1);
}

RayHit Sphere::hit(Ray &ray) {
	RayHit rh;
	rh.t = NAN;

	Point3 o = invt.transformPoint(ray.origin);
	Vec3 d = invt.transformVector(ray.direction);

	float a = Vec3::dot(d, d);
	float b = 2 * Vec3::dot(d, o);
	float c = Vec3::dot(o, o) - 1;
	float delta = b * b - 4 * a * c;

	if (delta >= ERR) {
		float sqrtdelta = std::sqrt(delta);
		float t1 = (-b - sqrtdelta) / (2 * a);
		float t2 = (-b + sqrtdelta) / (2 * a);
		if (t1 < 0)
			rh.t = t2;
		else if (t2 < 0)
			rh.t = t1;
		else
			rh.t = std::min(t1, t2);

		if (rh.t < 0)
			return rh;

		rh.point = ray.at(rh.t);
		rh.normal = o + rh.t * d;

		float x = rh.normal.x, y = rh.normal.y, z = rh.normal.z;
		//float tetha = acos(z);
		//float phi = atan(y / x);
		//rh.texture = material->texture(tetha / (2 * PI), phi / (2 * PI), rh.normal);
		//std::cout << x << " " << y << " " << z << " " << tetha / PI * 180 << " " << phi / PI * 180 << std::endl;

		float phi = atan2(z, x);
    	float theta = asin(y);
	    float u = 1-(phi + PI) / (2*PI);
	    float v = (theta + PI/2) / PI;
	    rh.uv = Vec2(u, v);

		//float u = x / 2 + 0.5;
		//float v = y / 2 + 0.5;
		//rh.texture = material->texture(u, v, rh.normal);

		rh.normal = transform.transformVector(rh.normal);
		rh.normal = Vec3::normalize(rh.normal);

		//if (Vec3::dot(ray.direction, rh.normal) > 0)
			//std::cout << "bla" << std::endl;

		rh.hitable = this;
	}

	return rh;
}