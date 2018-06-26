#include "Sphere.h"
#include "../Math/Util.h"
#include <iostream>

Sphere::Sphere(int id, Matrix4 &xform, Point3 p, float rad) : center(p), radius(rad) {	
	this->id = id;
	Matrix4 translation = Matrix4::translation(center);
	Matrix4 scaling = Matrix4::scaling(radius);
	transform = scaling * translation;
	transform = xform * transform;
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
		float t = t1 < 0 ? t2 : t2 < 0 ? t1 : std::min(t1, t2);

		if (t < 0)
			return rh;

		rh.t = t;
		rh.point = ray.at(t);
		
		{ // UV map
			Vec3 n = o + rh.t * d;
			float phi = atan2(n.z, n.x);
	    	float theta = asin(n.y);
		    float u = 1 - (phi + PI) / (2 * PI);
		    float v = (theta + PI / 2) / PI;
		    rh.uv = Vec2(u, v);
		}

		rh.normal = rh.point - center;
		rh.normal = Vec3::normalize(rh.normal);
		rh.hitable = this;
	}

	return rh;
}