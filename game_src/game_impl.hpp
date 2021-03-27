#pragma once

#include "../include/engine.hpp"

#include "player.hpp"

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
    void on_render(my_engine::RenderObj&, Texture&) const final;
    void on_render() const final;

private:
    bool isRunning;
    std::shared_ptr<Player> player;
    my_engine::engine*   engine;
    // my_engine::matrix2x3 matrix;
    // my_engine::matrix2x3 aspect = my_engine::matrix2x3::scale(1, 640.f / 480.f);
    // my_engine::vec2      current_tank_pos       = { 0.f, 0.f };
    // float                current_tank_direction = 0.f;

    
};



// my_engine::game* create_game(
//     std::unique_ptr<my_engine::engine, void (*)(my_engine::engine*)>& engine);
// void destroy_game(my_engine::game* e);
} // namespace my_game