#include <algorithm>
#include <cmath>
#include <numbers>
#include <random>

#include <fmt/format.h>

#include "ant.h"

using namespace std::numbers;

auto Ant::update(const double time_delta, Environment& environment, AntHill& anthill, std::vector<Food>& food) -> void
{
    static auto dis = std::normal_distribution<>{ 0.0, 0.025 };
    static auto eng = std::mt19937{ 1337 };

    if (quantity == capacity)
    {
        direction = environment.get_pheremone_trail(position, direction, 0);
        position[0] += 3.0 * std::cos(direction) * time_delta;
        position[1] += 3.0 * std::sin(direction) * time_delta;
        environment.restrict_position(position);
        environment.drop_pheremone(position, 1);
        direction = environment.reflect(position, direction);
        auto dx = position[0] - anthill.position[0];
        auto dy = position[1] - anthill.position[1];
        auto distance_to_origin = std::hypot(dx, dy);
        if (distance_to_origin < 1.0)
        {
            anthill.deposit(quantity);
            quantity = 0;
            direction += pi;
        }
        return;
    }

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
            return f1.quantity > 0 && d1 < d2;
        };
    auto closest_food = std::ranges::min_element(food, get_closest_food);
    auto distance = std::hypot(
        closest_food->position[0] - position[0],
        closest_food->position[1] - position[1]
    );

    if (distance < 0.5)
    {
        quantity += closest_food->deplete(1);
        if (quantity == capacity)
        {
            direction += pi;
        }
    }
    else
    {
        direction = environment.get_pheremone_trail(position, direction, 1) + dis(eng);
        position[0] += 5.0 * std::cos(direction) * time_delta;
        position[1] += 5.0 * std::sin(direction) * time_delta;
        environment.restrict_position(position);
        environment.drop_pheremone(position, 0);
        direction = environment.reflect(position, direction);
    }
}
