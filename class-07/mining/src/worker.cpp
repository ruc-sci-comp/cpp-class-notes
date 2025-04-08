#include "mineral.h"
#include "worker.h"
#include <cmath>



auto Worker::move(double target_x, double target_y, double speed) -> void
{
	Mineral min;
	auto angle = double{ std::atan2(target_y - min.position_y, target_x - min.position_x) };
	min.position_x += speed * cos(angle);
	min.position_y += speed * sin(angle);
}

auto Worker::update(Mineral& mineral) -> void
{
	Mineral min;
	if (is_empty() && mineral.is_available() && !is_close_to_mineral(min))
	{
		move(min.position_x, min.position_y, 1.0);
	}
	else if (is_empty() && mineral.is_available() && is_close_to_mineral(min))
	{
		work(min);
	}
	else if (is_full() || !mineral.is_available())
	{
		move(0, 0, 1.0);
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
