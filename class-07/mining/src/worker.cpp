#include "mineral.h"
#include "worker.h"
#include <cmath>



auto Worker::move(double target_x, double target_y, double speed) -> void
{
    // MJS: this moves the worker, not the mineral
    // We have the worker position so we only need to move the worker,
    // not the mineral.
    // Mineral min;
    auto angle = double{ std::atan2(target_y - position_y, target_x - position_x) };
    position_x += speed * cos(angle);
    position_y += speed * sin(angle);
}

auto Worker::update(Mineral& mineral) -> void
{
    // MJS: didn't need local variable since we have a reference to variable
    // from the caller's scope
    // Mineral min;
    if (is_empty() && mineral.is_available() && !is_close_to_mineral(mineral))
    {
        move(mineral.position_x, mineral.position_y, 1.0);
    }
    // MJS: remember we have to mine until we are NOT full
    // full is not the opposite of empty here... we can be half full!
    else if (!is_full() && mineral.is_available() && is_close_to_mineral(mineral))
    {
        work(mineral);
    }
    else if (is_full() || !mineral.is_available())
    {
        // MJS: Added code to make worker slower with more minerals
        move(0, 0, 1.0 / (1.0 + units));

        // MJS: added code to zero units
        if (auto distance_to_origin = std::hypot(position_x, position_y); distance_to_origin < 1e-8)
        {
            units = 0;
        }
    }
}

auto Worker::work(Mineral& mineral) -> void
{
    auto requested_minerals = std::min(1, capacity - units);
    auto units_mined = mineral.mine(requested_minerals);
    units += units_mined;
}

auto Worker::is_empty() -> bool
{
    return units == 0;
}

auto Worker::is_full() -> bool
{
    return units == capacity;
}

auto Worker::is_close_to_mineral(const Mineral& mineral) -> bool
{
    return std::hypot(mineral.position_x - position_x, mineral.position_y - position_y) < 1.0;
}
