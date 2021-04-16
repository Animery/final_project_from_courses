#pragma once

// #include "Texture.hpp"
#include "Timer.hpp"

#include <memory>
#include <vector>

class Timer;

namespace Animate
{
class Texture;

class sprite
{
public:
    sprite();
    sprite(sprite& sprite_for_copy);
    sprite(float value_timer);
    ~sprite();
    void     set_timer_value(float value_timer);
    void     update_sprite(float delta);
    void     add_texture(std::string_view path,
                         std::string&     name_u,
                         int              wrap,
                         int              filter);
    void     cleanSprite();
    Texture* getCurrent_tex();

private:
    void swap_tex();

private:
    Timer sprite_timer;
    bool  timer_used  = false;
    float timer_value = 100.f;

    std::vector<std::shared_ptr<Animate::Texture>> vec_tex;

    size_t   tex_id      = 0;
    Texture* current_tex = nullptr;
};
} // namespace Animate