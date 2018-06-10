
#include "Vec2.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>

Vec2::Vec2() {

}

Vec2::Vec2(float _x, float _y) {
    x = _x;
    y = _y;
}

Vec2::Vec2(const Vec2 &V) {
    x = V.x;
    y = V.y;
}

bool Vec2::valid() const {
    return ((x == x) && (y == y));
}

Vec2& Vec2::operator = (const Vec2 &V) {
    x = V.x;
    y = V.y;
    return *this;
}

Vec2& Vec2::operator *= (float Right) {
    x *= Right;
    y *= Right;
    return *this;
}

Vec2& Vec2::operator /= (float Right) {
    x /= Right;
    y /= Right;
    return *this;
}

Vec2& Vec2::operator += (const Vec2 &Right) {
    x += Right.x;
    y += Right.y;
    return *this;
}

Vec2& Vec2::operator -= (const Vec2 &Right) {
    x -= Right.x;
    y -= Right.y;
    return *this;
}


Vec2 Vec2::randomVector() {
    return Vec2(float(rand()) / RAND_MAX * 2.0f - 1.0f,
                float(rand()) / RAND_MAX * 2.0f - 1.0f);
}

Vec2 Vec2::randomNormal() {
    return normalize(randomVector());
}

float Vec2::length() const {
    return sqrtf(x * x + y * y);
}

float Vec2::length2() const {
    return x * x + y * y;
}

Vec2 Vec2::round() const {
    return Vec2(roundf(x), roundf(y));
}

Vec2 Vec2::normalize(const Vec2 &V) {
    float Len = V.length();
    if(Len == 0.0f) {
        return V;
    } else {
        float Factor = 1.0f / Len;
        return Vec2(V.x * Factor, V.y * Factor);
    }
}

Vec2 Vec2::max(const Vec2 &Left, const Vec2 &Right) {
    Vec2 Result = Right;
    if(Left.x > Right.x) Result.x = Left.x;
    if(Left.y > Right.y) Result.y = Left.y;
    return Result;
}

Vec2 Vec2::min(const Vec2 &Left, const Vec2 &Right) {
    Vec2 Result = Right;
    if(Left.x < Right.x) Result.x = Left.x;
    if(Left.y < Right.y) Result.y = Left.y;
    return Result;
}

Vec2 Vec2::lerp(const Vec2 &Left, const Vec2 &Right, float s) {
    return (Left + s * (Right - Left));
}

float Vec2::dot(const Vec2 &Left, const Vec2 &Right) {
    return (Left.x * Right.x + Left.y * Right.y);
}

float Vec2::cross(const Vec2 &Left, const Vec2 &Right) {
    return (Left.x * Right.y - Left.y * Right.x);
}

float Vec2::angleBetween(const Vec2 &Left, const Vec2 &Right) {
    float LeftLength = Left.length();
    float RightLength = Right.length();
    if(LeftLength > 0.0f && RightLength > 0.0f) {
        return acosf(std::min(std::max(Vec2::dot(Left, Right) / LeftLength / RightLength, -1.0f), 1.0f));
    } else {
        return 0.0f;
    }
}

float Vec2::distance(const Vec2 &Left, const Vec2 &Right) {
    return (Right - Left).length();
}

float Vec2::distance2(const Vec2 &Left, const Vec2 &Right) {
    return (Right - Left).length2();
}

Vec2 operator * (const Vec2 &Left, float Right) {
    Vec2 Return;
    Return.x = Left.x * Right;
    Return.y = Left.y * Right;
    return Return;
}

Vec2 operator * (float Right, const Vec2 &Left)  {
    Vec2 Return;
    Return.x = Left.x * Right;
    Return.y = Left.y * Right;
    return Return;
}

Vec2 operator / (const Vec2 &Left, float Right) {
    Vec2 Return;
    Return.x = Left.x / Right;
    Return.y = Left.y / Right;
    return Return;
}

Vec2 operator + (const Vec2 &Left, const Vec2 &Right) {
    Vec2 Return;
    Return.x = Left.x + Right.x;
    Return.y = Left.y + Right.y;
    return Return;
}

Vec2 operator - (const Vec2 &Left, const Vec2 &Right) {
    Vec2 Return;
    Return.x = Left.x - Right.x;
    Return.y = Left.y - Right.y;
    return Return;
}

Vec2 operator - (const Vec2 &V) {
    return Vec2(-V.x, -V.y);
}

ostream& operator<<(ostream& os, const Vec2& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}