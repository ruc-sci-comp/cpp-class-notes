#include <cmath>
#include <fmt/format.h>

auto main() -> int
{
    auto time_delta = double{ 604800.0 };
    auto sun_mass = double{ 1.989e30 };

    auto earth_x = double{ 147.88e9 };
    auto earth_y = double{ 0.0 };
    auto earth_vx = double{ 0.0 };
    auto earth_vy = double{ 29784.8 };

    auto gravitational_constant = double{ 6.674e-11 };

    fmt::print("{} {} {} {} {}\n", 0.0, earth_x, earth_y, earth_vx, earth_vy);

    for (auto time = double{}; time < 3.154e8; time += time_delta)
    {
        // F = G * m1 * m2 / (d * d)
        auto distance = std::hypot(earth_x, earth_y);
        auto angle = std::atan2(-earth_y, -earth_x);
        auto force = gravitational_constant * sun_mass / (distance * distance);

        auto earth_ax = force * std::cos(angle);
        auto earth_ay = force * std::sin(angle);

        earth_vx += earth_ax * time_delta;
        earth_vy += earth_ay * time_delta;

        earth_x += earth_vx * time_delta;
        earth_y += earth_vy * time_delta;

        fmt::print("{} {} {} {} {}\n", time, earth_x, earth_y, earth_vx, earth_vy);
    }
}
