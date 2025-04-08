#include "mineral.h"
#include "worker.h"
#include <cmath>



auto Worker::move(double target_x, double target_y, double speed) -> void
{
    Mineral min;
	auto angle = double{std::atan2(target_y - min.position_y, target_x - min.position_x)};
	min.position_x += speed * cos(angle);
	min.position_y += speed * sin(angle);
}

auto Worker::update(Mineral& mineral) -> void
{
    Mineral min;
	if(is_empty() && mineral.is_available() && !is_close_to_mineral(min))
	{
	move(min.position_x, min.position_y, 1.0);
	}
	else if(is_empty() && mineral.is_available() && is_close_to_mineral(min))
	{
	work(min);
	}
	else if(is_full() || !mineral.is_available())
	{
	move(0, 0, 1.0);
	}
}
