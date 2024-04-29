
#include <algorithm>
#include <fstream>
#include <random>

#include "model.h"


void Model::initialize(std::filesystem::path configuration_file)
{
    auto cfs = std::ifstream{ configuration_file };
    initialize(json::parse(cfs));
}

void Model::initialize(const nlohmann::json& configuration)
{
    time = 0.0;

    auto seed = configuration["seed"].get<std::size_t>();
    auto bounds = configuration["bounds"].get<double>();
    auto number_of_entities = configuration["number_of_entities"].get<int>();

    auto eng = std::mt19937{ seed };
    auto dis = std::uniform_real_distribution{ -bounds, bounds };

    auto mass_dis = std::uniform_real_distribution{ 5.0e6, 5.0e7 };

    std::ranges::generate_n(std::back_inserter(entities), number_of_entities, [&]() -> Entity {
        return {
            .position={dis(eng), dis(eng)},
            .mass=mass_dis(eng)
        };
        });
    entities[0].position = {0.0, 0.0};
    entities[0].mass = 1.0e12;

    number_of_threads = configuration["number_of_threads"].get<std::size_t>();
    slice_size = entities.size() / number_of_threads;
}

void Model::update(const double delta_time)
{    
    time += delta_time;

    auto threaded_update = [&](const auto start, const auto stop) -> void {
        for (auto idx = start; idx < stop; ++idx)
        {
            auto &e1 = entities[idx];
            for (auto& e2 : entities)
            {
                if (&e1 == &e2)
                {
                    continue;
                }
                e1.compute(e2);
            }
        }

        sync_point.arrive_and_wait();

        for (auto idx = start; idx < stop; ++idx)
        {
            entities[idx].update(delta_time);
        }
        };

    for (auto i = std::size_t{}; i < number_of_threads; ++i)
    {
        threads.emplace_back(threaded_update, i * slice_size, (i+1) * slice_size);
    }
    threads.clear();
}

void Model::finalize()
{
    entities.clear();
}
