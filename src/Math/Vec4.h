
#pragma once

#include "Vec3.h"

struct Vec4 {
    //
    // Initalization
    //
     Vec4();
     Vec4(const Vec4 &V);
     Vec4(const Vec3 &V, float _w);
     Vec4(float _x, float _y, float _z, float _w);

    //
    // Assignment
    //
     Vec4& operator = (const Vec4 &V);

    //
    // Overloaded operators
    //
     Vec4& operator *= (float Right);
     Vec4& operator /= (float Right);
     Vec4& operator += (const Vec4 &Right);
     Vec4& operator -= (const Vec4 &Right);

    //
    // Accessors
    //
     float length() const;
     float length2() const;

     float& Element(int Index) {
        return ((float *)this)[Index];
    }
     float Element(int Index) const {
        return ((float *)this)[Index];
    }

     float& operator[](int Index) {
        return ((float *)this)[Index];
    }
     float operator[](int Index) const {
        return ((float *)this)[Index];
    }

    //
    // Local data
    //
    float x, y, z, w;

    //
    // Constants
    //
    static const Vec4 Origin;
    static const Vec4 eX;
    static const Vec4 eY;
    static const Vec4 eZ;
    static const Vec4 eW;

    //
    // Static functions
    //
     static Vec4 normalize(const Vec4 &V);
     static float dot(const Vec4 &Left, const Vec4 &Right);
     static Vec4 lerp(const Vec4 &Left, const Vec4 &Right, float s);
     static Vec4 max(const Vec4 &Left, const Vec4 &Right);
     static Vec4 min(const Vec4 &Left, const Vec4 &Right);
     static Vec4 abs(const Vec4 &V);
};

Vec4 operator * (const Vec4 &Left, float Right);
Vec4 operator * (float Right, const Vec4 &Left);
Vec4 operator / (const Vec4 &Left, float Right);
Vec4 operator + (const Vec4 &Left, const Vec4 &Right);
Vec4 operator - (const Vec4 &Left, const Vec4 &Right);
