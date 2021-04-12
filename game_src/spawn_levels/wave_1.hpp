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

    Timer               m_timer;
    size_t              current_timer = 0;
    size_t              count_timer   = 1;
    float               timer_length  = 300;

    bool                readyTimer    = true;
};
} // namespace spawn