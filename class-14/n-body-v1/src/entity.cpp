#include <cmath>

#include "entity.h"
#include "utility.h"

auto Entity::compute(const Entity& other) -> void
{
    constexpr auto G = 6.67430e-11;
    static_cast<void>(G);
    static_cast<void>(other);
    // compute the force of gravity imparted by other!
}

auto Entity::update(const double delta_time) -> void
{
    static_cast<void>(delta_time);
    // update the position, velocity, and acceleration!
}
