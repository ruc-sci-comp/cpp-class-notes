#pragma once

#include <filesystem>
#include <random>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "entity.h"
#include "environment.h"

using json = nlohmann::json;

struct Model
{
    void initialize(std::filesystem::path configuration_file);

    void update(const double time_delta);

    void finalize();

    json configuration{};
    std::vector<Entity> entities{};
    Environment environment{};
    double time{};
};