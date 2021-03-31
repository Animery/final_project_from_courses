#pragma once

// #include "../include/engine.hpp"
#include "Gun.hpp"

class GunSimple : public Gun
{

public:
    GunSimple();
    ~GunSimple();
    void shoot(my_engine::vec2& temp_position, float temp_direction) override;
    void update_gun() override;
    unsigned int        count_bullets() override;
    std::list<Bullet*>* getList_bullets();

protected:
    void update_bullets() override;

private:
    std::list<Bullet*> bullets;

    Timer timer_to_shoot;
    bool  readyGun;

    // Bullet* bullet = nullptr;
};