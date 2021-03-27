// #include "../include/engine.hpp"
#include "../include/figure_struct.hpp"

#include <istream>

namespace my_engine
{

std::istream& operator>>(std::istream& is, vertex& temp)
{
    is >> temp.x;
    is >> temp.y;
    is >> temp.z;
    is >> temp.r;
    is >> temp.g;
    is >> temp.b;
    is >> temp.tx;
    is >> temp.ty;

    return is;
}

std::istream& operator>>(std::istream& is, triangle& triang)
{
    is >> triang.v[0];
    is >> triang.v[1];
    is >> triang.v[2];
    return is;
}

triangle blend(const triangle& left, const triangle& right, const float a)
{
    triangle result;

    result.v[0] = blend(left.v[0], right.v[0], a);
    result.v[1] = blend(left.v[1], right.v[1], a);
    result.v[2] = blend(left.v[2], right.v[2], a);

    return result;
}

vertex blend(const vertex& left, const vertex& right, const float a)
{
    vertex result;

    result.x = (1 - a) * left.x + a * right.x;
    result.y = (1 - a) * left.y + a * right.y;
    result.z = (1 - a) * left.z + a * right.z;

    result.r = (1 - a) * left.r + a * right.r;
    result.g = (1 - a) * left.g + a * right.g;
    result.b = (1 - a) * left.b + a * right.b;

    result.tx = (1 - a) * left.tx + a * right.tx;
    result.ty = (1 - a) * left.ty + a * right.ty;

    return result;
}

} // namespace my_engine