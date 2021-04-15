#include "score.hpp"

namespace gameState
{

score::score() {}

void score::value(size_t val)
{
    // static score instance;
    // instance.val_score += val;
    val_score += val;
}

size_t score::value()
{
    // static score instance;
    // return instance.val_score;
    return val_score;
}

score& score::getInstance()
{
    static score instance;
    return instance;
}
} // namespace gameState