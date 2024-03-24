#include <fmt/format.h>
#include "simple_entity.h"

auto SimpleEntity::update(const double delta_time) -> void
{
    position[0] += velocity[0] * delta_time;
    position[1] += velocity[1] * delta_time;
}

auto SimpleEntity::log(const double time) -> void
{
    fmt::print("{:.2f},{:.2f},{:.2f}\n", time, position[0], position[1]);
}
