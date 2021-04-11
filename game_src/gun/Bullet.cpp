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
#ifdef DEBUG_LEVEL
    std::cout << "+++ ctor Bullet" << std::endl;
#endif
}

Bullet::~Bullet()
{
#ifdef DEBUG_LEVEL
    std::cout << "--- destor Bullet" << std::endl;
#endif
}

void Bullet::update_bullet(float delta)
{
    my_engine::matrix2x3 rotation_matrix =
        my_engine::matrix2x3::rotation(direction);

    position.x += (delta * speed * rotation_matrix.col1.x);
    position.y += -(delta * speed * rotation_matrix.col1.y);

    my_engine::matrix2x3 move = my_engine::matrix2x3::move(position);

    matrix =
        rotation_matrix * move * gameConst::aspect_mat * gameConst::size_mat;
}

float Bullet::getDamage() const
{
    return damage;
}

const my_engine::matrix2x3& Bullet::getMatrix() const
{
    return matrix;
}

const my_engine::vec2& Bullet::getPosition() const
{
    return position;
}

const my_engine::vec2 Bullet::getPosition_A() const
{
    my_engine::vec2 result = { position.x - half_size, position.y - half_size };
    return result;
}

const my_engine::vec2 Bullet::getPosition_B() const
{
    my_engine::vec2 result = { position.x + half_size, position.y + half_size };
    return result;
}