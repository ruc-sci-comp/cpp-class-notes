#pragma once

#include <array>

struct Food
{
    auto deplete(int requested) -> int;

    int quantity{ 10 };
    std::array<double, 2> position{};
};
