#include <cmath>

#include "entity.h"
#include "utility.h"

auto Entity::compute(const Entity& other) -> void
{
    constexpr auto G = 6.67430e-11;
    auto distance = compute_distance(other.position, position);
    auto angle = compute_angle(other.position, position);
    auto force = G * mass * other.mass / (distance * distance);
    acceleration[0] += force * std::cos(angle) / mass;
    acceleration[1] += force * std::sin(angle) / mass;
}

auto Entity::update(const double delta_time) -> void
{
    velocity[0] += acceleration[0] * delta_time;
    velocity[1] += acceleration[1] * delta_time;
    position[0] += velocity[0] * delta_time + 0.5 * acceleration[0] * delta_time * delta_time;
    position[1] += velocity[1] * delta_time + 0.5 * acceleration[1] * delta_time * delta_time;
    acceleration = { 0.0, 0.0 };
}
