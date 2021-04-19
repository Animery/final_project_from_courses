#include "wave_1.hpp"
#include "../score.hpp"

#include "../../engine/debug_level.hpp"

#if defined(DEBUG_LEVEL)
#include <iostream>
#endif // DEBUG_LEVEL

namespace spawn
{

wave_1::wave_1(my_game::game_impl* t_game, Player* t_player)
{
    wave_game   = t_game;
    wave_player = t_player;

    m_timer.setCallback([this]() {
        float           temp_pos_x;
        float           temp_pos_y;
        my_engine::vec2 pos_player;
        float           max_min = 0.5f;
        do
        {
            temp_pos_x = (rand() % 3001 / 1000.0f) - 1.5f;
            temp_pos_y = (rand() % 3001 / 1000.0f) - 1.5f;
            pos_player = wave_player->getCurrent_current_pos();
        } while (!((temp_pos_x < pos_player.x - max_min ||
                    temp_pos_x > pos_player.x + max_min) &&
                   (temp_pos_y < pos_player.y - max_min ||
                    temp_pos_y > pos_player.y + max_min)));

        if (current_timer < count_timer)
        {
            wave_game->add_spider({ temp_pos_x, temp_pos_y });
            current_timer++;
        }
        else
        {
            wave_game->add_big_spider({ temp_pos_x, temp_pos_y });
            if ((gameInfo::score::getInstance().value()) > step_lvl)
            {
                timer_length -= timer_length * 0.1f;
                step_lvl += step;
            }
            current_timer = 0;
        }

        readyTimer = true;
    });

#if defined(DEBUG_LEVEL)
    std::cout << "--- ctor wave_1" << std::endl;
#endif // DEBUG_LEVEL
}

wave_1::~wave_1()
{
#if defined(DEBUG_LEVEL)
    std::cout << "--- destor wave_1" << std::endl;
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

void wave_1::start_lvl()
{
    for (size_t i = 0; i < 1; i++)
    {
        for (size_t j = 0; j < 500; j++)
        {
            wave_game->add_spider({  0.6f,  0.6f });
            wave_game->add_spider({ -0.6f,  0.6f });
            wave_game->add_spider({  0.6f, -0.6f });
            wave_game->add_spider({ -0.6f, -0.6f });
        }
        
        // clang-format off
        wave_game->add_nest      ({  0.6f,  0.6f });
        wave_game->add_nest      ({ -0.6f,  0.6f });
        wave_game->add_nest      ({  0.6f, -0.6f });
        wave_game->add_nest      ({ -0.6f, -0.6f });
        wave_game->add_big_spider({  0.6f,  0.6f });
        wave_game->add_big_spider({ -0.6f,  0.6f });
        wave_game->add_big_spider({  0.6f, -0.6f });
        wave_game->add_big_spider({ -0.6f, -0.6f });
        // clang-format on
    }
}

} // namespace spawn