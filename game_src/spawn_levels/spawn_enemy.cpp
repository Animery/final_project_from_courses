#include "spawn_enemy.hpp"

#if defined(DEBUG_LEVEL)
#include <iostream>
#endif // DEBUG_LEVEL

namespace spawn
{

spawn_enemy::~spawn_enemy()
{
#if defined(DEBUG_LEVEL)
    std::cout << "destor spawn_enemy" << std::endl;
#endif // DEBUG_LEVEL
}

} // namespace spawn