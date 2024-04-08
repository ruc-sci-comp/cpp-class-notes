#include <numbers>

#include "anthill.h"


auto AntHill::deposit(int food) -> void
{
    quantity += food;
}

auto AntHill::spawn_ants(const double delta_time) -> int
{
    spawn_cooldown += delta_time;
    if (spawn_cooldown > 1.0)
    {
        spawn_cooldown = 0.0;
        if (quantity >= 100)
        {
            quantity -= 100;
            return 1;
        }
    }

    return 0;
}