#pragma once

#include <array>
#include <cmath>

static inline auto compute_difference(const std::array<double, 2>& p1, const std::array<double, 2>& p2) -> std::array<double, 2>
{
    return { p1[0] - p2[0], p1[1] - p2[1] };
}

static inline auto compute_distance(const std::array<double, 2>& p1, const std::array<double, 2>& p2) -> double
{
    return std::hypot(p1[0] - p2[0], p1[1] - p2[1]);
}

static inline auto compute_angle(const std::array<double, 2>& p1, const std::array<double, 2>& p2) -> double
{
    return std::atan2(p1[1] - p2[1], p1[0] - p2[0]);
}