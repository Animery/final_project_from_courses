#pragma once

// #include "../include/engine.hpp"
#include "Gun.hpp"

class GunSimple : public Gun
{

public:
    GunSimple();
    ~GunSimple();
    void shoot(my_engine::vec2& temp_position, float temp_direction) override;
    void update_gun(float delta, std::list<Enemy*>& enemy_list) override;
    unsigned int        count_bullets() override;
    std::list<Bullet*>* getList_bullets();

protected:
    bool check_collision(Bullet*            bullet,
                         std::list<Enemy*>& enemy_list) override;
    void update_bullets(float delta, std::list<Enemy*>& enemy_list) override;

private:
    std::list<Bullet*> bullets;

    Timer timer_to_shoot;
    // 1 in X milliseconds
    unsigned int speed_shoot   = 500;
    float        speed_bullet  = 0.001f;
    float        damage_bullet = 10.f;
    bool         readyGun;

    // Bullet* bullet = nullptr;
};