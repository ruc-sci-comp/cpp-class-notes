#include "environment.h"
#include "projectile.h"

auto Projectile::update(const double delta_time, const Environment& environment) -> void
{
    auto acceleration = environment.get_drag(*this);
    acceleration[1] += environment.get_gravity(position[1]);
    velocity[0] += acceleration[0] * delta_time;
    velocity[1] += acceleration[1] * delta_time;
    position[0] += velocity[0] * delta_time + 0.5 * acceleration[0] * delta_time * delta_time;
    position[1] += velocity[1] * delta_time + 0.5 * acceleration[1] * delta_time * delta_time;
}
