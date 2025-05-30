#pragma once

#include <array>
#include <vector>
#include "environment.h"
#include "food.h"

struct Ant
{
    auto update(const double time_delta, const Environment& environment, std::vector<Food>& food) -> void;

    std::array<double, 2> position{};
    std::array<double, 2> velocity{};
    int quantity{ 0 };
    int capacity{ 5 };
};
