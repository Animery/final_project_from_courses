// #include "game_src/game_impl.hpp"
// #include "include/Image.hpp"
// #include "include/Texture.hpp"
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
    time_point  start    = timer.now();
    size_t      frame_if = 0;
    // Time

    while (game->getIsRunning())
    {
        
        time_point       end_last_frame = timer.now();
        my_engine::event event;

        game->on_event(event);

        milli_sec frame_delta =
            std::chrono::duration_cast<milli_sec>(end_last_frame - start);

        if (frame_delta.count() < 16.666) // 1000 % 60 = 16.666 FPS
        {
            std::cout << "frame_delta:\t" << frame_delta.count() << "\tframe_if"
                      << frame_if << std::endl;
            frame_if++;
            std::this_thread::yield(); // too fast, give other apps CPU time
            continue;                  // wait till more time
        }
        
       
        game->on_update(frame_delta);
        game->on_render();

        engine->swap_buffers();
    }

    engine->uninitialize();

    return EXIT_SUCCESS;
}