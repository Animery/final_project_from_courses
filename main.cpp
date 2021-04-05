// #include "game_src/game_impl.hpp"
// #include "include/Image.hpp"
// #include "include/Texture.hpp"
#include "game_src/GameConst.hpp"
#include "include/engine.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <numbers>
#include <string_view>
#include <thread>

using clock_timer = std::chrono::high_resolution_clock;
using nano_sec    = std::chrono::nanoseconds;
using micro_sec   = std::chrono::microseconds;
using milli_sec   = std::chrono::milliseconds;
using time_point  = std::chrono::time_point<clock_timer, nano_sec>;

int main(int /*argc*/, char* /*argv*/[])
{

    std::unique_ptr<my_engine::engine, void (*)(my_engine::engine*)> engine(
        my_engine::create_engine(), my_engine::destroy_engine);

    std::unique_ptr<my_engine::game, void (*)(my_engine::game*)> game(
        my_engine::create_game(engine.get()), my_engine::destroy_game);

    game->on_initialize();

    // SoundBuffer
    my_engine::SoundBuffer* s =
        engine->create_sound_buffer("res/t2_no_problemo.wav");
    my_engine::SoundBuffer* music =
        engine->create_sound_buffer("res/8-bit_detective.wav");
    assert(music != nullptr);

    music->play(my_engine::SoundBuffer::properties::looped);
    // SoundBuffer

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

        // if (frame_delta.count() / 1000.0f < 16.667f) // 1000 % 60 = 16.6 FPS
        // {
        //     std::this_thread::yield(); // too fast, give other apps CPU time
        //     micro_sec duration(50);
        //     std::this_thread::sleep_for(duration); // sleep 50 microsecconds
        //     continue; // wait till more time
        // }

        start = timer.now();

        my_engine::event event;
        game->on_event(event);
        game->on_update(frame_delta);


        game->on_render();
        engine->swap_buffers();
    }

    engine->uninitialize();

    return EXIT_SUCCESS;
}