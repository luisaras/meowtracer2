#include "Sphere.h"
#include <iostream>
#include <cmath>
#include <ctgmath>
#define PI 3.14159265359

Sphere::Sphere(Matrix4 &xform, Point3 p, float rad) : center(p), radius(rad) {	
	Matrix4 translation = Matrix4::Translation(center);
	Matrix4 scaling = Matrix4::Scaling(radius);
	transform = scaling * translation;
	transform = transform * xform;
	invt = transform.Inverse();
}

Box Sphere::hitBox() {
	// Bounding box
	Matrix4 s = Matrix4::Identity();
	s[3][3] = -1;
	Matrix4 si = s.Inverse();

	Matrix4 &m = transform;
	Matrix4 mt = m.Transpose();

	Vec3 bounds[2];
	Matrix4 r = mt * si * m;
	bounds[0].z = (r[2][3] + sqrt(pow(r[2][3],2) - (r[3][3]*r[2][2])) ) * 1.0 / r[3][3]; 
	bounds[1].z = (r[2][3] - sqrt(pow(r[2][3],2) - (r[3][3]*r[2][2])) ) * 1.0 / r[3][3]; 
	bounds[0].y = (r[1][3] + sqrt(pow(r[1][3],2) - (r[3][3]*r[1][1])) ) * 1.0 / r[3][3]; 
	bounds[1].y = (r[1][3] - sqrt(pow(r[1][3],2) - (r[3][3]*r[1][1])) ) * 1.0 / r[3][3]; 
	bounds[0].x = (r[0][3] + sqrt(pow(r[0][3],2) - (r[3][3]*r[0][0])) ) * 1.0 / r[3][3]; 
	bounds[1].x = (r[0][3] - sqrt(pow(r[0][3],2) - (r[3][3]*r[0][0])) ) * 1.0 / r[3][3];

	return Box(bounds[0], bounds[1]);
}

Color Sphere::getTexture(Vec2& uv) {
	return Color(1, 1, 1);
}

RayHit Sphere::hit(Ray &ray) {
	RayHit hr;

	Point3 o = invt.TransformPoint(ray.origin);
	Vec3 d = invt.TransformVector(ray.direction);

	float a = Vec3::Dot(d, d);
	float b = 2 * Vec3::Dot(d, o);
	float c = Vec3::Dot(o, o) - 1;
	float delta = b * b - 4 * a * c;

	if (delta >= 0) {
		Ray tRay = Ray(o, d);
		float sqrtdelta = std::sqrt(delta);
		float t1 = (-b - sqrtdelta) / (2 * a);
		float t2 = (-b + sqrtdelta) / (2 * a);
		hr.t = std::min(t1, t2);
		if (hr.t >= 0) {
			hr.point = ray.at(hr.t);
			hr.normal = tRay.at(hr.t);

			float x = hr.normal.x, y = hr.normal.y, z = hr.normal.z;
			//float tetha = acos(z);
			//float phi = atan(y / x);
			//hr.texture = material->texture(tetha / (2 * PI), phi / (2 * PI), hr.normal);
			//std::cout << x << " " << y << " " << z << " " << tetha / PI * 180 << " " << phi / PI * 180 << std::endl;

			float phi = atan2(z, x);
	    	float theta = asin(y);
		    float u = 1-(phi + PI) / (2*PI);
		    float v = (theta + PI/2) / PI;
		    hr.uv = Vec2(u, v);

			//float u = x / 2 + 0.5;
			//float v = y / 2 + 0.5;
			//hr.texture = material->texture(u, v, hr.normal);

			hr.normal = transform.TransformVector(hr.normal);
			hr.normal = Vec3::Normalize(hr.normal);
		}
	} else {
		hr.t = NAN;
	}
	return hr;
}