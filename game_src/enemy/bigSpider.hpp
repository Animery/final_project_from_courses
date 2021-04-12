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

    void update(
        const float                                 delta,
        const my_engine::vec2&                      player_pos,
        std::deque<std::unique_ptr<enemy::iEnemy>>& enemy_list) override;
    void render_enemy() override;

    my_engine::vec2 getPosition_A() override;
    my_engine::vec2 getPosition_B() override;

    float getHealth() override;
    void  setHealth(float damage) override;

private:
    void update_direction(const float delta, const my_engine::vec2& player_pos);
    void shoot();
    bool check_collision(Bullet*                              bullet,
                         std::deque<std::unique_ptr<iEnemy>>& enemy_list);
    bool out_screen(const Bullet* bullet);
    void update_bullets(float                                delta,
                        std::deque<std::unique_ptr<iEnemy>>& enemy_list);

    Texture*              tex_corpse = nullptr;
    my_engine::RenderObj* obj_corpse = nullptr;
    Texture*              tex_bullet = nullptr;
    my_engine::RenderObj* obj_bullet = nullptr;

    std::deque<std::unique_ptr<Bullet>> bullets;

    Timer timer_to_shoot;
    bool  timer_use = false;
    // 1 in X milliseconds
    unsigned int speed_shoot   = 3000;
    float        speed_bullet  = 0.0003f;
    float        damage_bullet = 10.f;

    my_engine::matrix2x3 matrix_corpse;

    my_engine::vec2 current_tank_pos       = { 0.0f, 0.0f };
    float           half_size              = 0.015f;
    float           current_tank_direction = 0.0f;
    // float           current_head_direction = 0.0f;

    float health         = 200;
    float speed          = 0.00031250f / 4.5f;
    float speed_diagonal = 0.00022097f / 4.5f;
    float speed_rotation = 0.0015f / 2.5f;
};

} // namespace enemy