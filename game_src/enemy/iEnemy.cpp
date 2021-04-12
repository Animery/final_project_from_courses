#include "iEnemy.hpp"

#ifdef DEBUG_LEVEL
#include <iostream>
#endif

namespace enemy
{

iEnemy::~iEnemy()
{
#ifdef DEBUG_LEVEL
    std::cout << "--- destor iEnemy" << std::endl;
#endif
}

} // namespace enemy