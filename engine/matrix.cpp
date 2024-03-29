#include "matrix.hpp"

#include <cmath>
#include <iostream>

namespace my_engine
{
vec2::vec2()
    : x(0.f)
    , y(0.f)
{
}

vec2::vec2(float x_, float y_)
    : x(x_)
    , y(y_)
{
}

bool vec2::check_AABB(vec2 vec_1_A, vec2 vec_1_B, vec2 vec_2_A, vec2 vec_2_B)
{
    if (vec_1_A.x < vec_2_B.x && vec_1_A.y < vec_2_B.y &&
        vec_1_B.x > vec_2_A.x && vec_1_B.y > vec_2_A.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

vec3::vec3()
    : x(0.f)
    , y(0.f)
    , z(0.f)
{
}

vec3::vec3(float x_, float y_, float z_)
    : x(x_)
    , y(y_)
    , z(z_)
{
}

matrix2x3::matrix2x3()
    : col0(1.0f, 0.f)
    , col1(0.f, 1.f)
    , delta(0.f, 0.f)
{
}

matrix2x3 matrix2x3::invert()
{
    matrix2x3 result;
    result.col0.x = -1;
    result.col1.y = -1;
    return result;
}

matrix2x3 matrix2x3::identiry()
{
    return matrix2x3::scale(1.f);
}

matrix2x3 matrix2x3::scale(float scale)
{
    matrix2x3 result;
    result.col0.x = scale;
    result.col1.y = scale;
    return result;
}

matrix2x3 matrix2x3::scale(float sx, float sy)
{
    matrix2x3 r;
    r.col0.x = sx;
    r.col1.y = sy;
    return r;
}

matrix2x3 matrix2x3::rotation(float thetha)
{
    matrix2x3 result;

    result.col0.x = std::cos(thetha);
    result.col0.y = std::sin(thetha);

    result.col1.x = -std::sin(thetha);
    result.col1.y = std::cos(thetha);

    return result;
}

matrix2x3 matrix2x3::move(const vec2& delta)
{
    matrix2x3 r = matrix2x3::identiry();
    r.delta     = delta;
    return r;
}

std::istream& operator>>(std::istream& is, vec2& v)
{
    is >> v.x;
    is >> v.y;
    return is;
}

std::istream& operator>>(std::istream& is, vec3& v)
{
    is >> v.x;
    is >> v.y;
    is >> v.z;
    return is;
}

std::istream& operator>>(std::istream& is, matrix2x3& m)
{
    is >> m.col0.x;
    is >> m.col1.x;
    is >> m.col0.y;
    is >> m.col1.y;
    return is;
}

std::ostream& operator<<(std::ostream& out, vec2& vec)
{
    out << "x: " << vec.x << "||" << " y: "<< vec.y;
    return out;
}

std::ostream& operator<<(std::ostream& out, matrix2x3& m)
{
    out << m.col0.x << "||" << m.col0.y << "\n";
    out << m.col1.x << "||" << m.col1.y << "\n";
    out << m.delta.x << "||" << m.delta.y;
    return out;
}

vec2 operator*(const vec2& v, const matrix2x3& m)
{
    vec2 result;
    result.x = v.x * m.col0.x + v.y * m.col0.y + m.delta.x;
    result.y = v.x * m.col1.x + v.y * m.col1.y + m.delta.y;
    return result;
}

matrix2x3 operator*(const matrix2x3& m1, const matrix2x3& m2)
{
    matrix2x3 r;

    r.col0.x = m1.col0.x * m2.col0.x + m1.col1.x * m2.col0.y;
    r.col1.x = m1.col0.x * m2.col1.x + m1.col1.x * m2.col1.y;
    r.col0.y = m1.col0.y * m2.col0.x + m1.col1.y * m2.col0.y;
    r.col1.y = m1.col0.y * m2.col1.x + m1.col1.y * m2.col1.y;

    r.delta.x = m1.delta.x * m2.col0.x + m1.delta.y * m2.col0.y + m2.delta.x;
    r.delta.y = m1.delta.x * m2.col1.x + m1.delta.y * m2.col1.y + m2.delta.y;

    return r;
}

vec2 operator+(const vec2& l, const vec2& r)
{
    vec2 result;
    result.x = l.x + r.x;
    result.y = l.y + r.y;
    return result;
}

vec2 operator-(const vec2& l, const vec2& r)
{
    vec2 result;
    result.x = l.x - r.x;
    result.y = l.y - r.y;
    return result;
}

} // namespace my_engine
