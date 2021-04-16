#pragma once

#include "../../engine/Timer.hpp"
#include "../GameConst.hpp"
#include "../game_impl.hpp"
#include "iEnemy.hpp"

namespace enemy
{

class nest : public iEnemy
{
public:
    nest(my_engine::vec2       pos,
         my_engine::RenderObj* temp_obj,
         Animate::Texture*     temp_tex,
         my_game::game_impl*   temp_game);
    ~nest();

    void update(const float                          delta,
                Player*                              t_player,
                std::deque<std::unique_ptr<iEnemy>>& enemy_list) override;
    void render_enemy() override;

    my_engine::vec2 getPosition_A() override;
    my_engine::vec2 getPosition_B() override;

    float getHealth() override;
    void  setHealth(float damage) override;

private:
    my_game::game_impl* m_game;
    // void update_direction(const float delta, const my_engine::vec2&
    // player_pos); bool check_collison_player(const my_engine::vec2&
    // pos_player_A,
    //                            const my_engine::vec2& pos_player_B);

    my_engine::RenderObj* obj_corpse = nullptr;
    Animate::Texture*     tex_corpse = nullptr;

    my_engine::matrix2x3 matrix_corpse;

    my_engine::vec2 current_pos = { 0.0f, 0.0f };
    float           half_size        = 0.03f;

    Timer   spawn_timer;
    float   spawn_speed = 1000.f;
    uint8_t spawn_value = 1;
    bool    spawn_used = false;

    float health = 2000;
    // float speed;
    // float speed_diagonal;
    // float speed_rotation;
};
} // namespace enemy