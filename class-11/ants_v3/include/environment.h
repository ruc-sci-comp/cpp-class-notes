#pragma once

#include <array>

struct Environment
{
    auto restrict_position(std::array<double, 2>& position) const->void;

    auto reflect(const std::array<double, 2>& position, const double direction) const -> double;

    auto get_multiplier(const std::array<double, 2>& position) const->std::array<double, 2>;

    std::array<double, 2> x_limit{ -100, 100 };
    std::array<double, 2> y_limit{ -100, 100 };
};
