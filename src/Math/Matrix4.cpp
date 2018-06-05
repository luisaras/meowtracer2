/*
Matrix4.cpp
Written by Matthew Fisher

a 4x4 Matrix4 structure.  Used very often for affine vector transformations.
*/

#include "Matrix4.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

Matrix4::Matrix4()
{
    
}

Matrix4::Matrix4(const Matrix4 &M)
{
    for(int Row = 0; Row < 4; Row++)
    {
        for(int Col = 0; Col < 4; Col++)
        {
            _Entries[Row][Col] = M[Row][Col];
        }
    }
}

Matrix4::Matrix4(const Vec3 &V0, const Vec3 &V1, const Vec3 &V2)
{
    _Entries[0][0] = V0.x;
    _Entries[0][1] = V0.y;
    _Entries[0][2] = V0.z;
    _Entries[0][3] = 0.0f;

    _Entries[1][0] = V1.x;
    _Entries[1][1] = V1.y;
    _Entries[1][2] = V1.z;
    _Entries[1][3] = 0.0f;

    _Entries[2][0] = V2.x;
    _Entries[2][1] = V2.y;
    _Entries[2][2] = V2.z;
    _Entries[2][3] = 0.0f;

    _Entries[3][0] = 0.0f;
    _Entries[3][1] = 0.0f;
    _Entries[3][2] = 0.0f;
    _Entries[3][3] = 1.0f;
}

Matrix4& Matrix4::operator = (const Matrix4 &M)
{
    for(int Row = 0; Row < 4; Row++)
    {
        for(int Col = 0; Col < 4; Col++)
        {
            _Entries[Row][Col] = M[Row][Col];
        }
    }
    return (*this);
}

void Matrix4::Print() {
    for (int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << _Entries[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

Vec3 Matrix4::Transform(const Vec3 &v, float w) const
{
    return Vec3( (v.x * _Entries[0][0] + v.y * _Entries[1][0] + v.z * _Entries[2][0] + _Entries[3][0]) * w,
                  (v.x * _Entries[0][1] + v.y * _Entries[1][1] + v.z * _Entries[2][1] + _Entries[3][1]) * w,
                  (v.x * _Entries[0][2] + v.y * _Entries[1][2] + v.z * _Entries[2][2] + _Entries[3][2]) * w);
}

Point3 Matrix4::TransformPoint(const Point3 &point) const
{
    float w = point.x * _Entries[0][3] + point.y * _Entries[1][3] + point.z * _Entries[2][3] + _Entries[3][3];
    if(w)
    {
        const float invW = 1.0f / w;
        return Transform(point, invW);
    }
    else
    {
        return Vec3(0, 0, 0);
    }
}
    
Vec3 Matrix4::TransformVector(const Vec3 &normal) const
{
    return Vec3(normal.x * _Entries[0][0] + normal.y * _Entries[1][0] + normal.z * _Entries[2][0],
                 normal.x * _Entries[0][1] + normal.y * _Entries[1][1] + normal.z * _Entries[2][1],
                 normal.x * _Entries[0][2] + normal.y * _Entries[1][2] + normal.z * _Entries[2][2]);
}

Matrix4 Matrix4::Inverse() const
{
    //
    // Inversion by Cramer's rule.  Code taken from an Intel publication
    //
    float Result[4][4];
    float tmp[12]; /* temp array for pairs */
    float src[16]; /* array of transpose source matrix */
    float det; /* determinant */
    /* transpose matrix */
    for (int i = 0; i < 4; i++)
    {
        src[i + 0 ] = (*this)[i][0];
        src[i + 4 ] = (*this)[i][1];
        src[i + 8 ] = (*this)[i][2];
        src[i + 12] = (*this)[i][3];
    }
    /* calculate pairs for first 8 elements (cofactors) */
    tmp[0] = src[10] * src[15];
    tmp[1] = src[11] * src[14];
    tmp[2] = src[9] * src[15];
    tmp[3] = src[11] * src[13];
    tmp[4] = src[9] * src[14];
    tmp[5] = src[10] * src[13];
    tmp[6] = src[8] * src[15];
    tmp[7] = src[11] * src[12];
    tmp[8] = src[8] * src[14];
    tmp[9] = src[10] * src[12];
    tmp[10] = src[8] * src[13];
    tmp[11] = src[9] * src[12];
    /* calculate first 8 elements (cofactors) */
    Result[0][0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
    Result[0][0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
    Result[0][1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
    Result[0][1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
    Result[0][2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
    Result[0][2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
    Result[0][3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
    Result[0][3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
    Result[1][0] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
    Result[1][0] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
    Result[1][1] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
    Result[1][1] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
    Result[1][2] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
    Result[1][2] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
    Result[1][3] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
    Result[1][3] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
    /* calculate pairs for second 8 elements (cofactors) */
    tmp[0] = src[2]*src[7];
    tmp[1] = src[3]*src[6];
    tmp[2] = src[1]*src[7];
    tmp[3] = src[3]*src[5];
    tmp[4] = src[1]*src[6];
    tmp[5] = src[2]*src[5];

    tmp[6] = src[0]*src[7];
    tmp[7] = src[3]*src[4];
    tmp[8] = src[0]*src[6];
    tmp[9] = src[2]*src[4];
    tmp[10] = src[0]*src[5];
    tmp[11] = src[1]*src[4];
    /* calculate second 8 elements (cofactors) */
    Result[2][0] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
    Result[2][0] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
    Result[2][1] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
    Result[2][1] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
    Result[2][2] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
    Result[2][2] -= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
    Result[2][3] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
    Result[2][3] -= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
    Result[3][0] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
    Result[3][0] -= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
    Result[3][1] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
    Result[3][1] -= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
    Result[3][2] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
    Result[3][2] -= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
    Result[3][3] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
    Result[3][3] -= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
    /* calculate determinant */
    det=src[0]*Result[0][0]+src[1]*Result[0][1]+src[2]*Result[0][2]+src[3]*Result[0][3];
    /* calculate matrix inverse */
    det = 1.0f / det;

    Matrix4 FloatResult;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            FloatResult[i][j] = float(Result[i][j] * det);
        }
    }
    return FloatResult;

    //
    // Inversion by LU decomposition, alternate implementation
    //
    /*int i, j, k;

    for (i = 1; i < 4; i++)
    {
        _Entries[0][i] /= _Entries[0][0];
    }

    for (i = 1; i < 4; i++)
    {
        for (j = i; j < 4; j++)
        {
            float sum = 0.0;
            for (k = 0; k < i; k++)
            {
                sum += _Entries[j][k] * _Entries[k][i];
            }
            _Entries[j][i] -= sum;
        }
        if (i == 4-1) continue;
        for (j=i+1; j < 4; j++)
        {
            float sum = 0.0;
            for (int k = 0; k < i; k++)
                sum += _Entries[i][k]*_Entries[k][j];
            _Entries[i][j] = 
               (_Entries[i][j]-sum) / _Entries[i][i];
        }
    }

    //
    // Invert L
    //
    for ( i = 0; i < 4; i++ )
    {
        for ( int j = i; j < 4; j++ )
        {
            float x = 1.0;
            if ( i != j )
            {
                x = 0.0;
                for ( int k = i; k < j; k++ ) 
                    x -= _Entries[j][k]*_Entries[k][i];
            }
            _Entries[j][i] = x / _Entries[j][j];
        }
    }

    //
    // Invert U
    //
    for ( i = 0; i < 4; i++ )
    {
        for ( j = i; j < 4; j++ )
        {
            if ( i == j ) continue;
            float sum = 0.0;
            for ( int k = i; k < j; k++ )
                sum += _Entries[k][j]*( (i==k) ? 1.0f : _Entries[i][k] );
            _Entries[i][j] = -sum;
        }
    }

    //
    // Final Inversion
    //
    for ( i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 4; j++ )
        {
            float sum = 0.0;
            for ( int k = ((i>j)?i:j); k < 4; k++ )  
                sum += ((j==k)?1.0f:_Entries[j][k])*_Entries[k][i];
            _Entries[j][i] = sum;
        }
    }*/
}

Matrix4 Matrix4::Transpose() const
{
    Matrix4 Result;
    for(int i = 0; i < 4; i++)
    {
        for(int i2 = 0; i2 < 4; i2++)
        {
            Result[i2][i] = _Entries[i][i2];
        }
    }
    return Result;
}

Matrix4 Matrix4::Identity()
{
    Matrix4 Result;
    for(int i = 0; i < 4; i++)
    {
        for(int i2 = 0; i2 < 4; i2++)
        {
            if(i == i2)
            {
                Result[i][i2] = 1.0f;
            }
            else
            {
                Result[i][i2] = 0.0f;
            }
        }
    }
    return Result;
}

Matrix4 Matrix4::Rotation(const Vec3 &_Basis1, const Vec3 &_Basis2, const Vec3 &_Basis3)
{
    //
    // Verify everything is normalized
    //
    Vec3 Basis1 = Vec3::Normalize(_Basis1);
    Vec3 Basis2 = Vec3::Normalize(_Basis2);
    Vec3 Basis3 = Vec3::Normalize(_Basis3);

    Matrix4 Result;
    Result[0][0] = Basis1.x;
    Result[1][0] = Basis1.y;
    Result[2][0] = Basis1.z;
    Result[3][0] = 0.0f;

    Result[0][1] = Basis2.x;
    Result[1][1] = Basis2.y;
    Result[2][1] = Basis2.z;
    Result[3][1] = 0.0f;

    Result[0][2] = Basis3.x;
    Result[1][2] = Basis3.y;
    Result[2][2] = Basis3.z;
    Result[3][2] = 0.0f;

    Result[0][3] = 0.0f;
    Result[1][3] = 0.0f;
    Result[2][3] = 0.0f;
    Result[3][3] = 1.0f;
    return Result;
}

Matrix4 Matrix4::Camera(const Vec3 &Eye, const Vec3 &_Look, const Vec3 &_Up, const Vec3 &_Right)
{
    //
    // Verify everything is normalized
    //
    Vec3 Look = Vec3::Normalize(_Look);
    Vec3 Up = Vec3::Normalize(_Up);
    Vec3 Right = Vec3::Normalize(_Right);

    Matrix4 Result;
    Result[0][0] = Right.x;
    Result[1][0] = Right.y;
    Result[2][0] = Right.z;
    Result[3][0] = -Vec3::Dot(Right, Eye);

    Result[0][1] = Up.x;
    Result[1][1] = Up.y;
    Result[2][1] = Up.z;
    Result[3][1] = -Vec3::Dot(Up, Eye);

    Result[0][2] = Look.x;
    Result[1][2] = Look.y;
    Result[2][2] = Look.z;
    Result[3][2] = -Vec3::Dot(Look, Eye);

    Result[0][3] = 0.0f;
    Result[1][3] = 0.0f;
    Result[2][3] = 0.0f;
    Result[3][3] = 1.0f;
    return Result;
}

Matrix4 Matrix4::LookAt(const Vec3 &Eye, const Vec3 &At, const Vec3 &Up)
{
    Vec3 XAxis, YAxis, ZAxis;
    ZAxis = Vec3::Normalize(Eye - At);
    XAxis = Vec3::Normalize(Vec3::Cross(Up, ZAxis));
    YAxis = Vec3::Normalize(Vec3::Cross(ZAxis, XAxis));

    Matrix4 Result;
    Result[0][0] = XAxis.x;
    Result[1][0] = XAxis.y;
    Result[2][0] = XAxis.z;
    Result[3][0] = -Vec3::Dot(XAxis,Eye);

    Result[0][1] = YAxis.x;
    Result[1][1] = YAxis.y;
    Result[2][1] = YAxis.z;
    Result[3][1] = -Vec3::Dot(YAxis,Eye);

    Result[0][2] = ZAxis.x;
    Result[1][2] = ZAxis.y;
    Result[2][2] = ZAxis.z;
    Result[3][2] = -Vec3::Dot(ZAxis,Eye);

    Result[0][3] = 0.0f;
    Result[1][3] = 0.0f;
    Result[2][3] = 0.0f;
    Result[3][3] = 1.0f;
    return Result;
}

Matrix4 Matrix4::Orthogonal(float Width, float Height, float ZNear, float ZFar)
{
    Matrix4 Result;
    Result[0][0] = 2.0f / Width;
    Result[1][0] = 0.0f;
    Result[2][0] = 0.0f;
    Result[3][0] = 0.0f;

    Result[0][1] = 0.0f;
    Result[1][1] = 2.0f / Height;
    Result[2][1] = 0.0f;
    Result[3][1] = 0.0f;

    Result[0][2] = 0.0f;
    Result[1][2] = 0.0f;
    Result[2][2] = 1.0f / (ZNear - ZFar);
    Result[3][2] = ZNear / (ZNear - ZFar);

    Result[0][3] = 0.0f;
    Result[1][3] = 0.0f;
    Result[2][3] = 0.0f;
    Result[3][3] = 1.0f;
    return Result;
}

Matrix4 Matrix4::Perspective(float Width, float Height, float ZNear, float ZFar)
{
    Matrix4 Result;
    Result[0][0] = 2.0f * ZNear / Width;
    Result[1][0] = 0.0f;
    Result[2][0] = 0.0f;
    Result[3][0] = 0.0f;

    Result[0][1] = 0.0f;
    Result[1][1] = 2.0f * ZNear / Height;
    Result[2][1] = 0.0f;
    Result[3][1] = 0.0f;

    Result[0][2] = 0.0f;
    Result[1][2] = 0.0f;
    Result[2][2] = ZFar / (ZNear - ZFar);
    Result[3][2] = ZFar * ZNear / (ZNear - ZFar);

    Result[0][3] = 0.0f;
    Result[1][3] = 0.0f;
    Result[2][3] = -1.0f;
    Result[3][3] = 0.0f;
    return Result;
}

Matrix4 Matrix4::PerspectiveFov(float FOV, float Aspect, float ZNear, float ZFar)
{
    float Width = 1.0f / tanf(FOV/2.0f), Height = Aspect / tanf(FOV/2.0f);

    Matrix4 Result;
    Result[0][0] = Width;
    Result[1][0] = 0.0f;
    Result[2][0] = 0.0f;
    Result[3][0] = 0.0f;

    Result[0][1] = 0.0f;
    Result[1][1] = Height;
    Result[2][1] = 0.0f;
    Result[3][1] = 0.0f;

    Result[0][2] = 0.0f;
    Result[1][2] = 0.0f;
    Result[2][2] = ZFar / (ZNear - ZFar);
    Result[3][2] = ZFar * ZNear / (ZNear - ZFar);

    Result[0][3] = 0.0f;
    Result[1][3] = 0.0f;
    Result[2][3] = -1.0f;
    Result[3][3] = 0.0f;
    return Result;
}

Matrix4 Matrix4::PerspectiveMultiFov(float FovX, float FovY, float ZNear, float ZFar)
{
    float Width = 1.0f / tanf(FovX / 2.0f), Height = 1.0f / tanf(FovY / 2.0f);

    Matrix4 Result;
    Result[0][0] = Width;
    Result[1][0] = 0.0f;
    Result[2][0] = 0.0f;
    Result[3][0] = 0.0f;

    Result[0][1] = 0.0f;
    Result[1][1] = Height;
    Result[2][1] = 0.0f;
    Result[3][1] = 0.0f;

    Result[0][2] = 0.0f;
    Result[1][2] = 0.0f;
    Result[2][2] = ZFar / (ZNear - ZFar);
    Result[3][2] = ZFar * ZNear / (ZNear - ZFar);

    Result[0][3] = 0.0f;
    Result[1][3] = 0.0f;
    Result[2][3] = -1.0f;
    Result[3][3] = 0.0f;
    return Result;
}

Matrix4 Matrix4::Rotation(const Vec3 &Axis, float Angle)
{
    float c = cosf(Angle);
    float s = sinf(Angle);
    float t = 1.0f - c;

    Vec3 NormalizedAxis = Vec3::Normalize(Axis);
    float x = NormalizedAxis.x;
    float y = NormalizedAxis.y;
    float z = NormalizedAxis.z;

    Matrix4 Result;
    Result[0][0] = 1 + t*(x*x-1);
    Result[0][1] = z*s+t*x*y;
    Result[0][2] = -y*s+t*x*z;
    Result[0][3] = 0.0f;

    Result[1][0] = -z*s+t*x*y;
    Result[1][1] = 1+t*(y*y-1);
    Result[1][2] = x*s+t*y*z;
    Result[1][3] = 0.0f;

    Result[2][0] = y*s+t*x*z;
    Result[2][1] = -x*s+t*y*z;
    Result[2][2] = 1+t*(z*z-1);
    Result[2][3] = 0.0f;

    Result[3][0] = 0.0f;
    Result[3][1] = 0.0f;
    Result[3][2] = 0.0f;
    Result[3][3] = 1.0f;
    return Result;
}

Matrix4 Matrix4::Rotation(float Pitch, float Yaw, float Roll)
{
    return RotationX(Pitch) * RotationY(Yaw) * RotationZ(Roll);
}

Matrix4 Matrix4::Rotation(const Vec3 &Axis, float Angle, const Vec3 &Center)
{
    return Translation(-Center) * Rotation(Axis, Angle) * Translation(Center);
}

Matrix4 Matrix4::RotationX(float Theta)
{
    float CosT = cosf(Theta);
    float SinT = sinf(Theta);

    Matrix4 Result = Identity();
    Result[1][1] = CosT;
    Result[1][2] = SinT;
    Result[2][1] = -SinT;
    Result[2][2] = CosT;
    return Result;
}

Matrix4 Matrix4::RotationY(float Theta)
{
    float CosT = cosf(Theta);
    float SinT = sinf(Theta);

    Matrix4 Result = Identity();
    Result[0][0] = CosT;
    Result[0][2] = SinT;
    Result[2][0] = -SinT;
    Result[2][2] = CosT;
    return Result;
}

Matrix4 Matrix4::RotationZ(float Theta)
{
    float CosT = cosf(Theta);
    float SinT = sinf(Theta);

    Matrix4 Result = Identity();
    Result[0][0] = CosT;
    Result[0][1] = SinT;
    Result[1][0] = -SinT;
    Result[1][1] = CosT;
    return Result;
}

Matrix4 Matrix4::Scaling(const Vec3 &ScaleFactors)
{
    Matrix4 Result;
    Result[0][0] = ScaleFactors.x;
    Result[1][0] = 0.0f;
    Result[2][0] = 0.0f;
    Result[3][0] = 0.0f;

    Result[0][1] = 0.0f;
    Result[1][1] = ScaleFactors.y;
    Result[2][1] = 0.0f;
    Result[3][1] = 0.0f;

    Result[0][2] = 0.0f;
    Result[1][2] = 0.0f;
    Result[2][2] = ScaleFactors.z;
    Result[3][2] = 0.0f;

    Result[0][3] = 0.0f;
    Result[1][3] = 0.0f;
    Result[2][3] = 0.0f;
    Result[3][3] = 1.0f;
    return Result;
}

Matrix4 Matrix4::Translation(const Vec3 &Pos)
{
    Matrix4 Result;
    Result[0][0] = 1.0f;
    Result[1][0] = 0.0f;
    Result[2][0] = 0.0f;
    Result[3][0] = Pos.x;

    Result[0][1] = 0.0f;
    Result[1][1] = 1.0f;
    Result[2][1] = 0.0f;
    Result[3][1] = Pos.y;

    Result[0][2] = 0.0f;
    Result[1][2] = 0.0f;
    Result[2][2] = 1.0f;
    Result[3][2] = Pos.z;

    Result[0][3] = 0.0f;
    Result[1][3] = 0.0f;
    Result[2][3] = 0.0f;
    Result[3][3] = 1.0f;
    return Result;
}

Matrix4 Matrix4::ChangeOfBasis(const Vec3 &Source0, const Vec3 &Source1, const Vec3 &Source2, const Vec3 &SourceOrigin, 
                               const Vec3 &Target0, const Vec3 &Target1, const Vec3 &Target2, const Vec3 &TargetOrigin)
{
    Matrix4 RotationComponent = Matrix4(Source0, Source1, Source2).Inverse() * Matrix4(Target0, Target1, Target2);
    //Matrix4 TranslationComponent = Translation(TargetOrigin - SourceOrigin);
    Matrix4 Result = Translation(-SourceOrigin) * RotationComponent * Translation(TargetOrigin);
    return Result;
    //return Translation(TargetOrigin - SourceOrigin);
}

Matrix4 Matrix4::Viewport(float Width, float Height)
{
    return Matrix4::Scaling(Vec3(Width * 0.5f, -Height * 0.5f, 1.0f)) * Matrix4::Translation(Vec3(Width * 0.5f, Height * 0.5f, 0.0f));
}

/*Vec3 Matrix4::TransformPoint(const Vec3 &Point) const
{
    Vec4f UnprojectedResult = Vec4f(Point, 1.0f) * (*this);
    if(UnprojectedResult.w == 0.0f || UnprojectedResult.w == 1.0f)
    {
        return Vec3(UnprojectedResult.x, UnprojectedResult.y, UnprojectedResult.z);
    }
    else
    {
        return Vec3(UnprojectedResult.x / UnprojectedResult.w,
                     UnprojectedResult.y / UnprojectedResult.w,
                     UnprojectedResult.z / UnprojectedResult.w);
    }
}

Vec3 Matrix4::TransformNormal(const Vec3 &Normal) const
{
    Vec4f UnprojectedResult = Vec4f(Normal, 0.0f) * (*this);
    if(UnprojectedResult.w == 0.0f)
    {
        UnprojectedResult.w = 1.0f;
    }

    Vec3 Result(UnprojectedResult.x / UnprojectedResult.w,
                UnprojectedResult.y / UnprojectedResult.w,
                UnprojectedResult.z / UnprojectedResult.w);

    if(UnprojectedResult.w < 0.0f)
    {
        Result = -Result;
    }
    return Result;
}*/

Matrix4 operator * (const Matrix4 &Left, const Matrix4 &Right)
{
    Matrix4 Result;
    for(int i = 0; i < 4; i++)
    {
        for(int i2 = 0; i2 < 4; i2++)
        {
            float Total = 0.0f;
            for(int i3 = 0; i3 < 4; i3++)
            {
                Total += Left[i][i3] * Right[i3][i2];
            }
            Result[i][i2] = Total;
        }
    }
    return Result;
}

Matrix4 operator * (const Matrix4 &Left, float &Right)
{
    Matrix4 Result;
    for(int i = 0; i < 4; i++)
    {
        for(int i2 = 0; i2 < 4; i2++)
        {
            Result[i][i2] = Left[i][i2] * Right;
        }
    }
    return Result;
}

Matrix4 operator * (float &Left, const Matrix4 &Right)
{
    Matrix4 Result;
    for(int i = 0; i < 4; i++)
    {
        for(int i2 = 0; i2 < 4; i2++)
        {
            Result[i][i2] = Right[i][i2] * Left;
        }
    }
    return Result;
}

Vec4 operator * (const Vec4 &Right, const Matrix4 &Left)
{
    return Vec4(Right.x * Left[0][0] + Right.y * Left[1][0] + Right.z * Left[2][0] + Right.w * Left[3][0],
                Right.x * Left[0][1] + Right.y * Left[1][1] + Right.z * Left[2][1] + Right.w * Left[3][1],
                Right.x * Left[0][2] + Right.y * Left[1][2] + Right.z * Left[2][2] + Right.w * Left[3][2],
                Right.x * Left[0][3] + Right.y * Left[1][3] + Right.z * Left[2][3] + Right.w * Left[3][3]);
}

Matrix4 operator + (const Matrix4 &Left, const Matrix4 &Right)
{
    Matrix4 Result;
    for(int i = 0; i < 4; i++)
    {
        for(int i2 = 0; i2 < 4; i2++)
        {
            Result[i][i2] = Left[i][i2] + Right[i][i2];
        }
    }
    return Result;
}

Matrix4 operator - (const Matrix4 &Left, const Matrix4 &Right)
{
    Matrix4 Result;
    for(int i = 0; i < 4; i++)
    {
        for(int i2 = 0; i2 < 4; i2++)
        {
            Result[i][i2] = Left[i][i2] - Right[i][i2];
        }
    }
    return Result;
}