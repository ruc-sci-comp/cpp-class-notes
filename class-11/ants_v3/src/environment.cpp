#include <algorithm>
#include <cmath>
#include <numbers>

#include "ant.h"
#include "environment.h"

using namespace std::numbers;

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

auto Environment::get_multiplier(const std::array<double, 2>&) const->std::array<double, 2>
{
    return { 1.0, 1.0 };
}
