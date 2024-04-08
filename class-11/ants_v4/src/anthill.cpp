#include "anthill.h"

auto AntHill::deposit(int food) -> void
{
    quantity += food;
}

auto AntHill::spawn_ants() -> int
{
    auto new_ants = quantity / 100;
    quantity %= 100;
    return new_ants;
}