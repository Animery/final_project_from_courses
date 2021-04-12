#include "game_impl.hpp"

#include "gun/shotGun.hpp"
#include "gun/simpleGun.hpp"
#include "spawn_levels/wave_1.hpp"

#include <algorithm>
#include <cassert>
#include <ctime>
// #include "Bullet.hpp"
// #include <cmath>

#include "../imgui_src/imgui.h"
#include "../imgui_src/imgui_impl_opengl3.h"
#include "../imgui_src/imgui_impl_sdl.h"

namespace my_game
{

game_impl::game_impl()
{

    // std::cout << "sizeof Player" << sizeof(Player) << std::endl;
    // std::cout << "sizeof Enemy" << sizeof(Enemy) << std::endl;
    std::cout << "+++ ctor game_impl" << std::endl;
}

game_impl::~game_impl()
{
    // delete engine;
    for (auto it : enemy_list)
    {
        delete it;
    }

    for (auto tex : map_texture)
    {
        delete tex;
    }

    delete gfx_obj;
    delete gfx_map;
    std::cout << "--- destor game_impl" << std::endl;
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

    enemy_1 = my_engine::create_RenderObj();
    enemy_1->setProg(gfx_obj);
    enemy_1->load_mesh_from_file("res/enemy_1.txt");

    map_obj = my_engine::create_RenderObj();
    map_obj->setProg(gfx_map);
    map_obj->load_mesh_from_file("res/map.txt");
    // Render_obj

    // SoundBuffer

    sounds.push_back(my_engine::create_sound_buffer("res/8-bit_detective.wav"));
    sounds.push_back(my_engine::create_sound_buffer("res/t2_no_problemo.wav"));

    for (const auto it : sounds)
    {
        assert(it != nullptr);
    }

    // assert(sounds[0] != nullptr);
    // assert(sounds[1] != nullptr);

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

        Texture*                   temp_tex_map =
            new Texture(tex_name[0], GL_CLAMP_TO_BORDER, GL_LINEAR);
        map_texture.push_back(temp_tex_map);
        {
            // Image image = Image::loadFromFile(tex_path[0]);
            // temp_tex_map->setImage(image);
            map_texture[0]->loadImage(tex_path[0]);
        }

        for (size_t i = 1; i < 5; i++)
        {
            Texture* temp_tex =
                new Texture(tex_name[i], GL_REPEAT, GL_NEAREST);
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

    texture_head = std::make_unique<Texture>(tex_name, GL_REPEAT, GL_NEAREST);
    {
        Image image = Image::loadFromFile("res/head.png");
        texture_head->setImage(image);
    }

    texture_corpse = std::make_unique<Texture>(tex_name, GL_REPEAT, GL_NEAREST);
    {
        Image image = Image::loadFromFile("res/corpse.png");
        texture_corpse->setImage(image);
    }

    texture_bullet = std::make_unique<Texture>(tex_name, GL_REPEAT, GL_NEAREST);
    {
        Image image = Image::loadFromFile("res/bullet.png");
        texture_bullet->setImage(image);
    }

    texture_spider = std::make_unique<Texture>(tex_name, GL_REPEAT, GL_NEAREST);
    {
        Image image = Image::loadFromFile("res/spider.png");
        texture_spider->setImage(image);
    }

    // Texture init


    // Gun
    // gun_current = std::make_unique<guns::shotGun>();
    gun_current =
        std::make_unique<guns::shotGun>(texture_bullet.get(), bullet_obj);
    // Gun

    player = std::make_unique<Player>(
        tank_obj, texture_head.get(), texture_corpse.get());

    //  SPAWN Enemy
    srand(time(NULL));
    spawn_monster = std::make_unique<spawn::wave_1>(this,player.get());

    // float temp_pos_x;
    // float temp_pos_y;
    for (size_t i = 0; i < 1; i++)
    {
        // temp_pos_x = (rand() % 2000 / 1000.0f) - 1;
        // temp_pos_y = (rand() % 2000 / 1000.0f) - 1;
        // add_enemy({ temp_pos_x, temp_pos_y });
        add_enemy({ 0.5, 0.5 });
        add_enemy({ -0.5, 0.5 });
        add_enemy({ 0.5, -0.5 });
        add_enemy({ -0.5, -0.5 });
    }

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
                    else if (key_data.key == my_engine::keys_type::button2)
                    {
                        // s->play(my_engine::SoundBuffer::properties::looped);
                    }
                }
                break;
            }
            case my_engine::event_type::mouse:
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
    // std::cout << "frame_delta:\t" << frame_delta.count() << std::endl;
    const float delta = frame_delta.count() * 0.001f;

    // UPDATE Monsters
    spawn_monster->update_wave(delta);

    for (auto&& monster : enemy_list)
    {
        monster->update(delta, player->getCurrent_tank_pos());
    }
    // UPDATE Monsters

    // UPDATE Bullets
    if (controls[static_cast<unsigned>(my_engine::keys_type::button2)])
    {
        gun_current->shoot(player->getCurrent_tank_pos(),
                           player->getCurrent_head_direction());
    }
    // UPDATE Bullets

    // UPDATE Gun
    gun_current->update_gun(delta, enemy_list);
    // UPDATE Gun

    // UPDATE Player
    player->update(controls, delta);
    // UPDATE Player

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
    gun_current->render_bullets();
    // RENDER BULLETS

    // RENDER ENEMY
    for (auto enemy = enemy_list.begin(); enemy != enemy_list.end();)
    {
        if ((*enemy)->getHealth() <= 0)
        {
            delete (*enemy);
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

void game_impl::add_enemy(my_engine::vec2 pos_enemy)
{
    if (enemy_list.size() < max_enemy)
    {
        enemy_list.push_back(
            new Enemy(pos_enemy, enemy_1, texture_spider.get()));
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
        ImGui::Begin(
            "fps",
            nullptr,
            ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoBackground); // Create a window called
                                                // "Hello, world!" and append
                                                // into it.
        ImGui::Text(" %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Begin("Player Status", nullptr, ImGuiWindowFlags_NoBackground
                     /*ImGuiWindowFlags_NoTitleBar */);

        ImGui::Text("Health  =  %.0f", player->getHealth());
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
            ImGui::Text("%s %s", gun_current->getNameGun().data(), "reload");
        }

        // ImGui::SameLine();
        // ImGui::NewLine();
        // ImGui::Text("Player Status"); // Display some text (you can
        // use a format strings too)
        // ImGui::ColorEdit3(
        //     "clear color",
        //     (float*)&clear_color); // Edit 3 floats representing a color

        // if (ImGui::Button(
        //         "Button")) // Buttons return true when clicked (most widgets
        //                    // return true when edited/activated)
        //     counter++;

        ImGui::End();
    }
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