#pragma once

#include "../Enemy.hpp"
#include "../Timer.hpp"
#include "Bullet.hpp"

#include <list>

namespace guns
{

class Gun
{
public:
    virtual ~Gun();
    virtual void             shoot(my_engine::vec2& temp_position,
                                   float            temp_direction) = 0;
    virtual std::string_view getNameGun()                = 0;
    virtual uint16_t         getMaxClip() const          = 0;
    virtual uint16_t         getCurrentClip() const      = 0;
#if defined(TEST_VECTOR)
    virtual void update_gun(float delta, std::vector<Enemy*>& enemy_list) = 0;
    virtual std::vector<Bullet*>* getList_bullets()                       = 0;
#else
    virtual void update_gun(float delta, std::list<Enemy*>& enemy_list) = 0;
    virtual std::list<Bullet*>* getList_bullets()                       = 0;
#endif // TEST_VECTOR

    virtual unsigned int count_bullets() = 0;

protected:
#if defined(TEST_VECTOR)
    virtual bool check_collision(Bullet*              bullet,
                                 std::vector<Enemy*>& enemy_list) = 0;
    virtual void update_bullets(float                delta,
                                std::vector<Enemy*>& enemy_list)  = 0;
#else
    virtual bool check_collision(Bullet*            bullet,
                                 std::list<Enemy*>& enemy_list)             = 0;
    virtual void update_bullets(float delta, std::list<Enemy*>& enemy_list) = 0;
#endif // TEST_VECTOR

private:
};
} // namespace guns