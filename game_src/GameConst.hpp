#pragma once

#define DEBUG_LEVEL
// #define DEBUG_LEVEL_2


#include "../include/matrix.hpp"

#include <iostream>
#include <numbers>

namespace gameConst
{

constexpr size_t screen_width  = 320 * 6; // * 6 = 1920
constexpr size_t screen_height = 180 * 6; // * 6 = 1080
// constexpr size_t screen_width  = 1080;
// constexpr size_t screen_height = 1080;

constexpr float aspect =
    static_cast<float>(screen_width) / static_cast<float>(screen_height);
const my_engine::matrix2x3 aspect_mat = my_engine::matrix2x3::scale(1, aspect);

constexpr float            size     = 1.0f;
const my_engine::matrix2x3 size_mat = my_engine::matrix2x3::scale(size);
constexpr float            pi       = std::numbers::pi_v<float>;

} // namespace gameConst