/*
Vec3.inl
Written by Matthew Fisher

Inline file for a 3-dimensional vector of floats
*/

#include "Vec3.h"
#include <cmath>
#include <cstdlib>

using namespace std;

Vec3::Vec3() {

}

Vec3::Vec3(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
}

Vec3::Vec3(const Vec3 &V) {
    x = V.x;
    y = V.y;
    z = V.z;
}

 Vec3& Vec3::operator = (const Vec3 &V) {
    x = V.x;
    y = V.y;
    z = V.z;
    return *this;
}

 Vec3 Vec3::StdRandomVector() {
    float x, y, z;
    x = float(rand()) / RAND_MAX * 2.0f - 1.0f;
    y = float(rand()) / RAND_MAX * 2.0f - 1.0f;
    z = float(rand()) / RAND_MAX * 2.0f - 1.0f;
    return Vec3(x, y, z);
}

 Vec3 Vec3::StdRandomNormal() {
    return Normalize(StdRandomVector());
}

 float Vec3::Length() const {
    return sqrtf(x * x + y * y + z * z);
}

 float Vec3::LengthSq() const {
    return x * x + y * y + z * z;
}

 bool Vec3::Valid() const {
    return ((x == x) && (y == y) && (z == z));
}

 Vec3 Vec3::Normalize(const Vec3 &V) {
    float Len = V.Length();
    if(Len == 0.0f) {
        return V;
    } else {
        float Factor = 1.0f / Len;
        return Vec3(V.x * Factor, V.y * Factor, V.z * Factor);
    }
}

 void Vec3::SetLength(float NewLength) {
    float Len = Length();
    if(Len != 0.0f) {
        float Factor = NewLength / Len;
        x *= Factor;
        y *= Factor;
        z *= Factor;
    }
}

#ifdef USE_D3D
 Vec3::operator D3DXVECTOR3() const {
    D3DXVECTOR3 V(x, y, z);
    return V;
}
#endif

 Vec3 Vec3::Cross(const Vec3 &Left, const Vec3 &Right) {
    Vec3 Result;
    Result.x = Left.y * Right.z - Left.z * Right.y;
    Result.y = Left.z * Right.x - Left.x * Right.z;
    Result.z = Left.x * Right.y - Left.y * Right.x;
    return Result;
}

 float Vec3::Dot(const Vec3 &Left, const Vec3 &Right) {
    return (Left.x * Right.x + Left.y * Right.y + Left.z * Right.z);
}

 Vec3 Vec3::DirectProduct(const Vec3 &Left, const Vec3 &Right) {
    return Vec3(Left.x * Right.x, Left.y * Right.y, Left.z * Right.z);
}

 Vec3 Vec3::Lerp(const Vec3 &Left, const Vec3 &Right, float s) {
    return (Left + s * (Right - Left));
}

 Vec3 Vec3::Maximize(const Vec3 &Left, const Vec3 &Right) {
    Vec3 Result = Right;
    if(Left.x > Right.x) Result.x = Left.x;
    if(Left.y > Right.y) Result.y = Left.y;
    if(Left.z > Right.z) Result.z = Left.z;
    return Result;
}

 Vec3 Vec3::Minimize(const Vec3 &Left, const Vec3 &Right) {
    Vec3 Result = Right;
    if(Left.x < Right.x) Result.x = Left.x;
    if(Left.y < Right.y) Result.y = Left.y;
    if(Left.z < Right.z) Result.z = Left.z;
    return Result;
}

 Vec3 Vec3::SphericalFromCartesian(const Vec3 &Cartesian) {
    Vec3 Result;
    Result.x = Cartesian.Length();
    Result.y = atan2f(Cartesian.y, Cartesian.x);
    if(Result.x == 0.0f) {
        Result.z = 0.0f;
    } else {
        Result.z = acosf(Cartesian.z / Result.x);
    }
    return Result;
}

 Vec3 Vec3::CartesianFromSpherical(const Vec3 &Spherical) {
    const float &r = Spherical.x;
    const float &Theta = Spherical.y;
    const float &Phi = Spherical.z;
    float RSinPhi = r * sinf(Phi);
    return Vec3(cosf(Theta) * RSinPhi, sinf(Theta) * RSinPhi, r * cosf(Phi));
}
/*
 bool Vec3::WithinRect(const Vec3 &Pt, const Rectangle3f &Rect) {
    return((Pt.x >= Rect.Min.x && Pt.x <= Rect.Max.x) &&
           (Pt.y >= Rect.Min.y && Pt.y <= Rect.Max.y) &&
           (Pt.z >= Rect.Min.z && Pt.z <= Rect.Max.z));
}*/

 Vec3& Vec3::operator *= (float Right) {
    x *= Right;
    y *= Right;
    z *= Right;
    return *this;
}

 Vec3& Vec3::operator /= (float Right) {
    x /= Right;
    y /= Right;
    z /= Right;
    return *this;
}

 Vec3& Vec3::operator *= (int Right) {
    x *= Right;
    y *= Right;
    z *= Right;
    return *this;
}

 Vec3& Vec3::operator /= (int Right) {
    x /= Right;
    y /= Right;
    z /= Right;
    return *this;
}

 Vec3& Vec3::operator += (const Vec3 &Right) {
    x += Right.x;
    y += Right.y;
    z += Right.z;
    return *this;
}

 Vec3& Vec3::operator -= (const Vec3 &Right) {
    x -= Right.x;
    y -= Right.y;
    z -= Right.z;
    return *this;
}

Vec3 operator* (const Vec3 &Left, const Vec3 &Right) {
    return Vec3(Right.x * Left.x,
                Right.y * Left.y,
                Right.z * Left.z);
}

 Vec3 operator* (const Vec3 &Left, float Right) {
    return Vec3(Left.x * Right,
                Left.y * Right,
                Left.z * Right);
}

 Vec3 operator* (float Left, const Vec3 &Right) {
    return Vec3(Right.x * Left,
                Right.y * Left,
                Right.z * Left);
}

 Vec3 operator / (const Vec3 &Left, float Right) {
    return Vec3(Left.x / Right,
                Left.y / Right,
                Left.z / Right);
}

 Vec3 operator + (const Vec3 &Left, const Vec3 &Right) {
    return Vec3(Left.x + Right.x,
                Left.y + Right.y,
                Left.z + Right.z);
}

 Vec3 operator - (const Vec3 &Left, const Vec3 &Right) {
    return Vec3(Left.x - Right.x,
                Left.y - Right.y,
                Left.z - Right.z);
}

 Vec3 operator - (const Vec3 &V) {
    return Vec3(-V.x, -V.y, -V.z);
}

 float Vec3::Dist(const Vec3 &Left, const Vec3 &Right) {
    const float XDiff = Right.x - Left.x;
    const float YDiff = Right.y - Left.y;
    const float ZDiff = Right.z - Left.z;
    return sqrtf(XDiff * XDiff + YDiff * YDiff + ZDiff * ZDiff);
}

 float Vec3::DistSq(const Vec3 &Left, const Vec3 &Right) {
    const float XDiff = Right.x - Left.x;
    const float YDiff = Right.y - Left.y;
    const float ZDiff = Right.z - Left.z;
    return (XDiff * XDiff + YDiff * YDiff + ZDiff * ZDiff);
}

ostream& operator<<(ostream& os, const Vec3& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}