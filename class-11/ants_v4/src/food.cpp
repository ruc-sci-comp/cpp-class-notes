#include <algorithm>

#include "food.h"

auto Food::deplete(int requested) -> int
{
    auto amount = std::min(requested, quantity);
    quantity -= amount;
    return amount;
}
