#include <algorithm>
#include <cmath>
#include <numbers>
#include <random>

#include <fmt/format.h>

#include "ant.h"
#include "environment.h"

using namespace std::numbers;

auto Environment::update()->void
{
    for (auto& p : pheremones)
    {
        p[0] *= 0.975;
        p[1] *= 0.975;
    }
}


auto Environment::restrict_position(std::array<double, 2>& position, const double direction) const->double
{
    if (position[0] < x_limit[0])
    {
        position[0] = x_limit[0] + (x_limit[0] - position[0]);
        return 2.0 * pi - pi - direction;
    }
    if (position[0] > x_limit[1])
    {
        position[0] = x_limit[1] - (position[0] - x_limit[1]);
        return 2.0 * pi - pi - direction;
    }
    if (position[1] < y_limit[0])
    {
        position[1] = y_limit[0] + (y_limit[0] - position[1]);
        return 2.0 * pi - direction;
    }
    if (position[1] > y_limit[1])
    {
        position[1] = y_limit[1] - (position[1] - y_limit[1]);
        return 2.0 * pi - direction;
    }
    return direction;
}

auto Environment::drop_pheremone(const std::array<double, 2>& position, const int pheremone_type) -> void
{
    auto index = get_index_from_position(position);
    pheremones[index][pheremone_type] = std::min(pheremones[index][pheremone_type] + 0.1, 1.0);
}

auto Environment::get_pheremone_trail(const std::array<double, 2>& position, const double direction, const int pheremone_type) -> double
{
    auto w = 100;
    auto h = 100;
    auto index = get_index_from_position(position);
    auto x = index % w;
    auto y = index / h;

    auto new_direction = direction;
    auto ph = 0.0;

    constexpr auto range = int{ 4 };
    constexpr auto short_range = int{ 1 };

    for (auto i = int{ -range }; i <= range; ++i)
    {
        for (auto j = int{ -range }; j <= range; ++j)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            auto dx = std::clamp(x + i, 0, w - 1);
            auto dy = std::clamp(y + j, 0, h - 1);
            auto di = w * dy + dx;

            auto angle_to_cell = std::atan2(dy - y, dx - x);
            auto angle_offset_deg = std::abs(direction - angle_to_cell) * (180.0 / pi);

            if (angle_offset_deg < 15.0 || (std::abs(i) <= short_range && std::abs(j) <= short_range && angle_offset_deg < 75.0))
            {
                if (pheremones[di][pheremone_type] > ph)
                {
                    new_direction = angle_to_cell;
                    ph = pheremones[di][pheremone_type];
                }
            }
        }
    }

    return new_direction;
}

auto Environment::get_index_from_position(const std::array<double, 2>& position) -> int
{
    auto w = static_cast<int>(100);
    auto h = static_cast<int>(100);

    auto x = std::clamp(static_cast<int>(std::lround(position[0] - x_limit[0])), 0, w - 1);
    auto y = std::clamp(static_cast<int>(std::lround(position[1] - y_limit[0])), 0, h - 1);
    return w * y + x;
}