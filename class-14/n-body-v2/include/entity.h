#pragma once

#include <array>

struct Entity
{
    auto compute(const Entity& other) -> void;

    auto update(const double delta_time) -> void;

    std::array<double, 2> position{};
    std::array<double, 2> velocity{};
    std::array<double, 2> acceleration{};
    double mass{ 5e9 };
};
