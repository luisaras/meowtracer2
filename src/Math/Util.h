#pragma once

#include "Vec2.h"
#include "Vec3.h"

#include <cstdlib>
#include <ctime>
#include <limits>
#include <cmath>

#define INF std::numeric_limits<float>::infinity()
#define PI2 6.28318530718 
#define PI 3.14159265359
#define DEG2RAD PI/180
#define RAD2DEG 180/PI
#define ERR 0.00001

Vec2 randomVec2();
Vec3 randomUnitVec3();

Vec3 reflect(Vec3& v, Vec3& n);
Vec3 refract(Vec3& v, Vec3& n, float refr);
float fresnel(float c, float nt, float ni);
Vec3 schlick(float h_wi, Vec3& f0);

Vec3 angle2Vec(float tetha, float phi);
Vec2 vec2Angle(Vec3& v);

float chi(float val);
float clamp(float x, float min, float max);
float saturate(float x);

// Beckmann
Vec2 random_Beckmann(Vec2& e, float ab2);
float D_Beckmann(float ab2, float m_n);
float G_Beckmann(float n_h, float n_wi, float n_w0, float h_w0);
float G1_Beckmann(float v_m, float v_n, float rough);
