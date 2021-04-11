#pragma once

#include "../include/engine.hpp"
#include "GameConst.hpp"
// #include "../include/matrix.hpp"
// #include <iostream>
// #include <numbers>
#include <memory>

class Player
{
public:
    Player(my_engine::RenderObj*, Texture*, Texture*);
    ~Player();
    void update(std::array<bool, 8>& controls, float delta);

    // my_engine::RenderObj* getCorpse();
    // my_engine::RenderObj* getHead();

    // my_engine::matrix2x3& getMatrix_corpse();
    // my_engine::matrix2x3& getMatrix_head();

    float getCurrent_head_direction();
    void  setCurrent_head_direction(const float a);
    void  setMouse_pos(const my_engine::vec2& a);

    my_engine::vec2& getCurrent_tank_pos();
    my_engine::vec2  getPosition_A();
    my_engine::vec2  getPosition_B();

    float getHealth();
    void  setHealth(float damage);

    void render_player();

protected:
    my_engine::RenderObj* tank_obj       = nullptr;
    Texture*              texture_corpse = nullptr;
    Texture*              texture_head   = nullptr;

    my_engine::matrix2x3 matrix_corpse;
    my_engine::matrix2x3 matrix_head;

    my_engine::vec2 current_tank_pos       = { -1.0f, -0.00000000000000001f };
    float           half_size              = 0.05;
    float           current_tank_direction = 0.0f;
    float           current_head_direction = 0.0f;

    float health         = 100;
    float speed          = 0.0003125f;
    float speed_diagonal = 0.00022097f;

private:
    void            update_Head_dirrection();
    my_engine::vec2 mouse_pos_vec = { 0, 0 };
};