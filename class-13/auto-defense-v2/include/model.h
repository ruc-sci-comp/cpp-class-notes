#pragma once

#include <filesystem>
#include <numbers>
#include <random>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "environment.h"
#include "projectile.h"
#include "radar_system.h"
#include "weapons_system.h"

using namespace std::numbers;
using json = nlohmann::json;

struct Model
{
    void initialize(std::filesystem::path configuration_file, const int seed);

    void update(const double delta_time);

    void finalize();

    json configuration{};
    std::vector<Projectile> projectiles{};
    Environment environment{};
    RadarSystem radar_system{};
    WeaponsSystem weapons_system{};
    double time{};
    bool simulation_complete = false;

    std::mt19937 eng{ 1337 };
};