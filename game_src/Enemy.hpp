#pragma once

#include "../include/engine.hpp"
#include "GameConst.hpp"
// #include "Player.hpp"

class Enemy //: public Player
{
public:
    Enemy(my_engine::vec2       pos,
          my_engine::RenderObj* temp_obj,
          Texture*              temp_tex);
    ~Enemy();

    // my_engine::matrix2x3& getMatrix_corpse();

    void update(const float delta, const my_engine::vec2& player_pos);
    void render_enemy();

    my_engine::vec2 getPosition_A();
    my_engine::vec2 getPosition_B();

    float getHealth();
    void  setHealth(float damage);

private:
    void update_direction(const float delta, const my_engine::vec2& player_pos);

    my_engine::RenderObj* obj_corpse = nullptr;
    Texture*              tex_corpse = nullptr;

    my_engine::matrix2x3 matrix_corpse;

    my_engine::vec2 current_tank_pos       = { 0.0f, 0.0f };
    float           half_size              = 0.015f;
    float           current_tank_direction = 0.0f;
    float           current_head_direction = 0.0f;

    float health         = 20;
    float speed          = 0.00031250f / 3.5f;
    float speed_diagonal = 0.00022097f / 3.5f;
    float speed_rotation = 0.0015f / 2.5f;
};