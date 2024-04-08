#pragma once

#include <array>

struct Food
{
    auto deplete(int requested) -> int;

    std::array<double, 2> position{};
    int quantity{ 10 };
};
