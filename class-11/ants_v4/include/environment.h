#pragma once

#include <array>
#include <vector>



struct Environment
{
    auto update() -> void;

    auto restrict_position(std::array<double, 2>& position) const->void;

    auto reflect(const std::array<double, 2>& position, const double direction) const -> double;

    auto drop_pheremone(const std::array<double, 2>& position, const int pheremone_type) -> void;

    auto get_pheremone_trail(const std::array<double, 2>& position, const double direction, const int pheremone_type) -> double;

    auto get_index_from_position(const std::array<double, 2>& position) -> int;

    std::array<double, 2> x_limit{ -100, 100 };
    std::array<double, 2> y_limit{ -100, 100 };

    std::vector<std::array<double, 2>> pheremones{};
};
