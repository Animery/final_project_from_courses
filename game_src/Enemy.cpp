#include "Enemy.hpp"

Enemy::Enemy(my_engine::vec2 pos)
{
    current_tank_pos       = pos;
    half_size              = 0.07f;
    current_tank_direction = 0.0f;
    current_head_direction = 0.0f;
    health                 = 100;
    speed                  = 0.0003125f;
    speed_diagonal         = 0.00022097f;

    current_tank_pos.y /= gameConst::aspect;
    std::cout << "+++ ctor Enemy" << std::endl;
}

Enemy::~Enemy()
{
    std::cout << "--- destor Enemy" << std::endl;
}

// my_engine::matrix2x3& Enemy::getMatrix_corpse()
// {
//     return matrix_corpse;
// }

// my_engine::matrix2x3& Enemy::getMatrix_head()
// {
//     return matrix_head;
// }

// my_engine::vec2 Enemy::getPosition_A()
// {
//     my_engine::vec2 result = { current_tank_pos.x - half_size,
//                                current_tank_pos.y - half_size };
//     return result;
// }

// my_engine::vec2 Enemy::getPosition_B()
// {
//     my_engine::vec2 result = { current_tank_pos.x + half_size,
//                                current_tank_pos.y + half_size };
//     return result;
// }

// float Enemy::getHealth() {}

// void Enemy::setHealth() {}

void Enemy::update()
{
    my_engine::matrix2x3 move = my_engine::matrix2x3::move(current_tank_pos);
    my_engine::matrix2x3 rot =
        my_engine::matrix2x3::rotation(current_tank_direction);

    matrix_corpse = rot * move * gameConst::aspect_mat * gameConst::size_mat;

    my_engine::matrix2x3 rot_head =
        my_engine::matrix2x3::rotation(current_head_direction);

    matrix_head = rot_head * move * gameConst::aspect_mat * gameConst::size_mat;
}
