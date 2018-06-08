#include "Box.h"

Box Box::cluster(Box& other) {
  Vec3 min = Vec3::Minimize(bounds[0], other.bounds[0]);
  Vec3 max = Vec3::Maximize(bounds[1], other.bounds[1]);
  return Box(min, max);
}

float Box::volume() {
  float x = bounds[1].x - bounds[0].x;
  float y = bounds[1].y - bounds[0].y;
  float z = bounds[1].z - bounds[0].z;
  return x * y * z;
}

bool Box::hit(Ray &r) {
  float tmin, tmax, tymin, tymax, tzmin, tzmax; 
 
  tmin = (bounds[r.sign[0]].x - r.origin.x) * r.invdir.x; 
  tmax = (bounds[1-r.sign[0]].x - r.origin.x) * r.invdir.x; 
  tymin = (bounds[r.sign[1]].y - r.origin.y) * r.invdir.y; 
  tymax = (bounds[1-r.sign[1]].y - r.origin.y) * r.invdir.y; 

  if ((tmin > tymax) || (tymin > tmax)) 
      return false; 
  if (tymin > tmin) 
      tmin = tymin; 
  if (tymax < tmax) 
      tmax = tymax; 

  tzmin = (bounds[r.sign[2]].z - r.origin.z) * r.invdir.z; 
  tzmax = (bounds[1-r.sign[2]].z - r.origin.z) * r.invdir.z; 

  if ((tmin > tzmax) || (tzmin > tmax)) 
      return false; 

  return true;
}

bool Box::hit(Vec3 other[2]) {
  if (bounds[1].x < other[0].x || other[1].x < bounds[0].x)
    return false;
  if (bounds[1].y < other[0].y || other[1].y < bounds[0].y)
    return false;
  if (bounds[1].z < other[0].z || other[1].z < bounds[0].z)
    return false;
  return true;
}