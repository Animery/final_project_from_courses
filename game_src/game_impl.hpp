#pragma once

#include "../include/engine.hpp"
#include "Gun.hpp"
#include "GunSimple.hpp"

#include "Enemy.hpp"
#include "Player.hpp"
#include "spawn_levels/spawn_enemy.hpp"
// #include "Bullet.hpp"

#include <memory>
// #include <chrono>

namespace my_game
{

class game_impl : public my_engine::game
{
public:
    game_impl(my_engine::engine* _engine);
    ~game_impl();
    virtual bool getIsRunning() final;
    void         on_initialize() final;
    void         on_event(my_engine::event&) final;
    void         on_update(std::chrono::microseconds frame_delta) final;
    // void on_render(my_engine::RenderObj&, Texture&) const final;
    void on_render() final;
    void add_enemy(my_engine::vec2 pos_enemy);

private:
    bool                 isRunning;
    my_engine::engine*   engine;
    my_engine::gfx_prog* gfx01;

    std::unique_ptr<Player> player;
    std::unique_ptr<Gun>    gun_current;

#if defined(TEST_VECTOR)
    std::vector<Enemy*> enemy_list;
#else
    std::list<Enemy*> enemy_list;
#endif // TEST_VECTOR

    std::unique_ptr<Texture> texture_corpse;
    std::unique_ptr<Texture> texture_head;
    std::unique_ptr<Texture> texture_bullet;
    std::unique_ptr<Texture> texture_spider;

    my_engine::RenderObj* tank_obj   = nullptr;
    my_engine::RenderObj* bullet_obj = nullptr;
    my_engine::RenderObj* enemy_1   = nullptr;

    std::array<bool, 8> controls{ false };

    std::unique_ptr<spawn::spawn_enemy> spawn_monster;
    size_t max_enemy = 10;
    // my_engine::matrix2x3 matrix;
    // my_engine::matrix2x3 aspect = my_engine::matrix2x3::scale(1, 640.f /
    // 480.f); my_engine::vec2      current_tank_pos       = { 0.f, 0.f }; float
    // current_tank_direction = 0.f;
};

} // namespace my_game