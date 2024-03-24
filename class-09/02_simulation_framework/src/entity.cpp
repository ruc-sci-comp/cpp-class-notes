#include <fmt/format.h>
#include "entity.h"

auto Entity::update(const double time_delta) -> void
{
    position[0] += velocity[0] * time_delta;
    position[1] += velocity[1] * time_delta;
}

auto Entity::log(const double time) -> void
{
    fmt::print("{:.2f},{:.2f},{:.2f}\n", time, position[0], position[1]);
}