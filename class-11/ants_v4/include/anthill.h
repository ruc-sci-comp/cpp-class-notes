#pragma once

#include <array>

struct AntHill
{
    auto deposit(int food) -> void;

    auto spawn_ants() -> int;

    std::array<double, 2> position{};
    int quantity{};
};