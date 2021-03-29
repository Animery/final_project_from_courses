#include "Player.hpp"
#include "../include/RenderObj_impl.hpp"

#include <iostream>

Player::Player()
{

    // // Corpse
    // corpse = my_engine::create_RenderObj();
    // corpse->setProg(gfx_prog);
    // corpse->load_mesh_from_file("res/tank.txt");
    // // Corpse

    // Head
    // head = my_engine::create_RenderObj();
    // head->setProg(gfx_prog);
    // head->load_mesh_from_file("res/tank.txt");

    // std::cout << "corpse_prt\t" << &corpse << std::endl;
    // std::cout << "head_prt\t" << &head << std::endl;

    std::cout << "+++ ctor Player" << std::endl;
}

Player::~Player()
{
    // my_engine::destroy_RenderObj(corpse);
    // my_engine::destroy_RenderObj(head);

    std::cout << "--- destor Player" << std::endl;
}

void Player::update(std::array<bool, 8>& controls)
{
    if (controls[static_cast<unsigned>(my_engine::keys_type::left)] &&
        controls[static_cast<unsigned>(my_engine::keys_type::up)])
    {
        if (current_tank_pos.x > -(1 / size - (0.15 / 2)))
        {
            current_tank_pos.x -= speed_diagonal;
        }
        if (current_tank_pos.y < (1 / aspect - (0.15 / 2)) / size)
        {
            current_tank_pos.y += speed_diagonal;
        }

        current_tank_direction = 3 * (pi / 4.f);
    }
    else if (controls[static_cast<unsigned>(my_engine::keys_type::left)] &&
             controls[static_cast<unsigned>(my_engine::keys_type::down)])
    {
        if (current_tank_pos.x > -(1 / size - (0.15 / 2)))
        {
            current_tank_pos.x -= speed_diagonal;
        }
        if (current_tank_pos.y > -(1 / aspect - (0.15 / 2)) / size)
        {
            current_tank_pos.y -= speed_diagonal;
        }
        current_tank_direction = pi / 4.f;
    }
    else if (controls[static_cast<unsigned>(my_engine::keys_type::right)] &&
             controls[static_cast<unsigned>(my_engine::keys_type::up)])
    {
        if (current_tank_pos.x < (1 / size - (0.15 / 2)))
        {
            current_tank_pos.x += speed_diagonal;
        }
        if (current_tank_pos.y < (1 / aspect - (0.15 / 2)) / size)
        {
            current_tank_pos.y += speed_diagonal;
        }
        current_tank_direction = -3 * (pi / 4.f);
    }
    else if (controls[static_cast<unsigned>(my_engine::keys_type::right)] &&
             controls[static_cast<unsigned>(my_engine::keys_type::down)])
    {
        if (current_tank_pos.x < (1 / size - (0.15 / 2)))
        {
            current_tank_pos.x += speed_diagonal;
        }
        if (current_tank_pos.y > -(1 / aspect - (0.15 / 2)) / size)
        {
            current_tank_pos.y -= speed_diagonal;
        }
        current_tank_direction = -pi / 4.f;
    }

    // Simple
    else if (controls[static_cast<unsigned>(my_engine::keys_type::left)])
    {
        if (current_tank_pos.x > -(1 / size - (0.15 / 2)))
        {
            current_tank_pos.x -= speed;
        }

        current_tank_direction = pi / 2.f;
    }
    else if (controls[static_cast<unsigned>(my_engine::keys_type::right)])
    {
        if (current_tank_pos.x < (1 / size - (0.15 / 2)))
        {
            current_tank_pos.x += speed;
        }
        current_tank_direction = -pi / 2.f;
    }
    else if (controls[static_cast<unsigned>(my_engine::keys_type::up)])
    {
        if (current_tank_pos.y < (1 / aspect - (0.15 / 2)) / size)
        {
            current_tank_pos.y += speed;
        }
        current_tank_direction = pi;
    }
    else if (controls[static_cast<unsigned>(my_engine::keys_type::down)])
    {
        if (current_tank_pos.y > -(1 / aspect - (0.15 / 2)) / size)
        {
            current_tank_pos.y -= speed;
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

    // std::cout << current_tank_pos.x << "\t" << current_tank_pos.y << std::endl;

    my_engine::matrix2x3 move = my_engine::matrix2x3::move(current_tank_pos);
    my_engine::matrix2x3 rot =
        my_engine::matrix2x3::rotation(current_tank_direction);

    matrix_corpse = rot * move * aspect_mat * size_mat;

    my_engine::matrix2x3 rot_head =
        my_engine::matrix2x3::rotation(current_head_direction);

    matrix_head = rot_head * move * aspect_mat * size_mat;
}

// my_engine::RenderObj* Player::getCorpse()
// {
//     return corpse;
// }

// my_engine::RenderObj* Player::getHead()
// {
//     return head;
// }

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

my_engine::vec2& Player::getCurrent_tank_pos()
{
    return current_tank_pos;
}

void Player::loadRenderObj() {}