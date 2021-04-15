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
    wave_1(my_game::game_impl* temp_game, Player* t_player);
    ~wave_1();
    void update_wave(const float delta) final;
    void start() final;

protected:
private:
    my_game::game_impl* wave_game;
    Player*             wave_player;

    Timer   m_timer;
    uint8_t current_timer = 0;
    uint8_t count_timer   = 5;
    const uint16_t step = 100;
    size_t  step_lvl = step;
    // const uint8_t count_for_big_spider = 10;
    float timer_length = 500.f;

    bool readyTimer = true;
};
} // namespace spawn