#pragma once
#include "../GameConst.hpp"
#include <deque>
#include <memory>

namespace enemy
{

class iEnemy
{
public:
    virtual ~iEnemy();
    virtual void            update(const float                          delta,
                                   const my_engine::vec2&               player_pos,
                                   std::deque<std::unique_ptr<iEnemy>>& enemy_list) = 0;
    virtual void            render_enemy()          = 0;
    virtual my_engine::vec2 getPosition_A()         = 0;
    virtual my_engine::vec2 getPosition_B()         = 0;
    virtual float           getHealth()             = 0;
    virtual void            setHealth(float damage) = 0;
};
} // namespace enemy