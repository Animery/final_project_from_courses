#pragma once

#include "../include/engine.hpp"

#include <memory>
#include <numbers>

constexpr float            aspect     = 320.0f / 180.0f;
const my_engine::matrix2x3 aspect_mat = my_engine::matrix2x3::scale(1, aspect);

constexpr float            size       = 1.0f;
const my_engine::matrix2x3 size_mat   = my_engine::matrix2x3::scale(size);
constexpr float pi = std::numbers::pi_v<float>;

class Player
{
public:
    // Player(my_engine::gfx_prog* gfx_prog);
    Player();
    ~Player();
    void update(std::array<bool, 8>& controls);

    // my_engine::RenderObj* getCorpse();
    // my_engine::RenderObj* getHead();

    my_engine::matrix2x3& getMatrix_corpse();
    my_engine::matrix2x3& getMatrix_head();

    float getCurrent_head_direction();
    void  setCurrent_head_direction(const float a);

    my_engine::vec2& getCurrent_tank_pos();

private:
    void loadRenderObj();
    // my_engine::RenderObj* corpse = nullptr;
    // my_engine::RenderObj* head   = nullptr;

    my_engine::matrix2x3 matrix_corpse;
    my_engine::matrix2x3 matrix_head;

    // my_engine::matrix2x3 aspect = my_engine::matrix2x3::scale(1, 640.f /
    // 480.f);

    my_engine::vec2 current_tank_pos       = { 0.0f, 0.0f };
    float           current_tank_direction = 0.0f;
    float           current_head_direction = 0.0f;
    float           speed                  = 0.01f;
    float           speed_diagonal         = 0.0071f;
};