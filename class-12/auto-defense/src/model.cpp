#include "model.h"

#include <algorithm>
#include <fstream>

void Model::initialize(std::filesystem::path configuration_file)
{
    time = 0.0;
    auto cfs = std::ifstream{ configuration_file };
    configuration = json::parse(cfs);
    std::ranges::generate_n(std::back_inserter(projectiles), 1, [&]() -> Projectile {
        return {
            {0.0, 0.0},
            {195.0, 195.0}
        };
        });
}

void Model::update(const double time_delta)
{
    time += time_delta;
    for (auto& projectile : projectiles)
    {
        projectile.update(time_delta, environment);
    }

    simulation_complete = std::ranges::all_of(projectiles, [](const auto& projectile) { return projectile.position[1] <= 0.0; });
}

void Model::finalize()
{
    projectiles.clear();
}