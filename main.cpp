#include "engine/engine.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <thread>

using clock_timer = std::chrono::high_resolution_clock;
using nano_sec    = std::chrono::nanoseconds;
using micro_sec   = std::chrono::microseconds;
using milli_sec   = std::chrono::milliseconds;
using time_point  = std::chrono::time_point<clock_timer, nano_sec>;

int main(int /*argc*/, char* /*argv*/[])
{

    std::unique_ptr<my_engine::game, void (*)(my_engine::game*)> game(
        my_engine::create_game(), my_engine::destroy_game);

    game->on_initialize();

    // Time
    clock_timer timer;
    time_point  start = timer.now();
    time_point  end_last_frame;
    // Time

    while (game->getIsRunning())
    {
        end_last_frame = timer.now();
        micro_sec frame_delta =
            std::chrono::duration_cast<micro_sec>(end_last_frame - start);

        if (frame_delta.count() / 1000.0f < 16.667f) // 1000 % 60 = 16.6 FPS
        {
            std::this_thread::yield(); // too fast, give other apps CPU time
            micro_sec duration(50);
            std::this_thread::sleep_for(duration); // sleep 50 microsecconds
            continue; // wait till more time
        }

        start = timer.now();

        my_engine::event event;
        game->on_event(event);
        game->on_update(frame_delta);


        game->on_render();
        my_engine::swap_buffers();
    }

    my_engine::uninitialize();

    return EXIT_SUCCESS;
}