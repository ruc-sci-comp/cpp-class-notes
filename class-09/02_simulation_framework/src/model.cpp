#include "model.h"

#include <algorithm>
#include <fstream>

Model::Model()
{}

void Model::initialize(std::filesystem::path configuration_file)
{
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
}

void Model::update(const double time_delta)
{
    time += time_delta;
    for (auto& entity : entities)
    {
        entity.update(time_delta);
    }
}

void Model::finalize()
{}