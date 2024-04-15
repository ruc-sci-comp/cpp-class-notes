#pragma once

#include <array>

struct Environment;

struct Projectile
{
    auto update(const double delta_time, const Environment& environment) -> void;

    std::array<double, 2> position{};
    std::array<double, 2> velocity{};
    double mass{ 19.0 };
    double radius{ 0.17 };
};
