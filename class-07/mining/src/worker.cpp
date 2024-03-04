#include <cmath>
#include "worker.h"

// Updating the worker means doing one of the following:
// - if the worker is carrying nothing, the mineral is still available, and the
//   worker is too far to mine, then the worker will move towards the mineral.
// - else, if the worker is not full, the mineral is still available, and the
//   worker is close enough to mine, then the worker will mine minerals
// - else, if the worker is full or if the mineral is unavailable, return to the
//   origin, and "drop off" when arriving.
auto Worker::update(Mineral& mineral) -> void
{
    if (is_empty() && mineral.is_available() && !is_close_to_mineral(mineral))
    {
        move(mineral.position_x, mineral.position_y, 1.0);
    }
    else if (!is_full() && mineral.is_available() && is_close_to_mineral(mineral))
    {
        work(mineral);
    }
    else if (is_full() || !mineral.is_available())
    {
        move(0.0, 0.0, 1.0 / (1.0 + units));
        if (auto distance_to_origin = std::hypot(position_x, position_y); distance_to_origin < 1e-8)
        {
            units = 0;
        }
    }
}

auto Worker::move(double target_x, double target_y, double speed) -> void
{
    auto angle = std::atan2(target_y - position_y, target_x - position_x);
    position_x += speed * std::cos(angle);
    position_y += speed * std::sin(angle);
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
