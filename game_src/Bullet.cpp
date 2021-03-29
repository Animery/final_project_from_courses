#include "Bullet.hpp"
#include "Player.hpp"


Bullet::Bullet(my_engine::vec2& temp_position, float temp_direction)
    : position (temp_position),
    direction (temp_direction)
{
    std::cout << "+++ ctor Bullet" << std::endl;
}

Bullet::~Bullet() {
    std::cout << "--- destor Bullet" << std::endl;
}

void Bullet::update() 
{
    my_engine::matrix2x3 rotation_matrix =
        my_engine::matrix2x3::rotation(direction);

    position.x += (speed*rotation_matrix.col1.x);
    position.y += -(speed*rotation_matrix.col1.y);

    my_engine::matrix2x3 move = my_engine::matrix2x3::move(position);


    matrix = rotation_matrix * move * aspect_mat * size_mat;
}

my_engine::matrix2x3& Bullet::getMatrix() 
{
    return matrix;
}

my_engine::vec2& Bullet::getPosition() 
{
    return position;
}