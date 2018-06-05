#ifndef __VEC3__
#define __VEC3__

#include <iostream>

struct Vec3
{
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
     void SetLength(float NewLength);

    //
    // Accessors
    //
     float Length() const;
     float LengthSq() const;

     bool Valid() const;

     void Print() const {
        std::cout << x << " " << y << " " << z;
     }

#ifdef USE_D3D
     operator D3DXVECTOR3() const;
#endif

     float& operator[](int Index)
    {
        return ((float *)this)[Index];
    }
     float operator[](int Index) const
    {
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
     static Vec3 Normalize(const Vec3 &V);
     static Vec3 StdRandomVector();
     static Vec3 StdRandomNormal();
     static Vec3 Cross(const Vec3 &Left, const Vec3 &Right);
     static Vec3 DirectProduct(const Vec3 &Left, const Vec3 &Right);
     static float Dot(const Vec3 &Left, const Vec3 &Right);
     static float Dist(const Vec3 &Left, const Vec3 &Right);
     static float DistSq(const Vec3 &Left, const Vec3 &Right);
     static Vec3 Lerp(const Vec3 &Left, const Vec3 &Right, float s);
     static Vec3 Maximize(const Vec3 &Left, const Vec3 &Right);
     static Vec3 Minimize(const Vec3 &Left, const Vec3 &Right);
     //static bool WithinRect(const Vec3 &Pt, const Rectangle3f &Rect);
     static Vec3 SphericalFromCartesian(const Vec3 &Cartesian);
     static Vec3 CartesianFromSpherical(const Vec3 &Spherical);
};

Vec3 operator* (const Vec3 &Left, float Right);
Vec3 operator* (float Left, const Vec3 &Right);
Vec3 operator* (const Vec3 &Left, const Vec3 &Right);
Vec3 operator / (const Vec3 &Left, float Right);
Vec3 operator + (const Vec3 &Left, const Vec3 &Right);
Vec3 operator - (const Vec3 &Left, const Vec3 &Right);
Vec3 operator - (const Vec3 &V);

#define Point3 Vec3
#define Color Vec3

#endif