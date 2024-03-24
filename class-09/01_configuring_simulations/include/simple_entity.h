#pragma once

#include <array>

struct SimpleEntity
{
    auto update(const double delta_time) -> void;

    auto log(const double time) -> void;

    std::array<double, 2> position{};
    std::array<double, 2> velocity{};
};
