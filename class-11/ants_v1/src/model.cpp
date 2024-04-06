#include "model.h"

#include <algorithm>
#include <fstream>
#include <numbers>

using namespace std::numbers;

void Model::initialize(std::filesystem::path configuration_file)
{
    time = 0.0;

    auto cfs = std::ifstream{ configuration_file };
    configuration = json::parse(cfs);

    auto number_of_entities = configuration["number_of_entities"].get<std::size_t>();
    entities.resize(number_of_entities);

    auto eng = std::mt19937{ 1337 };
    auto dis = std::uniform_real_distribution<>{ -10, 10 };

    std::ranges::generate(entities, [&]() -> Entity {
        return {
            {dis(eng), dis(eng)},
            {dis(eng), dis(eng)}
        };});

    auto food_dis = std::uniform_real_distribution<>{ -100, 100 };
    auto food_center = std::array<double, 2>{
        food_dis(eng),
            food_dis(eng)
    };

    auto radius_dis = std::uniform_real_distribution<>{ 0.0, 1.0 };
    auto theta_dis = std::uniform_real_distribution<>{ 0.0, 2.0 * pi };

    auto number_of_food = configuration["number_of_food"].get<std::size_t>();
    food.resize(number_of_food);
    std::ranges::generate(food, [&]() -> Food {
        auto radius = radius_dis(eng);
        auto theta = theta_dis(eng);
        return {
            10,
            {
                food_center[0] + radius * std::cos(theta),
                food_center[1] + radius * std::sin(theta)
            }
        };
        });
}

void Model::update(const double time_delta)
{
    time += time_delta;
    for (auto& entity : entities)
    {
        entity.update(time_delta, environment, food);
    }
}

void Model::finalize()
{}