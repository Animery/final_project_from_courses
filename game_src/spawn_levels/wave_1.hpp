#pragma once
#include "../Timer.hpp"
#include "../game_impl.hpp"
#include "spawn_enemy.hpp"

// #include <functional>

namespace spawn
{

class wave_1 : public spawn_enemy
{

public:
    wave_1(my_game::game_impl* temp_game);
    ~wave_1();
    void update_wave(const float delta) final;
    void start() final;

protected:

private:
    my_game::game_impl* game;

    Timer  m_timer;
    size_t current_timer = 0;
    size_t count_timer   = 1;
    float  timer_length  = 100;
    bool   readyTimer    = true;
};
} // namespace spawn