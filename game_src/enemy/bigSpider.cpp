#include "bigSpider.hpp"
#include "../score.hpp"

#include "../../engine/engine.hpp"
#include "../../engine/debug_level.hpp"

#include <cmath>

namespace enemy
{

// bigSpider::bigSpider(my_engine::vec2       pos,
//                      my_engine::RenderObj* t_obj,
//                      Animate::Texture*              t_tex,
//                      Animate::Texture*              t_tex_bullet,
//                      my_engine::RenderObj* t_bul_obj)
// {
//     obj_corpse = t_obj;
//     tex_corpse = t_tex;
//     obj_bullet = t_bul_obj;
//     tex_bullet = t_tex_bullet;

//     timer_to_shoot.setCallback([this]() {
//         shoot();
//         timer_use = false;
//     });

//     melee_timer.setCallback([this]() { melee_ready = true; });

//     current_tank_pos = pos;
//     half_size        = 0.025f;
//     current_tank_direction =
//         rand() % 31415 * 2 / 10000.f - gameConst::half_pi - 0.0001f;

//     // status
//     health         = 200;
//     speed          = 0.00031250f / 6.f;
//     speed_diagonal = 0.00022097f / 6.f;
//     speed_rotation = 0.0015f / 2.5f;
//     // status

//     current_tank_pos.y /= gameConst::aspect;
// #ifdef DEBUG_LEVEL
//     std::cout << "+++ ctor bigSpider" << std::endl;
// #endif
// }
bigSpider::bigSpider(my_engine::vec2       pos,
                     my_engine::RenderObj* t_obj,
                     Animate::sprite*              t_sprite,
                     Animate::Texture*              t_tex_bullet,
                     my_engine::RenderObj* t_bul_obj)
{
    obj_corpse = t_obj;
    sprite_corpse = t_sprite;
    obj_bullet = t_bul_obj;
    tex_bullet = t_tex_bullet;

    timer_to_shoot.setCallback([this]() {
        shoot();
        timer_use = false;
    });

    melee_timer.setCallback([this]() { melee_ready = true; });

    current_tank_pos = pos;
    half_size        = 0.025f;
    current_tank_direction =
        rand() % 31415 * 2 / 10000.f - gameConst::half_pi - 0.0001f;

    // status
    health         = 200;
    speed          = 0.00031250f / 6.f;
    speed_diagonal = 0.00022097f / 6.f;
    speed_rotation = 0.0015f / 2.5f;
    // status

    current_tank_pos.y /= gameConst::aspect;
#ifdef DEBUG_LEVEL
    std::cout << "+++ ctor bigSpider" << std::endl;
#endif
}

bigSpider::~bigSpider()
{
    gameInfo::score::getInstance().value(2);
#ifdef DEBUG_LEVEL
    std::cout << "--- destor bigSpider" << std::endl;
#endif
}

my_engine::vec2 bigSpider::getPosition_A()
{
    my_engine::vec2 result = { current_tank_pos.x - half_size,
                               current_tank_pos.y - half_size };
    return result;
}

my_engine::vec2 bigSpider::getPosition_B()
{
    my_engine::vec2 result = { current_tank_pos.x + half_size,
                               current_tank_pos.y + half_size };
    return result;
}

float bigSpider::getHealth()
{
    return health;
}

void bigSpider::setHealth(float damage)
{
    health -= damage;
}

void bigSpider::update(const float                          delta,
                       Player*                              t_player,
                       std::deque<std::unique_ptr<iEnemy>>& enemy_list)
{
    sprite_corpse->update_sprite(delta);

    update_direction(delta, t_player->getCurrent_current_pos());

    if (timer_use)
    {
        timer_to_shoot.update_timer(delta);
    }
    else
    {
        timer_to_shoot.start(speed_shoot);
        timer_use = true;
    }

    update_bullets(delta, enemy_list, t_player);
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

void bigSpider::render_enemy()
{
    // my_engine::render(*obj_corpse, *tex_corpse, matrix_corpse);
    my_engine::render(*obj_corpse, *(sprite_corpse->getCurrent_tex()), matrix_corpse);
    for (auto&& bullet : bullets)
    {
        my_engine::render(*obj_bullet, *tex_bullet, bullet->getMatrix());
    }
}

void bigSpider::update_direction(const float            delta,
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
}

void bigSpider::shoot()
{
    my_engine::vec2 pos_to_shoot = {
        current_tank_pos.x + half_size * 1.5f * matrix_corpse.col1.x,
        current_tank_pos.y - half_size * 1.5f * matrix_corpse.col1.y
    };
    bullets.push_back(std::make_unique<Bullet>(
        pos_to_shoot, current_tank_direction, speed_bullet, damage_bullet));
}

bool bigSpider::check_collision(Bullet*                              bullet,
                                std::deque<std::unique_ptr<iEnemy>>& enemy_list,
                                Player*                              t_player)
{
    my_engine::vec2 pos_bullet_A = bullet->getPosition_A();
    my_engine::vec2 pos_bullet_B = bullet->getPosition_B();

    my_engine::vec2 pos_player_A = t_player->getPosition_A();
    my_engine::vec2 pos_player_B = t_player->getPosition_B();

    for (auto enemy = enemy_list.begin(); enemy != enemy_list.end();)
    {
        my_engine::vec2 pos_enemy_A = (*enemy)->getPosition_A();
        my_engine::vec2 pos_enemy_B = (*enemy)->getPosition_B();
        if (my_engine::vec2::check_AABB(
                pos_bullet_A, pos_bullet_B, pos_enemy_A, pos_enemy_B))
        {
            (*enemy)->setHealth(bullet->getDamage());

#ifdef DEBUG_LEVEL
            std::cout << "hit" << std::endl;
            std::cout << "health enemy:\t" << (*enemy)->getHealth()
                      << std::endl;
#endif
            // TODO fix
            // if ((*enemy)->getHealth() < 0)
            // {
            //     enemy_list.erase(enemy);
            // }
            return true;
        }
        else
        {
            enemy++;
        }
    }
    if (my_engine::vec2::check_AABB(
            pos_bullet_A, pos_bullet_B, pos_player_A, pos_player_B))
    {
        t_player->setHealth(bullet->getDamage());
        return true;
    }
    return false;
}

bool bigSpider::check_collison_player(const my_engine::vec2& pos_player_A,
                                      const my_engine::vec2& pos_player_B)
{
    my_engine::vec2 pos_monster_A = getPosition_A();
    my_engine::vec2 pos_monster_B = getPosition_B();

    return my_engine::vec2::check_AABB(
        pos_monster_A, pos_monster_B, pos_player_A, pos_player_B);
}

bool bigSpider::out_screen(const Bullet* bullet)
{
    return bullet->getPosition().x > (1 / gameConst::size) ||
           bullet->getPosition().x < -(1 / gameConst::size) ||
           bullet->getPosition().y >
               (1 / gameConst::aspect) / gameConst::size ||
           bullet->getPosition().y < -(1 / gameConst::aspect) / gameConst::size;
}

void bigSpider::update_bullets(float                                delta,
                               std::deque<std::unique_ptr<iEnemy>>& enemy_list,
                               Player*                              t_player)
{
    bullets.erase(std::remove_if(bullets.begin(),
                                 bullets.end(),
                                 [&enemy_list, delta, this, t_player](
                                     std::unique_ptr<Bullet>& elem) {
                                     (*elem).update_bullet(delta);
                                     return check_collision(elem.get(),
                                                            enemy_list,
                                                            t_player) ||
                                            out_screen(elem.get());
                                 }),
                  bullets.end());
}
} // namespace enemy