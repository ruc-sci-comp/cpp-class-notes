#include <cmath>
#include "mineral.h"

auto Mineral::mine(int requested) -> int
{
    auto mined = std::min(units, requested);
    units -= mined;
    return mined;
}

auto Mineral::is_available() -> bool
{
    return units > 0;
}
