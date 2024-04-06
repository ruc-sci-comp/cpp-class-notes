#pragma once

#include <filesystem>
#include <random>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "ant.h"
#include "environment.h"
#include "food.h"

using json = nlohmann::json;

struct Model
{
    void initialize(std::filesystem::path configuration_file);

    void update(const double time_delta);

    void finalize();

    json configuration{};
    std::vector<Ant> ants{};
    std::vector<Food> food{};
    Environment environment{};
    double time{};
};