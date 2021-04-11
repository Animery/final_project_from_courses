#pragma once
// #include "Player.hpp"
// #include "../include/engine.hpp"
// #include "../include/matrix.hpp"
#include "../GameConst.hpp"
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

    float getDamage() const;

    const my_engine::matrix2x3& getMatrix() const;
    const my_engine::vec2&      getPosition() const;
    const my_engine::vec2       getPosition_A() const;
    const my_engine::vec2       getPosition_B() const;

private:
    my_engine::vec2      position;
    float                half_size = 0.004;
    my_engine::matrix2x3 matrix;
    float                direction;
    float                speed;
    float                damage;
};