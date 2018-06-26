
#pragma once

#include <iostream>  
using namespace std; 

struct Vec3 {
    //
    // Initalization
    //
     Vec3();
     Vec3(const Vec3 &V);
     Vec3(float _x, float _y, float _z);

    //
    // Assignment
    //
     Vec3& operator = (const Vec3 &V);

    //
    // Overloaded operators
    //
     Vec3& operator *= (float Right);
     Vec3& operator *= (int Right);
     Vec3& operator /= (float Right);
     Vec3& operator /= (int Right);
     Vec3& operator += (const Vec3 &Right);
     Vec3& operator -= (const Vec3 &Right);

    //
    // Normalization
    //
     void setLength(float NewLength);


    //
    // Accessors
    //
     float length() const;
     float length2() const;

     bool valid() const;
     bool inf() const;

     Vec3 exp() const;
     Vec3 clamp(float min, float max) const;

#ifdef USE_D3D
     operator D3DXVECTOR3() const;
#endif

     float& operator[](int Index) {
        return ((float *)this)[Index];
    }
     float operator[](int Index) const {
        return ((float *)this)[Index];
    }

    //
    // Local data
    //
    float x, y, z;

    //
    // Constants
    //
    static const Vec3 Origin;
    static const Vec3 eX;
    static const Vec3 eY;
    static const Vec3 eZ;

    //
    // Static functions
    //
     static Vec3 normalize(const Vec3 &V);
     static Vec3 randomVector();
     static Vec3 randomNormal();
     static Vec3 cross(const Vec3 &Left, const Vec3 &Right);
     static float dot(const Vec3 &Left, const Vec3 &Right);
     static float distance(const Vec3 &Left, const Vec3 &Right);
     static float distance2(const Vec3 &Left, const Vec3 &Right);
     static Vec3 lerp(const Vec3 &Left, const Vec3 &Right, float s);
     static Vec3 max(const Vec3 &Left, const Vec3 &Right);
     static Vec3 min(const Vec3 &Left, const Vec3 &Right);
     static Vec3 cartesian2Spherical(const Vec3 &Cartesian);
     static Vec3 spherical2Cartesian(const Vec3 &Spherical);
};

Vec3 operator* (const Vec3 &Left, float Right);
Vec3 operator* (float Left, const Vec3 &Right);
Vec3 operator* (const Vec3 &Left, const Vec3 &Right);
Vec3 operator / (const Vec3 &Left, float Right);
Vec3 operator + (const Vec3 &Left, const Vec3 &Right);
Vec3 operator - (const Vec3 &Left, const Vec3 &Right);
Vec3 operator - (const Vec3 &V);
Vec3 operator - (float x, const Vec3& V);
ostream& operator <<(ostream& os, const Vec3& vec);

#define Point3 Vec3
#define Color Vec3