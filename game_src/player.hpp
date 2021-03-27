#pragma once

#include "../include/engine.hpp"

#include <memory>

class Player
{
public:
    Player();
    ~Player();
    // Texture*              getTexture_corpse();
    // my_engine::RenderObj* getCorpse();
    // Texture*              getTexture_head();
    // my_engine::RenderObj* getHead();

// private:
    void                     loadRenderObj();
    my_engine::RenderObj*    corpse = nullptr;
    std::unique_ptr<Texture> texture_corpse;
    my_engine::gfx_prog*     gfx_corpse = nullptr;

    my_engine::RenderObj*    head;
    std::unique_ptr<Texture> texture_head;
    my_engine::gfx_prog*     gfx_head = nullptr;

    my_engine::matrix2x3 matrix_corpse;
    my_engine::matrix2x3 matrix_head;
    my_engine::matrix2x3 aspect = my_engine::matrix2x3::scale(1, 640.f / 480.f);
    my_engine::vec2      current_tank_pos       = { 0.f, 0.f };
    float                current_tank_direction = 0.f;
    float                current_head_direction = 0.f;
};