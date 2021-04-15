#include "score.hpp"

namespace gameInfo
{

score::score() {}

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
} // namespace gameState