#pragma once

// #include "../include/engine.hpp"
#include "Gun.hpp"


class GunSimple : public Gun
{

public:
    GunSimple();
    ~GunSimple();
    void shoot(my_engine::vec2& temp_position, float temp_direction) override;
    unsigned int count_bullets() override;
    void update_bullets() override;
    std::list<Bullet*>& getList_bullets();
    // void render_bullets();

private:
    std::list<Bullet*> bullets;
    // Bullet* bullet = nullptr;

};