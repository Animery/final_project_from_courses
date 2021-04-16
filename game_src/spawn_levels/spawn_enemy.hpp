#pragma once

#include "../GameConst.hpp"


namespace spawn
{

class spawn_enemy
{

public:
    virtual ~spawn_enemy();
    virtual void update_wave(const float delta) = 0;
    virtual void start_lvl()                   = 0;

protected:
};
} // namespace spawn