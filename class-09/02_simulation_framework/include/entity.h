#pragma once

#include <array>

struct Entity
{
    auto update(const double time_delta) -> void;

    auto log(const double time) -> void;

    std::array<double, 2> position{};
    std::array<double, 2> velocity{};
};
