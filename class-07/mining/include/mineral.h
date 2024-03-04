#pragma once

struct Mineral
{
    auto mine(int requested) -> int;

    auto is_available() -> bool;

    double position_x{ 0.0 };
    double position_y{ 0.0 };
    int units{ 100 };
};
