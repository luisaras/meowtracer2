/*
Vec4.inl
Written by Matthew Fisher

Inline file for a 4-dimensional vector of floats
*/

#include "Vec4.h"
#include <cmath>
#include <cstdlib>

Vec4::Vec4() { }

Vec4::Vec4(float _x, float _y, float _z, float _w) {
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

Vec4::Vec4(const Vec3 &V, float _w) {
    x = V.x;
    y = V.y;
    z = V.z;
    w = _w;
}

Vec4::Vec4(const Vec4 &V) {
    x = V.x;
    y = V.y;
    z = V.z;
    w = V.w;
}

Vec4& Vec4::operator = (const Vec4 &V) {
    x = V.x;
    y = V.y;
    z = V.z;
    w = V.w;
    return *this;
}

float Vec4::length() const {
    return sqrtf(x * x + y * y + z * z + w * w);
}

float Vec4::LengthSq() const {
    return x * x + y * y + z * z + w * w;
}

 Vec4 Vec4::Normalize(const Vec4 &V) {
    float Len = V.length();
    if(Len == 0.0f) {
        return V;
    } else {
        float Factor = 1.0f / Len;
        return Vec4(V.x * Factor, V.y * Factor, V.z * Factor, V.w * Factor);
    }
}


float Vec4::Dot(const Vec4 &Left, const Vec4 &Right) {
    return (Left.x * Right.x + Left.y * Right.y + Left.z * Right.z + Left.w * Right.w);
}

Vec4 Vec4::Lerp(const Vec4 &Left, const Vec4 &Right, float s) {
    return (Left + s * (Right - Left));
}

Vec4 Vec4::Maximize(const Vec4 &Left, const Vec4 &Right) {
    Vec4 Result = Right;
    if(Left.x > Right.x) Result.x = Left.x;
    if(Left.y > Right.y) Result.y = Left.y;
    if(Left.z > Right.z) Result.z = Left.z;
    if(Left.w > Right.w) Result.w = Left.w;
    return Result;
}

Vec4 Vec4::Minimize(const Vec4 &Left, const Vec4 &Right) {
    Vec4 Result = Right;
    if(Left.x < Right.x) Result.x = Left.x;
    if(Left.y < Right.y) Result.y = Left.y;
    if(Left.z < Right.z) Result.z = Left.z;
    if(Left.w < Right.w) Result.w = Left.w;
    return Result;
}

Vec4 Vec4::Abs(const Vec4 &V) {
    Vec4 Result = V;
    if(Result.x < 0.0f) {
        Result.x = -Result.x;
    }
    if(Result.y < 0.0f) {
        Result.y = -Result.y;
    }
    if(Result.z < 0.0f) {
        Result.z = -Result.z;
    }
    if(Result.w < 0.0f) {
        Result.w = -Result.w;
    }
    return Result;
}

Vec4 operator * (const Vec4 &Left, float Right) {
    Vec4 Return;
    Return.x = Left.x * Right;
    Return.y = Left.y * Right;
    Return.z = Left.z * Right;
    Return.w = Left.w * Right;
    return Return;
}

Vec4 operator * (float Right, const Vec4 &Left) {
    Vec4 Return;
    Return.x = Left.x * Right;
    Return.y = Left.y * Right;
    Return.z = Left.z * Right;
    Return.w = Left.w * Right;
    return Return;
}

Vec4 operator / (const Vec4 &Left, float Right) {
    Vec4 Return;
    Return.x = Left.x / Right;
    Return.y = Left.y / Right;
    Return.z = Left.z / Right;
    Return.w = Left.w / Right;
    return Return;
}

Vec4 operator + (const Vec4 &Left, const Vec4 &Right) {
    Vec4 Return;
    Return.x = Left.x + Right.x;
    Return.y = Left.y + Right.y;
    Return.z = Left.z + Right.z;
    Return.w = Left.w + Right.w;
    return Return;
}

Vec4 operator - (const Vec4 &Left, const Vec4 &Right) {
    Vec4 Return;
    Return.x = Left.x - Right.x;
    Return.y = Left.y - Right.y;
    Return.z = Left.z - Right.z;
    Return.w = Left.w - Right.w;
    return Return;
}

Vec4& Vec4::operator *= (float Right) {
    x *= Right;
    y *= Right;
    z *= Right;
    w *= Right;
    return *this;
}

Vec4& Vec4::operator /= (float Right) {
    x /= Right;
    y /= Right;
    z /= Right;
    w /= Right;
    return *this;
}

Vec4& Vec4::operator += (const Vec4 &Right) {
    x += Right.x;
    y += Right.y;
    z += Right.z;
    w += Right.w;
    return *this;
}

Vec4& Vec4::operator -= (const Vec4 &Right) {
    x -= Right.x;
    y -= Right.y;
    z -= Right.z;
    w -= Right.w;
    return *this;
}

Vec4 operator - (const Vec4 &V) {
    Vec4 Result;
    Result.x = -V.x;
    Result.y = -V.y;
    Result.z = -V.z;
    Result.w = -V.w;
    return Result;
}