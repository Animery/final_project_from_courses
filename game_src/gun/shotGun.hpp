#pragma once

#include "gun.hpp"

namespace guns
{

class shotGun : public Gun
{

public:
    shotGun();
    ~shotGun();

#if defined(TEST_VECTOR)
    void update_gun(float delta, std::vector<Enemy*>& enemy_list) override;
    std::vector<Bullet*>* getList_bullets();
#else
    void update_gun(float delta, std::list<Enemy*>& enemy_list) override;
    std::list<Bullet*>* getList_bullets();
#endif // TEST_VECTOR

    void shoot(my_engine::vec2& temp_position, float temp_direction) override;
    std::string_view getNameGun() override;
    uint16_t         getMaxClip() const override;
    uint16_t         getCurrentClip() const override;
    unsigned int     count_bullets() override;

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
    std::string name = "ShotGun";

#if defined(TEST_VECTOR)
    std::vector<Bullet*> bullets;
#else
    std::list<Bullet*> bullets;
#endif // TEST_VECTOR

    Timer timer_to_shoot;
    // 1 in X milliseconds
    unsigned int speed_shoot   = 500;
    float        speed_bullet  = 0.002f;
    float        damage_bullet = 10.f;

    Timer          timer_to_clip;
    uint16_t       currentClip  = 7;
    const uint16_t maxClip      = 7;
    float          speed_reload = 2000;

    bool readyGun;

    // Bullet* bullet = nullptr;
};
} // namespace guns