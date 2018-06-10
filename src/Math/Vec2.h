
#pragma once

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
    bool valid() const;

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
    float length2() const;

    //
    // Conversion
    //
    Vec2 round() const;

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
    static Vec2 normalize(const Vec2 &V);
    static Vec2 randomVector();
    static Vec2 randomNormal();
    static Vec2 max(const Vec2 &Left, const Vec2 &Right);
    static Vec2 min(const Vec2 &Left, const Vec2 &Right);
    static Vec2 lerp(const Vec2 &Left, const Vec2 &Right, float s);
    static float distance(const Vec2 &Left, const Vec2 &Right);
    static float distance2(const Vec2 &Left, const Vec2 &Right);
    static float dot(const Vec2 &Left, const Vec2 &Right);
    static float cross(const Vec2 &Left, const Vec2 &Right);
    static float angleBetween(const Vec2 &Left, const Vec2 &Right);
};


Vec2 operator * (const Vec2 &Left, float Right);
Vec2 operator * (float Right, const Vec2 &Left);
Vec2 operator / (const Vec2 &Left, float Right);
Vec2 operator + (const Vec2 &Left, const Vec2 &Right);
Vec2 operator - (const Vec2 &Left, const Vec2 &Right);
Vec2 operator - (const Vec2 &V);
ostream& operator << (ostream& os, const Vec2& vec);