#pragma once

#include <filesystem>
#include <numbers>
#include <random>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "ant.h"
#include "anthill.h"
#include "environment.h"
#include "food.h"

using namespace std::numbers;
using json = nlohmann::json;

struct Model
{
    void initialize(std::filesystem::path configuration_file);

    void update(const double time_delta);

    void finalize();

    json configuration{};
    std::vector<Ant> ants{};
    std::vector<Food> food{};
    AntHill anthill{};
    Environment environment{};
    double time{};

    std::mt19937 eng{ 1337 };
    std::uniform_real_distribution<> theta_dis{ 0.0, 2.0 * pi };
    std::uniform_real_distribution<> radius_dis{ 0.0, 1.0 };

};