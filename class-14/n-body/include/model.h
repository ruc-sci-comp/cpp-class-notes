#pragma once

#include <filesystem>
#include <vector>

#include <nlohmann/json.hpp>

#include "entity.h"

using json = nlohmann::json;

struct Model
{
    void initialize(std::filesystem::path configuration_file, const unsigned int seed);

    void update(const double delta_time);

    void finalize();

    json configuration{};
    std::vector<Entity> entities{};
    double time{};
};