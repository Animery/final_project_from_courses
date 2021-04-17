#include "nest.hpp"
#include "../score.hpp"

#include "../../engine/debug_level.hpp"
#include "../../engine/engine.hpp"

#include <cmath>

namespace enemy
{

nest::nest(my_engine::vec2       pos,
           my_engine::RenderObj* temp_obj,
           Animate::Texture*     temp_tex,
           my_game::game_impl*   temp_game)
{
    m_game     = temp_game;
    obj_corpse = temp_obj;
    tex_corpse = temp_tex;

    spawn_timer.setCallback([this]() {
        float rand_factor = ((rand() % 21) / 1000.f) - 0.01f;
        m_game->add_spider(
            { current_pos.x + rand_factor,
              (current_pos.y * gameConst::aspect) + rand_factor });
        spawn_used = false;
    });

    current_pos = pos;
    current_pos.y /= gameConst::aspect;
#ifdef DEBUG_LEVEL
    std::cout << "+++ ctor nest" << std::endl;
#endif
}

nest::~nest()
{
    gameInfo::score::getInstance().value(100);
#ifdef DEBUG_LEVEL
    std::cout << "--- destor nest" << std::endl;
#endif
}

my_engine::vec2 nest::getPosition_A()
{
    my_engine::vec2 result = { current_pos.x - half_size,
                               current_pos.y - half_size };
    return result;
}

my_engine::vec2 nest::getPosition_B()
{
    my_engine::vec2 result = { current_pos.x + half_size,
                               current_pos.y + half_size };
    return result;
}

float nest::getHealth()
{
    return health;
}

void nest::setHealth(float damage)
{
    health -= damage;
}

void nest::update(const float                          delta,
                  Player*                              t_player,
                  std::deque<std::unique_ptr<iEnemy>>& enemy_list)
{
    std::ignore = enemy_list;
    std::ignore = t_player;

    if (spawn_used)
    {
        spawn_timer.update_timer(delta);
    }
    else
    {
        spawn_timer.start(spawn_speed);
        spawn_used = true;
    }

    // update_direction(delta, t_player->getCurrent_current_pos());

    // my_engine::matrix2x3 rot =
    //     my_engine::matrix2x3::rotation(current_tank_direction);

    // current_tank_pos.x += (delta * speed * rot.col1.x);
    // current_tank_pos.y += -(delta * speed * rot.col1.y);

    my_engine::matrix2x3 move = my_engine::matrix2x3::move(current_pos);
    matrix_corpse = move * gameConst::aspect_mat * gameConst::size_mat;

    // if (melee_ready)
    // {
    //     if (check_collison_player(t_player->getPosition_A(),
    //                               t_player->getPosition_B()))
    //     {
    //         t_player->setHealth(melee_dmg);
    //         melee_ready = false;
    //         melee_timer.start(melee_speed);
    //     }
    // }
    // else
    // {
    //     melee_timer.update_timer(delta);
    // }
}

void nest::render_enemy()
{
    my_engine::render(*obj_corpse, *tex_corpse, matrix_corpse);
}

// void nest::update_direction(const float            delta,
//                               const my_engine::vec2& player_pos)
// {
//     my_engine::vec2 temp_pos_tank = current_tank_pos;
//     my_engine::vec2 temp          = player_pos - temp_pos_tank;

//     float need_direction;
//     need_direction = std::atan2(temp.y, temp.x) + gameConst::half_pi;

//     need_direction = -need_direction + gameConst::half_pi;
//     current_tank_direction += gameConst::half_pi;

//     float delta_rotation = need_direction - current_tank_direction;
//     delta_rotation       = -delta_rotation;

//     if (delta_rotation >= 0)
//     {
//         if (delta_rotation < delta * speed_rotation ||
//             delta_rotation >= gameConst::double_pi - 0.01f)
//         {
//             current_tank_direction = need_direction;
//         }
//         else if (delta_rotation <= gameConst::pi)
//         {
//             current_tank_direction -= delta * speed_rotation;
//         }
//         else
//         {
//             current_tank_direction += delta * speed_rotation;
//         }
//     }
//     else if (delta_rotation < 0)
//     {
//         if (-delta_rotation < delta * speed_rotation ||
//             -delta_rotation >= gameConst::double_pi - 0.01f)
//         {
//             current_tank_direction = need_direction;
//         }
//         else if (-delta_rotation <= gameConst::pi)
//         {
//             current_tank_direction += delta * speed_rotation;
//         }
//         else
//         {
//             current_tank_direction -= delta * speed_rotation;
//         }
//     }

//     current_tank_direction -= gameConst::half_pi;
//     // need_direction -= gameConst::half_pi;
//     // std::cout << "need_head_direction : " << need_direction << std::endl;
//     // std::cout << "current_head_direction : " << current_tank_direction
//     //           << std::endl;
//     // std::cout << "delta_rotation : " << delta_rotation << std::endl;
//     // std::cout << "####################" << std::endl;
// }

// bool nest::check_collison_player(const my_engine::vec2& pos_player_A,
//                                    const my_engine::vec2& pos_player_B)
// {
//     my_engine::vec2 pos_monster_A = getPosition_A();
//     my_engine::vec2 pos_monster_B = getPosition_B();

//     return my_engine::vec2::check_AABB(
//         pos_monster_A, pos_monster_B, pos_player_A, pos_player_B);
// }
} // namespace enemy