#include "Bullet.hpp"
#include <iostream>

Bullet::Bullet(my_engine::vec2& temp_position,
               float            temp_direction,
               float            temp_speed,
               float            temp_damage)
    : position(temp_position)
    , direction(temp_direction)
    , speed(temp_speed)
    , damage(temp_damage)
{
    std::cout << "+++ ctor Bullet" << std::endl;
}

Bullet::~Bullet()
{
    std::cout << "--- destor Bullet" << std::endl;
}

void Bullet::update(float delta)
{
    my_engine::matrix2x3 rotation_matrix =
        my_engine::matrix2x3::rotation(direction);

    position.x += (delta * speed * rotation_matrix.col1.x);
    position.y += -(delta * speed * rotation_matrix.col1.y);

    my_engine::matrix2x3 move = my_engine::matrix2x3::move(position);

    matrix =
        rotation_matrix * move * gameConst::aspect_mat * gameConst::size_mat;
}

float Bullet::getDamage() 
{
    return damage;
}

my_engine::matrix2x3& Bullet::getMatrix()
{
    return matrix;
}

my_engine::vec2& Bullet::getPosition()
{
    return position;
}

my_engine::vec2 Bullet::getPosition_A()
{
    my_engine::vec2 result = { position.x - half_size, position.y - half_size };
    return result;
}

my_engine::vec2 Bullet::getPosition_B()
{
    my_engine::vec2 result = { position.x + half_size, position.y + half_size };
    return result;
}