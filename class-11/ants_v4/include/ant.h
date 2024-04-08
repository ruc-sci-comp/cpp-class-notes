#pragma once

#include <array>
#include <vector>
#include "anthill.h"
#include "environment.h"
#include "food.h"

struct Ant
{
    auto update(const double time_delta, Environment& environment, AntHill& anthill, std::vector<Food>& food) -> void;

    std::array<double, 2> position{};
    double direction = {};
    int quantity{ 0 };
    int capacity{ 5 };
};
