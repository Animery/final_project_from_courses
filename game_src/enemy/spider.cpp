#include "spider.hpp"
#include "../score.hpp"

#include "../../engine/engine.hpp"
#include "../../engine/debug_level.hpp"

#include <cmath>

namespace enemy
{

spider::spider(my_engine::vec2       pos,
               my_engine::RenderObj* temp_obj,
               Animate::Texture*              temp_tex)
{
    obj_corpse = temp_obj;
    tex_corpse = temp_tex;

    melee_timer.setCallback([this]() { melee_ready = true; });

    current_tank_pos = pos;
    half_size        = 0.015f;
    current_tank_direction =
        rand() % 31415 * 2 / 10000.f - gameConst::half_pi - 0.0001f;

    // status
    health         = 20;
    speed          = 0.00031250f / 4.5f;
    speed_diagonal = 0.00022097f / 4.5f;
    speed_rotation = 0.0015f / 2.5f;
    // status

    current_tank_pos.y /= gameConst::aspect;
#ifdef DEBUG_LEVEL
    std::cout << "+++ ctor spider" << std::endl;
#endif
}

spider::~spider()
{
    gameInfo::score::getInstance().value(2);
#ifdef DEBUG_LEVEL
    std::cout << "--- destor spider" << std::endl;
#endif
}

my_engine::vec2 spider::getPosition_A()
{
    my_engine::vec2 result = { current_tank_pos.x - half_size,
                               current_tank_pos.y - half_size };
    return result;
}

my_engine::vec2 spider::getPosition_B()
{
    my_engine::vec2 result = { current_tank_pos.x + half_size,
                               current_tank_pos.y + half_size };
    return result;
}

float spider::getHealth()
{
    return health;
}

void spider::setHealth(float damage)
{
    health -= damage;
}

void spider::update(const float                          delta,
                    Player*                              t_player,
                    std::deque<std::unique_ptr<iEnemy>>& enemy_list)
{
    std::ignore = enemy_list;
    update_direction(delta, t_player->getCurrent_current_pos());

    my_engine::matrix2x3 rot =
        my_engine::matrix2x3::rotation(current_tank_direction);

    current_tank_pos.x += (delta * speed * rot.col1.x);
    current_tank_pos.y += -(delta * speed * rot.col1.y);

    my_engine::matrix2x3 move = my_engine::matrix2x3::move(current_tank_pos);
    matrix_corpse = rot * move * gameConst::aspect_mat * gameConst::size_mat;

    if (melee_ready)
    {
        if (check_collison_player(t_player->getPosition_A(),
                                  t_player->getPosition_B()))
        {
            t_player->setHealth(melee_dmg);
            melee_ready = false;
            melee_timer.start(melee_speed);
        }
    }
    else
    {
        melee_timer.update_timer(delta);
    }
}

void spider::render_enemy()
{
    my_engine::render(*obj_corpse, *tex_corpse, matrix_corpse);
}

void spider::update_direction(const float            delta,
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

bool spider::check_collison_player(const my_engine::vec2& pos_player_A,
                                   const my_engine::vec2& pos_player_B)
{
    my_engine::vec2 pos_monster_A = getPosition_A();
    my_engine::vec2 pos_monster_B = getPosition_B();

    return my_engine::vec2::check_AABB(
        pos_monster_A, pos_monster_B, pos_player_A, pos_player_B);
}
} // namespace enemy