#pragma once

#include "Texture.hpp"

#include <memory>

namespace Animate
{

class sprite
{

public:
    sprite();
    ~sprite();

private:
    std::vector<std::unique_ptr<Animate::Texture>> vec_tex;
};
} // namespace Animate