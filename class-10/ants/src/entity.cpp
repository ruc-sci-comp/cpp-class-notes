#include <fmt/format.h>
#include "entity.h"

auto Entity::update(const double time_delta, const Environment& environment) -> void
{
    auto modification = environment.get_multiplier(position);
    position[0] += velocity[0] * modification[0] * time_delta;
    position[1] += velocity[1] * modification[1] * time_delta;
    environment.restrict_position(position);
    environment.reflect(position, velocity);
}

auto Entity::log(const double time) -> void
{
    fmt::print("{:.2f},{:.2f},{:.2f}\n", time, position[0], position[1]);
}