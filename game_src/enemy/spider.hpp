#pragma once

#include "../GameConst.hpp"
#include "iEnemy.hpp"

class Texture;
namespace my_engine
{
class RenderObj;
    
} // namespace my_engine


namespace enemy
{

class spider : public iEnemy
{
public:
    spider(my_engine::vec2       pos,
          my_engine::RenderObj* temp_obj,
          Texture*              temp_tex);
    ~spider();


    void update(const float                          delta,
                                   const my_engine::vec2&               player_pos,
                                   std::deque<std::unique_ptr<iEnemy>>& enemy_list) override;
    void render_enemy() override;

    my_engine::vec2 getPosition_A() override;
    my_engine::vec2 getPosition_B() override;

    float getHealth() override;
    void  setHealth(float damage) override;

private:
    void update_direction(const float delta, const my_engine::vec2& player_pos);

    my_engine::RenderObj* obj_corpse = nullptr;
    Texture*              tex_corpse = nullptr;

    my_engine::matrix2x3 matrix_corpse;

    my_engine::vec2 current_tank_pos       = { 0.0f, 0.0f };
    float           half_size              = 0.015f;
    float           current_tank_direction = 0.0f;
    // float           current_head_direction = 0.0f;

    float health         = 20;
    float speed          = 0.00031250f / 3.5f;
    float speed_diagonal = 0.00022097f / 3.5f;
    float speed_rotation = 0.0015f / 2.5f;
};
} // namespace enemy