#pragma once

#include "GameConst.hpp"
#include "Player.hpp"

class Enemy : public Player
{
public:
    void update(const float delta, const my_engine::vec2& player_pos);
    Enemy(my_engine::vec2 pos);
    ~Enemy();

    // my_engine::matrix2x3& getMatrix_corpse();
    // my_engine::matrix2x3& getMatrix_head();

    // my_engine::vec2 getPosition_A();
    // my_engine::vec2 getPosition_B();

    // float           getHealth();
    // void            setHealth();

private:
    void update_direction(const float delta, const my_engine::vec2& player_pos);

    float speed_rotation = 0.0015f;
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