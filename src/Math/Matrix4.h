
#pragma once

#include "Vec3.h"
#include "Vec4.h"

class Matrix4 {
public:
    //
    // Initalization
    //
    Matrix4();
    Matrix4(const Matrix4 &M);
    Matrix4(const Vec3 &V0, const Vec3 &V1, const Vec3 &V2);

#ifdef USE_D3D
    Matrix4(const D3DXMATRIX &M);
#endif

    //
    // Assignment
    //
    Matrix4& operator = (const Matrix4 &M);

    void print();

    //
    // Math properties
    //
    float determinant() const;
    Matrix4 transposed() const;
    Matrix4 inverse() const;

    //
    // Vector transforms
    //
    Vec3 transform(const Vec3 &Right, float w = 1) const;
    Point3 transformPoint(const Point3 &point) const;
    Vec3 transformVector(const Vec3 &normal) const;

#ifdef USE_D3D
    operator D3DXMATRIX() const;
#endif
    
    //
    // Accessors
    //
     float* operator [] (int Row) {
        return _Entries[Row];
    }
     const float* operator [] (int Row) const {
        return _Entries[Row];
    }
     void setColumn(int Column, const Vec4 &Values) {
        _Entries[0][Column] = Values.x;
        _Entries[1][Column] = Values.y;
        _Entries[2][Column] = Values.z;
        _Entries[3][Column] = Values.w;
    }
     void setRow(int Row, const Vec4 &Values) {
        _Entries[Row][0] = Values.x;
        _Entries[Row][1] = Values.y;
        _Entries[Row][2] = Values.z;
        _Entries[Row][3] = Values.w;
    }
     Vec4 getColumn(int Column) {
        Vec4 Result;
        Result.x = _Entries[0][Column];
        Result.y = _Entries[1][Column];
        Result.z = _Entries[2][Column];
        Result.w = _Entries[3][Column];
        return Result;
    }
     Vec4 getRow(int Row) {
        Vec4 Result;
        Result.x = _Entries[Row][0];
        Result.y = _Entries[Row][1];
        Result.z = _Entries[Row][2];
        Result.w = _Entries[Row][3];
        return Result;
    }

    //
    // transformation matrices
    //
    static Matrix4 identity();
    static Matrix4 scaling(const Vec3 &ScaleFactors);
    static Matrix4 scaling(float ScaleFactor) {
        return scaling(Vec3(ScaleFactor, ScaleFactor, ScaleFactor));
    }
    static Matrix4 translation(const Vec3 &Pos);
    static Matrix4 rotation(const Vec3 &Axis, float Angle, const Vec3 &Center);
    static Matrix4 rotation(const Vec3 &Axis, float Angle);
    static Matrix4 rotation(float Yaw, float Pitch, float Roll);
    static Matrix4 rotation(const Vec3 &Basis1, const Vec3 &Basis2, const Vec3 &Basis3);
    static Matrix4 rotationX(float Theta);
    static Matrix4 rotationY(float Theta);
    static Matrix4 rotationZ(float Theta);
    static Matrix4 lookAt(const Vec3 &Eye, const Vec3 &At, const Vec3 &Up);
    static Matrix4 orthogonal(float Width, float Height, float ZNear, float ZFar);
    static Matrix4 perspective(float Width, float Height, float ZNear, float ZFar);
    static Matrix4 perspectiveFOV(float FOV, float Aspect, float ZNear, float ZFar);
    static Matrix4 perspectiveMultiFOV(float FovX, float FovY, float ZNear, float ZFar);
    static Matrix4 viewport(float Width, float Height);
    static Matrix4 changeOfBasis(const Vec3 &Source0, const Vec3 &Source1, const Vec3 &Source2, const Vec3 &SourceOrigin, 
                                 const Vec3 &Target0, const Vec3 &Target1, const Vec3 &Target2, const Vec3 &TargetOrigin);
    static Matrix4 changeOfBasis(const Vec3 &v);

private:
    float _Entries[4][4];
};

//
// Overloaded operators
//
Matrix4 operator * (const Matrix4 &Left, const Matrix4 &Right);
Matrix4 operator * (const Matrix4 &Left, float &Right);
Matrix4 operator * (float &Left, const Matrix4 &Right);
Matrix4 operator + (const Matrix4 &Left, const Matrix4 &Right);
Matrix4 operator - (const Matrix4 &Left, const Matrix4 &Right);
Vec4 operator * (const Vec4 &Right, const Matrix4 &Left);
