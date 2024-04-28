#include "model.h"

#include <algorithm>
#include <fstream>

void Model::initialize(std::filesystem::path configuration_file, const int seed)
{
    time = 0.0;

    auto cfs = std::ifstream{ configuration_file };
    configuration = json::parse(cfs);

    eng.seed(seed);

    std::ranges::generate_n(std::back_inserter(projectiles), 1, [&]() -> Projectile {
        return {
            {0.0, 0.0},
            {static_cast<double>(eng() % 27 + 100), static_cast<double>(eng() % 27 + 100)}
        };
        });

    radar_system = {
        .position = configuration["radar"]["position"].get<std::array<double, 2>>(),
        .detection_range = configuration["radar"]["detection_range"].get<double>(),
        .alpha = configuration["radar"]["alpha"].get<double>(),
        .beta = configuration["radar"]["beta"].get<double>()
    };

    weapons_system = {
        .position = configuration["weapons"]["position"].get<std::array<double, 2>>(),
        .engage_range = configuration["weapons"]["engage_range"].get<double>(),
        .exit_speed = configuration["weapons"]["exit_speed"].get<double>(),
        .interceptor_fuse = configuration["weapons"]["interceptor_fuse"].get<double>(),
        .blast_radius = configuration["weapons"]["blast_radius"].get<double>(),
        .cooldown = configuration["weapons"]["cooldown"].get<double>()
    };

    std::uniform_int_distribution<> wind_angle_dis(-10, 10);
    std::uniform_int_distribution<> wind_speed_dis(0, 10);

    auto wind_angle = wind_angle_dis(eng) * pi / 180.0;
    auto wind_speed = wind_speed_dis(eng);

    environment.wind = {
        wind_speed * std::cos(wind_angle),
        wind_speed * std::sin(wind_angle)
    };
}

void Model::update(const double delta_time)
{
    time += delta_time;
    for (auto& projectile : projectiles)
    {
        projectile.update(delta_time, environment);
        radar_system.update(delta_time, environment, projectile);
        weapons_system.engage(environment, projectile);
    }

    weapons_system.update(delta_time, environment, projectiles);

    simulation_complete = std::ranges::all_of(
        projectiles, [](const auto& projectile) { return projectile.position[1] <= 0.0; }
    );
}

void Model::finalize()
{
    projectiles.clear();
    weapons_system.interceptors.clear();
}