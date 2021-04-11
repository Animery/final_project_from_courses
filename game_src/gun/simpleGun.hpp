#pragma once

// #include "../include/engine.hpp"
#include "../include/engine.hpp"
#include "gun.hpp"

namespace guns
{

class GunSimple : public Gun
{

public:
    GunSimple(Texture* temp_tex_bul, my_engine::RenderObj* temp_bul_obj);
    ~GunSimple();

#if defined(TEST_VECTOR)
    void update_gun(float delta, std::vector<Enemy*>& enemy_list) override;
    std::vector<Bullet*>* getList_bullets();
#else
    void update_gun(float delta, std::deque<Enemy*>& enemy_list) override;
    // std::deque<Bullet*>* getList_bullets();
#endif // TEST_VECTOR

    void shoot(my_engine::vec2& temp_position, float temp_direction) override;
    std::string_view getNameGun() override;
    uint16_t         getMaxClip() const override;
    uint16_t         getCurrentClip() const override;
    unsigned int     count_bullets() override;
    void             render_bullets() override;

protected:
#if defined(TEST_VECTOR)
    bool check_collision(Bullet*              bullet,
                         std::vector<Enemy*>& enemy_list) override;
    void update_bullets(float delta, std::vector<Enemy*>& enemy_list) override;
#else
    bool check_collision(Bullet*             bullet,
                         std::deque<Enemy*>& enemy_list) override;
    bool out_screen(const Bullet* bullet) override;
    void update_bullets(float delta, std::deque<Enemy*>& enemy_list) override;
#endif // TEST_VECTOR

private:
    std::string name = "Rifle";

    Texture*              texture_bullet;
    my_engine::RenderObj* bullet_obj;

#if defined(TEST_VECTOR)
    std::vector<Bullet*> bullets;
#else
    // std::deque<Bullet*> bullets;
    std::deque<std::unique_ptr<Bullet>> bullets;
#endif // TEST_VECTOR

    Timer timer_to_shoot;
    // 1 in X milliseconds
    unsigned int speed_shoot   = 100;
    float        speed_bullet  = 0.001f;
    float        damage_bullet = 10.f;

    Timer          timer_to_clip;
    uint16_t       currentClip  = 30;
    const uint16_t maxClip      = 30;
    float          speed_reload = 2000;

    bool readyGun;

    // Bullet* bullet = nullptr;
};
} // namespace guns