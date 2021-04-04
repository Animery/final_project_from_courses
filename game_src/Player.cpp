#include "Player.hpp"
#include "../include/RenderObj_impl.hpp"

#include <cmath>
#include <iostream>

Player::Player()
{
    current_tank_pos.y /= gameConst::aspect;
#ifdef DEBUG_LEVEL
    std::cout << "+++ ctor Player" << std::endl;
#endif
}

Player::~Player()
{
#ifdef DEBUG_LEVEL
    std::cout << "--- destor Player" << std::endl;
#endif
}

void Player::update(std::array<bool, 8>& controls, float delta)
{
    update_Head_dirrection();

    if (controls[static_cast<unsigned>(my_engine::keys_type::left)] &&
        controls[static_cast<unsigned>(my_engine::keys_type::up)])
    {
        if (current_tank_pos.x > -(1 / gameConst::size - (0.15 / 2)))
        {
            current_tank_pos.x -= delta * speed_diagonal;
        }
        if (current_tank_pos.y <
            (1 / gameConst::aspect - (0.15 / 2)) / gameConst::size)
        {
            current_tank_pos.y += delta * speed_diagonal;
        }

        current_tank_direction = 3 * (gameConst::pi / 4.f);
    }
    else if (controls[static_cast<unsigned>(my_engine::keys_type::left)] &&
             controls[static_cast<unsigned>(my_engine::keys_type::down)])
    {
        if (current_tank_pos.x > -(1 / gameConst::size - (0.15 / 2)))
        {
            current_tank_pos.x -= delta * speed_diagonal;
        }
        if (current_tank_pos.y >
            -(1 / gameConst::aspect - (0.15 / 2)) / gameConst::size)
        {
            current_tank_pos.y -= delta * speed_diagonal;
        }
        current_tank_direction = gameConst::pi / 4.f;
    }
    else if (controls[static_cast<unsigned>(my_engine::keys_type::right)] &&
             controls[static_cast<unsigned>(my_engine::keys_type::up)])
    {
        if (current_tank_pos.x < (1 / gameConst::size - (0.15 / 2)))
        {
            current_tank_pos.x += delta * speed_diagonal;
        }
        if (current_tank_pos.y <
            (1 / gameConst::aspect - (0.15 / 2)) / gameConst::size)
        {
            current_tank_pos.y += delta * speed_diagonal;
        }
        current_tank_direction = -3 * (gameConst::pi / 4.f);
    }
    else if (controls[static_cast<unsigned>(my_engine::keys_type::right)] &&
             controls[static_cast<unsigned>(my_engine::keys_type::down)])
    {
        if (current_tank_pos.x < (1 / gameConst::size - (0.15 / 2)))
        {
            current_tank_pos.x += delta * speed_diagonal;
        }
        if (current_tank_pos.y >
            -(1 / gameConst::aspect - (0.15 / 2)) / gameConst::size)
        {
            current_tank_pos.y -= delta * speed_diagonal;
        }
        current_tank_direction = -gameConst::pi / 4.f;
    }

    // Simple move
    else if (controls[static_cast<unsigned>(my_engine::keys_type::left)])
    {
        if (current_tank_pos.x > -(1 / gameConst::size - (0.15 / 2)))
        {
            current_tank_pos.x -= delta * speed;
        }

        current_tank_direction = gameConst::pi / 2.f;
    }
    else if (controls[static_cast<unsigned>(my_engine::keys_type::right)])
    {
        if (current_tank_pos.x < (1 / gameConst::size - (0.15 / 2)))
        {
            current_tank_pos.x += delta * speed;
        }
        current_tank_direction = -gameConst::pi / 2.f;
    }
    else if (controls[static_cast<unsigned>(my_engine::keys_type::up)])
    {
        if (current_tank_pos.y <
            (1 / gameConst::aspect - (0.15 / 2)) / gameConst::size)
        {
            current_tank_pos.y += delta * speed;
        }
        current_tank_direction = gameConst::pi;
    }
    else if (controls[static_cast<unsigned>(my_engine::keys_type::down)])
    {
        if (current_tank_pos.y >
            -(1 / gameConst::aspect - (0.15 / 2)) / gameConst::size)
        {
            current_tank_pos.y -= delta * speed;
        }
        current_tank_direction = 0.0f;
    }

    // if (controls[static_cast<unsigned>(my_engine::keys_type::button1)])
    // {
    //     speed          = speed * 1.005f;
    //     speed_diagonal = speed_diagonal * 1.005f;

    //     std::cout << "speed:\t" << speed << "\nspeed_diagonal:\t"
    //               << speed_diagonal << std::endl;
    // }

    // if (controls[static_cast<unsigned>(my_engine::keys_type::button2)])
    // {
    //     speed          = speed / 1.005f;
    //     speed_diagonal = speed_diagonal / 1.005f;

    //     std::cout << "speed:\t" << speed << "\nspeed_diagonal:\t"
    //               << speed_diagonal << std::endl;
    // }

    // std::cout << current_tank_pos.x << "\t" << current_tank_pos.y <<
    // std::endl;

    my_engine::matrix2x3 move = my_engine::matrix2x3::move(current_tank_pos);
    my_engine::matrix2x3 rot =
        my_engine::matrix2x3::rotation(current_tank_direction);

    matrix_corpse = rot * move * gameConst::aspect_mat * gameConst::size_mat;

    my_engine::matrix2x3 rot_head =
        my_engine::matrix2x3::rotation(current_head_direction);

    matrix_head = rot_head * move * gameConst::aspect_mat * gameConst::size_mat;
}

my_engine::matrix2x3& Player::getMatrix_corpse()
{
    return matrix_corpse;
}

my_engine::matrix2x3& Player::getMatrix_head()
{
    return matrix_head;
}

float Player::getCurrent_head_direction()
{
    return current_head_direction;
}

void Player::setCurrent_head_direction(const float a)
{
    current_head_direction = a;
}

void Player::setMouse_pos(const my_engine::vec2& a)
{
    mouse_pos_vec = { a.x / (gameConst::screen_width / 2) - 1,
                      -(a.y / ((gameConst::screen_height / 2)) - 1) };
}

my_engine::vec2& Player::getCurrent_tank_pos()
{
    return current_tank_pos;
}

my_engine::vec2 Player::getPosition_A()
{
    my_engine::vec2 result = { current_tank_pos.x - half_size,
                               current_tank_pos.y - half_size };
    return result;
}

my_engine::vec2 Player::getPosition_B()
{
    my_engine::vec2 result = { current_tank_pos.x + half_size,
                               current_tank_pos.y + half_size };
    return result;
}

float Player::getHealth()
{
    return health;
}

void Player::setHealth(float damage)
{
    health -= damage;
}

void Player::update_Head_dirrection()
{
    my_engine::vec2 temp_pos_tank = current_tank_pos;
    temp_pos_tank.y *= gameConst::aspect;

    my_engine::vec2 temp = mouse_pos_vec - temp_pos_tank;

    temp.y /= gameConst::aspect;

    // std::cout << "current_pos_tank\t" << temp_pos_tank << std::endl;
    // std::cout << "mouse_pos\t" << mouse_pos_vec << std::endl;
    // std::cout << "temp\t" << temp << std::endl;

    float a;
    a = std::atan2(temp.y, temp.x) + gameConst::pi / 2.0f;

    // std::cout << "current_head_direction : " << a << std::endl;
    // std::cout << "####################" << std::endl;

    current_head_direction = -a;
}

// void Player::loadRenderObj() {}