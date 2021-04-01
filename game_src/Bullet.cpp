#include "Bullet.hpp"
#include <iostream>

Bullet::Bullet(my_engine::vec2& temp_position,
               float            temp_direction,
               float            temp_speed)
    : position(temp_position)
    , direction(temp_direction)
    , speed(temp_speed)
{
    std::cout << "+++ ctor Bullet" << std::endl;
}

Bullet::~Bullet()
{
    std::cout << "--- destor Bullet" << std::endl;
}

void Bullet::update()
{
    my_engine::matrix2x3 rotation_matrix =
        my_engine::matrix2x3::rotation(direction);

    position.x += (speed * rotation_matrix.col1.x);
    position.y += -(speed * rotation_matrix.col1.y);

    my_engine::matrix2x3 move = my_engine::matrix2x3::move(position);

    matrix =
        rotation_matrix * move * gameConst::aspect_mat * gameConst::size_mat;
}

my_engine::matrix2x3& Bullet::getMatrix()
{
    return matrix;
}

my_engine::vec2& Bullet::getPosition()
{
    return position;
}