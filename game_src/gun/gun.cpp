#include "gun.hpp"

#include "../../engine/debug_level.hpp"

#ifdef DEBUG_LEVEL
#include <iostream>
#endif

namespace guns
{

Gun::~Gun()
{
#ifdef DEBUG_LEVEL
    std::cout << "--- destor Gun" << std::endl;
#endif
}

} // namespace guns