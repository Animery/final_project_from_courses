#include "game_impl.hpp"

#include "enemy/bigSpider.hpp"
#include "enemy/nest.hpp"
#include "enemy/spider.hpp"

#include "gun/miniGun.hpp"
#include "gun/shotGun.hpp"
#include "gun/simpleGun.hpp"
#include "score.hpp"
#include "spawn_levels/wave_1.hpp"

#include <algorithm>
#include <cassert>
#include <ctime>

#include "../imgui_src/imgui.h"
#include "../imgui_src/imgui_impl_opengl3.h"
#include "../imgui_src/imgui_impl_sdl.h"

#include "../engine/debug_level.hpp"

namespace my_game
{

game_impl::game_impl()
{

#if defined(DEBUG_LEVEL)
    std::cout << "+++ ctor game_impl" << std::endl;
#endif // DEBUG_LEVEL
}

game_impl::~game_impl()
{
    // delete engine;
    // for (auto it : enemy_list)
    // {
    //     delete it;
    // }

    for (auto tex : map_texture)
    {
        delete tex;
    }

    delete gfx_obj;
    delete gfx_map;
    delete tank_obj;
    delete bullet_obj;
    delete spider;
    delete big_spider;
    delete fire_ball_obj;
    delete map_obj;

#if defined(DEBUG_LEVEL)
    std::cout << "--- destor game_impl" << std::endl;
#endif // DEBUG_LEVEL
}

bool game_impl::getIsRunning()
{
    return isRunning;
}

void game_impl::on_initialize()
{
    my_engine::window_mode mode{ gameConst::screen_width,
                                 gameConst::screen_height,
                                 false };
    my_engine::initialize("My_Game", mode);

    // gfx_prog
    std::string path("shader/");
    std::string name_shader("corpse");
    gfx_obj = my_engine::create_gfx_prog(path, name_shader);
    gfx_obj->bind_attrib(0, "a_position");
    gfx_obj->bind_attrib(1, "a_tex_coord");
    gfx_obj->bind_attrib(2, "a_color");
    gfx_obj->link();
    //////////////////////////////
    name_shader = "map";
    gfx_map     = my_engine::create_gfx_prog(path, name_shader);
    gfx_map->bind_attrib(0, "a_position");
    gfx_map->bind_attrib(1, "a_tex_coord");
    gfx_map->link();
    // gfx_prog

    // Render_obj
    tank_obj = my_engine::create_RenderObj();
    tank_obj->setProg(gfx_obj);
    tank_obj->load_mesh_from_file("res/tank.txt");

    bullet_obj = my_engine::create_RenderObj();
    bullet_obj->setProg(gfx_obj);
    bullet_obj->load_mesh_from_file("res/bullet.txt");

    spider = my_engine::create_RenderObj();
    spider->setProg(gfx_obj);
    spider->load_mesh_from_file("res/spider.txt");

    big_spider = my_engine::create_RenderObj();
    big_spider->setProg(gfx_obj);
    big_spider->load_mesh_from_file("res/big_spider.txt");

    fire_ball_obj = my_engine::create_RenderObj();
    fire_ball_obj->setProg(gfx_obj);
    fire_ball_obj->load_mesh_from_file("res/fire_ball.txt");

    nest_obj = my_engine::create_RenderObj();
    nest_obj->setProg(gfx_obj);
    nest_obj->load_mesh_from_file("res/nest.txt");

    map_obj = my_engine::create_RenderObj();
    map_obj->setProg(gfx_map);
    map_obj->load_mesh_from_file("res/map.txt");

    // Render_obj

    // SoundBuffer

    sounds.push_back(my_engine::create_sound_buffer("res/8-bit_detective.wav"));
    sounds.push_back(my_engine::create_sound_buffer("res/t2_no_problemo.wav"));
    sounds.push_back(my_engine::create_sound_buffer("res/impact_350ms.wav"));

    for (const auto it : sounds)
    {
        assert(it != nullptr);
    }

    sounds[0]->play(my_engine::SoundBuffer::properties::looped);
    // SoundBuffer

    // Texture init

    // test vec_texture
    {
        std::array<std::string, 5> tex_name{
            "texMap", "texSand", "texSpice", "texStone", "texRock"
        };
        std::array<std::string, 5> tex_path{ "res/map_test.png",
                                             "res/sand.png",
                                             "res/spice.png",
                                             "res/stone.png",
                                             "res/rock.png" };

        Animate::Texture* temp_tex_map =
            new Animate::Texture(tex_name[0], GL_CLAMP_TO_BORDER, GL_LINEAR);
        map_texture.push_back(temp_tex_map);
        {
            // Image image = Image::loadFromFile(tex_path[0]);
            // temp_tex_map->setImage(image);
            map_texture[0]->loadImage(tex_path[0]);
        }

        for (size_t i = 1; i < 5; i++)
        {
            Animate::Texture* temp_tex =
                new Animate::Texture(tex_name[i], GL_REPEAT, GL_NEAREST);
            map_texture.push_back(temp_tex);
            {
                // Image image = Image::loadFromFile(tex_path[i]);
                // temp_tex->setImage(image);
                map_texture[i]->loadImage(tex_path[i]);
            }
        }
    }
    // test vec_texture

    std::string tex_name = "s_texture";

    texture_head =
        std::make_unique<Animate::Texture>(tex_name, GL_REPEAT, GL_NEAREST);
    {
        Animate::Image image = Animate::Image::loadFromFile("res/head.png");
        texture_head->setImage(image);
    }

    texture_corpse =
        std::make_unique<Animate::Texture>(tex_name, GL_REPEAT, GL_NEAREST);
    {
        Animate::Image image = Animate::Image::loadFromFile("res/corpse.png");
        texture_corpse->setImage(image);
    }

    // texture_bullet =
    //     std::make_unique<Animate::Texture>(tex_name, GL_REPEAT, GL_NEAREST);
    // {
    //     Animate::Image image =
    //     Animate::Image::loadFromFile("res/bullet.png");
    //     texture_bullet->setImage(image);
    // }

    tex_light_shell =
        std::make_unique<Animate::Texture>(tex_name, GL_REPEAT, GL_NEAREST);
    tex_light_shell->loadImage("res/light_shell.png");
    tex_medium_shell =
        std::make_unique<Animate::Texture>(tex_name, GL_REPEAT, GL_NEAREST);
    tex_medium_shell->loadImage("res/medium_shell.png");
    tex_plasma =
        std::make_unique<Animate::Texture>(tex_name, GL_REPEAT, GL_NEAREST);
    tex_plasma->loadImage("res/plasma.png");

    texture_spider =
        std::make_unique<Animate::Texture>(tex_name, GL_REPEAT, GL_NEAREST);
    {
        Animate::Image image = Animate::Image::loadFromFile("res/spider.png");
        texture_spider->setImage(image);
    }

    // texture_big_spider =
    //     std::make_unique<Animate::Texture>(tex_name, GL_REPEAT, GL_NEAREST);
    // {
    //     Animate::Image image =
    //         Animate::Image::loadFromFile("res/big_spider2.png");
    //     texture_big_spider->setImage(image);
    // }

    texture_fire_ball =
        std::make_unique<Animate::Texture>(tex_name, GL_REPEAT, GL_NEAREST);
    {
        Animate::Image image =
            Animate::Image::loadFromFile("res/fire_ball.png");
        texture_fire_ball->setImage(image);
    }

    tex_nest =
        std::make_unique<Animate::Texture>(tex_name, GL_REPEAT, GL_NEAREST);
    tex_nest->loadImage("res/nest.png");

    // Texture init

    // SPRITE
    spire_big_spider = std::make_unique<Animate::sprite>(200.f);
    spire_big_spider->add_texture(
        "res/big_spider1.png", tex_name, GL_REPEAT, GL_NEAREST);
    spire_big_spider->add_texture(
        "res/big_spider2.png", tex_name, GL_REPEAT, GL_NEAREST);
    // SPRITE

    // Gun
    // gun_current = std::make_unique<guns::shotGun>();
    guns.push_back(
        std::make_unique<guns::GunSimple>(tex_plasma.get(), bullet_obj));
    guns.push_back(
        std::make_unique<guns::shotGun>(tex_light_shell.get(), bullet_obj));
    guns.push_back(
        std::make_unique<guns::miniGun>(tex_light_shell.get(), bullet_obj));
    gun_current = guns[gun_current_ID].get();
    // Gun

    player = std::make_unique<Player>(
        tank_obj, texture_head.get(), texture_corpse.get(), sounds[2]);

    //  SPAWN Enemy
    srand(time(NULL));
    spawn_monster = std::make_unique<spawn::wave_1>(this, player.get());
    spawn_monster->start_lvl();
    

    //  SPAWN Enemy
    isRunning = true;
}

void game_impl::on_event(my_engine::event& event)
{
    while (my_engine::read_event(event))
    {
        // std::cout << event << std::endl;
        switch (event.type)
        {
            case my_engine::event_type::hardware:
            {
                if (std::get<my_engine::hardware_data>(event.info).is_reset)
                {
                    isRunning = false;
                }
                break;
            }
            case my_engine::event_type::input_key:
            {
                const auto& key_data =
                    std::get<my_engine::input_data>(event.info);

                controls[static_cast<unsigned>(key_data.key)] =
                    key_data.is_down;

                if (key_data.is_down)
                {
                    if (key_data.key == my_engine::keys_type::select)
                    {
                        isRunning = false;
                    }

                    else if (key_data.key == my_engine::keys_type::button1)
                    {
                        my_engine::matrix2x3 rot_head =
                            my_engine::matrix2x3::rotation(
                                player->getCurrent_head_direction());
                        std::cout << "rotation head:\n"
                                  << rot_head << std::endl;
                        // s->play(my_engine::SoundBuffer::properties::once);
                    }
                    else if (key_data.key == my_engine::keys_type::start)
                    {
                        std::cout << static_cast<unsigned>(key_data.key)
                                  << std::endl;
                        // s->play(my_engine::SoundBuffer::properties::looped);
                    }
                    else if (key_data.key == my_engine::keys_type::button2)
                    {
                        swap_gun();
                    }
                }
                break;
            }
            case my_engine::event_type::mouse_motion:
            {
                player->setMouse_pos(my_engine::vec2{ event.x, event.y });
                break;
            }
            default:
                break;
        }
    }
}

void game_impl::on_update(std::chrono::microseconds frame_delta)
{
    if (player->getHealth() > 0)
    {
        /* code */

        // std::cout << "frame_delta:\t" << frame_delta.count() << std::endl;
        const float delta = frame_delta.count() * 0.001f;

        // UPDATE Monsters
        spawn_monster->update_wave(delta);

        for (auto&& monster : enemy_list)
        {
            monster->update(delta, player.get(), enemy_list);
        }
        // UPDATE Monsters

        // UPDATE Bullets
        if (controls[static_cast<unsigned>(my_engine::keys_type::mouse_L)])
        {
            gun_current->shoot(player->getCurrent_current_pos(),
                               player->getCurrent_head_direction());
        }
        // UPDATE Bullets

        // UPDATE Gun
        for (const auto& gun : guns)
        {
            gun->update_gun(delta, enemy_list);
        }

        // gun_current->update_gun(delta, enemy_list);
        // UPDATE Gun

        // UPDATE Player
        player->update(controls, delta);
        // UPDATE Player
    }

    // UPDATE ImGui
    update_imGui();
    // UPDATE ImGui
}

void game_impl::on_render()
{
    // RENDER MAP
    my_engine::render(*map_obj, map_texture, gameConst::aspect_mat);
    // RENDER MAP

    // RENDER BULLETS
    for (const auto& gun : guns)
    {
        gun->render_bullets();
    }
    // gun_current->render_bullets();
    // RENDER BULLETS

    // RENDER ENEMY
    for (auto enemy = enemy_list.begin(); enemy != enemy_list.end();)
    {
        if ((*enemy)->getHealth() <= 0)
        {
            // delete (*enemy);
            enemy = enemy_list.erase(enemy);
        }
        else
        {
            (*enemy)->render_enemy();
            ++enemy;
        }
    }
    // RENDER ENEMY

    // RENDER PLAYER
    player->render_player();
    // RENDER PLAYER

    // RENDER ImGui
    my_engine::render_imgui();
    // RENDER ImGui
}

void game_impl::add_big_spider(my_engine::vec2 pos_enemy)
{
    enemy_list.push_back(
        std::make_unique<enemy::bigSpider>(pos_enemy,
                                           big_spider,
                                           spire_big_spider.get(),
                                           texture_fire_ball.get(),
                                           fire_ball_obj));
}

void game_impl::add_nest(my_engine::vec2 pos_enemy)
{
    if (enemy_list.size() < max_enemy)
    {
        enemy_list.push_back(std::make_unique<enemy::nest>(
            pos_enemy, nest_obj, tex_nest.get(), this));
    }
}

void game_impl::add_spider(my_engine::vec2 pos_enemy)
{
    if (enemy_list.size() < max_enemy)
    {
        enemy_list.push_back(std::make_unique<enemy::spider>(
            pos_enemy, spider, texture_spider.get()));
    }
}

void game_impl::update_imGui()
{
    // ImGuiIO& io = ImGui::GetIO();
    // (void)io;

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(my_engine::getWindow());
    ImGui::NewFrame();

    {
        ImGui::Begin("fps",
                     nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoBackground |
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        ImGui::SetWindowPos(ImVec2{ 1530, 0 });
        ImGui::SetWindowSize(ImVec2{ 390, 100 });

        ImGui::Text(" %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Begin("Player Status",
                     nullptr,
                     ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove
                     /*ImGuiWindowFlags_NoTitleBar */);

        ImGui::SetWindowPos(ImVec2{ 0, 0 });
        ImGui::SetWindowSize(ImVec2{ 230, 200 });
        // ImGui::Text("Health  =  %.0f", player->getHealth());

        float alfa_red = 1 - player->getHealth() / 1000.f;
        if (alfa_red > 1)
        {
            alfa_red = 1.f;
        }
        float alfa_green = player->getHealth() / 1000.f;
        if (alfa_green < 0)
        {
            alfa_green = 0.f;
        }

        ImVec4 coloredHP{ alfa_red, alfa_green, 0.f, 1.f };
        ImGui::TextColored(coloredHP, "Health  =  %.0f", player->getHealth());
        // ImGui::ProgressBar();

        ImGui::NewLine();
        if (gun_current->getCurrentClip() > 0)
        {
            ImGui::Text("%s %u/%u",
                        gun_current->getNameGun().data(),
                        gun_current->getCurrentClip(),
                        gun_current->getMaxClip());
        }
        else
        {
            ImGui::TextColored(ImVec4{ 1.f, 0.f, 0.f, 1.f },
                               "%s %s",
                               gun_current->getNameGun().data(),
                               "reload");
        }
        ImGui::End();

        ImGui::Begin("Score",
                     nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoBackground |
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        ImGui::SetWindowPos(ImVec2{ 810, 0 });
        ImGui::SetWindowSize(ImVec2{ 300, 100 });

        ImGui::TextColored(ImVec4{ 0.f, 0.f, 0.f, 1.f },
                           " %i SCORE",
                           gameInfo::score::getInstance().value());
        ImGui::End();
        if (player->getHealth() <= 0)
        {
            ImGui::Begin("gameover",
                     nullptr,
                     ImGuiWindowFlags_NoTitleBar/* |
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove */);

            ImGui::SetWindowPos(ImVec2{ 0, 0 });
            ImGui::SetWindowSize(
                ImVec2{ gameConst::screen_width, gameConst::screen_height });

            ImGui::SetWindowFontScale(5.f);

            ImGui::TextColored(ImVec4{ 1.f, 0.f, 0.f, 1.f },
                               " %i SCORE",
                               gameInfo::score::getInstance().value());
            ImGui::TextColored(ImVec4{ 1.f, 0.f, 0.f, 1.f }, "Game Over");
            ImGui::End();
        }
    }
}

void game_impl::swap_gun()
{
    gun_current_ID++;
    gun_current_ID = gun_current_ID % guns.size();
    gun_current    = guns[gun_current_ID].get();
}

} // namespace my_game

static bool already_exist_game = false;

my_engine::game* my_engine::create_game()
{
    if (already_exist_game)
    {
        throw std::runtime_error("engine already exist");
    }
    my_engine::game* result = new my_game::game_impl();
    already_exist_game      = true;
    return result;
}

void my_engine::destroy_game(my_engine::game* e)
{
    if (already_exist_game == false)
    {
        throw std::runtime_error("game not created");
    }
    if (nullptr == e)
    {
        throw std::runtime_error("e is nullptr");
    }
    delete e;
}