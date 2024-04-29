#pragma once

#include <filesystem>
#include <vector>

#include <nlohmann/json.hpp>

#include "entity.h"

using json = nlohmann::json;

struct Model
{
    void initialize(std::filesystem::path configuration_file);

    void initialize(const nlohmann::json& configuration);

    void update(const double delta_time);

    void finalize();

    std::vector<Entity> entities{};
    double time{};
};