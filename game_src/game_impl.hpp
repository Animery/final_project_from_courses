#pragma once

#include "../include/engine.hpp"

#include "Enemy.hpp"
#include "Player.hpp"
#include "gun/gun.hpp"
#include "spawn_levels/spawn_enemy.hpp"
// #include "Bullet.hpp"

#include <memory>
// #include <chrono>

namespace my_game
{

class game_impl : public my_engine::game
{
public:
    game_impl();
    ~game_impl();
    virtual bool getIsRunning() final;
    void         on_initialize() final;
    void         on_event(my_engine::event&) final;
    void         on_update(std::chrono::microseconds frame_delta) final;
    // void on_render(my_engine::RenderObj&, Texture&) const final;
    void on_render() final;
    void add_enemy(my_engine::vec2 pos_enemy);

private:
    void update_imGui();
    
    bool isRunning;

    my_engine::gfx_prog* gfx01;

    std::unique_ptr<Player>    player;
    std::unique_ptr<guns::Gun> gun_current;

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
    my_engine::RenderObj* enemy_1    = nullptr;

    std::array<bool, 8> controls{ false };

    std::unique_ptr<spawn::spawn_enemy> spawn_monster;
    size_t                              max_enemy = 20;

    std::vector<my_engine::SoundBuffer*> sounds;
};

} // namespace my_game