#pragma once

#include "GameConst.hpp"
#include "Player.hpp"

class Enemy : public Player
{
public:
    void update();
    Enemy(my_engine::vec2 pos);
    ~Enemy();

    // my_engine::matrix2x3& getMatrix_corpse();
    // my_engine::matrix2x3& getMatrix_head();

    // my_engine::vec2 getPosition_A();
    // my_engine::vec2 getPosition_B();

    // float           getHealth();
    // void            setHealth();

private:
    // my_engine::matrix2x3 matrix_corpse;
    // my_engine::matrix2x3 matrix_head;

    // my_engine::vec2 current_tank_pos       = { 0.5f, 0.5f };
    // float           half_size              = 0.07f;
    // float           current_tank_direction = 0.0f;
    // float           current_head_direction = 0.0f;

    // float health         = 100;
    // float speed          = 0.0003125f;
    // float speed_diagonal = 0.00022097f;
};