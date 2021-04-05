#include "Enemy.hpp"

#include <cmath>

Enemy::Enemy(my_engine::vec2 pos)
{
    current_tank_pos = pos;
    half_size        = 0.015f;
    // current_tank_direction = 0.0f;
    current_tank_direction = rand() % 31415*2 / 10000.f - gameConst::half_pi -0.0001f;
    std::cout << "current_head_direction : " << current_tank_direction << std::endl;
    // current_head_direction = 0.0f;
    health         = 20;
    speed          = 0.00031250f / 4.f;
    speed_diagonal = 0.00022097f / 4.f;
    speed_rotation = 0.0015f / 3.f;

    current_tank_pos.y /= gameConst::aspect;
#ifdef DEBUG_LEVEL
    std::cout << "+++ ctor Enemy" << std::endl;
#endif
}

Enemy::~Enemy()
{
#ifdef DEBUG_LEVEL
    std::cout << "--- destor Enemy" << std::endl;
#endif
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

void Enemy::update(const float delta, const my_engine::vec2& player_pos)
{
    update_direction(delta, player_pos);

    my_engine::matrix2x3 rot =
        my_engine::matrix2x3::rotation(current_tank_direction);

    current_tank_pos.x += (delta * speed * rot.col1.x);
    current_tank_pos.y += -(delta * speed * rot.col1.y);

    my_engine::matrix2x3 move = my_engine::matrix2x3::move(current_tank_pos);

    matrix_corpse = rot * move * gameConst::aspect_mat * gameConst::size_mat;

    my_engine::matrix2x3 rot_head =
        my_engine::matrix2x3::rotation(current_head_direction);

    matrix_head = rot_head * move * gameConst::aspect_mat * gameConst::size_mat;

    // update_direction(delta, player_pos);
}

void Enemy::update_direction(const float            delta,
                             const my_engine::vec2& player_pos)
{
    my_engine::vec2 temp_pos_tank = current_tank_pos;
    my_engine::vec2 temp          = player_pos - temp_pos_tank;

    float need_direction;
    need_direction = std::atan2(temp.y, temp.x) + gameConst::half_pi;

    need_direction = -need_direction + gameConst::half_pi;
    current_tank_direction += gameConst::half_pi;

    float delta_rotation = need_direction - current_tank_direction;
    delta_rotation       = -delta_rotation;

    if (delta_rotation >= 0)
    {
        if (delta_rotation < delta * speed_rotation ||
            delta_rotation >= gameConst::double_pi - 0.01f)
        {
            current_tank_direction = need_direction;
        }
        else if (delta_rotation <= gameConst::pi)
        {
            current_tank_direction -= delta * speed_rotation;
        }
        else
        {
            current_tank_direction += delta * speed_rotation;
        }
    }
    else if (delta_rotation < 0)
    {
        if (-delta_rotation < delta * speed_rotation ||
            -delta_rotation >= gameConst::double_pi - 0.01f)
        {
            current_tank_direction = need_direction;
        }
        else if (-delta_rotation <= gameConst::pi)
        {
            current_tank_direction += delta * speed_rotation;
        }
        else
        {
            current_tank_direction -= delta * speed_rotation;
        }
    }

    current_tank_direction -= gameConst::half_pi;
    // need_direction -= gameConst::half_pi;
    // std::cout << "need_head_direction : " << need_direction << std::endl;
    // std::cout << "current_head_direction : " << current_tank_direction
    //           << std::endl;
    // std::cout << "delta_rotation : " << delta_rotation << std::endl;
    // std::cout << "####################" << std::endl;
}