#include "model.h"

#include <algorithm>
#include <fstream>

void Model::initialize(std::filesystem::path configuration_file)
{
    time = 0.0;
    auto cfs = std::ifstream{ configuration_file };
    configuration = json::parse(cfs);

    eng.seed(std::random_device{}());

    std::ranges::generate_n(std::back_inserter(projectiles), 1, [&]() -> Projectile {
        return {
            {0.0, 0.0},
            {static_cast<double>(eng() % 100 + 200), static_cast<double>(eng() % 100 + 200)}
        };
        });
}

void Model::update(const double delta_time)
{
    time += delta_time;
    for (auto& projectile : projectiles)
    {
        projectile.update(delta_time, environment);
    }

    simulation_complete = std::ranges::all_of(
        projectiles, [](const auto& projectile) { return projectile.position[1] <= 0.0; }
    );
}

void Model::finalize()
{
    projectiles.clear();
}