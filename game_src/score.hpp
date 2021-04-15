#pragma once

#include <cstddef>

namespace gameInfo
{

class score
{

public:
    static score& getInstance();

    score(score const&) = delete;
    void   operator=(score const&) = delete;
    void   value(size_t val);
    size_t value();

private:
    size_t val_score = 0;
    score();
};

} // namespace gameInfo