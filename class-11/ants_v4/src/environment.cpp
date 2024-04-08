#include <algorithm>
#include <cmath>
#include <numbers>
#include <random>
#include <set>

#include <fmt/format.h>


#include "ant.h"
#include "environment.h"

using namespace std::numbers;

auto Environment::update()->void
{
    for (auto& p : pheremones)
    {
        p[0] = std::max(p[0] - 0.00125, 0.0);
        p[1] = std::max(p[1] - 0.00125, 0.0);
    }
    // std::erase_if(pheremones, [](const auto& f) { return f[0] < 1.0e-8 && f[1] < 1.0e-8; });
}


auto Environment::restrict_position(std::array<double, 2>& position) const->void
{
    position[0] = std::clamp(position[0], x_limit[0], x_limit[1]);
    position[1] = std::clamp(position[1], y_limit[0], y_limit[1]);
}

auto Environment::reflect(const std::array<double, 2>& position, const double direction) const->double
{
    if (position[0] == x_limit[0] || position[0] == x_limit[1])
    {
        return 2.0 * pi - pi - direction;
    }
    if (position[1] == y_limit[0] || position[1] == y_limit[1])
    {
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
    auto w = static_cast<int>(std::lround(x_limit[1] - x_limit[0]));
    auto h = static_cast<int>(std::lround(y_limit[1] - y_limit[0]));
    auto index = get_index_from_position(position);
    auto x = index % w;
    auto y = index / h;

    auto angles = std::vector<double>{};

    for (auto i = int{ -5 }; i < 6; ++i)
    {
        for (auto j = int{ -5 }; j < 6; ++j)
        {
            auto dx = std::clamp(x + i, 0, w - 1);
            auto dy = std::clamp(y + j, 0, h - 1);
            auto di = w * dy + dx;

            auto angle_to_cell = std::atan2(dy - y, dx - x);
            if (std::abs(direction - angle_to_cell) < (45.0 * pi / 180.0))
            {
                if (pheremones[di][pheremone_type] > 0.0)
                {
                    angles.emplace_back(angle_to_cell);
                }
            }
        }
    }
    if (angles.empty())
        return direction;

    return angles[std::random_device{}() % angles.size()];
}

auto Environment::get_index_from_position(const std::array<double, 2>& position) -> int
{
    auto w = static_cast<int>(100);
    auto h = static_cast<int>(100);

    auto x = std::clamp(static_cast<int>(std::lround(position[0] - x_limit[0])), 0, w - 1);
    auto y = std::clamp(static_cast<int>(std::lround(position[1] - y_limit[0])), 0, h - 1);
    return w * y + x;
}