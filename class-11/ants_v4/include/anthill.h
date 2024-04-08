#pragma once

#include <array>

struct AntHill
{
    auto deposit(int food) -> void;

    auto spawn_ants(const double time) -> int;

    std::array<double, 2> position{};
    double spawn_cooldown{ 1.0 };
    int quantity{};
};