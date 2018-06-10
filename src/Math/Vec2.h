/*
Vec2.inl
Written by Matthew Fisher

Inline file for a 2-dimensional vector of floats
*/

#ifndef __VEC2__
#define __VEC2__

#include <iostream>  
using namespace std;  

struct Vec2 {
    //
    // Initalization
    //
    Vec2();
    Vec2(const Vec2 &V);
    Vec2(float _x, float _y);

    // Check
    bool Valid() const;

    //
    // Assignment
    //
    Vec2& operator = (const Vec2 &V);

    //
    // Overloaded operators
    //
    Vec2& operator *= (float Right);
    Vec2& operator /= (float Right);
    Vec2& operator += (const Vec2 &Right);
    Vec2& operator -= (const Vec2 &Right);

    //
    // Accessors
    //
    float length() const;
    float LengthSq() const;

    //
    // Conversion
    //
    Vec2 Round() const;

    //
    // Local data
    //
    float x, y;

    //
    // Constants
    //
    static const Vec2 Origin;
    static const Vec2 eX;
    static const Vec2 eY;

    //
    // Static functions
    //
    static Vec2 Normalize(const Vec2 &V);
    static Vec2 StdRandomVector();
    static Vec2 StdRandomNormal();
    static Vec2 Maximize(const Vec2 &Left, const Vec2 &Right);
    static Vec2 Minimize(const Vec2 &Left, const Vec2 &Right);
    static Vec2 Lerp(const Vec2 &Left, const Vec2 &Right, float s);
    static float Dist(const Vec2 &Left, const Vec2 &Right);
    static float DistSq(const Vec2 &Left, const Vec2 &Right);
    static Vec2 ConstructFromPolar(float Radius, float Theta);
    static Vec2 Reflect(const Vec2 &Input, const Vec2 &ReflectAbout);
    static float Dot(const Vec2 &Left, const Vec2 &Right);
    static float Cross(const Vec2 &Left, const Vec2 &Right);
    static float AngleBetween(const Vec2 &Left, const Vec2 &Right);
};


Vec2 operator * (const Vec2 &Left, float Right);
Vec2 operator * (float Right, const Vec2 &Left);
Vec2 operator / (const Vec2 &Left, float Right);
Vec2 operator + (const Vec2 &Left, const Vec2 &Right);
Vec2 operator - (const Vec2 &Left, const Vec2 &Right);
Vec2 operator - (const Vec2 &V);
ostream& operator << (ostream& os, const Vec2& vec);

#endif