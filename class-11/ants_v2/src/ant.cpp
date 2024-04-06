#include <algorithm>
#include <cmath>
#include <fmt/format.h>
#include "ant.h"

auto Ant::update(const double time_delta, const Environment& environment, std::vector<Food>& food) -> void
{
    // we need to find the closest food
    // check if that food is "close enough"
    // if close enough, deplete
    // else keep moving
    auto get_closest_food = [&](const Food& f1, const Food& f2) -> bool
        {
            // get the distance between the ant and f1, and the ant and f2,
            // and return if the first distance is less than the second
            auto d1 = std::hypot(
                f1.position[0] - position[0], // dx
                f1.position[1] - position[1]  // dy
            );
            auto d2 = std::hypot(
                f2.position[0] - position[0], // dx
                f2.position[1] - position[1]  // dy
            );
            return d1 < d2;
        };
    auto closest_food = std::ranges::min_element(food, get_closest_food);
    auto distance = std::hypot(
        closest_food->position[0] - position[0],
        closest_food->position[1] - position[1]
    );

    if (distance < 1.0 && closest_food->quantity > 0)
    {
        quantity += closest_food->deplete(1);
    }
    else
    {
        auto modification = environment.get_multiplier(position);
        position[0] += velocity[0] * modification[0] * time_delta;
        position[1] += velocity[1] * modification[1] * time_delta;
        environment.restrict_position(position);
        environment.reflect(position, velocity);
    }
}

auto Ant::log(const double time) -> void
{
    fmt::print("{:.2f},{:.2f},{:.2f}\n", time, position[0], position[1]);
}