#pragma once
// #include "Player.hpp"
// #include "../include/engine.hpp"
// #include "../include/matrix.hpp"
#include "GameConst.hpp"
// #include <memory>

class Bullet
{

public:
    Bullet(my_engine::vec2& temp_position,
           float            temp_direction,
           float            temp_speed,
           float            temp_damage);
    ~Bullet();
    void update_bullet(float delta);

    float getDamage();

    my_engine::matrix2x3& getMatrix();
    my_engine::vec2&      getPosition();
    my_engine::vec2       getPosition_A();
    my_engine::vec2       getPosition_B();

private:
    my_engine::vec2      position;
    float                half_size = 0.01;
    my_engine::matrix2x3 matrix;
    float                direction;
    float                speed;
    float                damage;
};