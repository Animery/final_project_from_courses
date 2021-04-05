#include "wave_1.hpp"

#if defined(DEBUG_LEVEL)
#include <iostream>
#endif // DEBUG_LEVEL

namespace spawn
{

wave_1::wave_1(my_game::game_impl* temp_game)
{
    game = temp_game;
    m_timer.setCallback([&]() {
        float temp_pos_x = (rand() % 2000 / 1000.0f) - 1;
        float temp_pos_y = (rand() % 2000 / 1000.0f) - 1;
        game->add_enemy({ temp_pos_x, temp_pos_y });
        readyTimer = true;
    });

#if defined(DEBUG_LEVEL)
    std::cout << "destor spawn_enemy" << std::endl;
#endif // DEBUG_LEVEL
}

wave_1::~wave_1()
{
#if defined(DEBUG_LEVEL)
    std::cout << "destor spawn_enemy" << std::endl;
#endif // DEBUG_LEVEL
}

void wave_1::update_wave(const float delta)
{
    if (readyTimer)
    {
        readyTimer = false;
        m_timer.start(timer_length);
    }
    else
    {
        m_timer.update_timer(delta);
    }
    
   
}

void wave_1::start()
{
    // if (!readyTimer)
    // {
    //     readyTimer = true;
    // }
}

} // namespace spawn