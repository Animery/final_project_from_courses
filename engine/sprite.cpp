#include "sprite.hpp"
#include "Texture.hpp"
#include "debug_level.hpp"

#ifdef DEBUG_LEVEL
#include <iostream>
#endif

namespace Animate
{

sprite::sprite()
{
    sprite_timer.setCallback([this]() {
        swap_tex();
        timer_used = false;
    });

#ifdef DEBUG_LEVEL
    std::cout << "+++ ctor Sprite" << std::endl;
#endif
}

sprite::sprite(float value_timer)
{
    timer_value = value_timer;
    sprite_timer.setCallback([this]() {
        swap_tex();
        timer_used = false;
    });
#ifdef DEBUG_LEVEL
    std::cout << "+++ ctor Sprite" << std::endl;
#endif
}

sprite::sprite(sprite& sprite_for_copy)
{
    this->timer_value = sprite_for_copy.timer_value;
    this->sprite_timer.setCallback([this]() {
        swap_tex();
        timer_used = false;
    });
    timer_value = sprite_for_copy.timer_value;
    vec_tex     = sprite_for_copy.vec_tex;
    if (current_tex == nullptr)
    {
        current_tex = vec_tex.front().get();
    }
#ifdef DEBUG_LEVEL
    std::cout << "+++ ctor Sprite" << std::endl;
#endif
}

sprite::~sprite()
{
#ifdef DEBUG_LEVEL
    std::cout << "--- destor Sprite" << std::endl;
#endif
}

void sprite::set_timer_value(float value_timer)
{
    timer_value = value_timer;
}

void sprite::update_sprite(float delta)
{
    if (timer_used)
    {
        sprite_timer.update_timer(delta);
    }
    else
    {
        sprite_timer.start(timer_value);
        timer_used = true;
    }
}

void sprite::add_texture(std::string_view path,
                         std::string&     name_u,
                         int              wrap,
                         int              filter)
{
    vec_tex.push_back(std::make_shared<Texture>(name_u, wrap, filter));
    vec_tex.back()->loadImage(path);

    if (current_tex == nullptr)
    {
        current_tex = vec_tex.front().get();
    }
}

void sprite::cleanSprite()
{
    current_tex == nullptr;
    vec_tex.clear();
}

Texture* sprite::getCurrent_tex()
{
    return current_tex;
}

void sprite::swap_tex()
{
    const size_t size = vec_tex.size();
    if (size != 0)
    {
        ++tex_id;
        tex_id = tex_id % size;
    }
    current_tex = vec_tex[tex_id].get();
}

} // namespace Animate