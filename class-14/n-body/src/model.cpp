
#include <algorithm>
#include <fstream>
#include <random>

#include "model.h"


void Model::initialize(std::filesystem::path configuration_file, const unsigned int seed)
{
    time = 0.0;

    auto cfs = std::ifstream{ configuration_file };
    configuration = json::parse(cfs);

    auto eng = std::mt19937{ seed };
    auto dis = std::uniform_real_distribution{ -100.0, 100.0 };

    std::ranges::generate_n(std::back_inserter(entities), 1000, [&]() -> Entity {
        return {
            {dis(eng), dis(eng)},
        };
        });
}

void Model::update(const double delta_time)
{
    time += delta_time;

    for (auto& e1 : entities)
    {
        for (auto& e2 : entities)
        {
            if (&e1 == &e2)
            {
                continue;
            }
            e1.compute(e2);
        }
    }

    for (auto& entity : entities)
    {
        entity.update(delta_time);
    }
}

void Model::finalize()
{
    entities.clear();
}