#include <algorithm>
#include <fstream>
#include <random>
#include <vector>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include "simple_entity.h"

using namespace nlohmann;

auto main() -> int
{
    auto configuration_stream = std::ifstream{ "data/config.json" };
    auto configuration = json::parse(configuration_stream);
    auto number_of_entities = int{ configuration["number_of_entities"] };

    auto engine = std::mt19937(1337);
    auto distribution = std::uniform_real_distribution<>{ -10.0, 10.0 };

    auto entities = std::vector<SimpleEntity>(number_of_entities);
    auto make_entity = [&]() -> SimpleEntity
        {
            auto position = std::array<double, 2>{distribution(engine), distribution(engine)};
            auto velocity = std::array<double, 2>{distribution(engine), distribution(engine)};
            return { position, velocity };
        };
    std::ranges::generate(entities, make_entity);

    auto simulation_end_time = double{ configuration["simulation_end_time"] };
    auto delta_time = double{ configuration["delta_time"] };

    for (auto time = double{}; time < simulation_end_time; time += delta_time)
    {
        for (auto& entity : entities)
        {
            entity.update(delta_time);
            entity.log(time);
        }
    }
}
