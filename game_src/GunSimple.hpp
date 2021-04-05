#pragma once

// #include "../include/engine.hpp"
#include "Gun.hpp"

class GunSimple : public Gun
{

public:
    GunSimple();
    ~GunSimple();

    #if defined(TEST_VECTOR)
    void update_gun(float delta, std::vector<Enemy*>& enemy_list) override;
    std::vector<Bullet*>* getList_bullets();
    #else
    void update_gun(float delta, std::list<Enemy*>& enemy_list) override;
    std::list<Bullet*>* getList_bullets();
    #endif // TEST_VECTOR
    
    void shoot(my_engine::vec2& temp_position, float temp_direction) override;
    unsigned int        count_bullets() override;

protected:

#if defined(TEST_VECTOR)
    bool check_collision(Bullet*              bullet,
                         std::vector<Enemy*>& enemy_list) override;
    void update_bullets(float delta, std::vector<Enemy*>& enemy_list) override;
#else
    bool check_collision(Bullet*            bullet,
                         std::list<Enemy*>& enemy_list) override;
    void update_bullets(float delta, std::list<Enemy*>& enemy_list) override;
#endif // TEST_VECTOR

private:
#if defined(TEST_VECTOR)
   std::vector<Bullet*> bullets;
#else
    std::list<Bullet*> bullets;
#endif // TEST_VECTOR

    Timer timer_to_shoot;
    // 1 in X milliseconds
    unsigned int speed_shoot   = 200;
    float        speed_bullet  = 0.001f;
    float        damage_bullet = 10.f;
    bool         readyGun;

    // Bullet* bullet = nullptr;
};