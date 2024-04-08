#include "model.h"

#include <algorithm>
#include <fstream>

void Model::initialize(std::filesystem::path configuration_file)
{
    time = 0.0;

    auto cfs = std::ifstream{ configuration_file };
    configuration = json::parse(cfs);

    auto bounds = configuration["environment_bounds"].get<double>();
    auto bounds_dis = std::uniform_real_distribution<>{ -bounds, bounds };
    environment.x_limit = { -bounds, bounds };
    environment.y_limit = { -bounds, bounds };

    environment.pheremones.resize(10000);

    anthill.position = { bounds_dis(eng), bounds_dis(eng) };

    auto number_of_ants = configuration["number_of_ants"].get<std::size_t>();
    ants.reserve(number_of_ants);
    anthill.quantity = number_of_ants * 100;

    auto number_of_food_piles = configuration["number_of_food_piles"].get<std::size_t>();
    auto number_of_food_per_pile = configuration["number_of_food_per_pile"].get<std::size_t>();
    for (auto i = std::size_t{}; i < number_of_food_piles; ++i)
    {
        auto food_center = std::array<double, 2>{bounds_dis(eng), bounds_dis(eng)};
        std::ranges::generate_n(std::back_inserter(food), number_of_food_per_pile, [&]() -> Food {
            auto radius = radius_dis(eng);
            auto theta = theta_dis(eng);
            return {
                {
                    food_center[0] + radius * std::cos(theta),
                    food_center[1] + radius * std::sin(theta)
                }
            };
            });
    }
}

void Model::update(const double time_delta)
{
    time += time_delta;
    for (auto& ant : ants)
    {
        ant.update(time_delta, environment, anthill, food);
    }

    environment.update();
    if (anthill.spawn_ants(time_delta))
    {
        ants.emplace_back(anthill.position, theta_dis(eng));
    }

    std::erase_if(food, [](const auto& f) { return f.quantity == 0; });
}

void Model::finalize()
{
    ants.clear();
    food.clear();
    environment.pheremones.clear();
}