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

    engine->initialize("");

    std::unique_ptr<my_engine::game, void (*)(my_engine::game*)> game(
        my_engine::create_game(engine.get()), my_engine::destroy_game);

    // // Texture
    // std::string tex_name = "s_texture";
    // Texture     texture(tex_name);
    // {
    //     Image image = Image::loadFromFile("res/tank.png");
    //     texture.setImage(image);
    // }
    // // Texture

    // // GFX
    // const std::string path("shader/");
    // const std::string name_shader("tank");
    // std::unique_ptr<my_engine::gfx_prog, void (*)(my_engine::gfx_prog*)> gfx01(
    //     my_engine::create_gfx_prog(path, name_shader),
    //     my_engine::destroy_gfx_prog);

    // gfx01->bind_attrib(0, "a_position");
    // gfx01->bind_attrib(1, "a_tex_coord");
    // gfx01->bind_attrib(2, "a_color");
    // gfx01->link();
    // // GFX

    // // RenderObj
    // std::unique_ptr<my_engine::RenderObj, void (*)(my_engine::RenderObj*)> tank(
    //     my_engine::create_RenderObj(), my_engine::destroy_RenderObj);

    // tank->setProg(gfx01.get());

    // constexpr float size_tank = 0.3f;
    // tank->load_mesh_from_file("res/tank2.txt");

    // // clang-format off
    // tank->addVertexBufferObject({
    //         { -size_tank,  size_tank },
    //         { -size_tank, -size_tank },
    //         {  size_tank,  size_tank },
    //         {  size_tank, -size_tank }});
    // tank->addVertexBufferObject({
    //         { 0, 1 },
    //         { 0, 0 },
    //         { 1, 1 },
    //         { 1, 0 }});
    // tank->addVertexBufferObject({
    //     { 1, 1 , 1},
    //     { 1, 1 , 1},
    //     { 1, 1 , 1},
    //     { 1, 1 , 1}});
    // // clang-format on

    // tank->addIndices({ 0, 1, 2, 2, 1, 3 });
    // vao->linkProg();
    // RenderObj

    // SoundBuffer
    my_engine::SoundBuffer* s =
        engine->create_sound_buffer("res/t2_no_problemo.wav");
    my_engine::SoundBuffer* music =
        engine->create_sound_buffer("res/8-bit_detective.wav");
    assert(music != nullptr);

    music->play(my_engine::SoundBuffer::properties::looped);
    // SoundBuffer

    // bool continue_loop = true;

    // my_engine::vec2 current_tank_pos(0.f, 0.f);
    // float           current_tank_direction(0.f);
    // float           size_tank_qw = 1.0f + size_tank / 2;
    // const float     pi = std::numbers::pi_v<float>;

    // Time
    clock_timer timer;
    time_point  start = timer.now();
    // Time

    while (game->getIsRunning())
    {
        time_point end_last_frame = timer.now();

        my_engine::event event;

        game->on_event(event);

        // while (engine->read_event(event))
        // {
        //     // std::cout << event << std::endl;
        //     switch (event.type)
        //     {
        //         case my_engine::event_type::hardware:
        //         {
        //             if
        //             (std::get<my_engine::hardware_data>(event.info).is_reset)
        //             {
        //                 continue_loop = false;
        //             }
        //             break;
        //         }
        //         case my_engine::event_type::input_key:
        //         {
        //             const auto& key_data =
        //                 std::get<my_engine::input_data>(event.info);
        //             if (key_data.is_down)
        //             {
        //                 if (key_data.key == my_engine::keys_type::select)
        //                 {
        //                     continue_loop = false;
        //                 }

        //                 else if (key_data.key ==
        //                 my_engine::keys_type::button1)
        //                 {
        //                     s->play(my_engine::SoundBuffer::properties::once);
        //                 }
        //                 else if (key_data.key ==
        //                 my_engine::keys_type::button2)
        //                 {
        //                     s->play(my_engine::SoundBuffer::properties::looped);
        //                 }
        //             }
        //             break;
        //         }
        //         default:
        //             break;
        //     }
        // }

        // if (engine->is_key_down(my_engine::keys_type::left))
        // {
        //     current_tank_pos.x -= 0.01f;
        //     if (current_tank_pos.x < -size_tank_qw)
        //     {
        //         current_tank_pos.x = size_tank_qw;
        //     }

        //     current_tank_direction = pi / 2.f;
        // }
        // else if (engine->is_key_down(my_engine::keys_type::right))
        // {
        //     current_tank_pos.x += 0.01f;
        //     if (current_tank_pos.x > size_tank_qw)
        //     {
        //         current_tank_pos.x = -size_tank_qw;
        //     }
        //     current_tank_direction = -pi / 2.f;
        // }
        // else if (engine->is_key_down(my_engine::keys_type::up))
        // {
        //     current_tank_pos.y += 0.01f;
        //     if (current_tank_pos.y > (size_tank_qw - size_tank))
        //     {
        //         current_tank_pos.y = -(size_tank_qw - size_tank);
        //     }
        //     current_tank_direction = -pi;
        // }
        // else if (engine->is_key_down(my_engine::keys_type::down))
        // {
        //     current_tank_pos.y -= 0.01f;
        //     if (current_tank_pos.y < -(size_tank_qw - size_tank))
        //     {
        //         current_tank_pos.y = (size_tank_qw - size_tank);
        //     }
        //     current_tank_direction = 0.0f;
        // }

        game->on_event(event);

        milli_sec frame_delta =
            std::chrono::duration_cast<milli_sec>(end_last_frame - start);

        if (frame_delta.count() < 16.666) // 1000 % 60 = 16.666 FPS
        {
            std::this_thread::yield(); // too fast, give other apps CPU time
            continue;                  // wait till more time
        }

        game->on_update(frame_delta);
        // std::cout << m << std::endl;
        // engine->render(*tank, texture, m);
        // game->on_render(*tank, texture);
        game->on_render();
        // m = m * my_engine::matrix2x3::invert();
        // engine->render(*tank, texture, m);
        // game->on_render(*tank, texture);

        // std::cout << "x: " << current_tank_pos.x
        //           << "\ty: " << current_tank_pos.y << std::endl;

        engine->swap_buffers();
    }

    engine->uninitialize();

    return EXIT_SUCCESS;
}