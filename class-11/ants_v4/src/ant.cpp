#include <algorithm>
#include <cmath>
#include <numbers>
#include <random>

#include <fmt/format.h>

#include "ant.h"

using namespace std::numbers;

auto Ant::update(const double time_delta, Environment& environment, AntHill& anthill, std::vector<Food>& food) -> void
{
    if (quantity == capacity)
    {
        auto dx = anthill.position[0] - position[0];
        auto dy = anthill.position[1] - position[1];
        auto distance_to_origin = std::hypot(dx, dy);
        if (distance_to_origin < 10.0)
        {
            direction = std::atan2(dy, dx);
        }
        else
        {
            direction = environment.get_pheremone_trail(position, direction, 0);
        }
        position[0] += 4.0 * std::cos(direction) * time_delta;
        position[1] += 4.0 * std::sin(direction) * time_delta;
        direction = environment.restrict_position(position, direction);
        if (pheremone)
        {
            pheremone--;
            environment.drop_pheremone(position, 1);
        }
        dx = position[0] - anthill.position[0];
        dy = position[1] - anthill.position[1];
        distance_to_origin = std::hypot(dx, dy);
        if (distance_to_origin < 1.0)
        {
            anthill.deposit(quantity);
            quantity = 0;
            pheremone = 100;
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

    if (distance < 1.0)
    {
        pheremone = 100;
        quantity += closest_food->deplete(1);
        if (quantity == capacity)
        {
            direction += pi;
        }
    }
    else
    {
        if (distance < 5.0)
        {
            auto dx = closest_food->position[0] - position[0];
            auto dy = closest_food->position[1] - position[1];
            direction = std::atan2(dy, dx);
        }
        else
        {
            direction = environment.get_pheremone_trail(position, direction, 1);
        }

        position[0] += 4.0 * std::cos(direction) * time_delta;
        position[1] += 4.0 * std::sin(direction) * time_delta;
        direction = environment.restrict_position(position, direction);
        if (pheremone)
        {
            pheremone--;
            environment.drop_pheremone(position, 0);
        }
    }
}
