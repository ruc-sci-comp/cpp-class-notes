#include "entity.h"

auto Entity::update(const double time_delta) -> void
{
    position[0] += velocity[0] * time_delta;
    position[1] += velocity[1] * time_delta;
}
