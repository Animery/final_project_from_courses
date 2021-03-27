#include "player.hpp"
#include "../include/RenderObj_impl.hpp"

#include <iostream>

Player::Player()
{

    // Corpse
    std::string tex_name = "s_texture";
    corpse               = my_engine::create_RenderObj();
    // corpse = new my_engine::RenderObj_impl();

    {
        const std::string path("shader/");
        const std::string name_shader("corpse");
        gfx_corpse = my_engine::create_gfx_prog(path, name_shader);
        gfx_corpse->bind_attrib(0, "a_position");
        gfx_corpse->bind_attrib(1, "a_tex_coord");
        gfx_corpse->bind_attrib(2, "a_color");
        gfx_corpse->link();
    }
    corpse->setProg(gfx_corpse);
    corpse->load_mesh_from_file("res/tank.txt");

    texture_corpse = std::make_unique<Texture>(tex_name);
    {
        Image image = Image::loadFromFile("res/corpse.png");
        texture_corpse->setImage(image);
    }
    // Corpse

    // Head
    head = my_engine::create_RenderObj();
    tex_name.clear();
    tex_name = "s_texture";
    {
        const std::string path("shader/");
        const std::string name_shader("head");
        gfx_head = my_engine::create_gfx_prog(path, name_shader);
        gfx_head->bind_attrib(0, "a_position");
        gfx_head->bind_attrib(1, "a_tex_coord");
        gfx_head->bind_attrib(2, "a_color");
        gfx_head->link();
    }
    head->setProg(gfx_head);
    head->load_mesh_from_file("res/tank.txt");

    texture_head = std::make_unique<Texture>(tex_name);
    {
        Image image = Image::loadFromFile("res/head.png");
        texture_head->setImage(image);
    }
    // Head

    // loadRenderObj();

    std::cout << "corpse_prt\t" << &corpse << std::endl;
    std::cout << "head_prt\t" << &head << std::endl;

    std::cout << "+++ ctor Player" << std::endl;
}

Player::~Player()
{
    my_engine::destroy_RenderObj(corpse);
    my_engine::destroy_gfx_prog(gfx_corpse);
    my_engine::destroy_RenderObj(head);

    std::cout << "--- destor Player" << std::endl;
}

// my_engine::RenderObj* Player::getCorpse()
// {
//     return corpse;
// }

// Texture* Player::getTexture_corpse()
// {
//     return texture_corpse.get();
// }

// Texture* Player::getTexture_head()
// {
//     return texture_head.get();
// }

// my_engine::RenderObj* Player::getHead()
// {
//     return head;
// }

void Player::loadRenderObj() {}