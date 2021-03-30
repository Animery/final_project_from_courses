#pragma once

#include "../include/engine.hpp"
#include "Gun.hpp"
#include "GunSimple.hpp"

#include "Player.hpp"
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
    void on_initialize();
    void on_event(my_engine::event&);
    void on_update(std::chrono::milliseconds frame_delta);
    // void on_render(my_engine::RenderObj&, Texture&) const final;
    void on_render() const final;

private:
    bool isRunning;
    my_engine::engine*   engine;
    std::unique_ptr<Player> player;
    std::unique_ptr<Gun> gun_current;
    // Bullet* bullet;
    my_engine::gfx_prog* gfx01;

    std::unique_ptr<Texture> texture_corpse;
    std::unique_ptr<Texture> texture_head;
    std::unique_ptr<Texture> texture_bullet;

    my_engine::RenderObj* tank_obj = nullptr;
    my_engine::RenderObj* bullet_obj = nullptr;
        
    

    
    std::array<bool,8> controls{false};
    // my_engine::matrix2x3 matrix;
    // my_engine::matrix2x3 aspect = my_engine::matrix2x3::scale(1, 640.f / 480.f);
    // my_engine::vec2      current_tank_pos       = { 0.f, 0.f };
    // float                current_tank_direction = 0.f;

    
};

} // namespace my_game