#include "entity.h"
#include "environment.h"

auto Environment::get_modification(const Entity& entity) const->std::array<double, 2>
{
    auto modification = std::array<double, 2>{0.0, 0.0};
    if (entity.position[0] > 0.0 && entity.position[1] > 0.0)
    {
        modification[0] -= 1.0;
    }
    if (entity.position[0] < 0.0 && entity.position[1] < 0.0)
    {
        modification[0] += 1.0;
    }
    return modification;
}
