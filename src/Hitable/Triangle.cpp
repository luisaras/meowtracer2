#include "Triangle.h"
#define E 0.0000000000001

Triangle::Triangle(Vec3& p0, Vec3& p1, Vec3& p2) {
	culling = true;
	e1 = p1 - p0;
	e2 = p2 - p0;
	origin = p0;
}

inline float max3( float a, float b, float c ) {
  float max = ( a < b ) ? b : a;
  return ( ( max < c ) ? c : max );
}

inline float min3(float a, float b, float c) {
  float min = ( a > b ) ? b : a;
  return ( ( min > c ) ? c : min );
}

Box Triangle::hitBox() {
  Vec3 p1 = origin + e1;
  Vec3 p2 = origin + e2;
  Vec3 min = Vec3(min3(origin.x, p1.x, p2.x), min3(origin.y, p1.y, p2.y), min3(origin.z, p1.z, p2.z));
  Vec3 max = Vec3(max3(origin.x, p1.x, p2.x), max3(origin.y, p1.y, p2.y), max3(origin.z, p1.z, p2.z));
  return Box(min, max);
}

Color Triangle::getTexture(Vec2& uv) {
  return Color(1, 1, 1);
}

RayHit Triangle::hit(Ray &ray) {
  RayHit rh;
  Vec3 e2vec = Vec3::cross(ray.direction, e2);
  float det = Vec3::dot(e1, e2vec);
  float u, v;
  if (culling) {
    if (det < E)
      return rh;

    Vec3 tvec = ray.origin - origin;
    u = Vec3::dot(tvec, e2vec);
    if (u < 0 || u > det)
      return rh;
    Vec3 e1vec = Vec3::cross(tvec, e1);
    v = Vec3::dot(ray.direction, e1vec);
    if (v < 0 || u + v > det)
      return rh;

    rh.t = Vec3::dot(e2, e1vec) / det;
  } else {
    if (det > -E && det < E)
      return rh;

    float inv_det = 1.0 / det;
    Vec3 tvec = ray.origin - origin;
    u = Vec3::dot(tvec, e2vec) * inv_det;
    if (u < 0 || u > 1)
      return rh;

    Vec3 e1vec = Vec3::cross(tvec, e1);
    v = Vec3::dot(ray.direction, e1vec) * inv_det;
    if (v < 0 || u + v > 1)
      return rh;

    rh.t = Vec3::dot(e2, e1vec) * inv_det;
  }
  rh.hitable = this;
  rh.uv = Vec2(u, v);
  return rh;
}

bool Triangle::isInside(Vec3& point) {
  Vec3 w = point - origin;
  Vec3 n = calculateNormal();
  Vec3 e1w = Vec3::cross(e1, w);
  Vec3 e2w = Vec3::cross(e2, w);
  float nn = Vec3::dot(n, n);
  float y = Vec3::dot(e1w, n) / nn;
  float b = Vec3::dot(e2w, n) / nn;
  float a = 1 - y - b;
  return a >= 0 && a <= 1 && b >= 0 && b <= 1 && y >= 0 && y <= 1;
}

Vec3 Triangle::calculateNormal() {
  Vec3 v = Vec3::cross(e1, e2);
  return Vec3::normalize(v);
}