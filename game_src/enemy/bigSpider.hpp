#pragma once
#include "../GameConst.hpp"
#include "../Timer.hpp"
#include "../gun/Bullet.hpp"
#include "iEnemy.hpp"

class Texture;
namespace my_engine
{
class RenderObj;

} // namespace my_engine

namespace enemy
{

class bigSpider : public iEnemy
{

public:
    bigSpider(my_engine::vec2       pos,
              my_engine::RenderObj* t_obj,
              Texture*              t_tex,
              Texture*              t_tex_bullet,
              my_engine::RenderObj* t_bul_obj);
    ~bigSpider();

    void update(const float                          delta,
                Player*                              t_player,
                std::deque<std::unique_ptr<iEnemy>>& enemy_list) override;
    void render_enemy() override;

    my_engine::vec2 getPosition_A() override;
    my_engine::vec2 getPosition_B() override;

    float getHealth() override;
    void  setHealth(float damage) override;

private:
    void update_direction(const float delta, const my_engine::vec2& player_pos);
    void shoot();
    bool check_collision(Bullet*                              bullet,
                         std::deque<std::unique_ptr<iEnemy>>& enemy_list,
                         Player*                              t_player);
    bool check_collison_player(const my_engine::vec2& pos_player_A,
                               const my_engine::vec2& pos_player_B);
    bool out_screen(const Bullet* bullet);
    void update_bullets(float                                delta,
                        std::deque<std::unique_ptr<iEnemy>>& enemy_list,
                        Player*                              t_player);

    Texture*              tex_corpse = nullptr;
    my_engine::RenderObj* obj_corpse = nullptr;
    Texture*              tex_bullet = nullptr;
    my_engine::RenderObj* obj_bullet = nullptr;

    my_engine::matrix2x3 matrix_corpse;

    // RANGE
    Timer                               timer_to_shoot;
    bool                                timer_use = false;
    std::deque<std::unique_ptr<Bullet>> bullets;
    // 1 in X milliseconds
    unsigned int speed_shoot   = 3000;
    float        speed_bullet  = 0.0003f;
    float        damage_bullet = 10.f;
    // RANGE

    // MELEE
    float melee_dmg   = 50;
    float melee_speed = 1000;
    Timer melee_timer;
    bool  melee_ready = true;
    // MELEE

    my_engine::vec2 current_tank_pos       = { 0.0f, 0.0f };
    float           half_size              = 0.02f;
    float           current_tank_direction = 0.0f;
    // float           current_head_direction = 0.0f;

    float health;
    float speed;
    float speed_diagonal;
    float speed_rotation;
};

} // namespace enemy