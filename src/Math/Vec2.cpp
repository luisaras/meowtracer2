/*const
Vec2.inl
Written by Matthew Fisher

Inline file for a 2-dimensional vector of floats
*/

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

bool Vec2::Valid() const {
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


Vec2 Vec2::StdRandomVector() {
    return Vec2(float(rand()) / RAND_MAX * 2.0f - 1.0f,
                float(rand()) / RAND_MAX * 2.0f - 1.0f);
}

Vec2 Vec2::StdRandomNormal() {
    return Normalize(StdRandomVector());
}

float Vec2::length() const {
    return sqrtf(x * x + y * y);
}

float Vec2::LengthSq() const {
    return x * x + y * y;
}

Vec2 Vec2::Round() const {
    return Vec2(roundf(x), roundf(y));
}

Vec2 Vec2::Normalize(const Vec2 &V) {
    float Len = V.length();
    if(Len == 0.0f) {
        return V;
    } else {
        float Factor = 1.0f / Len;
        return Vec2(V.x * Factor, V.y * Factor);
    }
}

Vec2 Vec2::Maximize(const Vec2 &Left, const Vec2 &Right) {
    Vec2 Result = Right;
    if(Left.x > Right.x) Result.x = Left.x;
    if(Left.y > Right.y) Result.y = Left.y;
    return Result;
}

Vec2 Vec2::Minimize(const Vec2 &Left, const Vec2 &Right) {
    Vec2 Result = Right;
    if(Left.x < Right.x) Result.x = Left.x;
    if(Left.y < Right.y) Result.y = Left.y;
    return Result;
}

Vec2 Vec2::Lerp(const Vec2 &Left, const Vec2 &Right, float s) {
    return (Left + s * (Right - Left));
}

Vec2 Vec2::ConstructFromPolar(float Radius, float Theta) {
    return Vec2(Radius * cosf(Theta), Radius * sinf(Theta));
}

float Vec2::Dot(const Vec2 &Left, const Vec2 &Right) {
    return (Left.x * Right.x + Left.y * Right.y);
}

float Vec2::Cross(const Vec2 &Left, const Vec2 &Right) {
    return (Left.x * Right.y - Left.y * Right.x);
}

Vec2 Vec2::Reflect(const Vec2 &Input, const Vec2 &ReflectAbout) {
    Vec2 ReflectAboutNormalized = Vec2::Normalize(ReflectAbout);
    Vec2 InputNormalized = Vec2::Normalize(Input);
    return (2.0f * Vec2::Dot(InputNormalized, ReflectAboutNormalized) * ReflectAboutNormalized - InputNormalized);
}

float Vec2::AngleBetween(const Vec2 &Left, const Vec2 &Right) {
    float LeftLength = Left.length();
    float RightLength = Right.length();
    if(LeftLength > 0.0f && RightLength > 0.0f) {
        return acosf(std::min(std::max(Vec2::Dot(Left, Right) / LeftLength / RightLength, -1.0f), 1.0f));
    } else {
        return 0.0f;
    }
}

float Vec2::Dist(const Vec2 &Left, const Vec2 &Right) {
    return (Right - Left).length();
}

float Vec2::DistSq(const Vec2 &Left, const Vec2 &Right) {
    return (Right - Left).LengthSq();
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