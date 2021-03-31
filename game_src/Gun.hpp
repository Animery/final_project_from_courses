#pragma once

#include "Bullet.hpp"
#include "Timer.hpp"
#include <list>

class Gun
{
public:
    virtual ~Gun();
    virtual void                shoot(my_engine::vec2& temp_position,
                                      float            temp_direction) = 0;
    virtual void                update_gun()                = 0;
    virtual unsigned int        count_bullets()             = 0;
    virtual std::list<Bullet*>* getList_bullets()           = 0;

protected:
    virtual void update_bullets() = 0;

private:
};