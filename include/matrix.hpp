#pragma once
#include <iosfwd>

namespace my_engine
{
struct vec2
{
    vec2();
    vec2(float x, float y);
    static bool check_AABB(vec2 vec_1_A,
                           vec2 vec_1_B,
                           vec2 vec_2_A,
                           vec2 vec_2_B);
    float       x = 0;
    float       y = 0;
};

struct vec3
{
    vec3();
    vec3(float x, float y, float z);
    float x = 0;
    float y = 0;
    float z = 0;
};

struct matrix2x3
{
    matrix2x3();
    static matrix2x3 identiry();
    static matrix2x3 invert();
    static matrix2x3 scale(float scale);
    static matrix2x3 scale(float sx, float sy);
    static matrix2x3 rotation(float thetha);
    static matrix2x3 move(const vec2& delta);
    vec2             col0;
    vec2             col1;
    vec2             delta;
};

vec2 operator+(const vec2& l, const vec2& r);
vec2 operator-(const vec2& l, const vec2& r);

vec2      operator*(const vec2& v, const matrix2x3& m);
matrix2x3 operator*(const matrix2x3& m1, const matrix2x3& m2);

std::istream& operator>>(std::istream& is, vec2&);
std::istream& operator>>(std::istream& is, vec3& v);
std::istream& operator>>(std::istream& is, matrix2x3&);

std::ostream& operator<<(std::ostream& out, vec2&);
std::ostream& operator<<(std::ostream& out, matrix2x3&);
} // namespace my_engine
