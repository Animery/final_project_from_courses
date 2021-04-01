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
           float            temp_speed);
    ~Bullet();
    void                  update();
    my_engine::matrix2x3& getMatrix();
    my_engine::vec2&      getPosition();

private:
    my_engine::vec2      position;
    my_engine::matrix2x3 matrix;
    float                direction;
    float                speed;
};