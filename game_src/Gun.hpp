#pragma once

#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Timer.hpp"

#include <list>

class Gun
{
public:
    virtual ~Gun();
    virtual void shoot(my_engine::vec2& temp_position,
                       float            temp_direction)                            = 0;
    virtual void update_gun(float delta, std::list<Enemy*>& enemy_list) = 0;
    virtual unsigned int        count_bullets()                         = 0;
    virtual std::list<Bullet*>* getList_bullets()                       = 0;

protected:
    virtual bool check_collision(Bullet*            bullet,
                                 std::list<Enemy*>& enemy_list)             = 0;
    virtual void update_bullets(float delta, std::list<Enemy*>& enemy_list) = 0;

private:
};