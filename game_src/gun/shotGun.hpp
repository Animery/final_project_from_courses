#pragma once

#include "../include/engine.hpp"
#include "gun.hpp"

namespace guns
{

class shotGun : public Gun
{

public:
    shotGun(Texture* temp_tex_bul, my_engine::RenderObj* temp_bul_obj);
    ~shotGun();

#if defined(TEST_VECTOR)
    void update_gun(float delta, std::vector<Enemy*>& enemy_list) override;
    std::vector<Bullet*>* getList_bullets();
#else
    void update_gun(float delta, std::deque<std::unique_ptr<enemy::iEnemy>>& enemy_list) override;
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
                         std::deque<std::unique_ptr<enemy::iEnemy>>& enemy_list) override;
    bool out_screen(const Bullet* bullet) override;
    void update_bullets(float delta,std::deque<std::unique_ptr<enemy::iEnemy>>& enemy_list) override;
#endif // TEST_VECTOR

private:
    std::string name = "ShotGun";

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
    unsigned int speed_shoot   = 10;
    float        speed_bullet  = 0.002f;
    float        damage_bullet = 10.f;

    Timer          timer_to_clip;
    uint16_t       currentClip  = 10;
    const uint16_t maxClip      = 10;
    float          speed_reload = 1500;

    bool readyGun;

    // Bullet* bullet = nullptr;
};
} // namespace guns