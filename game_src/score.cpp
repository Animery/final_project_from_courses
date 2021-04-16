#include "score.hpp"

#include "../engine/debug_level.hpp"
#ifdef DEBUG_LEVEL
#include <iostream>
#endif

namespace gameInfo
{

score::score()
{
#ifdef DEBUG_LEVEL
    std::cout << "+++ ctor score" << std::endl;
#endif
}

score::~score() 
{
    #ifdef DEBUG_LEVEL
    std::cout << "--- destor score" << std::endl;
#endif
}

void score::value(size_t val)
{
    val_score += val;
}

size_t score::value()
{
    return val_score;
}

score& score::getInstance()
{
    static score instance;
    return instance;
}
} // namespace gameInfo