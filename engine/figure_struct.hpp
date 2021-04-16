// #pragma once

// #include <iosfwd>

// namespace my_engine
// {

// struct vertex
// {
//     float x = 0.f;
//     float y = 0.f;
//     float z = 0.f;

//     float r = 0.f;
//     float g = 0.f;
//     float b = 0.f;

//     float tx = 0.f;
//     float ty = 0.f;
// };

// struct triangle
// {
//     triangle()
//     {
//         v[0] = vertex();
//         v[1] = vertex();
//         v[2] = vertex();
//     }
//     vertex v[3];
// };

// std::istream& operator>>(std::istream& is, vertex&);
// std::istream& operator>>(std::istream& is, triangle&);

// triangle blend(const triangle& left, const triangle& right, const float a);

// vertex blend(const vertex& left, const vertex& right, const float a);

// } // namespace my_engine
