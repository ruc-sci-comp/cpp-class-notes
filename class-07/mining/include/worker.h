#pragma once

#include "mineral.h"

struct Worker
{
    auto update(Mineral& mineral) -> void;

    auto move(double target_x, double target_y, double speed) -> void;

    auto work(Mineral &mineral) -> void;

    auto is_empty() -> bool;

    auto is_full() -> bool;

    auto is_close_to_mineral(const Mineral& mineral) -> bool;

    double position_x{ 0.0 };
    double position_y{ 0.0 };

    int units{ 0 };
    int capacity{ 5 };
};
